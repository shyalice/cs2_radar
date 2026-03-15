//
// Created by Kai Tears on 15/03/2026.
//

#ifndef CS2_RADAR_CROSSOVER_OFFSETS_HPP
#define CS2_RADAR_CROSSOVER_OFFSETS_HPP

#include <cstddef>

namespace alice::offsets
{
    namespace client_dll
    {
        constexpr std::ptrdiff_t m_hPlayerPawn = 0x90C;
        constexpr std::ptrdiff_t m_iszPlayerName = 0x6F8;
        constexpr std::ptrdiff_t m_iTeamNum = 0x3F3;
        constexpr std::ptrdiff_t m_iHealth = 0x354;
        constexpr std::ptrdiff_t m_vOldOrigin = 0x1588;

        constexpr std::ptrdiff_t dwCSGOInput = 0x2318FC0;
        constexpr std::ptrdiff_t dwEntityList = 0x24AE268;
        constexpr std::ptrdiff_t dwGameEntitySystem = 0x24AE268;
        constexpr std::ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x20A0;
        constexpr std::ptrdiff_t dwGameRules = 0x230CEE0;
        constexpr std::ptrdiff_t dwGlobalVars = 0x205D5C0;
        constexpr std::ptrdiff_t dwGlowManager = 0x2309CE8;
        constexpr std::ptrdiff_t dwLocalPlayerController = 0x22F3178;
        constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x2068B60;
        constexpr std::ptrdiff_t dwPlantedC4 = 0x2316740;
        constexpr std::ptrdiff_t dwPrediction = 0x2068A70;
        constexpr std::ptrdiff_t dwSensitivity = 0x230A7F8;
        constexpr std::ptrdiff_t dwSensitivity_sensitivity = 0x58;
        constexpr std::ptrdiff_t dwViewAngles = 0x2319648;
        constexpr std::ptrdiff_t dwViewMatrix = 0x230EF20;
        constexpr std::ptrdiff_t dwViewRender = 0x230F338;
        constexpr std::ptrdiff_t dwWeaponC4 = 0x2298380;
    }
}

#endif // CS2_RADAR_CROSSOVER_OFFSETS_HPP
