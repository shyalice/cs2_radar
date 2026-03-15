//
// Created by Kai Tears on 15/03/2026.
//

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#include "maps.hpp"

#include "glad/glad.h"

#include <filesystem>
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

        if ( !m_coordsList.contains( name ) )
            continue;

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