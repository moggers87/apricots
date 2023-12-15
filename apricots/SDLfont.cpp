// A very simple SDLfont class definition
// Author: M.D.Snellgrove
// Date: 24/7/2003
// History:

#include "SDLfont.h"
#include <filesystem>

// Default constructor

SDLfont ::SDLfont() {

  width = 0;
  height = 0;
  for (int i = 0; i < 256; i++) {
    symbolbuffer[i] = NULL;
    symbol[i] = NULL;
    symbolmask[i] = NULL;
  }
}

// Copy constructor (deep copy)

SDLfont ::SDLfont(const SDLfont &f) {

  width = f.width;
  height = f.height;
  int buffersize = width * height;
  for (int i = 0; i < 256; i++) {
    if (f.symbolbuffer[i] != NULL) {
      symbolbuffer[i] = new char[width * height];
      for (int j = 0; j < buffersize; j++) {
        symbolbuffer[i][j] = f.symbolbuffer[i][j];
      }
      symbol[i] = SDL_CreateRGBSurfaceFrom(symbolbuffer[i], width, height, 8, width, 0, 0, 0, 0);
      symbolmask[i] = SDL_CreateRGBSurfaceFrom(symbolbuffer[i], width, height, 8, width, 0, 0, 0, 0);
      SDL_SetColorKey(symbolmask[i], SDL_TRUE, 0);
    }
  }
}

// Assignment = operator (deep copy)

SDLfont &SDLfont ::operator=(const SDLfont &f) {

  if (this != &f) {
    width = f.width;
    height = f.height;
    int buffersize = width * height;
    for (int i = 0; i < 256; i++) {
      if (symbol[i] != NULL) {
        SDL_FreeSurface(symbol[i]);
        SDL_FreeSurface(symbolmask[i]);
        symbol[i] = NULL;
      }
      if (symbolbuffer[i] != NULL) {
        delete[] symbolbuffer[i];
        symbolbuffer[i] = NULL;
      }
      if (f.symbolbuffer[i] != NULL) {
        symbolbuffer[i] = new char[width * height];
        for (int j = 0; j < buffersize; j++) {
          symbolbuffer[i][j] = f.symbolbuffer[i][j];
        }
        symbol[i] = SDL_CreateRGBSurfaceFrom(symbolbuffer[i], width, height, 8, width, 0, 0, 0, 0);
        symbolmask[i] = SDL_CreateRGBSurfaceFrom(symbolbuffer[i], width, height, 8, width, 0, 0, 0, 0);
        SDL_SetColorKey(symbolmask[i], SDL_TRUE, 0);
      }
    }
  }
  return *this;
}

// Destructor

SDLfont ::~SDLfont() {

  for (int i = 0; i < 256; i++) {
    if (symbol[i] != NULL)
      SDL_FreeSurface(symbol[i]);
    if (symbolmask[i] != NULL)
      SDL_FreeSurface(symbolmask[i]);
    if (symbolbuffer[i] != NULL)
      delete[] symbolbuffer[i];
  }
}

// Load psf font from file

void SDLfont ::loadpsf(fontinfo font) {

  // Open file
  ifstream fin(font.path, ios::binary);
  if (fin.fail()) {
    cerr << "SDLFont: File " << font.path << " not found" << endl;
    exit(EXIT_FAILURE);
  }

  int file_size = std::filesystem::file_size(font.path);
  char *buffer = new char[file_size];
  fin.read(buffer, file_size);
  fin.close();

  width = font.width;
  height = font.height;

  // Expand compressed font
  for (int i = 0; i < font.chars_count; i++) {
    symbolbuffer[i] = new char[width * height];
    int bufferpos = i * width * height / 8 + 4;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width / 8; x++) {
        int byte = (int)buffer[bufferpos + y * width / 8 + x];
        int factor = height * width;
        for (int bit = 0; bit < 8; bit++) {
          if ((byte & factor) == 0) {
            symbolbuffer[i][width * y + 8 * x + bit] = 0;
          } else {
            symbolbuffer[i][width * y + 8 * x + bit] = 1;
          }
          factor = factor / 2;
        }
      }
    }

    symbol[i] = SDL_CreateRGBSurfaceFrom(symbolbuffer[i], width, height, 8, width, 0, 0, 0, 0);
    symbolmask[i] = SDL_CreateRGBSurfaceFrom(symbolbuffer[i], width, height, 8, width, 0, 0, 0, 0);
    SDL_SetColorKey(symbolmask[i], SDL_TRUE, 0);
  }

  delete[] buffer;
}

// Set colours 0 and 1 to be chosen font colour

void SDLfont ::colour(SDL_Surface *surface, int fgc, int bgc) {

  SDL_Color palette[2];
  Uint8 rgb[3];
  SDL_GetRGB(bgc, surface->format, &rgb[0], &rgb[1], &rgb[2]);
  palette[0].r = rgb[0];
  palette[0].g = rgb[1];
  palette[0].b = rgb[2];
  SDL_GetRGB(fgc, surface->format, &rgb[0], &rgb[1], &rgb[2]);
  palette[1].r = rgb[0];
  palette[1].g = rgb[1];
  palette[1].b = rgb[2];
  for (int i = 0; i < 256; i++) {
    SDL_SetPaletteColors(symbol[i]->format->palette, palette, 0, 2);
    SDL_SetPaletteColors(symbolmask[i]->format->palette, palette, 0, 2);
  }
}

// Write text at x,y with mask

void SDLfont ::writemask(SDL_Surface *surface, int x, int y, char *text) {

  // Blit each letter in turn
  int letter = 0;

  while ((int)text[letter] != 0) {

    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = width;
    srcrect.h = height;
    SDL_Rect dstrect;
    dstrect.x = x + width * letter;
    dstrect.y = y;
    dstrect.w = 0;
    dstrect.h = 0;
    SDL_BlitSurface(symbolmask[(int)text[letter]], &srcrect, surface, &dstrect);

    letter++;
  }
}

// Write text at x,y without mask

void SDLfont ::write(SDL_Surface *surface, int x, int y, char *text) {

  // Blit each letter in turn
  int letter = 0;

  while ((int)text[letter] != 0) {

    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = width;
    srcrect.h = height;
    SDL_Rect dstrect;
    dstrect.x = x + width * letter;
    dstrect.y = y;
    dstrect.w = 0;
    dstrect.h = 0;
    SDL_BlitSurface(symbol[(int)text[letter]], &srcrect, surface, &dstrect);

    letter++;
  }
}
