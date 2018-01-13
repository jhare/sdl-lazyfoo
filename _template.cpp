#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

int main(int argc, char** argv) {
  if(!init()) {
      printf("Could not initialize!\n");
    } else {
      bool quit = false;
      SDL_Event e;
      while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
          if(e.type == SDL_QUIT) {
            printf("Quitting on purpopse!\n");
            quit = true;
          }

        }
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
    gWindow = SDL_CreateWindow("jhare SDL lesson 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      printf("Could not create window: %s\n", SDL_GetError());
      success = false;
    } else {
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

bool loadMedia()
{
  bool success = true;

  /* example from last
  gXOut = SDL_LoadBMP("03_event_driven_programming/x.bmp");
  if(gXOut == NULL) {
    printf("Could not load xout bmp: %s\n", SDL_GetError());
    success = false;
  }
  */
  return success;
}

void close() 
{
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();
}
