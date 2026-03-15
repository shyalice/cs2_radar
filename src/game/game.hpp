//
// Created by Kai Tears on 15/03/2026.
//

#ifndef CS2_RADAR_CROSSOVER_GAME_HPP
#define CS2_RADAR_CROSSOVER_GAME_HPP

#include "common.hpp"

#include <array>
#include <cstdint>

namespace alice
{
    class CGame
    {
    public:
        CGame( );
        ~CGame( );

        uintptr_t localPlayerPawn;

        int playersCount;
        std::array< PlayerData, 64 > playersList;

        void updateMapData( );
        void updatePlayersData( );

        void renderRadar( );
    };
} // namespace alice

#endif // CS2_RADAR_CROSSOVER_GAME_HPP
