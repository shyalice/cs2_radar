#import <Cocoa/Cocoa.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

void setup_window( GLFWwindow* window )
{
    NSWindow* nswin = glfwGetCocoaWindow( window );
    [nswin setLevel: NSScreenSaverWindowLevel];
    [nswin setIgnoresMouseEvents: YES];
    [nswin setCollectionBehavior:
        NSWindowCollectionBehaviorCanJoinAllSpaces    |
        NSWindowCollectionBehaviorStationary          |
        NSWindowCollectionBehaviorFullScreenAuxiliary ];
}