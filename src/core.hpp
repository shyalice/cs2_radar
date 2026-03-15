//
// Created by Kai Tears on 15/03/2026.
//

#ifndef CS2_RADAR_CROSSOVER_CORE_HPP
#define CS2_RADAR_CROSSOVER_CORE_HPP

#include <memory>
#include <string>

namespace alice
{
    class CMemory;
    class CMaps;
    class CGame;

    class CCore
    {
    public:
        ~CCore( );
        CCore( );

        std::unique_ptr< CMemory > memory;
        std::unique_ptr< CGame > game;
        std::unique_ptr< CMaps > maps;

        std::string currentMapName { };

        void entry( );
    };

    extern std::unique_ptr< CCore > g;
} // namespace alice

#endif // CS2_RADAR_CROSSOVER_CORE_HPP
