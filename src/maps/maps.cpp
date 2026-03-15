//
// Created by Kai Tears on 15/03/2026.
//

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include "maps.hpp"

#include "glad/glad.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using alice::CMaps;
using alice::MapData;

ImVec2 MapData::worldToRadar( const Vector3& ent ) const
{
    float const xPos = abs( ent.x - coords.x ) / coords.z - 10;
    float const yPos = abs( ent.y - coords.y ) / coords.z - 10;

    return { xPos, yPos };
}

std::unordered_map< std::string, MapData > CMaps::m_list { };

CMaps::CMaps( ) = default;
CMaps::~CMaps( ) = default;

alice::Vector3 CMaps::parseCoords( const std::string& txt_path )
{
    Vector3 coords { 0.f, 0.f, 1.f };

    std::ifstream f( txt_path );
    if ( !f.is_open( ) )
        return coords;

    std::string line;
    while ( std::getline( f, line ) )
    {
        auto parse_val = []( const std::string& l, const std::string& key ) -> std::optional< float > {
            size_t const k = l.find( key );
            if ( k == std::string::npos )
                return std::nullopt;
            size_t const q1 = l.find( '"', k + key.size( ) );
            if ( q1 == std::string::npos )
                return std::nullopt;
            size_t const q2 = l.find( '"', q1 + 1 );
            if ( q2 == std::string::npos )
                return std::nullopt;
            return std::stof( l.substr( q1 + 1, q2 - q1 - 1 ) );
        };

        if ( auto v = parse_val( line, "\"pos_x\"" ) )
            coords.x = *v;
        if ( auto v = parse_val( line, "\"pos_y\"" ) )
            coords.y = *v;
        if ( auto v = parse_val( line, "\"scale\"" ) )
            coords.z = *v;
    }

    return coords;
}

MapData CMaps::get( const std::string& key )
{
    auto data = m_list[ key ];
    data.coords = m_coordsList[ key ];
    return data;
}

void CMaps::loadFrom( const std::string& directory )
{
    for ( auto const& entry : std::filesystem::directory_iterator( directory ) )
    {
        std::cout << "[i] loading: " << entry << std::endl;

        if ( entry.path( ).extension( ) != ".png" )
            continue;

        std::string const name = entry.path( ).stem( ).string( );
        std::string const txt_path = directory + "/" + name + ".txt";
        Vector3 coords { 0.f, 0.f, 1.f };

        if ( std::filesystem::exists( txt_path ) )
        {
            coords = parseCoords( txt_path );
            std::cout << "[i] coords for " << name
                      << ": " << coords.x << " " << coords.y << " " << coords.z << std::endl;
        } else if ( m_coordsList.contains( name ) )
        {
            coords = m_coordsList[ name ];
        } else
        {
            std::cerr << "[x] no coords for: " << name << std::endl;
            continue;
        }

        int width, height, channels;
        uint8_t* data = stbi_load( entry.path( ).string( ).c_str( ), &width, &height, &channels, 4 );
        if ( !data )
        {
            std::cerr << "[x] failed to load: " << entry.path( ) << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                      GL_RGBA, GL_UNSIGNED_BYTE, data );
        stbi_image_free( data );

        MapData md;
        md.texture = ( ImTextureID ) ( uintptr_t ) texture;
        md.width = width;
        md.height = height;
        md.coords = m_coordsList[ name ];
        m_list[ name ] = md;

        std::cout << "[i] loaded map: " << name << std::endl;
    }
}