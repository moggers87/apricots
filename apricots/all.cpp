// Apricots all routine
// Author: M.D.Snellgrove
// Date: 26/3/2002
// History:

// Changes by M Snellgrove 6/7/2003
// drand() function needed for Windows portability now located here

#include "apricots.h"

// Random number generator function

double drand(){

  return (((double)(rand() % RAND_MAX)) / ( ((double)RAND_MAX) - 1.0 ));

}

// Wrap function

int wrap(int n, int min, int max){

  return ((((n-min) % (max-min)) + max-min) % (max-min)) + min;

}

// Limit function (integers)

int limit(int n, int min, int max){

  if (n > max) return max;
  if (n < min) return min;
  return n;

}

// Limit function (doubles)

double dlimit(double n, double min, double max){

  if (n > max) return max;
  if (n < min) return min;
  return n;

}

// Sign function

int sign(int n){

  if (n > 0) return 1;
  if (n < 0) return -1;
  return 0;

}

// Animate smoke

void animate_smoke(linkedlist <smoketype> &smoke){

  smoke.reset();
  while(smoke.next()){
    smoke().y = smoke().y - (2.0*GAME_SPEED);
    smoke().time++;
    if (smoke().time == int(28/GAME_SPEED)) smoke.kill();
  }

}

// Animate flames

void animate_flames(linkedlist <firetype> &flame,
                    linkedlist <smoketype> &smoke){

  flame.reset();
  while(flame.next()){
    flame().time++;
    if (flame().time % (int(10/GAME_SPEED)) == 0){
      smoketype newsmoke;
      newsmoke.x = flame().x;
      newsmoke.y = flame().y;
      newsmoke.time = 0;
      smoke.add(newsmoke);
    }
    if (flame().time == int(280/GAME_SPEED)) flame.kill();
  }

}

// Animate explosions

void animate_explosions(linkedlist <firetype> &explosion){

  explosion.reset();
  while(explosion.next()){
    explosion().time++;
    int maxtime = 0;
    switch(explosion().type){
      case 0:
        maxtime = int(14/GAME_SPEED);
        break;
      case 1:
        maxtime = int(22/GAME_SPEED);
        break;
      case 2:
        maxtime = int(4/GAME_SPEED);
        break;
      case 3:
        maxtime = int(20/GAME_SPEED);
        break;
      case 4:
        maxtime = int(5/GAME_SPEED);
        break;
    }
    if (explosion().time == maxtime) explosion.kill();
  }

}

// Rotate radars

void rotate_radars(linkedlist <radartype> &radar){

  radar.reset();
  while(radar.next()){
    radar().rotate = wrap(radar().rotate+1,0,int(3/GAME_SPEED));
    if (radar().rotate == 0) radar().image =
                             wrap(radar().image+1,238,246);
  }

}

// Decay lasers

void decay_lasers(linkedlist <lasertype> &laser){

  laser.reset();
  while (laser.next()){
    laser().time--;
    if (laser().time == 0) laser.kill();
  }

}

// Clone planes (for collision detection purposes)

void clone_planes(linkedlist <plane> &p, linkedlist <planeclone> &dp,
                  int mission, int &winner){

// Do the cloning
  p.reset();
  dp.reset();
  while (p.next()){
    dp.next();
    dp().x = p().x;
    dp().y = p().y;
    dp().xs = p().xs;
    dp().ys = p().ys;
    dp().d = p().d;
    dp().state = p().state;
    dp().hide = p().hide;
// Conduct Scoreloss
    p().score -= dp().scoreloss;
    dp().scoreloss = 0;
    p().targetscore = dp().buildingwin;
// Check for mission 2 win
    if ((mission == 2) && (dp().buildingwin == 0) && (p().land == 0) &&
         (!p().drak)){
      winner = p().side;
      p().targetx = -10;
    }      
  }

}

// Check for planes colliding with each other

void plane_collisions(gamedata &g){

  g.p.reset();
  while (g.p.next()){
    g.dp.reset();
    while (g.dp.next()){
      if (g.p().id > g.dp().id){
        if ((g.p().state<3) && (g.dp().state<3) &&
            ((g.p().state<2) || (g.dp().state<2))){
          if (g.images[g.p().image+g.p().d].collide(int(g.p().x),int(g.p().y),
             g.images[g.dp().image+g.dp().d],int(g.dp().x),int(g.dp().y))){
// Planes have collided
             if (g.p().state < 2) g.p().score -= 25;
             g.p().state = 2;
             g.p().land = 2;
             g.p().xs = g.p().xs * 0.5;
             g.p().ys = g.p().ys * 0.5;
             g.p().s = 0.0;
             g.dp().collide = true;
             firetype bang;
             bang.x = int(g.p().x);
             bang.y = int(g.p().y);
             bang.type = 0;
             bang.time = 0;
             g.explosion.add(bang);
             falltype shrapnel;
             shrapnel.x = g.p().x + 6.0;
             shrapnel.y = g.p().y;
             shrapnel.xs = g.p().xs;
             shrapnel.ys = g.p().ys;
             shrapnel.image = g.p().shrapnelimage + int(drand()*3);
             shrapnel.type = 1;
             g.fall.add(shrapnel);
             g.sound.play(SOUND_EXPLODE);
          }
        }
      }
    }
  }

// Check to see if plane is already hit by another
  g.p.reset();
  g.dp.reset();
  while (g.p.next()){
    g.dp.next();
    if (g.dp().collide){
      if (g.p().state < 2) g.p().score -= 25;
      g.p().state = 2;
      g.p().land = 2;
      g.p().xs = g.p().xs * 0.5;
      g.p().ys = g.p().ys * 0.5;
      g.p().s = 0.0;
      g.dp().collide = false;
      falltype shrapnel;
      shrapnel.x = g.p().x + 6.0;
      shrapnel.y = g.p().y;
      shrapnel.xs = g.p().xs;
      shrapnel.ys = g.p().ys;
      shrapnel.image = g.p().shrapnelimage + int(drand()*3);
      shrapnel.type = 1;
      g.fall.add(shrapnel);
    }
  }
      
}

// Move falls

void move_falls(gamedata &g){

  g.fall.reset();
  while(g.fall.next()){
    g.fall().ys += 0.1 * GAME_SPEED * GAME_SPEED;
    g.fall().x += g.fall().xs;
    g.fall().y += g.fall().ys;
    if (g.fall().type == 3){
      if (g.fall().rotatedelay == 0){
        g.fall().image = wrap(g.fall().image+g.fall().bombrotate,115,123);
        g.fall().rotatedelay = int(1/GAME_SPEED);
      }else{
        g.fall().rotatedelay--;
      }
    }
    if (fall_collision(g, g.fall())){
      g.fall.kill();
    }

  }

}

// Move shots

void move_shots(linkedlist <shottype> &shot, shape &ground, shape &shotimage,
                shape &drakmsimage, drakmstype &drakms){

  shot.reset();
  while (shot.next()){
// move
    shot().x += shot().xs;
    shot().y += shot().ys;
    shot().time--;
// ground collisions / off screen / timed out / drakms collision
    if ((ground.collide(0, 0, shotimage, (int)shot().x, (int)shot().y)) ||
        (shot().x < -16.0) || (shot().x > GAME_WIDTH) || (shot().y < -16.0) ||
        (shot().y > GAME_HEIGHT) || (shot().time == 0) ||
        ((drakms.exist == 1) && (drakmsimage.collide((int)drakms.x,(int)drakms.y,
                                   shotimage,(int)shot().x,(int)shot().y)))){
      shot.kill();
    }
  }

}

// Create shot from gun

void gunshoot(guntype &gun, linkedlist <shottype> &shot, sampleio &sound,
              double xmove[17], double ymove[17]){

  int d = ((20-gun.d) % 16) + 1;
  shottype shell;
  shell.x = gun.x+6.0+12.0*xmove[d];
  shell.y = gun.y+10.0+12.0*ymove[d];
  shell.xs = 8.0*xmove[d]*GAME_SPEED;
  shell.ys = 8.0*ymove[d]*GAME_SPEED;
  shell.side = gun.side;
  shell.time = int(40/GAME_SPEED);
  shot.add(shell);
  gun.ammo--;
  gun.firedelay = int(3/GAME_SPEED);
  sound.play(SOUND_GUNSHOT);

}

// Fire guns
void fire_guns(linkedlist <guntype> &gun, linkedlist <plane> &p, sampleio &sound,
               building b[MAP_W*2], linkedlist <shottype> &shot,
               double xmove[17], double ymove[17]){


  gun.reset();
  while (gun.next()){
// Find new target for gun if untargeted
    if (gun().target == 0){
      int maxfind = 200;
      p.reset();
      while (p.next()){
        if ((gun().side != p().side) && (p().state < 2) && (p().land==2) &&
            (gun().y > p().y) && (!p().hide)){
          int dx = abs(gun().x - int(p().x));
          if (dx < maxfind){
            maxfind = dx;
            gun().target = p().id;
          }
        }
      }
    }else{
// Track the target of the gun
      p.reset();
      while (p.next()){
        if (p().id == gun().target){
          p().gunthreat = gun().xpos;
          double dx = p().x - double(gun().x);        
          double dy = double(gun().y) - p().y;
// Rotate gun
          if ((gun().rotate == 0) && (dy > 0)){        
            double smartsine = (dy*p().xs+dx*p().ys)/
                               (8.0*GAME_SPEED*sqrt(dx*dx+dy*dy));
            smartsine = dlimit(smartsine, -1.0, 1.0);
            double smartangle = asin(smartsine);
            int move = sign(int(((atan(dx/dy)+smartangle)*8.0/PI)+4.5)-gun().d);
            if (move != 0){

              gun().d = limit(gun().d+move,1,7);
              b[gun().xpos].image = 162 + gun().d;
              gun().rotate = int(2/GAME_SPEED);
            }
          }
// Shoot gun
          if ((gun().firedelay == 0) && (gun().ammo > 0) && (abs(int(dx))<150)){
            gunshoot(gun(), shot, sound, xmove, ymove);
          }
// Lose target

          if ((abs(int(dx))>200) || (p().state>1) || (p().hide) ||
              (p().land !=2)){
            gun().target = 0;
          }
        }
      }
    }
// Sort out guntimers
    if (gun().firedelay > 0) gun().firedelay--;
    if (gun().rotate > 0) gun().rotate--;
    if (gun().ammo < 3){
      gun().reload++;
      if (gun().reload == int(20/GAME_SPEED)){
        gun().reload = 0;
        gun().ammo++;
      }
    }
  }

}
// Do everything

void all(gamedata &g){

  rotate_radars(g.radar);
  animate_smoke(g.smoke);
  animate_flames(g.flame, g.smoke);
  animate_explosions(g.explosion);
  decay_lasers(g.laser);
  clone_planes(g.p, g.dp, g.mission, g.winner);
  plane_collisions(g);
  move_falls(g);
  move_shots(g.shot,g.gamemap.ground,g.images[114],g.images[318],g.drakms);
  fire_guns(g.gun, g.p, g.sound, g.gamemap.b, g.shot, g.xmove, g.ymove);
  if (g.drak) drak_main(g);

}
