//
// Created by Kai Tears on 15/03/2026.
//

#include "game.hpp"

#include "common.hpp"
#include "core.hpp"
#include "offsets.hpp"
#include "memory/memory.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "maps/maps.hpp"

using alice::CGame;

CGame::CGame( ) = default;
CGame::~CGame( ) = default;

void CGame::updateMapData( )
{
    // shitcode but idk lowkey
    auto const ret_name = [ & ]( ) -> void {
        g->currentMapName = "<empty>";
    };

    auto const gv = g->memory->read< uintptr_t >( g->memory->getClientDLL( ) + offsets::client_dll::dwGlobalVars );
    if ( !gv )
        return ret_name( );

    auto const map_ptr = g->memory->read< uintptr_t >( gv + 0x188 );
    if ( !map_ptr )
        return ret_name( );

    auto buf = g->memory->read< std::array< char, 32 > >( map_ptr );
    buf[ 31 ] = '\0';
    std::string full( buf.data( ) );
    size_t const sl = full.rfind( '/' );
    if ( sl != std::string::npos )
        full = full.substr( sl + 1 );
    size_t const dot = full.rfind( '.' );
    if ( dot != std::string::npos )
        full = full.substr( 0, dot );

    char ret[ 100 ] { };
    strncpy( ret, full.c_str( ), 31 );
    g->currentMapName = ret;
}

void CGame::updatePlayersData( )
{
    playersCount = 0;
    playersList = { };

    auto const client = g->memory->getClientDLL( );
    if ( !client )
        return;

    localPlayerPawn = g->memory->read< uintptr_t >( client + offsets::client_dll::dwLocalPlayerPawn );

    auto const entityList = g->memory->read< uintptr_t >( client + offsets::client_dll::dwEntityList );

    for ( int id = 0; id < 64; id++ )
    {
        int const entryIndex = ( id & 0x7FFF ) >> 9;
        auto const listEntry = g->memory->read< uintptr_t >( entityList + 8 * entryIndex + 16 );
        if ( !listEntry )
            continue;
        auto const controller = g->memory->read< uintptr_t >( listEntry + 112 * ( id & 0x1FF ) );
        if ( !controller )
            continue;

        auto const playerPawn = g->memory->read< int32_t >( controller + offsets::client_dll::m_hPlayerPawn ); // m_hPawn
        if ( !playerPawn )
            continue;
        int const pawnIndex = ( playerPawn & 0x7FFF ) >> 9;
        auto const listEntry2 = g->memory->read< uintptr_t >( entityList + 0x8 * pawnIndex + 16 );
        if ( !listEntry2 )
            continue;
        auto const pawn = g->memory->read< uintptr_t >( listEntry2 + 112 * ( playerPawn & 0x1FF ) );
        if ( !pawn )
            continue;

        auto const health = g->memory->read< int32_t >( pawn + offsets::client_dll::m_iHealth );
        auto const team = g->memory->read< int32_t >( pawn + offsets::client_dll::m_iTeamNum );

        auto const position = g->memory->read< Vector3 >( pawn + offsets::client_dll::m_vOldOrigin );

        bool const self = ( pawn == localPlayerPawn );
        bool const dead = ( health <= 0 || health > 100 );

        if ( dead )
            continue;

        playersList[ playersCount++ ] = { health, team, self, position };
    }
}

void CGame::renderRadar( )
{
    auto* draw = ImGui::GetBackgroundDrawList( );

    float const radar_size = 300.f;
    float const padding = 20.f;
    ImVec2 const tl = { padding, padding };
    ImVec2 const br = { padding + radar_size, padding + radar_size };

    auto const data = g->maps->get( g->currentMapName );

    draw->AddRectFilled( tl, br, IM_COL32( 0, 0, 0, 160 ) );

    if ( data.texture )
        draw->AddImage( data.texture, tl, br );

    draw->AddRect( tl, br, IM_COL32( 255, 255, 255, 60 ) );

    for ( int i = 0; i < playersCount; i++ )
    {
        auto const& p = playersList[ i ];

        ImVec2 const rp = data.worldToRadar( p.pos );
        float const scale = radar_size / 1024.f;

        ImVec2 const dot = {
            tl.x + rp.x * scale,
            tl.y + rp.y * scale };

        ImU32 col;
        if ( p.self )
            col = ImColor( 0, 255, 0, 255 );
        else if ( p.team == 2 )
            col = ImColor( 255, 180, 0, 255 );
        else
            col = ImColor( 100, 180, 255, 255 );

        draw->AddCircleFilled( dot, 5.f, col );
        draw->AddCircle( dot, 5.f, ImColor( 0, 0, 0, 180 ) );
    }
}