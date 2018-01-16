#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Following along with the example we'll just do
 * the declaration and definition of this class in the same
 * file. It simplifies the code for begineers. Otherwise you'd
 * have to inject the gRenderer instance into the LTexture class
 * with a pointer and store that persistently in the class.
 *  Not a huge deal, but a detour for beginners.
 */

class LTexture
{
  
  public:
    LTexture();
    ~LTexture();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void render(int x, int y, SDL_Rect* clip);
    void free();
    bool loadFromFile(std::string path);

    int getWidth();
    int getHeight();

  private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};


LTexture::LTexture()
{
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0; 
}

LTexture::~LTexture()
{
  free();
}

void LTexture::free()
{
  if(mTexture != NULL) {
    SDL_DestroyTexture(mTexture);
    mWidth = 0;
    mHeight = 0;
    mTexture = NULL;
  }
}

int LTexture::getWidth()
{
  return mWidth;
}

int LTexture::getHeight()
{
  return mHeight;
}

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gModulatedTexture;

// lol I dunno if this is lexically necessary to be here but...
// they want that gRenderer reference
bool LTexture::loadFromFile(std::string path)
{
  free();

  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL) {
    printf("Houston we have a problem in SDL, loading an image %s: %s\n", path.c_str(), SDL_GetError());
  } else {
    // do transparency, give example pixel
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if(newTexture == NULL) {
      printf("Could not create texture from surface: %s\n", SDL_GetError());
    } else {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
  }

  mTexture = newTexture;
  return mTexture != NULL;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  if(clip != NULL) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

bool init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
void reportError(std::string msg);
void close();

int main(int argc, char* argv[])
{
  if(!init()) {
    printf("Could not initialize SDL!\n");
  } else {
    if(!loadMedia()) {
      printf("Could not load media for this exercise!\n");
    } else {
      SDL_Event e;
      bool quit = false;

      Uint8 r = 255;
      Uint8 g = 255;
      Uint8 b = 255;

      while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
          if(e.type == SDL_QUIT) {
            quit = true;
          } else if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
              case SDLK_q:
                r += 32;
                break;

              case SDLK_w:
                g += 32;
                break;

              case SDLK_e:
                b += 32;
                break;

              case SDLK_a:
                r -= 32;
                break;

              case SDLK_s:
                g -= 32;
                break;

              case SDLK_d:
                b -= 32;
                break;
            }
          }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        gModulatedTexture.setColor(r, g, b);
        gModulatedTexture.render(0, 0, NULL);

        SDL_RenderPresent(gRenderer);
      }
    }
  }

  close();
  return 0;
}

void close()
{
  gModulatedTexture.free();

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

  if(!gModulatedTexture.loadFromFile("./12_color_modulation/colors.png")) {
    printf("Couldn't load modulation example: %s\n", SDL_GetError());
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
