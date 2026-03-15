#include "core.hpp"
#include "game/game.hpp"
#include "glad/glad.h"
#include "maps/maps.hpp"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct GLFWwindow;
void setup_window( GLFWwindow* window );

int main( )
{
    glfwInit( );
    glfwWindowHint( GLFW_FLOATING, GLFW_TRUE );
    glfwWindowHint( GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE );
    glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
    glfwWindowHint( GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE );
    glfwWindowHint( GLFW_FOCUS_ON_SHOW, GLFW_FALSE );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );

    auto* mon = glfwGetPrimaryMonitor( );
    auto* mode = glfwGetVideoMode( mon );

    GLFWwindow* window = glfwCreateWindow( mode->width, mode->height,
                                           "Spotify", nullptr, nullptr );
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );

    setup_window( window );

    gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress );

    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGui::GetIO( ).IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init( "#version 150" );

    alice::g = std::make_unique< alice::CCore >( );
    alice::g->entry( );

    while ( !glfwWindowShouldClose( window ) )
    {
        glfwPollEvents( );

        ImGui_ImplOpenGL3_NewFrame( );
        ImGui_ImplGlfw_NewFrame( );
        ImGui::NewFrame( );

        alice::g->game->updateMapData( );
        alice::g->game->updatePlayersData( );
        alice::g->game->renderRadar( );

        ImGui::Render( );
        glViewport( 0, 0, mode->width, mode->height );
        glClearColor( 0.f, 0.f, 0.f, 0.f );
        glClear( GL_COLOR_BUFFER_BIT );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData( ) );
        glfwSwapBuffers( window );
    }

    ImGui_ImplOpenGL3_Shutdown( );
    ImGui_ImplGlfw_Shutdown( );
    ImGui::DestroyContext( );
    glfwDestroyWindow( window );
    glfwTerminate( );
}