// Apricots falling object collision routine
// Author: M.D.Snellgrove
// Date: 25/3/2002
// History:

#include "apricots.h"

bool fall_collision(gamedata &g, falltype &fall){

// Check for ground collision
  if ((g.gamemap.ground.collide(0,0,
        g.images[fall.image],(int)fall.x,(int)fall.y)) ||
        (int(fall.x) < -16) || (int(fall.x) > GAME_WIDTH) ||
        (int(fall.y) > GAME_HEIGHT)){
    switch(fall.type){
      case 1: // Shrapnel
        { int px = limit(int((fall.x+18.0)/32),0,MAP_W-1); 
          if ((g.gamemap.groundheight[px] == GAME_HEIGHT - 2) &&
              (fall.y > GAME_HEIGHT - 11)){ // hits sea
            firetype splash;
            splash.x = int(fall.x) - 5; 
            splash.y = GAME_HEIGHT - 20;
            splash.time = 0;
            splash.type = 4;
            g.explosion.add(splash);
          }else{ // hits land
            firetype boom;
            boom.x = int(fall.x);
            boom.y = int(fall.y);
            boom.time = 0;
            boom.type = 2;
            g.explosion.add(boom);
          }
        }
        break;

      case 2: case 3: // Large bits and bombs
        if (fall.type == 3) fall.x -= 5;
        { int px = limit(int((fall.x+24.0)/32),0,MAP_W-1); 
          if ((g.gamemap.groundheight[px] == GAME_HEIGHT - 2) &&
              (fall.y > GAME_HEIGHT - 21)){ // hits sea
            firetype splash;
            splash.x = int(fall.x);
            splash.y = GAME_HEIGHT - 20;
            splash.time = 0;
            splash.type = 3;
            g.explosion.add(splash);
            g.sound.play(SOUND_SPLASH);
          }else{ // hits land
            firetype boom;
            boom.x = int(fall.x);
            boom.y = int(fall.y);
            boom.time = 0;
            boom.type = 0;
            g.explosion.add(boom);
            g.sound.play(SOUND_GROUNDHIT);
          }
        }
        break;
      
    }
    return true;
  }

// Check for drak mothership collision

  if ((g.drakms.exist == 1) && (g.images[318].collide((int)g.drakms.x,(int)g.drakms.y,
        g.images[fall.image],(int)fall.x,(int)fall.y))){
    switch(fall.type){
      case 1: // Shrapnel
        { firetype boom;
          boom.x = int(fall.x);
          boom.y = int(fall.y);
          boom.time = 0;
          boom.type = 2;
          g.explosion.add(boom);
        }
        break;

      case 2: case 3: // Large bits and bombs
        if (fall.type == 3) fall.x -= 5;
        { firetype boom;
          boom.x = int(fall.x);
          boom.y = int(fall.y);
          boom.time = 0;
          boom.type = 0;
          g.explosion.add(boom);
        }
        g.sound.play(SOUND_GROUNDHIT);
// Damage mothership
        g.drakms.damage += 2;
// Find which plane (if any) the fall belongs to
        if (fall.side > 0){
          g.p.reset();
          while (g.p.next()){
            if (fall.side == g.p().side){

// Add to score
              g.p().score += 40;
            }
          }
        }
        break;

    }
    return true;
  }

  if (fall.type < 2) return false;

// Check for building etc collisions
  int px = int((fall.x+8)/16);
  for (int x=px;x<=px+1;x++){
    switch(g.gamemap.b[x].type){
      case 1: case 3: case 4: case 5:// buildings and trees
        if (g.images[g.gamemap.b[x].image].collide(g.gamemap.b[x].x,
            g.gamemap.b[x].y,g.images[fall.image],(int)fall.x,(int)fall.y)){

// Find which plane (if any) the fall belongs to
          if ((fall.side > 0) && (fall.side != g.gamemap.b[x].side)){
            g.p.reset();
            while (g.p.next()){
              if (fall.side == g.p().side){
// Add 25% of points value for crashing into building (or full value if -ve)
                if ((g.gamemap.b[x].points > 0) && (fall.type == 2)){
                  g.p().score += int(g.gamemap.b[x].points * 0.25);
                }else{
                  g.p().score += g.gamemap.b[x].points;
                }
              }
            }
          }
          killbuilding(g, g.gamemap.b[x]);
          return true;
        }
        break;

      case 2: // towers
        { int ty = limit(int(fall.y),
                    g.gamemap.b[x].y -g.gamemap.b[x].towersize*16,int(fall.y));       
          if (g.images[197].collide(g.gamemap.b[x].x,ty,
              g.images[fall.image],(int)fall.x,(int)fall.y)){
// Calculate height of tower strike
            int h = limit(int((g.gamemap.b[x].y - fall.y)/16),0,100);
// Find which plane (if any) the fall belongs to
            if (fall.side > 0){
              g.p.reset();
              while (g.p.next()){
                if (fall.side == g.p().side){
// Knock off score
                  g.p().score -= 40 * (g.gamemap.b[x].towersize + 1 - h);
                }
              }
            }
            killtower(g, g.gamemap.b[x], fall.xs*0.25, fall.ys*0.25, h, fall.side);
	    return true;
          }
        }
        break;
    }
  }

// Drak gun collisions
  if (g.drakms.exist == 1){
    g.drakgun.reset();
    while (g.drakgun.next()){ 
      if (g.images[g.drakgun().image[g.drakgun().d]].collide(
            (int)g.drakms.x+g.drakgun().x, (int)g.drakms.y+g.drakgun().y,
            g.images[fall.image], (int)fall.x, (int)fall.y)){
// Find out which plane the fall belongs to
        if (fall.side > 0){
          g.p.reset();
          while (g.p.next()){
            if (fall.side == g.p().side){
// Add to score
              g.p().score += 200;
            }
          }
        }
// Explosion
        firetype boom;
        boom.x = int(fall.x);
        boom.y = int(fall.y);
        boom.time = 0;
        boom.type = 0;
        g.explosion.add(boom);
// Remove laser flags
        if (g.drakgun().type == -1) g.drakms.lgun = false;
        if (g.drakgun().type == 1) g.drakms.rgun = false;
// Kill the gun
        g.drakgun.kill();
        g.sound.play(SOUND_EXPLODE);
        return true;
      }
    }
  }

  return false;

}
