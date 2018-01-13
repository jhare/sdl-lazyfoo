#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();

void close();

SDL_Window* gWindow;

int main(int argc, char** argv) {

  close();
  return 0;
}

bool init() {
  bool success = true;


  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not init video: %s\n", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow("jhare SDL lesson 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      printf("Could not create window: %s\n", SDL_GetError());
    } else {

    }
  }

  close();
  return success;
}

void close() {
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();
}

