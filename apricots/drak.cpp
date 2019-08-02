// Apricots Drak routines
// Author: M.D.Snellgrove
// Date: 26/3/2002
// History:

#include "apricots.h"

// Setup Draks routine

void setup_draks(drakmstype &drakms, linkedlist <drakguntype> &drakgun){

// Setup Mothership
  drakms = DRAKMSINIT;
  drakms.x = drand() * (GAME_WIDTH-296) + 100.0;
// Setup Drakguns
  drakgun.add(DGUN_LASER_LEFT);
  drakgun.add(DGUN_LASER_RIGHT);
  drakgun.add(DGUN_TOP_LEFT);
  drakgun.add(DGUN_TOP_RIGHT);
  drakgun.add(DGUN_SIDE_LEFT);
  drakgun.add(DGUN_SIDE_RIGHT);

}

// Fire laser

void fire_laser(gamedata &g, drakguntype &drakgun, drakmstype &drakms, sampleio &sound,
                building b[MAP_W*2], linkedlist <lasertype> &laser,
                int groundheight[MAP_W]){

// Warm up laser if already activated
  if (drakgun.time > 0){
    drakgun.time++;
    drakgun.image[0] = 307 + int(drakgun.time*GAME_SPEED/4);
  }
// Shoot laser
  if (drakgun.time == int(23/GAME_SPEED)){
// Create laser images
    drakgun.time = 0;
    drakgun.image[0] = 305;
    int targetx = drakms.targetx + drakms.targetmod + drakgun.type;
    int laserx = drakgun.x + int(drakms.x);
    int lasery = drakgun.y + int(drakms.y) + 4;
    lasertype toplaser;
    toplaser.x = laserx;
    toplaser.y = lasery;
    toplaser.image = 313;
    toplaser.time = int(2/GAME_SPEED);
    laser.add(toplaser);
    for (int y = lasery+8; y<=groundheight[int(targetx/2)]-8; y+=8){
      lasertype midlaser;
      midlaser.x = laserx;
      midlaser.y = y;
      midlaser.image = 314;
      midlaser.time = int(2/GAME_SPEED);
      laser.add(midlaser);
    }
    lasertype endlaser;
    endlaser.x = laserx;
    endlaser.y = groundheight[int(targetx/2)] - 8;
    endlaser.image = 314;
    endlaser.time = int(2/GAME_SPEED);
    laser.add(endlaser);
// Destroy buildings
    if (b[targetx].type >= 3) killbuilding(g, b[targetx]);
// Destroy tower
    if (b[targetx].type == 2){
      for (int h = b[targetx].towersize; h>=0; h--){
        killtower(g, b[targetx], 0.0, 0.0, h, 0);
      }
    }
// Destroy planes
    g.p.reset();
    while (g.p.next()){
      if ((int(g.p().y) > lasery) && (abs(int(g.p().x)-laserx)<6) && (g.p().state < 2)){
        g.p().score -= 25;
        g.p().land = 2;
        g.p().s = 0;
        firetype boom;
        boom.x = int(g.p().x);
        boom.y = int(g.p().y);
        boom.type = 0;
        boom.time = 0;
        g.explosion.add(boom);
      }
    }
    sound.play(SOUND_LASER);
  }
// Prime laser for firing
  if ((drakgun.time == 0) && (drakms.movedelay > int(30/GAME_SPEED))){
    int targetx = drakms.targetx + drakms.targetmod + drakgun.type;
    if (b[targetx].type >= 2) drakgun.time = 1;
  }

}

// Create shot from drakgun

void drakgunshoot(drakguntype &drakgun, drakmstype &drakms, linkedlist <shottype> &shot,
                  sampleio &sound, double xmove[17], double ymove[17]){

  double ymod = 10.0;
  if (drakgun.y == 20) ymod = 4.0;
  shottype shell;
  shell.x = drakms.x + drakgun.x+6.0+12.0*xmove[drakgun.d];
  shell.y = drakms.y + drakgun.y+ymod+12.0*ymove[drakgun.d];
  shell.xs = 8.0*xmove[drakgun.d]*GAME_SPEED;
  shell.ys = 8.0*ymove[drakgun.d]*GAME_SPEED;
  shell.side = 0;
  shell.time = int(40/GAME_SPEED);
  shot.add(shell);
  drakgun.reload = int(5/GAME_SPEED);
  sound.play(SOUND_GUNSHOT2);

}

// Fire drakguns

void fire_drakguns(gamedata &g, linkedlist <drakguntype> &drakgun, drakmstype &drakms,
                   linkedlist <plane> &p, sampleio &sound,
                   linkedlist <lasertype> &laser,
                   building b[MAP_W*2], linkedlist <shottype> &shot, int groundheight[MAP_W],
                   double xmove[17], double ymove[17]){

  drakgun.reset();
  while (drakgun.next()){
    if (drakgun().type != 0){
// Fire the laser
      fire_laser(g, drakgun(), drakms, sound, b, laser, groundheight);     
    }else{
// Fire more normal drakgun
// Find new target for drakgun if untargeted
      if (drakgun().target == 0){
        int maxfind = 200;
        p.reset();
        while (p.next()){
          if ((0 != p().side) && (p().state < 2) && (!p().hide)){
            int dx = int(drakms.x) + drakgun().x - int(p().x);
            if (abs(dx) < maxfind){
// See if drakgun can point at potential target
              int dy = int(drakms.y) + drakgun().y - int(p().y);
              int targetd = wrap(int((atan2(double(dy),double(-dx))*8.0/PI)+13.5), 1, 17);
              if (drakgun().image[targetd] != 0){
                maxfind = abs(dx);
                drakgun().target = p().id;
              }
            }
          }
        }
      }else{
// Track the target of the drakgun
        p.reset();
        while (p.next()){
          if (p().id == drakgun().target){
            double dx = drakms.x + double(drakgun().x) - p().x;
            double dy = drakms.y + double(drakgun().y) - p().y;
// Rotate drakgun
            if (drakgun().rotate == 0){
              double smartsine = (dy*p().xs-dx*p().ys)/
                                 (8.0*GAME_SPEED*sqrt(dx*dx+dy*dy));
              smartsine = dlimit(smartsine, -1.0, 1.0);
              double smartangle = asin(smartsine);
              int targetd = wrap(int(((atan2(dy,-dx)-smartangle)*8.0/PI)+13.5), 1, 17);
              int move = sign(wrap(targetd - drakgun().d, -7, 9));
              if (move != 0){
                int newd = wrap(drakgun().d + move, 1, 17);
                if (drakgun().image[newd] != 0){
                  drakgun().d = newd;
                  drakgun().rotate = int(2/GAME_SPEED);
                }else{
                  drakgun().target = 0;
                }
              }
            }
// Shoot drakgun
            if ((drakgun().reload == 0) && (drakgun().target > 0) && (abs(int(dx))<150)){
              drakgunshoot(drakgun(), drakms, shot, sound, xmove, ymove);              
            }
// Lose target
            if ((abs(int(dx))>200) || (p().state>1) || (p().hide)){
              drakgun().target = 0;
            }
          }
        }
      }
// Sort out drakguntimers
      if (drakgun().reload > 0) drakgun().reload--;
      if (drakgun().rotate > 0) drakgun().rotate--;
    }
  }
}

// Launch drak fighter

void launch_drakfighter(drakmstype &drakms, linkedlist <plane> &p, linkedlist <planeclone> &dp){

// Change counters
  drakms.fightersout++;
  drakms.launchdelay = int(50/GAME_SPEED);
  drakms.movedelay += int(20/GAME_SPEED);
  int drakid = 7;
  if (drakms.fighter[0]) drakid = 8;
  if ((drakms.fighter[0]) && (drakms.fighter[1])) drakid = 9;
  drakms.fighter[drakid-7] = true;
// Create new drak fighter
  plane newdrak = DRAK_FIGHTER;
  newdrak.x = drakms.x + 40.0;
  newdrak.y = drakms.y + 18.0;
  newdrak.xs = drakms.xs;
  newdrak.id = drakid;
  p.add(newdrak);
// Create planeclone
  planeclone newclone;
  newclone.x = newdrak.x;
  newclone.y = newdrak.y;
  newclone.xs = newdrak.xs;
  newclone.ys = newdrak.ys;
  newclone.d = newdrak.d;
  newclone.image = newdrak.image;
  newclone.state = newdrak.state;
  newclone.hide = newdrak.hide;
  newclone.id = newdrak.id;
  newclone.side = newdrak.side;
  newclone.collide = false;
  newclone.scoreloss = 0;
  newclone.buildingwin = 0;
  dp.add(newclone);

}


// Drak main routine

void drak_main(gamedata &g){

// Control Mothership
  switch(g.drakms.exist){
    case 0: // Mothership not yet appeared
      g.drakms.movedelay--;
      if (g.drakms.movedelay == 0) g.drakms.exist = 1;
      break;
    case 1: // Mothership active
      if (g.drakms.damage < 30){
// Move downwards
        if (g.drakms.y < 38.0) g.drakms.y += 2.0*GAME_SPEED;
// Reset delay counters
        if (g.drakms.launchdelay > 0) g.drakms.launchdelay--;
        if (g.drakms.movedelay > 0){
           g.drakms.movedelay--;
           if (g.drakms.movedelay == 0) g.drakms.targetx = 0;
        }
// Select target
        if (g.drakms.targetx == 0){
          g.drakms.targetx = int(drand()*(MAP_W*2-16)) + 8;
          if (g.gamemap.b[g.drakms.targetx].type < 2){
            g.drakms.targetx = 0;
            g.drakms.movedelay = 1;
          }else{
// Target found
            g.drakms.targetmod = int(drand()*2)*2 - 1;
            if ((g.drakms.lgun) && (!g.drakms.rgun)) g.drakms.targetmod = 1;
            if ((!g.drakms.lgun) && (g.drakms.rgun)) g.drakms.targetmod = -1;
          }
// Apply Brakes
          if (g.drakms.xs > 0.0){
            g.drakms.xs = dlimit(g.drakms.xs - 0.5 * GAME_SPEED * GAME_SPEED,
                                 0.0, 4.0 * GAME_SPEED);
          }else{
            g.drakms.xs = dlimit(g.drakms.xs + 0.5 * GAME_SPEED * GAME_SPEED,
                                  -4.0 * GAME_SPEED, 0.0);
          }
        }else{
// Follow Target

          int dx = (g.drakms.targetx + g.drakms.targetmod)*16 - 48 - int(g.drakms.x);
// Wait above target
          if ((dx == 0) && (g.drakms.movedelay == 0)) g.drakms.movedelay = int(50/GAME_SPEED);
          if (dx == 0) g.drakms.xs = 0.0;
// Calculate stopping distance
          double stopd = (g.drakms.xs * g.drakms.xs * 2.0 +
                          fabs(g.drakms.xs) / GAME_SPEED) * 0.5;
          if ((abs(dx) > stopd) && (g.drakms.movedelay == 0)){
// Accelerate
            g.drakms.xs = dlimit(g.drakms.xs + 0.5 * GAME_SPEED * GAME_SPEED * sign(dx),
                                                    -4.0 * GAME_SPEED, 4.0 * GAME_SPEED);
          }else{
// Apply Brakes
            if (g.drakms.xs > 0.0){
              g.drakms.xs = dlimit(g.drakms.xs - 0.5 * GAME_SPEED * GAME_SPEED,
                                   0.0, 4.0 * GAME_SPEED);
            }else{
              g.drakms.xs = dlimit(g.drakms.xs + 0.5 * GAME_SPEED * GAME_SPEED,
                                    -4.0 * GAME_SPEED, 0.0);
            }
          }
// Target destroyed
          if ((g.gamemap.b[g.drakms.targetx].type == 0) && (g.drakms.movedelay == 0)){
            g.drakms.targetx = 0;
          }
        }
// Move Sideways
        g.drakms.x += g.drakms.xs;
// Fire Drak guns
        fire_drakguns(g, g.drakgun, g.drakms, g.p, g.sound, g.laser, g.gamemap.b, g.shot,
                      g.gamemap.groundheight, g.xmove, g.ymove);
// Launch fighters
        if ((g.drakms.fightersout < 3) && (!(g.drakms.y < 38)) && (int(drand()*20)==10) &&
           (g.drakms.launchdelay == 0) && (g.drakms.movedelay > 0)){
          launch_drakfighter(g.drakms, g.p, g.dp);
        }

      }else{
// Destroy drak mothership
        for (int i=0;i<6;i++){
          int x = int(g.drakms.x) + i*16;
          firetype boom;
          boom.x = x + int(drand()*16) - 8;
          boom.y = int(g.drakms.y);
          boom.time = 0;
          boom.type = 0;
          g.explosion.add(boom);
          for (int m=1;m<=2;m++){
            falltype shrapnel;
            shrapnel.x = double(x) + 6.0;
            shrapnel.y = g.drakms.y + 8.0;
            shrapnel.xs = g.drakms.xs + (drand()*5.0 - 2.5)*GAME_SPEED;
            shrapnel.ys = (1.0 - drand()*4.0)*GAME_SPEED;
            shrapnel.image = 235 + int(drand()*3);
            shrapnel.type = 1;
            g.fall.add(shrapnel);
            falltype bigshrapnel;
            bigshrapnel.x = double(x);
            bigshrapnel.y = g.drakms.y + 8.0;
            bigshrapnel.xs = g.drakms.xs + (drand()*5.0 - 2.5)*GAME_SPEED;
            bigshrapnel.ys = (1.0 - drand()*4.0)*GAME_SPEED;
            bigshrapnel.image = 315 + int(drand()*3);
            bigshrapnel.side = 0;
            bigshrapnel.type = 2;
            g.fall.add(bigshrapnel);
          }
        }
        g.drakgun.reset();
        while (g.drakgun.next()){
          falltype gunshrapnel;
          gunshrapnel.x = g.drakms.x + double(g.drakgun().x);
          gunshrapnel.y = g.drakms.y + double(g.drakgun().y);
          gunshrapnel.xs = g.drakms.xs + (drand()*5.0 - 2.5)*GAME_SPEED;
          gunshrapnel.ys = (1.0 - drand()*4.0)*GAME_SPEED;
          gunshrapnel.image = g.drakgun().image[g.drakgun().d];
          gunshrapnel.side = 0;
          gunshrapnel.type = 2;
          g.fall.add(gunshrapnel);
          g.drakgun.kill();
        }         
        g.drakms.exist = 2;
        g.sound.play(SOUND_FUELEXPLODE);
      }
      break;
    case 2: // Drak mothership destroyed
      break;
  }

}
