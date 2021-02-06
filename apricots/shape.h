// Shape class header file
// Author: M.D.Snellgrove
// Date: 27/1/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Changes for graphics related stuff.

#include "SDL.h"
#include <fstream>
using namespace std;

class shape{
  private:
    SDL_Surface *surface;
    int width;
    int height;
    char *buffer;
    int max(int, int);
    int min(int, int);
  public:
    shape();
    shape(SDL_Surface *source, int x, int y, int w, int h);
    shape(const shape &);
    ~shape();
    shape& operator= (const shape &);
    int getwidth();
    int getheight();
    SDL_Surface* getSurface();
    //char* getbuffer();
    void grab(SDL_Surface *source, int x, int y, int w, int h);
    bool read(ifstream &);
    bool readfile(char*);
    bool read(SDL_Surface*, ifstream &);
    bool readfile(SDL_Surface*, char*);
    void blit(SDL_Surface *dest, int x, int y);
    void blit(SDL_Surface *dest, int x, int y, bool mask);
    bool write(ofstream &);
    bool writefile(char*);
    bool collide(int, int, const shape &, int, int);
};
