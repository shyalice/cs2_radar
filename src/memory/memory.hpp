//
// Created by Kai Tears on 15/03/2026.
//

#ifndef CS2_RADAR_CROSSOVER_MEMORY_HPP
#define CS2_RADAR_CROSSOVER_MEMORY_HPP

#include <iostream>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <string>

namespace alice
{
    class CMemory
    {
        pid_t m_pid { -1 };
        uintptr_t m_clientDLL { 0 };
        task_t m_task { 0 };

        static pid_t getTargetPID( const std::string& target );

    public:
        CMemory( const std::string& name );
        ~CMemory( );

        uintptr_t getModuleBase( const std::string& module_name );

        uintptr_t getClientDLL( );
        pid_t getPID( );

        template < typename T >
        T read( uintptr_t address )
        {
            T value { };
            mach_vm_size_t bytes_read = 0;
            kern_return_t const kr = mach_vm_read_overwrite( m_task,
                                                             ( mach_vm_address_t ) address,
                                                             sizeof( T ),
                                                             ( mach_vm_address_t ) &value,
                                                             &bytes_read );

            if ( kr != KERN_SUCCESS || bytes_read != sizeof( T ) )
                std::cerr << "[x] failed to read at: " << std::hex << address << std::endl;

            return value;
        }
    };
} // namespace alice

#endif // CS2_RADAR_CROSSOVER_MEMORY_HPP
