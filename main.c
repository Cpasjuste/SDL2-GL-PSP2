/**
 * Simple SDL2 program using OpenGL as rendering pipeline.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

SDL_Window* gWindow = NULL;
SDL_GLContext gGlContext;

void initGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

int setViewport(int width, int height)
{
    GLfloat ratio;

    if (height == 0) {
        height = 1;
    }

    ratio = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return 1;
}

void render()
{
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glTranslatef(3.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd();
}

int main(int argc, char *argv[])
{
    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
    {
        printf("SDL cannot init with error : %s\n", SDL_GetError());
        return -1;
    }

    // create window
    gWindow = SDL_CreateWindow("GL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("Cannot create window with error : %s\n", SDL_GetError());
        return -1;
    }

    // create opengl context
    gGlContext = SDL_GL_CreateContext(gWindow);
    if (gGlContext == NULL)
    {
        printf("Cannot create OpenGL context with error : %s\n", SDL_GetError());
        return -1;
    }
    
    // initialize opengl
    initGL();
    
    // set camera
    setViewport(SCREEN_WIDTH, SCREEN_HEIGHT);

    int quit = 0;
    SDL_Event sdlEvent;
    while (!quit)
    {
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            if (sdlEvent.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
        
        // clear bg color and depth buffer
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render stuff here
        render();

        // swap window inorder to update opengl
        SDL_GL_SwapWindow(gWindow);
    }

    // clear resource
    SDL_GL_DeleteContext(gGlContext);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();

    return 0;
}
