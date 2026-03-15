//
// Created by Kai Tears on 15/03/2026.
//

#include "core.hpp"

#include "common.hpp"
#include "offsets.hpp"
#include "memory/memory.hpp"
#include "game/game.hpp"
#include "maps/maps.hpp"

#include <iostream>
#include <ostream>

using alice::CMemory;

using alice::CCore;

std::unique_ptr< CCore > alice::g { nullptr };

CCore::~CCore( ) = default;

CCore::CCore( )
{
    game = std::make_unique< CGame >( );
    maps = std::make_unique< CMaps >( );
}

void CCore::entry( )
{
    std::cout << "[i] shyalice/cs2_radar <--> alina.pw @evernesseu" << std::endl;

    memory = std::make_unique< CMemory >( "cs2.exe" );

    maps->loadFrom( "/Users/rei/cs2_radar_images/" );
}