#ifndef __LTEXTURE_H
#define __LTEXTURE_H

#include <SDL2/SDL.h>

class LTexture
{

  public:
    LTexture();
    ~LTexture();

  private:
    SDL_Texture* mTexture;

    int mWidth;
    int mHeight;
};

#endif
