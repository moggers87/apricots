// Shape class implementation file
// Author: M.D.Snellgrove
// Date: 13/2/2001
// History: Original Version 13/2/2001
//          Collision added 27/1/2002

// Changes by M Harman for Windows version, June 2003:
//   Changes for graphics related stuff.

// Changes by M Snellgrove 7/7/2003
//   Bugfix in screengrab constructor

// Changes by M Snellgrove 10/7/2003
//   Filename bugfix in readfile, writefile
//   Palette bugfix in grab, copy constructor, assigment=
//   Palette specification overloaded in read, readfile

#include "shape.h"

const int BYTESPERPIXEL = 1;

// Default Constructor

shape :: shape(){
  surface = NULL;
  width = 0;
  height = 0;
  buffer = 0;
}

// Screengrab Constructor

shape :: shape(SDL_Surface *source, int x, int y, int w, int h){
  width = w;
  height = h;
  buffer = new char[w * h * BYTESPERPIXEL];
  char *src_pixels = (char *)source->pixels;
  for(int i=x;i<x+w;i++){
    for(int j=y;j<y+h;j++){
      buffer[ (j-y) * w + (i-x) ] = src_pixels[ j * source->pitch + i ];
    }
  }
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);
  for (int c=0;c<256;c++){
    Uint8 rgb[3];
    SDL_GetRGB(c, source->format, &rgb[0], &rgb[1], &rgb[2]);
    SDL_Color col = { rgb[0], rgb[1], rgb[2], 0 };
    SDL_SetColors(surface, &col, c, 1);
  }

}

// Copy Constructor (deep copy)

shape :: shape(const shape &s){
  width = s.width;
  height = s.height;
  int buffersize = width * height * BYTESPERPIXEL;
  buffer = new char[buffersize];
  for (int i = 0; i < buffersize; i++)
    buffer[i] = s.buffer[i];
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);
  for (int c=0;c<256;c++){
    Uint8 rgb[3];
    SDL_GetRGB(c, s.surface->format, &rgb[0], &rgb[1], &rgb[2]);
    SDL_Color col = { rgb[0], rgb[1], rgb[2], 0 };
    SDL_SetColors(surface, &col, c, 1);
  }

}

// Destructor

shape :: ~shape(){
  if (surface != NULL)
    SDL_FreeSurface(surface);
  if (buffer != 0) delete [ ]buffer;
}

// Assignment Operator

shape& shape :: operator= (const shape &s){
  if (this != &s){
    if (surface != NULL) SDL_FreeSurface(surface);
    if (buffer != 0) delete [ ]buffer;
    width = s.width;
    height = s.height;
    int buffersize = width * height * BYTESPERPIXEL;
    buffer = new char[buffersize];
    for (int i = 0; i < buffersize; i++)
      buffer[i] = s.buffer[i];
    surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                       width * BYTESPERPIXEL, 0, 0, 0, 0);
    for (int c=0;c<256;c++){
      Uint8 rgb[3];
      SDL_GetRGB(c, s.surface->format, &rgb[0], &rgb[1], &rgb[2]);
      SDL_Color col = { rgb[0], rgb[1], rgb[2], 0 };
      SDL_SetColors(surface, &col, c, 1);
    }

  }
  return *this;
}

// Accessor functions for private variables

int shape :: getwidth(){return width;}
int shape :: getheight(){return height;}
SDL_Surface* shape :: getSurface(){return surface;}
//char* shape :: getbuffer(){return buffer;}

// Shape Grabber function

void shape :: grab(SDL_Surface *source, int x, int y, int w, int h){
  if (surface != NULL) SDL_FreeSurface(surface);
  if (buffer != 0) delete [ ]buffer;
  width = w;
  height = h;

  buffer = new char[w * h * BYTESPERPIXEL];
  char *src_pixels = (char *)source->pixels;
  for(int i=x;i<x+w;i++){
    for(int j=y;j<y+h;j++){
      buffer[ (j-y) * w + (i-x) ] = src_pixels[ j * source->pitch + i ];
    }
  }
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);
  for (int c=0;c<256;c++){
    Uint8 rgb[3];
    SDL_GetRGB(c, source->format, &rgb[0], &rgb[1], &rgb[2]);
    SDL_Color col = { rgb[0], rgb[1], rgb[2], 0 };
    SDL_SetColors(surface, &col, c, 1);
  }


}

// Shape file read function (reference file)

bool shape :: read(ifstream &fin){
  if (fin.fail()){
    return false;
  }

  if (surface != NULL) SDL_FreeSurface(surface);
  if (buffer != 0) delete [ ]buffer;
  fin >> width >> height;
  if (fin.fail()){
    return false;
  }
  int buffersize = width * height * BYTESPERPIXEL;
  buffer = new char[buffersize];
  fin.read(buffer, 1); // Read the extra byte
  fin.read(buffer, buffersize);
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);

  return true;
}

// Shape file read function (new file)

bool shape :: readfile(char* filename){
  ifstream fin(filename, ios::binary);

  if (fin.fail()) return false;

  if (surface != NULL) SDL_FreeSurface(surface);
  if (buffer != 0) delete [ ]buffer;
  fin >> width >> height;
  int buffersize = width * height * BYTESPERPIXEL;
  buffer = new char[buffersize];
  fin.read(buffer, 1); // Read the extra byte
  fin.read(buffer, buffersize);
  fin.close();
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);

  return true;
}

// Shape file read function (reference file with palette)

bool shape :: read(SDL_Surface *palettesource, ifstream &fin){
  if (fin.fail()){
    return false;
  }

  if (surface != NULL) SDL_FreeSurface(surface);
  if (buffer != 0) delete [ ]buffer;
  fin >> width >> height;
  if (fin.fail()){
    return false;
  }
  int buffersize = width * height * BYTESPERPIXEL;
  buffer = new char[buffersize];
  fin.read(buffer, 1); // Read the extra byte
  fin.read(buffer, buffersize);
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);

  for (int c=0;c<256;c++){
    Uint8 rgb[3];
    SDL_GetRGB(c, palettesource->format, &rgb[0], &rgb[1], &rgb[2]);
    SDL_Color col = { rgb[0], rgb[1], rgb[2], 0 };
    SDL_SetColors(surface, &col, c, 1);
  }

  return true;
}

// Shape file read function (new file with palette)

bool shape :: readfile(SDL_Surface *palettesource, char* filename){
  ifstream fin(filename, ios::binary);

  if (fin.fail()) return false;

  if (surface != NULL) SDL_FreeSurface(surface);
  if (buffer != 0) delete [ ]buffer;
  fin >> width >> height;
  int buffersize = width * height * BYTESPERPIXEL;
  buffer = new char[buffersize];
  fin.read(buffer, 1); // Read the extra byte
  fin.read(buffer, buffersize);
  fin.close();
  surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8 * BYTESPERPIXEL,
                                     width * BYTESPERPIXEL, 0, 0, 0, 0);
  for (int c=0;c<256;c++){
    Uint8 rgb[3];
    SDL_GetRGB(c, palettesource->format, &rgb[0], &rgb[1], &rgb[2]);
    SDL_Color col = { rgb[0], rgb[1], rgb[2], 0 };
    SDL_SetColors(surface, &col, c, 1);
  }


  return true;
}


// Shape blitter (mask on)

void shape :: blit(SDL_Surface *dest,int x, int y){
  SDL_SetColorKey(surface, SDL_SRCCOLORKEY, 0);
  SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = width;
    srcrect.h = height;
  SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = 0;
    dstrect.h = 0;
  SDL_BlitSurface(surface, &srcrect, dest, &dstrect);
  SDL_SetColorKey(surface, 0, 0);
}

// Shape blitter (mask chosen)

void shape :: blit(SDL_Surface *dest,int x, int y, bool mask){
  SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = width;
    srcrect.h = height;
  SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = 0;
    dstrect.h = 0;
  if (mask)
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY, 0);
  SDL_BlitSurface(surface, &srcrect, dest, &dstrect);
  if (mask)
    SDL_SetColorKey(surface, 0, 0); 
}

// Shape file write function (reference file)

bool shape :: write(ofstream &fout){
  if (fout.fail()) return false;

  fout << width << " " << height << " ";
  fout.write(buffer, width * height * BYTESPERPIXEL);

  return true;
}

// Shape file write function (new file)

bool shape :: writefile(char* filename){
  ofstream fout(filename, ios::binary);

  if (fout.fail()) return false;

  fout << width << " " << height << " ";
  fout.write(buffer, width * height * BYTESPERPIXEL);
  fout.close();

  return true;
}

// Maximization function (for collision detection)

int shape :: max(int a, int b){
  if (a > b) return a;
  return b;
}

// Minimization function (for collision detection)

int shape :: min(int a, int b){
  if (a < b) return a;
  return b;
}

// Shape collision detection routine
// Checks if the shape at x,y collides with another shape s at sx,sy
// NB assumes BYTESPERPIXEL == 1

bool shape :: collide(int x, int y, const shape &s, int sx, int sy){
  for(int cx = max(x,sx);cx<min(x+width,sx+s.width);cx++){
    for(int cy = max(y,sy);cy<min(y+height,sy+s.height);cy++){
      if ((buffer[cx-x + width*(cy-y)] != 0) &&
          (s.buffer[cx-sx + s.width*(cy-sy)] != 0)){
        return true;
      }
    }
  }

  return false;
}
