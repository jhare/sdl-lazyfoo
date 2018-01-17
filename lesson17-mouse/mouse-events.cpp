#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
  BUTTON_SPRITE_MOUSE_OUT = 0,
  BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
  BUTTON_SPRITE_MOUSE_DOWN = 2,
  BUTTON_SPRITE_MOUSE_UP = 3,
  BUTTON_SPRITE_TOTAL = 4
};


class LButton
{
  public:
    LButton();

    void setPosition(int x, int y);
    void handleEvent(SDL_Event* e);
    void render();

  private:
    SDL_Point mPosition;
    LButtonSprite mCurrentSprite;
};

LButton::LButton()
{
  mPosition.x = 0;
  mPosition.y = 0;

  mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
  mPosition.x = x;
  mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
  if(e->type == SDL_MOUSEMOTION ||
     e->type == SDL_MOUSEBUTTONDOWN || 
     e->type == SDL_MOUSEBUTTONUP)
  {
    int x, y;
    SDL_GetMouseState(&x, &y);

    bool inside = true;

    // See if we're inside the button
    if(x < mPosition.x) {
      inside = false;
    } else if(x > mPosition.x + BUTTON_WIDTH) {
      inside = false;
    } else if(y < mPosition.y) {
      inside = false;
    } else if(y > mPosition.y + BUTTON_HEIGHT) {
      inside = false;
    }

    if(!inside) {
      mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    } else {
      switch(e->type) {
        case SDL_MOUSEMOTION:
          mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
          break;
        case SDL_MOUSEBUTTONDOWN:
          mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
          break;
        case SDL_MOUSEBUTTONUP:
          mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
          break;
      }
    }
  }
  
}


class LTexture
{
  
  public:
    LTexture();
    ~LTexture();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect* clip, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void free();
    bool loadFromFile(std::string path);

    // I'm skipping the #ifdef they do in the example we'll always just include it for now
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

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

SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture;

// dumbbbbb we need to organize this better than the example heh
void LButton::render()
{
  gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}
//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ]; 

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

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  if(clip != NULL) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
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

      while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
          if(e.type == SDL_QUIT) {
            quit = true;
          } else if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_q) {
              quit = true;
            }
          }

          for(int i = 0; i < TOTAL_BUTTONS; ++i) {
            gButtons[i].handleEvent(&e);
          }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        for(int i = 0; i < TOTAL_BUTTONS; ++i) {
          gButtons[i].render();
        }

        SDL_RenderPresent(gRenderer);
      }
    }
  }

  close();
  return 0;
}

void close()
{
  gButtonSpriteSheetTexture.free();

  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;

  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  IMG_Quit();
  SDL_Quit();
  TTF_Quit();
}

bool loadMedia() 
{
  bool success = true;

	if(!gButtonSpriteSheetTexture.loadFromFile("./17_mouse_events/button.png")) {
		printf("Failed to load button sprite texture: %s\n", SDL_GetError());
		success = false;
	}
	else {
		for(int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		gButtons[0].setPosition(0, 0);
		gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
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

      if(TTF_Init() == -1) {
        printf("Could not initialize TTF subsystem: %s\n", TTF_GetError());
        success = false;
      }
    }
  }

  return success;
}
