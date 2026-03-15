//
// Created by Kai Tears on 15/03/2026.
//

#include "memory.hpp"

#include <iostream>
#include <libproc.h>
#include <thread>
#include <sys/proc_info.h>
#include <sys/sysctl.h>

using alice::CMemory;

pid_t CMemory::getTargetPID( const std::string& target )
{
    int mib[ 4 ] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };
    size_t size = 0;

    sysctl( mib, 4, NULL, &size, NULL, 0 );

    auto* procs = static_cast< kinfo_proc* >( malloc( size ) );
    sysctl( mib, 4, procs, &size, NULL, 0 );

    int const count = size / sizeof( struct kinfo_proc );
    for ( int i = 0; i < count; i++ )
    {
        if ( strstr( procs[ i ].kp_proc.p_comm, target.c_str( ) ) )
        {
            pid_t const pid = procs[ i ].kp_proc.p_pid;
            free( procs );
            return pid;
        }
    }

    free( procs );
    return 0;
}

uintptr_t CMemory::getModuleBase( const std::string& module_name )
{
    if ( !m_pid )
        return 0;

    task_t task;
    if ( task_for_pid( mach_task_self( ), m_pid, &task ) != KERN_SUCCESS )
        return 0;

    mach_vm_address_t addr = 0;
    uintptr_t result = 0;

    while ( 1 )
    {
        mach_vm_size_t size = 0;
        vm_region_basic_info_data_64_t info;
        mach_msg_type_number_t info_count = VM_REGION_BASIC_INFO_COUNT_64;
        mach_port_t obj = MACH_PORT_NULL;

        if ( mach_vm_region( task, &addr, &size, VM_REGION_BASIC_INFO_64,
                             ( vm_region_info_t ) &info, &info_count, &obj ) != KERN_SUCCESS )
            break;

        char path[ PROC_PIDPATHINFO_MAXSIZE ] = { 0 };
        proc_regionfilename( m_pid, addr, path, sizeof( path ) );

        if ( strstr( path, module_name.c_str( ) ) && ( info.protection & VM_PROT_READ ) )
        {
            uint8_t magic[ 2 ] = { 0 };
            mach_vm_size_t bytes_read = 0;
            mach_vm_read_overwrite( task, addr, 2,
                                    ( mach_vm_address_t ) magic, &bytes_read );

            if ( magic[ 0 ] == 'M' && magic[ 1 ] == 'Z' )
            {
                result = ( uintptr_t ) addr;
                break;
            }
        }

        addr += size;
    }

    mach_port_deallocate( mach_task_self( ), task );
    return result;
}

uintptr_t CMemory::getClientDLL( )
{
    return m_clientDLL;
}

pid_t CMemory::getPID( )
{
    return m_pid;
}

CMemory::~CMemory( )
{
    if ( m_task )
        mach_port_deallocate( mach_task_self( ), m_task );

    std::cout << "[i] bye!" << std::endl;
}

CMemory::CMemory( const std::string& name )
{
    do
    {
        std::cout << "[i] trying to find cs2.exe...\n";

        auto const pid = getTargetPID( name );

        if ( pid < 1 )
            std::this_thread::sleep_for( std::chrono::seconds( 10 ) );
        else
            m_pid = pid;
    }
    while ( m_pid < 1 );

    std::cout << "[i] cs2.exe: " << m_pid << "\n";

    do
    {
        std::cout << "[i] trying to find client.dll...\n";

        auto const dll = getModuleBase( "client.dll" );

        if ( !dll )
            std::this_thread::sleep_for( std::chrono::seconds( 10 ) );
        else
            m_clientDLL = dll;
    }
    while ( !m_clientDLL );

    std::cout << "[i] client.dll start: " << std::hex << m_clientDLL << std::endl;

    if ( task_for_pid( mach_task_self( ), m_pid, &m_task ) != KERN_SUCCESS )
    {
        std::cerr << "[x] task_for_pid failed" << std::endl;
        return;
    }
}