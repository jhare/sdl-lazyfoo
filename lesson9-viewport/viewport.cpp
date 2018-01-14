#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Texture* loadTexture(std::string path);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

bool init();
bool loadMedia();
void reportError(std::string msg);
void close();

int main(int argc, char* argv[])
{
  if(!init()) {
    printf("Could not initialize SDL!\n");
  } else {
    if(!loadMedia()) {
      printf("Could not load media for texture demo!\n");
    } else {
      SDL_Event e;
      bool quit = false;

      while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
          if(e.type == SDL_QUIT) {
            quit = true;
          }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);
        SDL_RenderPresent(gRenderer);
      }
    }
  }

  close();
  return 0;
}

void close()
{
  SDL_DestroyTexture(gTexture);
  gTexture = NULL;

  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

bool loadMedia() 
{
  bool success = true;

  gTexture = loadTexture("./09_the_viewport/viewport.png");
  if(gTexture == NULL) {
    reportError("Could not load viewport marker");
    success = false;
  }

  return success;
}

/**
 * Cause we ain' playin'
 */
void reportError(std::string msg)
{
  printf("%s SDL Error: %s\n", msg.c_str(), SDL_GetError());
}

bool init()
{
  bool success = true;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    reportError("Could not initialize SDL");
    success = false;
  } else {
    gWindow = SDL_CreateWindow("SDL Textures", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      reportError("Could not create window");
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if(gRenderer == NULL) {
        reportError("Could not create renderer");
        success = false;
      } 

      int imgFlags = IMG_INIT_PNG;
      if(!(IMG_Init(imgFlags) & imgFlags)) {
        reportError("Could not initialize PNG library.");
      }
    }
  }

  return success;
}

SDL_Texture* loadTexture(std::string path)
{
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL) {

  } else {
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if(newTexture == NULL) {
      reportError("Unable to create texture from surface");
    }

    SDL_FreeSurface(loadedSurface);
  }

  return newTexture;
}
