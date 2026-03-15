//
// Created by Kai Tears on 15/03/2026.
//

#ifndef CS2_RADAR_CROSSOVER_COMMON_HPP
#define CS2_RADAR_CROSSOVER_COMMON_HPP

#include <cstdint>

namespace alice
{
    struct Vector3
    {
        float x, y, z;
    };

    struct PlayerData
    {
        int32_t health;
        int32_t team;
        bool self;

        Vector3 pos;
    };
} // namespace alice

#endif // CS2_RADAR_CROSSOVER_COMMON_HPP
