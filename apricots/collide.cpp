// Apricots collision detection routine
// Author: M.D.Snellgrove
// Date: 26/3/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Changes for graphics related stuff.

#include "apricots.h"

void detect_collisions(gamedata &g){

// Check for ground collision
  if (g.p().land == 2){
    if ((g.gamemap.ground.collide(0,0,
        g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y)) ||
        (int(g.p().x) < -16) || (int(g.p().x) > GAME_WIDTH)){
// Check for landing plane
      int dx = int(g.p().x) - 32*g.base[g.p().side].mapx -
               g.base[g.p().side].runwayx;
      if ((dx > -5) && (dx < 5 + g.base[g.p().side].runwaylength) &&
          (g.p().state==0) && (!g.p().drak) && ((g.p().d==12) ||
           (g.p().d==11) || (g.p().d==7) || (g.p().d==6))){
        g.p().land = 3;
        g.p().y = g.base[g.p().side].planey;
        g.p().d = 18 - g.base[g.p().side].planed;
        g.p().xs = 2.0 * g.xmove[g.p().d] * GAME_SPEED;
        g.p().s = 2.0 * GAME_SPEED;
      }else{
// Crashed
        g.p().score -= 25;
// Mission 1 scoreloss
        if ((g.mission == 1) && (g.p().score > g.targetscore-200)){
          g.p().score = g.targetscore - 200;
        }
        g.p().state = 3;
        g.p().crash = 0;
        g.p().xs = 0;
        g.p().ys = 0;
        g.p().s = 0.0;

        int px = limit(int((g.p().x+24.0)/32),0,MAP_W-1);
        if ((g.gamemap.groundheight[px] == GAME_HEIGHT - 2) &&


            (g.p().y > GAME_HEIGHT - 21)){ // hits sea
          firetype splash;
          splash.x = int(g.p().x);
          splash.y = GAME_HEIGHT - 20;
          splash.time = 0;
          splash.type = 3;
          g.explosion.add(splash);
          g.sound.play(SOUND_SPLASH);
        }else{ // hits land
// Draw wreck
          if ((g.p().y > 3.0) && (g.p().x > 0) && (g.p().x < GAME_WIDTH-16)){
            g.images[g.p().image+17].blit(g.gamescreen,(int)g.p().x,(int)g.p().y-3);
          }
          firetype boom;
          boom.x = int(g.p().x);
          boom.y = int(g.p().y);
          boom.time = 0;
          boom.type = 0;
          g.explosion.add(boom);
          firetype fire;
          fire.x = int(g.p().x);
          fire.y = int(g.p().y) - 7;
          fire.time = int(210/GAME_SPEED);
          g.flame.add(fire);
          for (int i=0;i<3;i++){
            falltype shrapnel;
            shrapnel.x = g.p().x + 6.0;
            shrapnel.y = g.p().y;
            shrapnel.xs = (2.0*drand() - 1.0) * GAME_SPEED;
            shrapnel.ys = (-2.0*drand() - 1.0) * GAME_SPEED;
            shrapnel.image = g.p().shrapnelimage + int(drand()*3);
            shrapnel.type = 1;
            g.fall.add(shrapnel);
          }
          g.sound.play(SOUND_GROUNDHIT);
        }
      }
    }
  }

// Check for drak mothership collision

  if ((g.drakms.exist == 1) && (g.images[318].collide((int)g.drakms.x,(int)g.drakms.y,
                         g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y))){
// Crashed
    g.p().score -= 25;
// Mission 1 scoreloss
    if ((g.mission == 1) && (g.p().score > g.targetscore-200)){
      g.p().score = g.targetscore - 200;
    }
    g.p().state = 3;
    g.p().crash = 0;
    g.p().xs = 0;
    g.p().ys = 0;
    g.p().s = 0.0;
// Damage to drak mothership
    g.drakms.damage++;
    firetype boom;


    boom.x = int(g.p().x);
    boom.y = int(g.p().y);
    boom.time = 0;
    boom.type = 0;
    g.explosion.add(boom);
    for (int i=0;i<3;i++){
      falltype shrapnel;
      shrapnel.x = g.p().x + 6.0;
      shrapnel.y = g.p().y;
      shrapnel.xs = (2.0*drand() - 1.0) * GAME_SPEED;
      shrapnel.ys = (-2.0*drand()) * GAME_SPEED;
      shrapnel.image = g.p().shrapnelimage + int(drand()*3);
      shrapnel.type = 1;
      g.fall.add(shrapnel);
    }
    g.sound.play(SOUND_EXPLODE);
  }


// Check for building etc collisions
  int px = int((g.p().x+8)/16);
  for (int x=px;x<=px+1;x++){
    switch(g.gamemap.b[x].type){
      case 1: case 3: case 4: case 5:// buildings and trees
        if (g.images[g.gamemap.b[x].image].collide(g.gamemap.b[x].x,
            g.gamemap.b[x].y,g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y)){
          g.p().state = 2;
          g.p().land = 2;
          g.p().xs = g.p().xs * 0.5;
          g.p().ys = g.p().ys * 0.5;
          g.p().s = 0.0;
// Add 25% of points value for crashing into building (or full value if -ve)
          if (g.p().side != g.gamemap.b[x].side){
            if (g.gamemap.b[x].points < 0){
              g.p().score += g.gamemap.b[x].points;
            }else{
              g.p().score += int(g.gamemap.b[x].points * 0.25);
            }
          }
          killbuilding(g, g.gamemap.b[x]);
        }
        break;

      case 2: // towers
        { int ty = limit(int(g.p().y),
                    g.gamemap.b[x].y -g.gamemap.b[x].towersize*16,int(g.p().y));
          if (g.images[197].collide(g.gamemap.b[x].x,ty,
              g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y)){
            g.p().state = 2;
            g.p().land = 2;
            g.p().xs = g.p().xs * 0.5;
            g.p().ys = g.p().ys * 0.5;
            g.p().s = 0.0;
// Calculate height of tower strike
            int h = limit(int((g.gamemap.b[x].y - g.p().y)/16),0,100);
// Knock off score
            g.p().score -= 40 * (g.gamemap.b[x].towersize + 1 - h);
            killtower(g, g.gamemap.b[x], g.p().xs*0.5, g.p().ys*0.5, h, g.p().id);
          }
        }
        break;
    }
  }

// Shot collisions
  g.shot.reset();
  while (g.shot.next()){
    if (g.images[114].collide((int)g.shot().x,(int)g.shot().y,
          g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y)){
      if (g.p().state < 2){
        g.p().score -= 50;
// Find owner of bullet and reward them
        if ((g.shot().side > 0) && (g.shot().side != g.p().side)){
          g.dp.reset();
          while (g.dp.next()){
            if (g.dp().side == g.shot().side){
              g.dp().scoreloss -= 150;
            }
          }
        }
      }
      g.p().state = 2;
      g.p().land = 2;
      g.p().s = 0.0;
      firetype boom;
      boom.x = int(g.p().x);
      boom.y = int(g.p().y);
      boom.time = 0;
      boom.type = 0;
      g.explosion.add(boom);
      g.shot.kill();
      g.sound.play(SOUND_EXPLODE);
    }

  }

// Drak gun collisions
  if (g.drakms.exist == 1){
    g.drakgun.reset();
    while (g.drakgun.next()){
      if (g.images[g.drakgun().image[g.drakgun().d]].collide(
          (int)g.drakms.x+g.drakgun().x, (int)g.drakms.y+g.drakgun().y,
          g.images[g.p().image+g.p().d], (int)g.p().x, (int)g.p().y)){
        if (g.p().state < 2) g.p().score -= 50;
        g.p().score += 50;
        g.p().state = 2;
        g.p().land = 2;
        g.p().xs = g.p().xs * 0.5;
        g.p().ys = g.p().ys * 0.5;
        g.p().s = 0.0;
        firetype boom;
        boom.x = int(g.p().x);
        boom.y = int(g.p().y);
        boom.time = 0;
        boom.type = 0;
        g.explosion.add(boom);
// Reset laser flags
        if (g.drakgun().type == -1) g.drakms.lgun = false;
        if (g.drakgun().type == 1) g.drakms.rgun = false;
        g.drakgun.kill();
        g.sound.play(SOUND_EXPLODE);
      }
    }
  }

  if (g.p().state < 2){

// Flame collisions
    g.flame.reset();
    while (g.flame.next()){
      if (g.images[94].collide(g.flame().x,g.flame().y,
          g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y)){
        g.p().score -= 10;
        g.p().state = 2;
        g.p().land = 2;
        g.p().xs = g.p().xs * 0.5;
        g.p().ys = g.p().ys * 0.5;
        g.p().s = 0.0;
        firetype boom;
        boom.x = int(g.p().x);
        boom.y = int(g.p().y);
        boom.time = 0;
        boom.type = 0;
        g.explosion.add(boom);
        g.sound.play(SOUND_EXPLODE);
      }
    }

// Falling collisions
    g.fall.reset();
    while (g.fall.next()){
      if (g.images[g.fall().image].collide((int)g.fall().x,(int)g.fall().y,
          g.images[g.p().image+g.p().d],(int)g.p().x,(int)g.p().y)){
        g.p().score -= 50;
// Work out which plane's bomb hits target
        if ((g.fall().side > 0) && (g.fall().side != g.p().side)){
          g.dp.reset();
          while (g.dp.next()){
            if (g.dp().side == g.fall().side){
              g.dp().scoreloss -= 100;
            }
          }
        }
        g.p().state = 2;
        g.p().land = 2;
        g.p().xs = g.p().xs * 0.5;
        g.p().ys = g.p().ys * 0.5;
        g.p().s = 0.0;
        firetype boom;
        boom.x = int(g.p().x);
        boom.y = int(g.p().y);
        boom.time = 0;
        boom.type = 0;
        g.explosion.add(boom);
        g.fall.kill();
        g.sound.play(SOUND_EXPLODE);
      }
    }

  }

}

// Kill building or tree

void killbuilding(gamedata &g, building &b){

// Draw dead image
  //gl_setcontext(&g.gamescreen);
  g.images[b.deadimage].blit(g.gamescreen,b.x, b.y);
  draw_dither(g.gamescreen, b.x, b.y, 16, 24);
// Subtract points if owned building
  if (b.side > 0)
  g.dp.reset();
  while (g.dp.next()){
    if (b.side == g.dp().side){
      g.dp().scoreloss += int(b.points*0.5);
    }else{
      g.dp().buildingwin--;
    }
  }
  if (b.image != 71){ // not a fuel canister
    if (b.type != 1){ // not a tree
      firetype boom;
      boom.x = b.x;
      boom.y = b.y;
      boom.time = 0;
      boom.type = 0;
      g.explosion.add(boom);
      firetype fire;
      fire.x = b.x;
      fire.y = b.y;
      fire.time = 0;
      g.flame.add(fire);
    }
    for (int i=0;i<3;i++){
      falltype shrapnel;
      shrapnel.x = double(b.x) + 6.0;
      shrapnel.y = double(b.y);
      shrapnel.xs = (2.0*drand() - 1.0) * GAME_SPEED;
      shrapnel.ys = (-1.5*drand() - 1.5) * GAME_SPEED;
      switch(i){
        case 0: case 1:
          shrapnel.image = b.shrapnelimage + int(drand()*3);
          break;
        case 2:
          shrapnel.image = b.shrapnelimage2 + int(drand()*3);
          break;
      }
      shrapnel.type = 1;
      if (b.type == 1) shrapnel.type = 0;
      g.fall.add(shrapnel);
    }
    g.sound.play(SOUND_EXPLODE);
  }else{ // fuel blows up
    firetype boom;
    boom.x = int(b.x-8);
    boom.y = int(b.y-16);
    boom.time = 0;
    boom.type = 1;
    g.explosion.add(boom);
    firetype fire;
    fire.x = b.x;
    fire.y = b.y;
    fire.time = 0;
    g.flame.add(fire);
    fire.x = b.x - 8;
    fire.time = int(drand()*10.0*GAME_SPEED);
    g.flame.add(fire);
    fire.x = b.x + 8;
    fire.time = int(drand()*10.0*GAME_SPEED);
    g.flame.add(fire);
    int shrapnelbits = 7 + int(drand()*4);
    for (int i=0;i<shrapnelbits;i++){
      falltype shrapnel;
      shrapnel.x = double(b.x) + 6.0;
      shrapnel.y = double(b.y);
      shrapnel.xs = (2.0*drand() - 1.0) * GAME_SPEED;
      shrapnel.ys = (-2.0*drand() - 4.0) * GAME_SPEED;
      shrapnel.image = b.shrapnelimage + int(drand()*3);
      shrapnel.type = 1;
      g.fall.add(shrapnel);
    }
    g.sound.play(SOUND_FUELEXPLODE);
  }
// Check for radar or gun kills
  switch(b.type){
    case 4: // radar
      g.radar.reset();
      while (g.radar.next()){
        if (b.id == g.radar().id) g.radar.kill();
      }
      break;
    case 5: // guns
      g.gun.reset();
      while (g.gun.next()){
        if (b.id == g.gun().id) g.gun.kill();
      }
      break;
  }
// Remove the building
  b.type = 0;

}

// Kill a tower

void killtower(gamedata &g, building &b, double xs, double ys, int h, int side){

// Redraw sky and wreckage
  SDL_Rect rect;
    rect.x = b.x;
    rect.y = b.y - b.towersize*16 - 8;
    rect.w = 16;
    rect.h = (b.towersize + 1 - h)*16 + 8;
  SDL_FillRect(g.gamescreen,&rect, 16);
  g.images[198].blit(g.gamescreen,b.x, b.y - h*16);
  draw_dither(g.gamescreen, b.x, b.y - b.towersize*16 - 8, 16,
              (b.towersize + 1 - h)*16 + 8);
  for (int i=h+1;i<=b.towersize;i++){
    falltype fragment;
    fragment.x = double(b.x);
    fragment.y = double(b.y) - i*16.0;
    fragment.xs = xs;
    fragment.ys = ys;
    fragment.image = 197;
    fragment.side = side;
    fragment.type = 2;
    g.fall.add(fragment);
    falltype shrapnel;
    shrapnel.x = double(b.x) + 6.0;
    shrapnel.y = double(b.y) - i*16.0;
    shrapnel.xs = (2.0*drand() - 1.0) * GAME_SPEED;
    shrapnel.ys = (-1.5*drand() - 1.5) * GAME_SPEED;
    shrapnel.image = b.shrapnelimage + int(drand()*3);
    shrapnel.type = 1;
    g.fall.add(shrapnel);
  }
  if ((b.deadimage > 0) && (h<b.towersize)){
    falltype roof;
    roof.x = double(b.x);
    roof.y = double(b.y) - (b.towersize+1)*16.0;
    roof.xs = xs;
    roof.ys = ys;
    roof.image = b.deadimage;
    roof.type = 1;
    g.fall.add(roof);
  }
  for (int j=0;j<3;j++){
    falltype shrapnel;
    shrapnel.x = double(b.x) + 6.0;
    shrapnel.y = double(b.y) - h*16.0;
    shrapnel.xs = (2.0*drand() - 1.0) * GAME_SPEED;
    shrapnel.ys = (-1.5*drand() - 1.5) * GAME_SPEED;
    shrapnel.image = b.shrapnelimage + int(drand()*3);
    shrapnel.type = 1;
    g.fall.add(shrapnel);
  }  
// Explosion at top
  firetype boom; 
  boom.x = b.x;
  boom.y = b.y - h*16;
  boom.time = 0;
  boom.type = 0;
  g.explosion.add(boom);
// Fire if tower levelled
  if (h == 0){
    firetype fire;
    fire.x = b.x;
    fire.y = b.y;
    fire.time = 0;
    g.flame.add(fire);
  }
// Recalculate intelligence map
  setup_intelligence(g.gamemap);

// Reduce height of tower
  b.towersize = h - 1;
// Remove tower if levelled
  if (h == 0){
    b.type = 0;
  }else{
    b.deadimage = 199;
  }
  g.sound.play(SOUND_EXPLODE);

}
