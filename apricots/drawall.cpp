// Apricots screen drawing and update
// Author: M.D.Snellgrove
// Date: 26/3/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Changes for graphics and font related stuff.

// Changes by M Snellgrove 8/7/2003
//    Player 2 playfield fixed

// Changes by M Snellgrove 15/7/2003
//    SDLfont class used for fonts

#include "apricots.h"

// Draw the blitted images on the screen

void drawblits(gamedata &g, int rx, int ry) {

  // Draw radars
  g.radar.reset();
  while (g.radar.next()) {
    g.images[g.radar().image].blit(g.virtualscreen, g.radar().x - rx, g.radar().y - ry);
  }

  // Draw guns
  g.gun.reset();
  while (g.gun.next()) {
    g.images[162 + g.gun().d].blit(g.virtualscreen, g.gun().x - rx, g.gun().y - ry);
    g.images[171 + g.gun().ammo].blit(g.virtualscreen, g.gun().x - rx, g.gun().y - ry);
  }

  // Draw planes
  g.p.reset();
  while (g.p.next()) {
    if ((g.p().state < 3) && (!g.p().hide)) {
      g.images[g.p().image + g.p().d].blit(g.virtualscreen, (int)g.p().x - rx, (int)g.p().y - ry);
      if (g.p().boost) {
        g.images[145 + g.p().d].blit(g.virtualscreen, (int)g.p().x + g.xboost[g.p().d] - rx,
                                     (int)g.p().y + g.yboost[g.p().d] - ry);
      }
    }
  }

  // Draw drak mothership & guns
  if ((g.drak) && (g.drakms.exist == 1)) {
    g.images[318].blit(g.virtualscreen, (int)g.drakms.x - rx, (int)g.drakms.y - ry);
    g.drakgun.reset();
    while (g.drakgun.next()) {
      g.images[g.drakgun().image[g.drakgun().d]].blit(g.virtualscreen, (int)g.drakms.x + g.drakgun().x - rx,
                                                      (int)g.drakms.y + g.drakgun().y - ry);
    }
  }

  // Draw falling things
  g.fall.reset();
  while (g.fall.next()) {
    g.images[g.fall().image].blit(g.virtualscreen, (int)g.fall().x - rx, (int)g.fall().y - ry);
  }

  // Draw shots
  g.shot.reset();
  while (g.shot.next()) {
    g.images[114].blit(g.virtualscreen, (int)g.shot().x - rx, (int)g.shot().y - ry);
  }

  // Draw laser
  g.laser.reset();
  while (g.laser.next()) {
    g.images[g.laser().image].blit(g.virtualscreen, g.laser().x - rx, g.laser().y - ry);
  }

  // Draw flames
  g.flame.reset();
  while (g.flame.next()) {
    g.images[94 + int(g.flame().time / 5 * GAME_SPEED) % 3].blit(g.virtualscreen, g.flame().x - rx, g.flame().y - ry);
  }

  // Draw smoke
  g.smoke.reset();
  while (g.smoke.next()) {
    g.images[97 + int(g.smoke().time / 7 * GAME_SPEED)].blit(g.virtualscreen, g.smoke().x - rx, (int)g.smoke().y - ry);
  }

  // Draw explosions
  g.explosion.reset();
  while (g.explosion.next()) {
    switch (g.explosion().type) {
    case 0:
      g.images[101 + int(g.explosion().time / 2 * GAME_SPEED)].blit(g.virtualscreen, g.explosion().x - rx,
                                                                    g.explosion().y - ry);
      break;
    case 1:
      g.images[247 + int(g.explosion().time / 2 * GAME_SPEED)].blit(g.virtualscreen, g.explosion().x - rx,
                                                                    g.explosion().y - ry);
      break;
    case 2:
      g.images[231 + int(g.explosion().time * GAME_SPEED)].blit(g.virtualscreen, g.explosion().x - rx,
                                                                g.explosion().y - ry);
      break;
    case 3:
      g.images[204 + int(g.explosion().time / 2 * GAME_SPEED)].blit(g.virtualscreen, g.explosion().x - rx,
                                                                    g.explosion().y - ry);
      break;
    case 4:
      g.images[214 + int(g.explosion().time * GAME_SPEED)].blit(g.virtualscreen, g.explosion().x - rx,
                                                                g.explosion().y - ry);
      break;
    default:
      switch_bad_default("explosion.type", __FILE__, __LINE__);
      break;
    }
  }
}

// Display the scores

void display_score(SDL_Surface *virtualscreen, plane &p, int x, int y, int mission, int targetscore,
                   SDLfont &whitefont, SDLfont &greenfont) {

  if ((mission == 0) || (mission == 1)) {
    char scoretitlestring[] = "Score:";
    whitefont.write(virtualscreen, x, y, scoretitlestring);
    char scorestring[] = " xxxx";
    int score = p.score;

    if (score < 0) {
      scorestring[0] = '-';
      score = -score;
    }
    int thousands = score / 1000;
    int hundreds = (score % 1000) / 100;
    int tens = (score % 100) / 10;
    int units = score % 10;
    scorestring[1] = '0' + thousands;
    scorestring[2] = '0' + hundreds;
    scorestring[3] = '0' + tens;
    scorestring[4] = '0' + units;
    if (p.score >= targetscore) {
      greenfont.write(virtualscreen, x + 56, y, scorestring);
    } else {
      whitefont.write(virtualscreen, x + 56, y, scorestring);
    }
  } else {
    char targettitlestring[] = "Targets:";
    whitefont.write(virtualscreen, x, y, targettitlestring);
    char targetstring[] = "xx";
    int tens = p.targetscore / 10;
    int units = p.targetscore % 10;
    targetstring[0] = '0' + tens;
    targetstring[1] = '0' + units;
    if (p.targetscore == 0) {
      greenfont.write(virtualscreen, x + 72, y, targetstring);
    } else {
      whitefont.write(virtualscreen, x + 72, y, targetstring);
    }
  }
}

// Display the players' info

void display_playinfo(SDL_Surface *virtualscreen, int player, plane &p, int y, shape images[319], int mission,
                      int targetscore, SDLfont &whitefont, SDLfont &greenfont) {

  // Display player
  SDL_Rect rect;
  rect.x = 0;
  rect.y = y;
  rect.w = 16;
  rect.h = 16;
  SDL_FillRect(virtualscreen, &rect, 16);
  images[p.image + 13].blit(virtualscreen, 0, y);
  char playerstring[] = "Player x";
  playerstring[7] = '0' + player;
  whitefont.write(virtualscreen, 24, y, playerstring);
  // Display score
  display_score(virtualscreen, p, 112, y, mission, targetscore, whitefont, greenfont);
  // Display shots
  images[114].blit(virtualscreen, 246, y + 6);
  char shotstring[] = "xx";
  shotstring[0] = '0' + (p.ammo / 10);
  shotstring[1] = '0' + (p.ammo % 10);
  whitefont.write(virtualscreen, 256, y, shotstring);
  // Display bombs
  images[119].blit(virtualscreen, 285, y + 5);
  char bombstring[] = "xx";
  bombstring[0] = '0' + (p.bombs / 10);
  bombstring[1] = '0' + (p.bombs % 10);
  whitefont.write(virtualscreen, 296, y, bombstring);
}

// Display the whole screen

void drawall(gamedata &g) {

  // Set screenheight
  int screenheight = 0;
  if (g.players == 1) {
    // TODO where do these numbers come from?
    screenheight = clamp(GAME_HEIGHT, GAME_HEIGHT, 464);
  } else {
    screenheight = clamp(GAME_HEIGHT, GAME_HEIGHT, 224);
  }
  // Player 1
  {
    // TODO where do these numbers come from?
    int x1 = clamp(int(g.player1->x) - 308, 0, GAME_WIDTH - SCREEN_WIDTH);
    int y1 = clamp(int(g.player1->y) - 54, 0, GAME_HEIGHT - screenheight);
    SDL_Rect srcrect;
    srcrect.x = x1;
    srcrect.y = y1;
    srcrect.w = SCREEN_WIDTH;
    srcrect.h = screenheight;
    SDL_BlitSurface(g.gamescreen, &srcrect, g.virtualscreen, NULL);
    SDL_Rect cliprect;
    cliprect.x = 0;
    cliprect.y = 0;
    cliprect.w = SCREEN_WIDTH;
    cliprect.h = screenheight;
    SDL_SetClipRect(g.virtualscreen, &cliprect);
    drawblits(g, x1, y1);
    SDL_SetClipRect(g.virtualscreen, NULL);
    display_playinfo(g.virtualscreen, 1, *g.player1, screenheight, g.images, g.mission, g.targetscore, g.whitefont,
                     g.greenfont);
  }

  // Player 2
  if (g.players == 2) {
    // TODO where do these numbers come from?
    int x2 = clamp(int(g.player2->x) - 308, 0, GAME_WIDTH - SCREEN_WIDTH);
    int y2 = clamp(int(g.player2->y) - 54, 0, GAME_HEIGHT - 224);
    SDL_Rect srcrect;
    srcrect.x = x2;
    srcrect.y = y2;
    srcrect.w = SCREEN_WIDTH;
    srcrect.h = screenheight;
    SDL_Rect desrect;
    desrect.x = 0;
    desrect.y = 240;
    desrect.w = SCREEN_WIDTH;
    desrect.h = screenheight;
    SDL_BlitSurface(g.gamescreen, &srcrect, g.virtualscreen, &desrect);
    SDL_Rect cliprect;
    cliprect.x = 0;
    cliprect.y = 240;
    cliprect.w = SCREEN_WIDTH;
    cliprect.h = screenheight;
    SDL_SetClipRect(g.virtualscreen, &cliprect);
    drawblits(g, x2, y2 - 240);
    SDL_SetClipRect(g.virtualscreen, NULL);
    display_playinfo(g.virtualscreen, 2, *g.player2, 464, g.images, g.mission, g.targetscore, g.whitefont,
                     g.greenfont);
  }

  // Best AI Score
  if (g.planes > g.players) {
    plane bestp = {};
    bool found = false;
    g.p.reset();
    while (g.p.next()) {
      if ((g.p().control == 0) && (g.p().side > 0)) {
        if (!found) {
          bestp = g.p();
          found = true;
        } else {
          if ((g.mission == 0) || (g.mission == 1)) {
            if (g.p().score > bestp.score) {
              bestp = g.p();
            }
          } else {
            if (g.p().targetscore < bestp.targetscore) {
              bestp = g.p();
            }
          }
        }
      }
    }
    // Display AI
    SDL_Rect rect;
    rect.x = 360;
    rect.y = screenheight;
    rect.w = 16;
    rect.h = 16;
    SDL_FillRect(g.virtualscreen, &rect, 16);
    g.images[bestp.image + 13].blit(g.virtualscreen, 360, screenheight);
    char compstring[] = "Computer (Plane x)";
    compstring[16] = '0' + bestp.side;
    g.whitefont.write(g.virtualscreen, 384, screenheight, compstring);
    // Display score
    display_score(g.virtualscreen, bestp, 536, screenheight, g.mission, g.targetscore, g.whitefont, g.greenfont);
  }

  // Update screen display
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = SCREEN_WIDTH;
  rect.h = SCREEN_HEIGHT;
  SDL_Texture *texture = SDL_CreateTextureFromSurface(g.renderer, g.virtualscreen);
  if (texture == NULL) {
    fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_RenderCopy(g.renderer, texture, &rect, NULL);
  SDL_RenderPresent(g.renderer);
  g.sound.update();
  SDL_DestroyTexture(texture);
}
