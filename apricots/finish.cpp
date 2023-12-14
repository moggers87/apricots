// Apricots finish game routine
// Author: M.D.Snellgrove
// Date: 26/3/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Changes for graphics and font related stuff.

// Changes by M Snellgrove 15/7/2003
//   SDLfont used for fonts

#include "apricots.h"

// Draw a winnerbox

void winnerbox(gamedata &g, int winner, int player, int y, int control) {

  int boxcolour = 4; // red for losers
  if (player == control) {
    boxcolour = 13; // green if winner
  }
  SDL_Rect rect;
  rect.x = 200;
  rect.y = 64 + y;
  rect.w = 240;
  rect.h = 80;
  SDL_FillRect(g.virtualscreen, &rect, 1);
  rect.x = 201;
  rect.y = 65 + y;
  rect.w = 238;
  rect.h = 78;
  SDL_FillRect(g.virtualscreen, &rect, boxcolour);

  if (player == control) {
    char winstring[] = "You win!";
    g.whitefont.writemask(g.virtualscreen, 288, 72 + y, winstring);
    char congrats[] = "Congratulations";
    g.whitefont.writemask(g.virtualscreen, 260, 122 + y, congrats);
  } else {
    char losestring[] = "You lose";
    g.whitefont.writemask(g.virtualscreen, 288, 72 + y, losestring);
    if (control > 0) {
      char winstring[] = "Player x wins";
      winstring[7] = '0' + control;
      g.whitefont.writemask(g.virtualscreen, 268, 112 + y, winstring);
    } else {
      char winstring[] = "Computer (Plane x) wins";
      winstring[16] = '0' + winner;
      g.whitefont.writemask(g.virtualscreen, 228, 112 + y, winstring);
    }
  }
}

// Main finish game routine

void finish_game(gamedata &g) {

  // Stop enginenoise
  g.sound.stop(0);
  g.sound.stop(1);
  // Display winnerbox
  if (g.winner > 0) {
    winnerbox(g, g.winner, 1, 0, g.planeinfo[g.winner].control);
    if (g.players == 2) {
      winnerbox(g, g.winner, 2, 240, g.planeinfo[g.winner].control);
    }
    // Update display
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = SCREEN_WIDTH;
    rect.h = SCREEN_HEIGHT;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(g.renderer, g.virtualscreen);
    if (texture == NULL) {
      fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    SDL_RenderCopy(g.renderer, texture, &rect, NULL);
    SDL_RenderPresent(g.renderer);
    g.sound.play(SOUND_FINISH);
    // Wait 4 Seconds
    int then = time(0);
    while (time(0) - then < 4) {
    }
    SDL_DestroyTexture(texture);
  }
  // Clean up linkedlists
  g.radar.clearlist();
  g.gun.clearlist();
  g.p.clearlist();
  g.dp.clearlist();
  g.explosion.clearlist();
  g.flame.clearlist();
  g.smoke.clearlist();
  g.fall.clearlist();
  g.shot.clearlist();
  g.drakgun.clearlist();
  g.laser.clearlist();
}
