#include "Game.hpp"
#include "GameObjectManager.hpp"
#include "Camera.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
Game::Game(int w, int h, OBJReader &myReader)
{
    m_screenHeight = h;
    m_screenWidth = w;
    // Initialization flag
    bool success = true;
    // String to hold any errors that occur.
    std::stringstream errorStream;
    // The window we'll be rendering to
    m_window = NULL;
    // Render flag

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else
    {
        // Use OpenGL 3.3 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        // We want to request a double buffer for smooth updating.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create window
        m_window = SDL_CreateWindow("Lab",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    m_screenWidth,
                                    m_screenHeight,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        // Check if Window did not create.
        if (m_window == NULL)
        {
            errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Create an OpenGL Graphics Context
        m_openGLContext = SDL_GL_CreateContext(m_window);
        if (m_openGLContext == NULL)
        {
            errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Initialize GLAD Library
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            errorStream << "Failed to iniitalize GLAD\n";
            success = false;
        }

        // Initialize OpenGL
        if (!InitGL())
        {
            errorStream << "Unable to initialize OpenGL!\n";
            success = false;
        }
    }

    // If initialization did not work, then print out a list of errors in the constructor.
    if (!success)
    {
        errorStream << "Game::Game - Failed to initialize!\n";
        std::string errors = errorStream.str();
        SDL_Log("%s\n", errors.c_str());
    }
    else
    {
        SDL_Log("Game::Game - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

    // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
    GetOpenGLVersionInfo();

    //Initialize game
    GameInit();

}

// Proper shutdown of SDL and destroy initialized objects
Game::~Game()
{
    // Reclaim all of our objects
    GameObjectManager::Instance().RemoveAll();

    // Destroy window
    SDL_DestroyWindow(m_window);
    // Point m_window to NULL to ensure it points to nothing.
    m_window = nullptr;
    // Quit SDL subsystems
    SDL_Quit();
}

// Set up board and chess pieces
void Game::GameInit() {

    // Read Obj files for pieces
    OBJReader queen = OBJReader("./../common/objects/Queen/Queen.obj");
    OBJReader castle = OBJReader("./../common/objects/Castle/castle.obj");
    OBJReader knight = OBJReader("./../common/objects/knight/Knight.obj");
    OBJReader king = OBJReader("./../common/objects/king/king.obj");
    OBJReader pawn = OBJReader("./../common/objects/pawn/pawn.obj");
    OBJReader bishop = OBJReader("./../common/objects/bishop/bishop.obj");

    // Game objects
    GameObject *queen_obj;
    GameObject *castle_obj;
    GameObject *knight_obj;
    GameObject *king_obj;
    GameObject *bishop_obj;
    GameObject *pawn_obj;


    int pawn_board_id = 8; 
    int other_piece_id = 1;
    int start_pt = 0; // start_pt for managing piece in the GameObjectManager
    int id_start = 0;
    float xstart = 5.0f; // starting point of chess piece drawing in the x
    float diff = 0.75; // dimension of each board piece

    // Texture for marble pieces
    std::string normalFileName = "./../common/objects/white_texture/whiteNormal.ppm";
    std::string colorFileName = "./../common/objects/marble_texture/Rock035_1K_Color.ppm";

    // Make pieces
    for (int i = 0; i < 2; i++)
    {
        if (i == 1)
        {
            start_pt = 16;
            // Texture for white pieces.
            normalFileName = "./../common/objects/white_texture/whiteNormal.ppm";
            colorFileName = "./../common/objects/white_texture/white.ppm";
            xstart = -0.25;
            diff = -0.75;
            id_start = 170;
            other_piece_id = 57;
        }
        

        queen_obj = new GameObject;
        queen_obj->MakeTexturedGameObject(colorFileName, normalFileName, queen.vertexTextures, queen.indexBuffer, id_start + 1);
        GameObjectManager::Instance().AddGameObject(queen_obj);

        castle_obj = new GameObject;
        castle_obj->MakeTexturedGameObject(colorFileName, normalFileName, castle.vertexTextures, castle.indexBuffer, id_start + 2);
        GameObjectManager::Instance().AddGameObject(castle_obj);

        knight_obj = new GameObject;
        knight_obj->MakeTexturedGameObject(colorFileName, normalFileName, knight.vertexTextures, knight.indexBuffer, id_start + 3);
        GameObjectManager::Instance().AddGameObject(knight_obj);

        king_obj = new GameObject;
        king_obj->MakeTexturedGameObject(colorFileName, normalFileName, king.vertexTextures, king.indexBuffer, id_start + 4);
        GameObjectManager::Instance().AddGameObject(king_obj);

        bishop_obj = new GameObject;
        bishop_obj->MakeTexturedGameObject(colorFileName, normalFileName, bishop.vertexTextures, bishop.indexBuffer, id_start + 5);
        GameObjectManager::Instance().AddGameObject(bishop_obj);


        GameObjectManager::Instance().GetGameObject(start_pt + 0).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 0).setLocation(xstart, 0.0f, -2.25f);
        GameObjectManager::Instance().GetGameObject(start_pt + 0).GetTransform().Translate(xstart, 0.0f, -2.25f);
        GameObjectManager::Instance().GetGameObject(start_pt + 0).GetTransform().Scale(0.5f, 0.3f, 0.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 0).setBoardId(other_piece_id + 3);

        GameObjectManager::Instance().GetGameObject(start_pt + 1).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 1).setLocation(xstart, 0.0f, 0.0f);
        GameObjectManager::Instance().GetGameObject(start_pt + 1).GetTransform().Translate(xstart, 0.0f, 0.0f);
        GameObjectManager::Instance().GetGameObject(start_pt + 1).GetTransform().Scale(0.5f, 0.5f, 0.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 1).setBoardId(other_piece_id + 0);

        GameObjectManager::Instance().GetGameObject(start_pt + 2).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 2).setLocation(xstart, 0.4f, -0.75f);
        GameObjectManager::Instance().GetGameObject(start_pt + 2).GetTransform().Translate(xstart, 0.4f, -0.75f);
        GameObjectManager::Instance().GetGameObject(start_pt + 2).GetTransform().Scale(0.25f, 0.25f, 0.25f);
        GameObjectManager::Instance().GetGameObject(start_pt + 2).setBoardId(other_piece_id + 1);

        GameObjectManager::Instance().GetGameObject(start_pt + 3).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 3).setLocation(xstart, 0.0f, -3.0f);
        GameObjectManager::Instance().GetGameObject(start_pt + 3).GetTransform().Translate(xstart, 0.0f, -3.0f);
        GameObjectManager::Instance().GetGameObject(start_pt + 3).GetTransform().Scale(0.5f, 0.3f, 0.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 3).setBoardId(other_piece_id + 4);

        GameObjectManager::Instance().GetGameObject(start_pt + 4).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 4).setLocation(xstart, 0.0f, -1.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 4).GetTransform().Translate(xstart, 0.0f, -1.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 4).GetTransform().Scale(0.5f, 0.5f, 0.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 4).setBoardId(other_piece_id + 2);

        
        castle_obj = new GameObject;
        castle_obj->MakeTexturedGameObject(colorFileName, normalFileName, castle.vertexTextures, castle.indexBuffer, id_start + 6);
        GameObjectManager::Instance().AddGameObject(castle_obj);

        knight_obj = new GameObject;
        knight_obj->MakeTexturedGameObject(colorFileName, normalFileName, knight.vertexTextures, knight.indexBuffer, id_start + 7);
        GameObjectManager::Instance().AddGameObject(knight_obj);

        bishop_obj = new GameObject;
        bishop_obj->MakeTexturedGameObject(colorFileName, normalFileName, bishop.vertexTextures, bishop.indexBuffer, id_start + 8);
        GameObjectManager::Instance().AddGameObject(bishop_obj);

        GameObjectManager::Instance().GetGameObject(start_pt + 5).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 5).setLocation(xstart, 0.0f, -5.25f);
        GameObjectManager::Instance().GetGameObject(start_pt + 5).GetTransform().Translate(xstart, 0.0f, -5.25f);
        GameObjectManager::Instance().GetGameObject(start_pt + 5).GetTransform().Scale(0.5f, 0.5f, 0.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 5).setBoardId(other_piece_id + 7);

        GameObjectManager::Instance().GetGameObject(start_pt + 6).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 6).setLocation(xstart, 0.4f, -4.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 6).GetTransform().Translate(xstart, 0.4f, -4.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 6).GetTransform().Scale(0.25f, 0.25f, 0.25f);
        GameObjectManager::Instance().GetGameObject(start_pt + 6).setBoardId(other_piece_id + 6);

        GameObjectManager::Instance().GetGameObject(start_pt + 7).GetTransform().LoadIdentity();
        GameObjectManager::Instance().GetGameObject(start_pt + 7).setLocation(xstart, 0.0f, -3.75f);
        GameObjectManager::Instance().GetGameObject(start_pt + 7).GetTransform().Translate(xstart, 0.0f, -3.75f);
        GameObjectManager::Instance().GetGameObject(start_pt + 7).GetTransform().Scale(0.5f, 0.5f, 0.5f);
        GameObjectManager::Instance().GetGameObject(start_pt + 7).setBoardId(other_piece_id + 5);

        // Generate Pawns.
        for (int j = 0; j < 8; j++)
        {
            pawn_obj = new GameObject;
            pawn_obj->MakeTexturedGameObject(colorFileName, normalFileName, pawn.vertexTextures, pawn.indexBuffer, id_start + 9 + j);
            GameObjectManager::Instance().AddGameObject(pawn_obj);

            GameObjectManager::Instance().GetGameObject(start_pt + 8 + j).GetTransform().LoadIdentity();
            GameObjectManager::Instance().GetGameObject(start_pt + 8 + j).setLocation(xstart - diff, 0.0f, 0.0f - (0.75 * j));
            GameObjectManager::Instance().GetGameObject(start_pt + 8 + j).GetTransform().Translate(xstart - diff, 0.0f, 0.0f - (0.75 * j));
            GameObjectManager::Instance().GetGameObject(start_pt + 8 + j).GetTransform().Scale(0.5f, 0.5f, 0.5f);

            if (i == 0) {
                pawn_board_id = 9;
            } else {
                pawn_board_id = 49;
            }
            GameObjectManager::Instance().GetGameObject(start_pt + 8 + j).setBoardId(pawn_board_id + j);
            
        }
    }

    // Make up board squares.
    GameObject *board;
    int counter1 = 0;
    int counter2= 0;
    int total = 1;
    float boardx = 5.0f;
    float boardz = 0.0f;
    for (int i =0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i % 2 == j % 2) {
                board = new GameObject;
                board->MakeTexturedQuad("./../common/objects/marble_texture/Rock035_1K_Color.ppm", 17+counter1);
                GameObjectManager::Instance().AddGameObject(board);
                GameObjectManager::Instance().GetGameObject(31 + total).GetTransform().LoadIdentity();
                GameObjectManager::Instance().GetGameObject(31 + total).setLocation(boardx, 0.0f, boardz);
                GameObjectManager::Instance().GetGameObject(31 + total).GetTransform().Translate(boardx, 0.0f, boardz);
                counter1++;
            } else {
                board = new GameObject;
                board->MakeTexturedQuad("./../common/objects/white_texture/white.ppm", 100+counter2);
                GameObjectManager::Instance().AddGameObject(board);
                GameObjectManager::Instance().GetGameObject(31 + total).GetTransform().LoadIdentity();
                GameObjectManager::Instance().GetGameObject(31 + total).setLocation(boardx, 0.0f, boardz);
                GameObjectManager::Instance().GetGameObject(31 + total).GetTransform().Translate(boardx, 0.0f, boardz);
                counter2++;
            }
            boardz -= 0.75;
            total += 1;
            

        }
        boardx -= 0.75f;
        boardz = 0.0f;
    }
    MarkBoard();
}

// Update board pieces with board IDs and existence of chess pieces.
// Update chess pieces with board Ids they're currently on.
void Game::MarkBoard() {
    for ( int i = 32; i < 96; i++) {
        if (i < 48 || i >= 80) {
            GameObjectManager::Instance().GetGameObject(i).boardSetter(true);
        } else {
            GameObjectManager::Instance().GetGameObject(i).boardSetter(false);
        }
        GameObjectManager::Instance().GetGameObject(i).setBoardId(i - 31);
    }
}

// Initialize OpenGL
// Setup any of our shaders here.
bool Game::InitGL()
{
    // Success flag
    bool success = true;

    return success;
}

// Update OpenGL
void Game::Update()
{
    // Rotate brick wall
    static float rot = 0;
    rot += 0.01;
    if (rot > 360)
    {
        rot = 0;
    }

    // Here we hard-code a giant scene
    // Yuck, we'll fix this in a future assignment.
    // GameObjectManager::Instance().GetGameObject(0).GetTransform().LoadIdentity();
    // Push back our wall a bit
    // GameObjectManager::Instance().GetGameObject(0).GetTransform().Translate(0.0f,0.0f,-8.0f);
    // Rotate on y-axis
    // GameObjectManager::Instance().GetGameObject(0).GetTransform().Rotate(rot,0.0f,1.0f,0.0f);
    // Make our wall a little bigger
    // GameObjectManager::Instance().GetGameObject(0).GetTransform().Scale(2.0f,2.0f,2.0f);

    // Update all of the objects
    GameObjectManager::Instance().UpdateAll(m_screenWidth, m_screenHeight);
}

// Render
// The render function gets called once per loop
void Game::Render()
{
    // Setup our OpenGL State machine
    // TODO: Read this
    // The below command is new!
    // What we are doing, is telling opengl to create a depth(or Z-buffer)
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    // TODO: Read this
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Nice way to debug your scene in wireframe!
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Render all of our objects in a simple loop
    GameObjectManager::Instance().RenderAll();

    // Delay to slow things down just a bit!
    SDL_Delay(50);
}

// Convert Red component of RGB value of objects to an object id.
// temp is the new Game object ID.
void getClickedGameObjectId (int &temp, int col, int& delta_black, int& delta_white ) {
    if (col < -124 && col > -129) {
        col  = 256 + col;
    }
    // Marble chess pieces
    if (col < 17 && col >= 0) {
        temp = col - 1;
    } 
    // White chess pieces
    else if (col < -69 && col > -86) {
        temp = 255 + col - 154;
    }
    //Marble Board Pieces
    else if (col < 49 && col > 16) {
        if (((col-17) - (col -17) % 4) % 8 == 0) {
            delta_black = 32;
        } else {
            delta_black = 33;
        }
        
        temp = delta_black + (col - 17) * 2;

    } 
    // White board pieces
    else if (col > 99 && col < 132) {
        
        if (((col-100) - (col - 100) % 4) % 8 == 0) {
            delta_white = 33;
        } else {
            delta_white = 32;
        }
        
        temp = delta_white + (col - 100) * 2;
    }
}

// Process a move 
void process_move_to_board(int& currentPieceId, int newPieceId, int &turn, bool &selected, int board) {
    // New piece: Destination board piece or chess piece to be removed
    // Current Piece: The chess piece that is making the move
    GameObject& newPiece = GameObjectManager::Instance().GetGameObject(newPieceId);
    GameObject& currentPiece = GameObjectManager::Instance().GetGameObject(currentPieceId);
    float destX = 0.0f;
    float destY = 0.0f;
    float destZ = 0.0f;
    destX = newPiece.getLocX();
    destY = currentPiece.getLocY();
    destZ = newPiece.getLocZ();
    bool flag = true;
    // Board represents if the new destination is a board(1)or a chess piece(0).
    // i
    if (board == 0) {
        // Handling if new click is on a friendly chess piece
        if ((currentPieceId < 16 && newPieceId < 16) || ((currentPieceId > 15 && currentPieceId < 32) && (newPieceId > 15 && newPieceId < 32) )) {
            currentPiece.unselect();
            newPiece.select();
            currentPieceId = newPieceId;
            flag = false;
        }
        // If chess click is on an oponent 
        else {
            newPiece.setLocation(100.0, 100.0f, 100.0f);
            newPiece.GetTransform().Translate(100.0, 100.0f, 100.0f);
        }
    }
    // Handles case where click was not on a friendly chess piece. i.e. an empty board 
    // or an oponent piece.
    if (flag) {     
        currentPiece.setLocation(destX, destY, destZ);
        currentPiece.GetTransform().Move(destX, destY, destZ);
        GameObjectManager::Instance().GetGameObject(currentPiece.getBoardId() + 31).boardSetter(false);
        GameObjectManager::Instance().GetGameObject(newPiece.getBoardId() + 31).boardSetter(true);
        currentPiece.setBoardId(newPiece.getBoardId());
        currentPiece.unselect();
        selected = false;
        currentPieceId = -1;
        turn = (turn == 1) ? 0 : 1;
    }
}

// Loops forever!
void Game::Loop()
{
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    bool wireFrameMode = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input

    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.05f;
    Uint32 buttons;
    int x, y;
    GLbyte color[4];
    GLfloat depth;
    int piece_id = -1;
    int delta_white = 32;
    int delta_black = 33;
    bool selected = false;
    float destX = 0.0f;
    float destY = 0.0f;
    float destZ = 0.0f;
    int turn = 0;
    int prevTurn = 0;

    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            buttons = SDL_GetMouseState(&x, &y);
            if ((buttons & SDL_BUTTON_LMASK) != 0)
            {
                // REad pixels from screen
                glReadPixels(x, m_screenHeight - y - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
                glReadPixels(x, m_screenHeight - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
                // Observe what has been clicked
                printf("Clicked on pixel %d, %d, color %d  %d %d %d, depth %f\n",
                       x, y, (int)color[0], (int)color[1], (int)color[2], (int)color[3], depth);
                int temp = 0;
                int col = (int) color[0];
                
                if (col != 51)
                {
                    getClickedGameObjectId (temp, col, delta_black, delta_white );
                    // Check if new click is on a chess piece and is with in turn
                    if (not selected && ((temp < 16 && turn == 0) || (temp > 15 && temp < 32 && turn == 1)))
                    {
                        GameObjectManager::Instance().GetGameObject(temp).select();
                        piece_id = temp;
                        selected = true;
                    } 
                    // Handle if first click is not on a chess piece.
                    else if (not selected && temp > 31) {
                            printf("Click on a piece first.");
                    } 
                    // Handle click that is on chess piece that is not allowed. (i.e. not players turn)
                    else if (not selected && temp < 32) {
                            printf("Not your turn!");
                    } else
                    {
                        GameObject& newPiece = GameObjectManager::Instance().GetGameObject(temp);
                        if (piece_id < 16) {
                            if (temp < 32) {
                                process_move_to_board(piece_id, temp, turn, selected, 0);
                                
                            } else if(temp > 31 && temp < 96 && not newPiece.hasPiece()){
                                process_move_to_board(piece_id, temp, turn, selected, 1);
                                
                            } 
                        } else if (piece_id < 32) {
                            if (temp < 32) {
                                process_move_to_board(piece_id, temp, turn, selected, 0);
                            } else if(temp > 31 && temp < 96 && not newPiece.hasPiece()){
                                process_move_to_board(piece_id, temp, turn, selected, 1);
                            }
                        }
                        
                    }
                }
            }
            // Handle keyboad input for the camera class
            if (e.type == SDL_MOUSEMOTION)
            {
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                // Camera::Instance().MouseLook(mouseX, mouseY);
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_w)
                {
                    // toggles between wireframe mode and textured mode.
                    if (wireFrameMode)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        wireFrameMode = false;
                    }
                    else
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        wireFrameMode = true;
                    }
                }
                else if (e.key.keysym.sym == SDLK_q)
                {
                    quit = true;
                    //  q quits the program
                }
            }
            switch (e.type)
            {
            // Handle keyboard presses
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    Camera::Instance().MoveLeft(cameraSpeed);
                    break;
                case SDLK_RIGHT:
                    Camera::Instance().MoveRight(cameraSpeed);
                    break;
                case SDLK_UP:
                    Camera::Instance().MoveForward(cameraSpeed);
                    break;
                case SDLK_DOWN:
                    Camera::Instance().MoveBackward(cameraSpeed);
                    break;
                case SDLK_PAGEUP:
                    Camera::Instance().MoveUp(cameraSpeed);
                    break;
                case SDLK_PAGEDOWN:
                    Camera::Instance().MoveDown(cameraSpeed);
                    break;
                }
                break;
            }
        } // End SDL_PollEvent loop.

        // Update our scene
        Update();
        // Render using OpenGL
        Render(); // TODO: potentially move this depending on your logic
                  // for how you handle drawing a triangle or rectangle.
        // Update screen of our specified window
        SDL_GL_SwapWindow(GetSDLWindow());
        // Swap camera based on turn.
        if (turn != prevTurn) {
            Sleep(500);
            Camera::Instance().revolve(turn);
            prevTurn = turn;
        }
    }
    // Disable text input
    SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window *Game::GetSDLWindow()
{
    return m_window;
}

// Helper Function to get OpenGL Version Information
void Game::GetOpenGLVersionInfo()
{
    SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
    SDL_Log("Vendor: %s", (const char *)glGetString(GL_VENDOR));
    SDL_Log("Renderer: %s", (const char *)glGetString(GL_RENDERER));
    SDL_Log("Version: %s", (const char *)glGetString(GL_VERSION));
    SDL_Log("Shading language: %s", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
