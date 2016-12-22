#include "WorldWardenUtils.hpp"
#include <windows.h>
#include <intrin.h>

namespace RCN { namespace World { namespace Warden {

    /// Get module mapped size
    /// @p_Input : Input decompressed warden module
    size_t GetModuleMapedSize(RCN::Network::ByteBuffer& p_Input)
    {
        ModuleHeader* l_Header = reinterpret_cast<ModuleHeader*>(const_cast<uint8_t*>(p_Input.GetData()));
        return l_Header->MapedSize;
    }
    /// Remap warden module
    /// @p_Input : Input decompressed warden module
    uint32_t RemapModule(RCN::Network::ByteBuffer& p_Input)
    {
#define module_get_int32(a, b)      (*(uint32_t*)(a + (uint32_t)b))
#define module_get_aint32(a, b)     (*(uint32_t*)(&a[b]))
#define module_get_int16(a, b)      (*(uint16_t*)&a[(uint32_t)b])
#define module_swap_int16(a)        (((a & 0xFF00) >> 8) | ((a & 0xFF) << 8))
#define module_set_int32(a, b, c)   (*(uint32_t*)(a + b) = c)

        auto AllocExecMemory = [](uint32_t size) -> uint32_t
        {
            LPVOID l_Result = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
            VirtualLock(l_Result, size);
            return (uint32_t)l_Result;
        };

        ModuleHeader *l_Header = reinterpret_cast<ModuleHeader*>(const_cast<uint8_t*>(p_Input.GetData()));

        uint32_t l_MapedSize    = GetModuleMapedSize(p_Input);
        uint32_t l_FinalModule  = AllocExecMemory(l_MapedSize);

        uint8_t* l_Source = const_cast<uint8_t*>(p_Input.GetData());

        memset((uint8_t*)l_FinalModule, 0,          l_MapedSize);           ///< Init destination
        memcpy((uint8_t*)l_FinalModule, l_Source,   sizeof(ModuleHeader));  ///< Copy module header

        printf("[WARDEN:RemapModule] Source size %u final size %u\n", p_Input.GetSize(), l_MapedSize);

        //////////////////////////////////////////////////////////////////////////
        /// Code blocks
        //////////////////////////////////////////////////////////////////////////

        uint32_t l_SourceLocation   = sizeof(ModuleHeader) + (l_Header->Unknown5 * 12);
        uint32_t l_DestLocation     = module_get_aint32(l_Source, 40);

        uint8_t l_BlockSkip = 0;
        while (l_DestLocation < l_MapedSize)
        {
            uint16_t l_Length = module_get_int16(l_Source, l_SourceLocation);
            l_SourceLocation += 2;

            if (!l_BlockSkip)
            {
                memcpy((uint8_t*)(l_FinalModule + l_DestLocation), l_Source + l_SourceLocation, l_Length);
                l_SourceLocation += l_Length;
            }
            l_BlockSkip = !l_BlockSkip;
            l_DestLocation += l_Length;
        }

        //////////////////////////////////////////////////////////////////////////
        /// Global vars
        //////////////////////////////////////////////////////////////////////////

        uint16_t * l_RefTables = (uint16_t*)(l_FinalModule + l_Header->RefTable);
        l_DestLocation = 0;

        printf("[WARDEN:RemapModule] Adjusting %d references to global variables\n", static_cast<uint32_t>(l_Header->RefCount));

        for (uint32_t l_X = 0; l_X < l_Header->RefCount; l_X++)
        {
            l_DestLocation += module_swap_int16(l_RefTables[l_X]);
            module_set_int32(l_FinalModule, l_DestLocation, (module_get_int32(l_FinalModule, l_DestLocation) % l_MapedSize) + l_FinalModule);
        }

        //////////////////////////////////////////////////////////////////////////
        /// Imports
        //////////////////////////////////////////////////////////////////////////

        printf("[WARDEN:RemapModule] Updating API library references\n");

        LibraryReferance* l_LibTables = reinterpret_cast<LibraryReferance*>(l_FinalModule + l_Header->LibTable);
        for (uint32_t l_X = 0; l_X < l_Header->LibCount; l_X++)
        {
            char* l_LibName = reinterpret_cast<char*>(l_FinalModule + l_LibTables[l_X].NameAddress);
            HMODULE l_LibHandle = LoadLibrary(l_LibName);

            uint32_t l_LibFunction = l_LibTables[l_X].FunctionTable;
            while (module_get_int32(l_FinalModule, l_LibFunction) != 0)
            {
                uint32_t l_FuncBaseAddr = module_get_int32(l_FinalModule, l_LibFunction);

                if ((l_FuncBaseAddr & 0x7FFFFFFF) > l_MapedSize)
                {
                    printf("[WARDEN:RemapModule] Attempted to read API from offset pass end of module: 0x%08X\n", static_cast<uint32_t>(l_FuncBaseAddr));
                    break;
                }

                uint32_t l_FinalAddress = 0;
                if (l_FuncBaseAddr & 0x80000000)
                    l_FinalAddress = (uint32_t)GetProcAddress(l_LibHandle, reinterpret_cast<char*>(l_FuncBaseAddr & 0x7FFFFFFF));
                else
                    l_FinalAddress = (uint32_t)GetProcAddress(l_LibHandle, reinterpret_cast<char*>(l_FinalModule + l_FuncBaseAddr));

                module_set_int32(l_FinalModule, l_LibFunction, l_FinalAddress);
                l_LibFunction += 4;
            }
        }

        return l_FinalModule;

#undef module_get_int32
#undef module_get_aint32
#undef module_get_int16
#undef module_swap_int16
#undef module_set_int32
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    template<class T> T __ROL__(T value, int count)
    {
        const uint nbits = sizeof(T) * 8;

        if (count > 0)
        {
            count %= nbits;
            T high = value >> (nbits - count);
            if (T(-1) < 0) // signed value
                high &= ~((T(-1) << count));
            value <<= count;
            value |= high;
        }
        else
        {
            count = -count % nbits;
            T low = value << (nbits - count);
            value >>= count;
            value |= low;
        }
        return value;
    }
    inline uint8_t  __ROL1__(uint8_t  value, int count) { return __ROL__((uint8_t)value, count); }
    inline uint8_t  __ROR1__(uint8_t value, int count) { return __ROL__((uint8_t)value, -count); }
    inline uint16_t __ROL2__(uint16_t value, int count) { return __ROL__((uint16_t)value, count); }
    inline uint32_t __ROL4__(uint32_t value, int count) { return __ROL__((uint32_t)value, count); }
    inline uint64_t __ROL8__(uint64_t value, int count) { return __ROL__((uint64_t)value, count); }
    inline uint16_t __ROR2__(uint16_t value, int count) { return __ROL__((uint16_t)value, -count); }
    inline uint32_t __ROR4__(uint32_t value, int count) { return __ROL__((uint32_t)value, -count); }
    inline uint64_t __ROR8__(uint64_t value, int count) { return __ROL__((uint64_t)value, -count); }
#define __PAIR__(high, low) (((uint64_t)(high)<<sizeof(high)*8) | low)


    int32_t ScrambleKey_UnkXorShifting(int32_t p_A, int32_t p_B, int32_t p_C)
    {
        return p_C & 1 ^ (p_C | 1) * (p_B & 1 ^ p_A * (p_B | 1));
    }

    void ScrambleKey_UnkMultSum(int32_t p_InOut_PartAHi, int32_t p_InOut_PartALo, uint32_t p_In_PartBHi, uint32_t p_In_PartBLo)
    {
        __m64 l_A;
        l_A.m64_i32[1] = p_InOut_PartAHi;
        l_A.m64_i32[0] = p_InOut_PartALo;

        __m64 l_B;
        l_B.m64_u32[1] = p_In_PartBHi;
        l_B.m64_u32[0] = p_In_PartBLo;

        __m64 l_Sum = _m_paddd(_m_pmaddwd(l_A, l_B), _m_pmullw(l_A, l_B));

        __m64 l_Shift;
        l_Shift.m64_i64 = 0x20;

        *(uint32_t *)p_InOut_PartAHi = _mm_cvtsi64_si32(l_Sum);
        *(uint32_t *)p_InOut_PartALo = _mm_cvtsi64_si32(_m_psrlq(l_Sum, l_Shift));
    }

    struct struct_p_This
    {
        uint32_t Key[4];
    };
    int ScrambleKey(struct_p_This *p_This)
    {
        int32_t v219; // [sp+18h] [bp-4h]@1

        int32_t l_ResultPartC; // ecx@1
        uint32_t l_ResultPartA; // [sp+Ch] [bp-10h]@1
        int32_t l_ResultPartD; // [sp+10h] [bp-Ch]@1
        int32_t l_ResultPartB; // [sp+14h] [bp-8h]@1

        uint32_t v2 = __ROL4__(p_This->Key[0] + 0x7D7D7BBE, 31) - 865160792;
        int32_t  v3 = __ROL4__(p_This->Key[1] + 0x67B6EA46 + v2, 27) + 0x4B13D756;

        l_ResultPartA = v2;
        l_ResultPartB = v3;
        l_ResultPartD = 0xB4EC28AA;

        int32_t v4 = __ROL4__((v2 ^ v3 & (v2 ^ 0xB4EC28AA)) + p_This->Key[2] - 1606725526, 5);
        int32_t v5 = v4 + v3 + 1;
        v219 = v5;

        l_ResultPartD = v5;
        l_ResultPartA = 0xB4EC28AA;

        int32_t v8 = __ROL4__(
            p_This->Key[3]
            + (l_ResultPartA & l_ResultPartB ^ v219 & (l_ResultPartA ^ l_ResultPartB))
            + l_ResultPartD
            - 0x5CF436A3,
            23);
        int32_t v9 = -l_ResultPartA ^ v8;
        l_ResultPartD = v9;
        int32_t v11 = __ROL4__(p_This->Key[0] + (v219 ^ l_ResultPartB & (v219 ^ v9)) + l_ResultPartA - 0xD4D9021, 25);
        int32_t v12 = v11 - _byteswap_ulong(l_ResultPartB);
        int32_t v13 = __ROL4__(l_ResultPartB + v219 + p_This->Key[1] - v9 + v12 - 0x4FBC7F55, 25);
        int32_t v14 = v12 ^ v13;
        int32_t v15 = __ROL4__(p_This->Key[2] + (v12 ^ (v9 | ~v14)) + v219 - 0x718FA137, 24);
        int32_t v16 = v9 + v15;
        l_ResultPartB = v14;
        int32_t v17 = ScrambleKey_UnkXorShifting(v16, v12, v14);
        int32_t v18 = __ROL4__(v17 + l_ResultPartD + p_This->Key[3] - 0x51F43506, 29);
        l_ResultPartD = v18 - v16;
        int32_t v20 = __ROL4__(p_This->Key[2] + (l_ResultPartB & v16 ^ l_ResultPartD & (l_ResultPartB ^ v16)) + v12 + 0x6B843618, 4);
        int32_t v21 = v20 + v16 + 1;
        l_ResultPartA = v21;
        int32_t v22 = __ROL4__(p_This->Key[0] + (v21 & v16 ^ l_ResultPartD & (v21 ^ v16)) + l_ResultPartB + 0x3F8FC37B, 7);
        l_ResultPartB = -v21 ^ v22;
        int32_t v24 = __ROL4__(v16 + l_ResultPartD + p_This->Key[0] - v21 + l_ResultPartB - 0x272B066D, 15);
        int32_t v25 = v24 - l_ResultPartD;
        v219 = v25;



        signed __int32 v25; // eax@1
        signed __int32 v28; // edx@1
        int v29; // eax@1
        signed __int32 v30; // eax@1
        int v33; // edi@1
        int v34; // edi@1
        int v35; // ebx@1
        int v36; // ebx@1
        int v37; // eax@1
        int v38; // eax@1
        int v39; // eax@1
        int v40; // ecx@1
        int v41; // ecx@1
        int v42; // edi@1
        int v43; // edi@1
        signed __int32 v44; // edx@1
        int v45; // ebx@1
        int v46; // eax@1
        signed __int32 v47; // eax@1
        int v50; // ecx@1
        unsigned int v51; // ebx@1
        int v52; // ecx@1
        int v53; // edi@1
        int v54; // edi@1
        int v55; // ecx@1
        int v56; // ecx@1
        int v57; // edx@1
        int v58; // edx@1
        int v59; // eax@1
        int v60; // ecx@1
        unsigned __int32 v61; // ebx@1
        int v62; // edi@1
        int v63; // eax@1
        int v64; // eax@1
        int v65; // edx@1
        int v66; // eax@1
        int v67; // edi@1
        int v68; // eax@1
        signed __int32 v69; // eax@1
        int v72; // ebx@1
        int v73; // eax@1
        int v74; // eax@1
        int v75; // edi@1
        int v76; // edi@1
        int v77; // eax@1
        int v78; // edx@1
        int v79; // eax@1
        int v80; // eax@1
        int v81; // edx@1
        int v82; // ebx@1
        int v83; // ebx@1
        int v84; // eax@1
        signed __int32 v85; // edx@1
        int v86; // eax@1
        int v87; // eax@1
        signed __int32 v88; // eax@1
        int v91; // ebx@1
        int v92; // eax@1
        int v93; // eax@1
        int v94; // edi@1
        int v95; // edi@1
        int v96; // eax@1
        int v97; // eax@1
        signed __int32 v98; // eax@1
        signed __int32 v100; // edx@1
        int v102; // ebx@1
        int v103; // eax@1
        int v104; // edi@1
        int v105; // edi@1
        int v106; // eax@1
        int v107; // eax@1
        unsigned __int32 v108; // eax@1
        int v109; // ecx@1
        int v110; // ecx@1
        int v111; // edi@1
        int v112; // edi@1
        signed __int32 v113; // edx@1
        int v114; // eax@1
        signed __int32 v115; // eax@1
        int v118; // ecx@1
        int v119; // ecx@1
        signed __int32 v120; // edx@1
        int v121; // eax@1
        int v122; // eax@1
        signed __int32 v123; // eax@1
        int v126; // eax@1
        int v127; // ebx@1
        int v128; // ebx@1
        int v129; // eax@1
        int v130; // eax@1
        int v131; // edi@1
        int v132; // edi@1
        int v133; // eax@1
        int v134; // eax@1
        int v135; // eax@1
        int v136; // edi@1
        int v137; // eax@1
        signed __int32 v138; // eax@1
        signed __int32 v140; // edx@1
        int v142; // edx@1
        int v143; // edx@1
        int v144; // eax@1
        int v145; // eax@1
        int v146; // edi@1
        int v147; // edi@1
        int v148; // ecx@1
        int v149; // ecx@1
        int v150; // edx@1
        int v151; // edx@1
        int v152; // edi@1
        int v153; // edi@1
        int v154; // eax@1
        int v155; // eax@1
        int v156; // ebx@1
        int v157; // ecx@1
        int v158; // ST04_4@1
        int v159; // edx@1
        int v160; // eax@1
        signed __int32 v161; // eax@1
        signed __int32 v163; // edx@1
        int v165; // edi@1
        int v166; // ebx@1
        int v167; // ebx@1
        int v168; // eax@1
        int v169; // ecx@1
        int v170; // ecx@1
        int v171; // edi@1
        int v172; // edi@1
        int v173; // eax@1
        int v174; // eax@1
        int v175; // ebx@1
        int v176; // ebx@1
        int v177; // eax@1
        int v178; // eax@1
        unsigned __int32 v179; // eax@1
        int v180; // edi@1
        signed __int32 v181; // edx@1
        signed __int32 v182; // eax@1
        int v183; // ecx@1
        int v186; // ebx@1
        int v187; // edi@1
        int v188; // edi@1
        int v189; // eax@1
        int v190; // eax@1
        int v191; // eax@1
        signed __int32 v193; // edx@1
        int v195; // eax@1
        int v196; // eax@1
        int v197; // edx@1
        int v198; // edx@1
        signed __int32 v199; // edx@1
        int v200; // ecx@1
        int v201; // eax@1
        int v202; // eax@1
        int v203; // eax@1
        int v204; // ecx@1
        int v205; // ecx@1
        int v206; // eax@1
        signed __int32 v209; // edx@1
        int v210; // eax@1
        int v211; // eax@1


        l_ResultPartB = v25;
        l_ResultPartA = -v21;

        v29 = __ROL4__(
            p_This->Key[3] + (l_ResultPartA ^ l_ResultPartB & (l_ResultPartA ^ v219)) + l_ResultPartD + 1935600934,
            20);
        v30 = v29 - l_ResultPartB;
        l_ResultPartD = v30;

        v219 = v30;
        l_ResultPartA = v28;

        v33 = __ROL4__(p_This->Key[3] + (v219 ^ (l_ResultPartD | ~l_ResultPartB)) + l_ResultPartA - 44424322, 16);
        v34 = v33 - _byteswap_ulong(~l_ResultPartD);
        v35 = __ROL4__(p_This->Key[1] + (v219 ^ (v34 | ~l_ResultPartD)) + l_ResultPartB - 1072881336, 24);
        v36 = v34 + v35;
        v37 = ScrambleKey_UnkXorShifting(v36, v34, l_ResultPartD);
        v38 = __ROL4__(p_This->Key[1] + v37 + v219 + 0x16CB7F43, 2);
        v39 = ~v36 + v38;
        v40 = __ROL4__(p_This->Key[1] + (v34 & v36 ^ v39 & (v34 ^ v36)) + l_ResultPartD - 57792775, 10);
        v41 = v40 - v34;
        v42 = v39 + v41 + p_This->Key[0] - v36 + v34 - 1140507570;
        l_ResultPartB = ~v39;
        v42 = __ROL4__(v42, 3);
        v43 = ~v39 + v42;
        l_ResultPartD = v41;
        v44 = p_This->Key[1] + (v43 & v39 ^ v41 & (v43 ^ v39));
        l_ResultPartA = v43;
        v45 = __ROL4__(v44 + v36 - 926107165, 14);
        l_ResultPartB = ~v39 ^ v45;
        v46 = __ROL4__(p_This->Key[3] + (v43 & l_ResultPartB ^ v41 & (v43 ^ l_ResultPartB)) + v39 - 1784804023, 18);
        v47 = v46 - v43;
        v219 = v47;
        
        l_ResultPartB = v47;
        l_ResultPartD = v44;

        v50 = __ROL4__(p_This->Key[1] + (l_ResultPartA ^ (v219 | ~l_ResultPartB)) + l_ResultPartD + 1931948882, 21);
        v51 = v50 + v219 + 1;
        v52 = p_This->Key[3];
        v53 = __ROL4__(v52 + (v219 ^ (v51 | ~l_ResultPartB)) + l_ResultPartA - 858370070, 22);
        v54 = _byteswap_ulong(v51) ^ v53;
        v55 = __ROL4__(l_ResultPartB + v51 + v52 - v219 + v54 - 1272824402, 17);
        v56 = v55 + v51 + 1;
        v57 = p_This->Key[0] + (v54 ^ v56 ^ v51);
        l_ResultPartB = v56;
        v58 = __ROL4__(v57 + v219 - 465401009, 18);
        v219 = v58 - v51;
        v59 = __ROL4__(ScrambleKey_UnkXorShifting(v54, v58 - v51, v56) + v51 + p_This->Key[3] - 1169475669, 24);
        l_ResultPartD = v59 - _byteswap_ulong(~v54);
        v60 = __ROL4__(p_This->Key[1] + (l_ResultPartB ^ v219 & (l_ResultPartB ^ l_ResultPartD)) + v54 + 1669942078, 2);
        l_ResultPartA = v60 - v219;
        ScrambleKey_UnkMultSum((int)&l_ResultPartA, (int)&v219, l_ResultPartD, l_ResultPartB);
        v61 = l_ResultPartA;
        v62 = l_ResultPartD;
        v63 = ScrambleKey_UnkXorShifting(v219, l_ResultPartD, l_ResultPartA);
        v64 = __ROL4__(p_This->Key[3] + v63 + l_ResultPartB + 341668086, 5);
        v65 = ~v219;
        v66 = ~v219 + v64;
        v67 = p_This->Key[1] + (v66 ^ (v61 | ~v62));
        l_ResultPartB = v66;
        v68 = __ROL4__(v67 + v219 + 1517207598, 5);
        v69 = ~v61 + v68;
        v219 = v69;

        l_ResultPartD = v69;
        l_ResultPartB = v65;

        v72 = p_This->Key[0];
        v73 = __ROL4__(p_This->Key[3] + (l_ResultPartA ^ l_ResultPartB ^ v219) + l_ResultPartD + 817002477, 20);
        v74 = _byteswap_ulong(~l_ResultPartA) ^ v73;
        v75 = __ROL4__(v219 + v74 + p_This->Key[0] - l_ResultPartB + l_ResultPartA - 576727961, 27);
        v76 = v75 - v219;
        l_ResultPartD = v74;
        v77 = ScrambleKey_UnkXorShifting(v219, v76, v74);
        v78 = __ROL4__(v77 + l_ResultPartB + p_This->Key[3] - 2061401035, 19);
        l_ResultPartB = v219 ^ v78;
        v79 = ScrambleKey_UnkXorShifting(v76, v219 ^ v78, l_ResultPartD);
        v80 = __ROL4__(v79 + v72 + v219 - 498018272, 24);
        v81 = v80 + v76 + 1;
        v82 = __ROL4__(l_ResultPartB + l_ResultPartD + v72 - v81 + v76 + 590866247, 30);
        v83 = _byteswap_ulong(~v76) + v82;
        v219 = v80 + v76 + 1;
        l_ResultPartD = v83;
        v84 = __ROL4__(ScrambleKey_UnkXorShifting(l_ResultPartB, v81, v83) + v76 + p_This->Key[2] - 463880395, 9);
        v85 = _byteswap_ulong(-l_ResultPartB);
        v86 = v84 - v85;
        l_ResultPartA = v86;
        v87 = __ROL4__(p_This->Key[3] + (v219 ^ (v83 | ~v86)) + l_ResultPartB - 1591766676, 24);
        v88 = _byteswap_ulong(-v83) + v87;
        l_ResultPartB = v88;
        
        l_ResultPartA = v88;
        v219 = v85;

        v91 = p_This->Key[3];
        v92 = __ROL4__(v91 + (l_ResultPartD ^ l_ResultPartB & (l_ResultPartA ^ l_ResultPartD)) + v219 + 749824959, 23);
        v93 = ~l_ResultPartB + v92;
        v94 = __ROL4__(v91 + (l_ResultPartA ^ l_ResultPartB ^ v93) + l_ResultPartD - 136550937, 21);
        v95 = -l_ResultPartB ^ v94;
        v219 = v93;
        l_ResultPartD = v95;
        v96 = ScrambleKey_UnkXorShifting(v95, l_ResultPartB, v93);
        v97 = __ROL4__(v96 + l_ResultPartA + v91 - 49354047, 30);
        v98 = ~v95 + v97;
        l_ResultPartA = v98;

        l_ResultPartB = v98;
        v219 = v100;

        v102 = l_ResultPartA;
        v103 = ScrambleKey_UnkXorShifting(l_ResultPartA, v219, l_ResultPartD);
        v104 = __ROL4__(p_This->Key[1] + v103 + l_ResultPartB + 1684704892, 21);
        v105 = v104 - v102;
        v219 = v219 + v105 + p_This->Key[0] + 2 * l_ResultPartD - v102 - 141223932;
        v106 = ScrambleKey_UnkXorShifting(v102, v219, v105);
        v107 = __ROL4__(v106 + l_ResultPartD + p_This->Key[0] - 1722647589, 10);
        v108 = _byteswap_ulong(~v102) ^ v107;
        v109 = p_This->Key[3] + (v105 ^ v219 ^ v108);
        l_ResultPartD = v108;
        v110 = __ROL4__(v109 + v102 + 0x70251807, 16);
        l_ResultPartA = _byteswap_ulong(v219) + v110;
        v111 = __ROL4__(p_This->Key[0] + (v219 ^ (l_ResultPartA | ~v108)) + v105 - 1019739042, 25);
        v112 = -l_ResultPartA ^ v111;
        v113 = v219;
        v114 = __ROL4__(p_This->Key[2] + (l_ResultPartA ^ v112 ^ v108) + v219 - 1888176249, 24);
        v115 = ~l_ResultPartA + v114;
        l_ResultPartB = v112;
        v219 = v115;

        l_ResultPartB = v115;
        l_ResultPartD = v113;

        v118 = __ROL4__(p_This->Key[3] + (v219 ^ (l_ResultPartB | ~l_ResultPartA)) + l_ResultPartD + 721969381, 5);
        v119 = ~l_ResultPartB + v118;
        v120 = _byteswap_ulong(~l_ResultPartB);
        v121 = p_This->Key[2] + (v119 ^ (l_ResultPartB | ~v219));
        l_ResultPartD = v119;
        v122 = __ROL4__(v121 + l_ResultPartA - 294827682, 2);
        v123 = v122 - v120;
        l_ResultPartA = v123;

        v219 = v123;
        l_ResultPartD = v120;

        v126 = p_This->Key[1];
        v127 = __ROL4__(v126 + (l_ResultPartA ^ v219 ^ l_ResultPartD) + l_ResultPartB - 1121068160, 1);
        v128 = v127 - _byteswap_ulong(l_ResultPartA);
        v129 = __ROL4__(v128 + v219 + v126 - l_ResultPartD + l_ResultPartA + 852905038, 17);
        v130 = v129 - v128;
        v131 = __ROL4__(p_This->Key[3] + (l_ResultPartA ^ v128 ^ v130) + l_ResultPartD - 1918557888, 28);
        v132 = l_ResultPartA ^ v131;
        v219 = v130;
        l_ResultPartD = v132;
        v133 = ScrambleKey_UnkXorShifting(v132, v128, v130);
        v134 = __ROL4__(v133 + l_ResultPartA + p_This->Key[0] - 586597598, 15);
        v135 = v134 - v132;
        v136 = p_This->Key[1] + (v135 ^ v219 & (v135 ^ v132));
        l_ResultPartA = v135;
        v137 = __ROL4__(v136 + v128 - 1256288990, 28);
        v138 = v137 + v219 + 1;

        l_ResultPartA = v140;
        l_ResultPartB = v138;
        l_ResultPartD = v138;

        v142 = __ROL4__(l_ResultPartB + v219 + p_This->Key[2] - l_ResultPartD + l_ResultPartA - 2139378914, 26);
        v143 = ~l_ResultPartA ^ v142;
        v144 = __ROL4__(p_This->Key[0] + (l_ResultPartA ^ l_ResultPartB ^ v143) + l_ResultPartD - 1138839693, 19);
        v145 = v144 - v143;
        v146 = __ROL4__(p_This->Key[3] + (l_ResultPartB ^ v143 ^ v145) + l_ResultPartA - 1344109271, 29);
        v147 = v146 - v145;
        v148 = v143 + v145 + p_This->Key[1] - v147 + l_ResultPartB + 1432226185;
        l_ResultPartA = v147;
        v148 = __ROL4__(v148, 30);
        v149 = v145 ^ v148;
        v150 = __ROL4__(p_This->Key[2] + (v149 ^ (v145 | ~v147)) + v143 - 1744504997, 4);
        v151 = v145 + v150;
        v152 = __ROL4__(p_This->Key[0] + (v151 ^ (v147 | ~v149)) + v145 - 1992707294, 4);
        v153 = v152 - l_ResultPartA;
        v154 = __ROL4__(p_This->Key[3] + (v149 & v151 ^ v153 & (v149 ^ v151)) + l_ResultPartA + 981988726, 9);
        v155 = v154 - _byteswap_ulong(-v149);
        v156 = p_This->Key[0] + (v155 ^ v151 ^ v153);
        l_ResultPartA = v155;
        v157 = __ROL4__(v156 + v149 - 930831509, 13);
        l_ResultPartB = v151 + v157;
        v158 = v151 + v157;
        v159 = __ROL4__(p_This->Key[2] + (v155 ^ (v153 | ~(v151 + v157))) + v151 + 354549815, 28);
        v219 = v153 + v159;
        v160 = __ROL4__(ScrambleKey_UnkXorShifting(v155, v153 + v159, v158) + v153 + p_This->Key[3] - 980228899, 23);
        v161 = l_ResultPartA + v160;
        l_ResultPartD = v161;

        v219 = v161;
        l_ResultPartB = v163;

        v165 = p_This->Key[3];
        v166 = __ROL4__(v165 + (l_ResultPartB ^ v219 & (l_ResultPartB ^ l_ResultPartD)) + l_ResultPartA + 1602925481, 24);
        v167 = _byteswap_ulong(~v219) ^ v166;
        v168 = ScrambleKey_UnkXorShifting(l_ResultPartD, v167, v219);
        v169 = __ROL4__(v168 + v165 + l_ResultPartB - 1900654549, 31);
        v170 = l_ResultPartD + v169;
        v171 = __ROL4__(v165 + (v167 ^ (v170 | ~l_ResultPartD)) + v219 - 1925935449, 11);
        v172 = ~v170 ^ v171;
        l_ResultPartB = v170;
        v173 = ScrambleKey_UnkXorShifting(v170, v172, v167);
        v174 = __ROL4__(p_This->Key[1] + v173 + l_ResultPartD + 1445381881, 7);
        l_ResultPartD = l_ResultPartB + v174;
        v175 = __ROL4__(p_This->Key[0] + ScrambleKey_UnkXorShifting(v172, l_ResultPartB, l_ResultPartB + v174) + v167 + 109428117, 29);
        v176 = ~v172 ^ v175;
        l_ResultPartA = v176;
        v177 = ScrambleKey_UnkXorShifting(v176, l_ResultPartD, v172);
        v178 = __ROL4__(v177 + l_ResultPartB + p_This->Key[2] - 1217426128, 22);
        v179 = _byteswap_ulong(~v176) ^ v178;
        l_ResultPartB = v179;
        v180 = __ROL4__(p_This->Key[3] + (l_ResultPartA & v179 ^ l_ResultPartD & (v176 ^ v179)) + v172 - 1216826231, 17);
        v219 = ~v179 ^ v180;
        v181 = p_This->Key[0] + (v219 ^ v179 & (l_ResultPartA ^ v219));
        v182 = -v179;
        v183 = __ROL4__(v181 + l_ResultPartD + 1130335055, 20);
        l_ResultPartD = v182 ^ v183;

        l_ResultPartA = v182;
        v219 = v181;

        v186 = p_This->Key[2];
        v187 = __ROL4__(v186 + (v219 ^ l_ResultPartB & (v219 ^ l_ResultPartD)) + l_ResultPartA - 1067365145, 29);
        v188 = -l_ResultPartB ^ v187;
        l_ResultPartA = v188;
        v189 = ScrambleKey_UnkXorShifting(v188, l_ResultPartD, v219);
        v190 = __ROL4__(v189 + l_ResultPartB + v186 - 943834555, 21);
        v191 = v188 ^ v190;
        l_ResultPartB = v191;

        l_ResultPartD = v191;
        v219 = v193;
        v195 = __ROL4__(
            p_This->Key[2]
            + (l_ResultPartA & l_ResultPartB ^ l_ResultPartD & (l_ResultPartA ^ l_ResultPartB))
            + v219
            - 408343377,
            13);
        v196 = l_ResultPartD ^ v195;
        v197 = p_This->Key[0] + (v196 ^ l_ResultPartB & (l_ResultPartA ^ v196));
        v219 = v196;
        v198 = __ROL4__(v197 + l_ResultPartD + 1076531487, 10);
        v199 = v198 - l_ResultPartB;
        v200 = v196 ^ v199;
        v201 = p_This->Key[1] + (l_ResultPartB ^ v196 ^ v199);
        l_ResultPartD = v199;
        v202 = __ROL4__(v201 + l_ResultPartA - 496270228, 4);
        v203 = v199 + v202;
        v204 = p_This->Key[2] + (v203 ^ v200);
        l_ResultPartA = v203;
        v205 = __ROL4__(v204 + l_ResultPartB - 30260576, 24);
        v206 = ~v203;

        l_ResultPartB = v206 + v205;
        l_ResultPartD = v206;

        v219 = v199;

        v210 = __ROL4__(p_This->Key[3] + (l_ResultPartB ^ (l_ResultPartD | ~l_ResultPartA)) + v219 + 0x74B03359, 2);
        v211 = l_ResultPartD ^ v210;
        v219 = v211;

        l_ResultPartA = v211;
        l_ResultPartA += 0x31307474;
        l_ResultPartB = v209;
        l_ResultPartB -= 875575088;
        l_ResultPartC = v219 ^ 0x38;

        p_This->Key[0] += l_ResultPartA;
        p_This->Key[1] += l_ResultPartB;
        p_This->Key[2] += l_ResultPartC;
        p_This->Key[3] += l_ResultPartD

        _m_femms();

        return l_ResultPartD;
    }
}   ///< namespace Warden
}   ///< namespace World
}   ///< namespace RCN
