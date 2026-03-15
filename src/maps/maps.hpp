//
// Created by Kai Tears on 15/03/2026.
//

#ifndef CS2_RADAR_CROSSOVER_MAPS_HPP
#define CS2_RADAR_CROSSOVER_MAPS_HPP

#include "common.hpp"
#include "imgui.h"

#include <map>
#include <string>
#include <unordered_map>

namespace alice
{
    struct MapData
    {
        ImTextureID texture;
        int width;
        int height;
        Vector3 coords;

        ImVec2 worldToRadar( const Vector3& ent ) const;
    };

    class CMaps
    {
        static std::unordered_map< std::string, MapData > m_list;

        std::map< std::string, Vector3 > m_coordsList = {
            { "cs_italy", { -2647.f, 2592.f, 4.6f } },
            { "cs_office", { -1838.f, 1858.f, 4.1f } },
            { "de_ancient", { -2953.f, 2164.f, 5.f } },
            { "de_anubis", { -2796.f, 3328.f, 5.22f } },
            { "de_dust", { -2850.f, 4073.f, 6.f } },
            { "de_dust2", { -2476.f, 3239.f, 4.4f } },
            { "de_inferno", { -2087.f, 3870.f, 4.9f } },
            { "de_mirage", { -3230.f, 1713.f, 5.f } },
            { "de_nuke", { -3453.f, 2887.f, 7.f } },
            { "de_overpass", { -4831.f, 1781.f, 5.2f } },
            { "de_vertigo", { -3168.f, 1762.f, 4.f } },
            { "<empty>", { 0.f, 0.f, 1.f } } };

        static Vector3 parseCoords( const std::string& txt_path );

    public:
        CMaps( );
        ~CMaps( );

        MapData get( const std::string& key );
        void loadFrom( const std::string& directory );
    };
} // namespace alice

#endif // CS2_RADAR_CROSSOVER_MAPS_HPP
