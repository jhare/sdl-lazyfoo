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
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

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

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
LTexture gSpriteSheetTexture;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gModulatedTexture;
LTexture gBackgroundTexture;

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

void LTexture::setBlendMode(SDL_BlendMode blending)
{
  SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
  SDL_SetTextureAlphaMod(mTexture, alpha);
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
      int frame = 0;

      SDL_Rect* currentClip = NULL;

      while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
          if(e.type == SDL_QUIT) {
            quit = true;
          } else if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_q) {
              quit = true;
            }

          }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        currentClip = &gSpriteClips[frame / 4];
        gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);

        SDL_RenderPresent(gRenderer);

        // after we render this frame, advance to the next, wrap around
        ++frame;
        if(frame/4 >= WALKING_ANIMATION_FRAMES) { // % assignment faster here?
          frame = 0;
        }
      }
    }
  }

  close();
  return 0;
}

void close()
{
  gSpriteSheetTexture.free();

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

  if(!gSpriteSheetTexture.loadFromFile("./14_animated_sprites_and_vsync/foo.png")) {
    printf("Couldn't load our foo! We need him.: %s\n", SDL_GetError());
    success = false;
  } else {
    // Okay I didn't type this bit again
    gSpriteClips[ 0 ].x =   0;
    gSpriteClips[ 0 ].y =   0;
    gSpriteClips[ 0 ].w =  64;
    gSpriteClips[ 0 ].h = 205;

    gSpriteClips[ 1 ].x =  64;
    gSpriteClips[ 1 ].y =   0;
    gSpriteClips[ 1 ].w =  64;
    gSpriteClips[ 1 ].h = 205;
    
    gSpriteClips[ 2 ].x = 128;
    gSpriteClips[ 2 ].y =   0;
    gSpriteClips[ 2 ].w =  64;
    gSpriteClips[ 2 ].h = 205;

    gSpriteClips[ 3 ].x = 196;
    gSpriteClips[ 3 ].y =   0;
    gSpriteClips[ 3 ].w =  64;
    gSpriteClips[ 3 ].h = 205;
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
      // Create renderer with vsync enabled. bauses would make this a setting
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
