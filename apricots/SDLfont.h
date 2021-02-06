// SDLfont class header
// Author: M.D.Snellgrove
// Date: 24/7/2003
// History:

#include "SDL.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

class SDLfont{
  private:
    int width;
    int height;
    char* symbolbuffer[256];
    SDL_Surface *symbol[256];
    SDL_Surface *symbolmask[256];
  public:
    SDLfont();
    SDLfont(const SDLfont &);
    ~SDLfont();
    SDLfont & operator=(const SDLfont &);
    void loadpsf(char*, int, int);
    void colour(SDL_Surface*, int, int);
    void writemask(SDL_Surface*, int, int, char*);
    void write(SDL_Surface*, int, int, char*);
};
