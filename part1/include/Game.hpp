#ifndef SDLGRAPHICSPROGRAM_HPP
#define SDLGRAPHICSPROGRAM_HPP

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>
#include <OBJReader.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


// Purpose:
// This class sets up a full graphics program using SDL
//
//
//
class Game{
public:

    // Constructor
    Game(int w, int h, OBJReader &myReader);
    // Desctructor
    ~Game();
    // Setup OpenGL
    bool InitGL();
    // Per frame update
    void Update();
    // Renders shapes to the screen
    void Render();
    // loop that runs forever
    void Loop();
    // Get Pointer to Window
    SDL_Window* GetSDLWindow();
    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();
    //Init Game
    void GameInit();
    // Match board pieces and chess pieces. i.e. mark board pieces with chess pieces
    // As having chess pieces.
    void MarkBoard();

private:
    // Screen dimension constants
    int m_screenHeight;
    int m_screenWidth;
    // The window we'll be rendering to
    SDL_Window* m_window ;
    // OpenGL context
    SDL_GLContext m_openGLContext;
};

#endif
