#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurface {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

bool init();
bool loadMedia();
void close();

SDL_Surface* loadSurface(std::string path);
SDL_Surface* gKeypressSurfaces[KEY_PRESS_SURFACE_TOTAL]; // remember, array of ptrs here

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gCurrentSurface = NULL;

int main(int argc, char** argv) {
  if(!init()) {
    printf("Could not initialize!\n");
  } else {
    bool quit = false;
    SDL_Event e;

    loadMedia();

    gCurrentSurface = gKeypressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(gWindow);

    while(!quit) {
      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          printf("Quitting on purpopse!\n");
          quit = true;
        } else if(e.type == SDL_KEYDOWN) {

          switch(e.key.keysym.sym) {
            case SDLK_UP:
              gCurrentSurface = gKeypressSurfaces[KEY_PRESS_SURFACE_UP];
              break;

            case SDLK_DOWN:
              gCurrentSurface = gKeypressSurfaces[KEY_PRESS_SURFACE_DOWN];
              break;

            case SDLK_LEFT:
              gCurrentSurface = gKeypressSurfaces[KEY_PRESS_SURFACE_LEFT];
              break;

            case SDLK_RIGHT:
              gCurrentSurface = gKeypressSurfaces[KEY_PRESS_SURFACE_RIGHT];
              break;

            default:
              gCurrentSurface = gKeypressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
              break;
          }
        }
      }
      SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
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

bool loadMedia() {
  bool success = true;

  // I'm not typing the fucking error handling here
  gKeypressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("./04_key_presses/press.bmp");
  gKeypressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("./04_key_presses/up.bmp");
  gKeypressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("./04_key_presses/down.bmp");
  gKeypressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("./04_key_presses/left.bmp");
  gKeypressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("./04_key_presses/right.bmp");

  return success;
}

SDL_Surface* loadSurface(std::string path)
{
  SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
  if(loadedSurface == NULL) {
    printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
  }

  return loadedSurface;
}

void close() 
{
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gCurrentSurface = NULL;

  for(int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i) {
    SDL_FreeSurface(gKeypressSurfaces[i]);
    gKeypressSurfaces[i] = NULL;
  }

  SDL_Quit();
}
