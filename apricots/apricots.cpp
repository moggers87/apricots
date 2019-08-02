// Apricots main program
// Author: M.D.Snellgrove
// Date: 17/3/2002
// History:

// Changed by M Snellgrove 3/8/2003
// Conditional compilation switch for Cygwin (thanks judebert)

#include "apricots.h"

// Definition of Main
// Cygwin expects a WinMain function
#ifdef CYGWIN
WINAPI int WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
#else
int main(int, char**){
#endif

  // Initialize data
  gamedata g;
  init_data(g);

  // Setup game
  setup_game(g);

  // Enter main loop
  game(g);

  // Finish game
  finish_game(g);

  // Shutdown
  SDL_Quit();
  g.sound.close();
  
  return 0;

}
