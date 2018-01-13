#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640*2;
const int SCREEN_HEIGHT = 480*2;

/**
 *
 */
SDL_Surface* loadSurface(std::string path);

bool init();
void loadMedia();
void reportError(std::string msg);
void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gStretchedSurface = NULL;

int main(int argc, char** argv) {
  SDL_Rect stretchRect;
  stretchRect.x = 0;
  stretchRect.y = 0;
  stretchRect.w = SCREEN_WIDTH;
  stretchRect.h = SCREEN_HEIGHT;

  if(!init()) {
    printf("Could not initialize!\n");
  } else {
    bool quit = false;
    SDL_Event e;

    loadMedia();

    while(!quit) {
      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          printf("Quitting on purpopse!\n");
          quit = true;
        } else if(e.type == SDL_KEYUP) {
          quit = true; 
        }
      }
      SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);
      SDL_UpdateWindowSurface(gWindow);
    }
  }

  close();
  return 0;
}

bool init()
{
  bool success = true;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not init video: %s\n", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("jhare SDL lesson 5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      printf("Could not create window: %s\n", SDL_GetError());
      success = false;
    } else {
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

void close() 
{
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_FreeSurface(gStretchedSurface);
  gStretchedSurface = NULL;

  SDL_Quit();
}

/**
 * Cause we ain' playin'
 */
void reportError(std::string msg)
{
  printf("%s SDL Error: %s\n", msg.c_str(), SDL_GetError());
}

SDL_Surface* loadSurface(std::string path)
{
  SDL_Surface* optimizedSurface = NULL;

  SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
  if(loadedSurface == NULL) {
    reportError("Could not load raw image");
  } else {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
    SDL_FreeSurface(loadedSurface);
  }

  return optimizedSurface;
}

void loadMedia()
{
  gStretchedSurface = loadSurface("./05_optimized_surface_loading_and_soft_stretching/stretch.bmp");
}
