// Apricots computer intelligence routine
// Author: M.D.Snellgrove
// Date: 25/3/2002
// History:

#include "apricots.h"

// Follow target routine

void followtarget(plane &p, int &jx, int &jy, int rx, int ry, bool reverse){
  int targetd = 0;
  if (abs(rx) > 150){
// Long distance cruising
    targetd = wrap(9 - 5*sign(rx) - p.d, -7, 9);
    if (p.y < p.cruiseheight) targetd = wrap(9 - 4*sign(rx) - p.d, -7, 9);
    if (p.y < GAME_HEIGHT - 160){
      if ((rx > 0) && (p.d == 13)){
        targetd = -1;
        p.coms = 9;
      }
      if ((rx < 0) && (p.d == 5)){
        targetd = 1;
        p.coms = 8;
      }
    }
// Afterburner if available
    if ((targetd == 0) && (p.burner) && (abs(rx) > 200)) jy = -1;
  }else{
// Track target directly    
     targetd = wrap(int((atan2(double(ry),double(-rx))/PI*8)+13.5) - p.d, -7, 9);
// Go other way if reverse flag set
     if (reverse) targetd = wrap(targetd+8, -7, 9);
  }
// Equilibrize
  if (targetd == 8) targetd = int(drand()*2.0)*16 - 8;
// Choose direction
  jx = -sign(targetd);

}

// Main Computer AI routine

void computer_ai(gamedata &g, plane &p, int &jx, int &jy, bool &jb){

  switch(g.p().land){
    case 0: // Landed Plane (takes off)
      jy = -1;
      break;

    case 1: // Taking off plane (accelerates)
      jy = -1;
      if (p.s > 5.0 * GAME_SPEED){
        jx = sign(9-p.d);
      }
      break;

    case 2: // Flying plane
      if (p.rotate == 0){
        if ((p.coms == 0) && (p.state == 0)){
// Stall Avoidance
          if (((p.y < 24.0) && (p.ys < 0.0)) || (p.s < 3.0*GAME_SPEED)) p.coms = 4;
// Drak mothership avoidance
          if (g.drakms.exist == 1){
            double dx = p.x - g.drakms.x + (p.xs - g.drakms.xs)*4.0/GAME_SPEED;
            if ((dx > -40.0) && (dx < 136.0)){
              if ((p.y > 46.0) && (p.y < 106.0) &&
                  ((p.ys < 0.0) || (p.y < 86.0))) p.coms = 4;
              if ((p.y <= 46.0) && (p.ys >= 0.0) &&
                  ((p.ys > 0.0) || (p.y > 26.0))) p.coms = 1;
            }
          }
// Groundheight tracking
          int px = int(p.x + 8)/16;
          if (((p.d == 5) || (p.d == 13)) && ((p.y+20.0 > g.gamemap.steepheight[px]) ||
               (p.y+20.0 > g.gamemap.steepheight[px+1]))) p.coms = 1;
          if ((p.d > 2) && (p.d < 8)) px = limit(px-1, 0, MAP_W*2-1);
          if ((p.d > 10) && (p.d < 16)) px = limit(px+1, 0, MAP_W*2-1);
          if (p.ys < 0.0){
            if ((p.y+5.0 > g.gamemap.smoothheight[px]) || 
                (p.y+5.0 > g.gamemap.smoothheight[px+1])) p.coms = 1;
    
          }else{
            if ((p.y+40.0 > g.gamemap.smoothheight[px]) ||
                (p.y+40.0 > g.gamemap.smoothheight[px+1])) p.coms = 1;
          }
          if ((p.y-40.0 > g.gamemap.realheight[px]) ||
              (p.y-40.0 > g.gamemap.realheight[px+1])) p.coms = 6;
// Landing runway approach
          if (p.targetx == -10){
            int dx = int(p.x) - 32*g.base[p.side].mapx - g.base[p.side].runwayx;
            if ((dx > 10) && (dx < -10 + g.base[p.side].runwaylength) &&
                ((p.d == 6) || (p.d == 7) || (p.d == 11) || (p.d == 12))){
              p.coms = 0;
              if (g.base[p.side].planey - int(p.y) < 50) p.coms = 11;
            }
          }
// Side of map avoidance
          if (int(p.x) < 100) p.coms = 2;
          if (int(p.x) > GAME_WIDTH - 116) p.coms = 3;
        }
// Recover from stall
        if (p.state == 1) p.coms = 5;
// Choose action depending on coms
        switch (p.coms){
          case 0: // Action!
// Land if out of shots and bombs
            if ((p.ammo == 0) && (p.bombs == 0)) p.targetx = -10;
// Land if mission is done
            if ((g.mission < 2) && (p.score >= g.targetscore)) p.targetx = -10;
            if ((g.mission == 2) && (p.targetscore == 0)) p.targetx = -10;
// Drak fighters don't land
            if ((p.targetx == -10) && (p.drak)) p.targetx = 0;
// Check for gunthreat and change target if target not already a gun
            if ((p.gunthreat > 0) && (p.bombs > 0)){
              if (p.targetx > 0){
                if (g.gamemap.b[p.targetx].type != 5){
                  p.targetx = p.gunthreat;
                  p.targety = g.gamemap.b[p.targetx].y;
                }
              }else{
                p.targetx = p.gunthreat;
                p.targety = g.gamemap.b[p.targetx].y;
              }
            }
// Find target if no target exists
            if (p.targetx == 0){
// Try a building
              p.targetx = int(drand()*(MAP_W*2-3))+2;
              if ((g.gamemap.b[p.targetx].type < 3) || (g.gamemap.b[p.targetx].points < 0) || 
                  (g.gamemap.b[p.targetx].side == p.side) || (p.bombs == 0) || 
                  ((g.gamemap.b[p.targetx].side == 0) && (g.mission == 2))){
// Building unsuitable target so try plane instead
                p.targetx = 0;
                int tryplaneid = int(drand()*g.planes) + 1;
                if ((((g.mission != 2) && (int(drand()*2) == 1)) || (int(drand()*30) == 1) ||
                     (p.drak)) && (p.ammo > 0)){
                  g.dp.reset();
                  while (g.dp.next()){
// Check if plane is suitable target and select if so
                    if ((g.dp().id == tryplaneid) && (g.dp().side != p.side) &&
                        (g.dp().state < 2) && (!g.dp().hide)) p.targetx = -tryplaneid;
                  }
                }
              }else{
// Building suitable so becomes target
                p.targety = g.gamemap.b[p.targetx].y;
              }
// Set new cruiseheight if changed target
              if (p.cruiseheight == 0) p.cruiseheight = 20 + int(drand()*(GAME_HEIGHT-176));
              if (p.targetx == 0){
// Chase false target
                if (p.xs > 0){
                  followtarget(p, jx, jy, -160, 0, false);
                }else{
                  followtarget(p, jx, jy, 160, 0, false);
                }
// If still no target for too long then land
                p.targety++;
                if (p.targety == int(200/GAME_SPEED)) p.targetx = -10;
              }
            }
// Target is building
            if (p.targetx > 0){
              int rx = int(p.x) - 16*p.targetx + 8;
              int ry = int(p.y) + 40 - p.targety;
              followtarget(p, jx, jy, rx, ry,
                           ((abs(rx) < 110) && (g.gamemap.b[p.targetx].type == 5)));
// Check to see if can still bomb the building
              if ((g.gamemap.b[p.targetx].type == 0) || (p.bombs == 0)){
                p.targetx = 0;
                p.targety = 0;
                p.cruiseheight = 0;
              }
            }
// Target is plane
            if ((p.targetx < 0) && (p.targetx > -10)){
              g.dp.reset();
              while (g.dp.next()){
                if (g.dp().id == -p.targetx){
// Check to see if can still shoot plane
                  if ((g.dp().state > 1) || (p.ammo == 0) || ((g.dp().hide) &&
                       (int(drand()*40) == 1))){
                    p.targetx = 0;
                    p.targety = 0;
                    p.cruiseheight = 0;
                  }else{
                    int rx = int(p.x - g.dp().x);
                    int ry = int(p.y - g.dp().y);
                    followtarget(p, jx, jy, rx, ry, (rx*rx + ry*ry < 2500));
                  }
                }         
              }
            }
// Target is runway
            if (p.targetx == -10){
              int rx = int(p.x) - 32*g.base[p.side].mapx - g.base[p.side].runwayx
                                - g.base[p.side].runwaylength/2;
              int ry = int(p.y) -g.base[p.side].planey;
              followtarget(p, jx, jy, rx, ry, false);
            }
            break;
  
          case 1: // Go upwards
            if ((p.d > 3) && (p.d < 9)) jx = 1;
            if ((p.d > 9) && (p.d < 15)) jx = -1;
            if (p.d == 9){
              if (int(p.x) < 100) jx = -1;
              if (int(p.x) > GAME_WIDTH - 116) jx = 1;
              if (jx == 0){            
                int px = int(p.x+8)/16 - 1;
                jx = sign(g.gamemap.smoothheight[px] - g.gamemap.smoothheight[px+3]);
                if (jx == 0) jx = int(drand()*2.0)*2 -1;
              }
            }
            p.coms = 0;
            break;

          case 2: // Go right
            if ((p.d == 1) || (p.d > 13)) jx = 1;
            if ((p.d > 7) && (p.d < 13)) jx = -1;
            if ((p.d < 8) && (p.d > 1)){
              jx = 1;
              if (p.s < 4.0 * GAME_SPEED) jx = -1;
              if (int(p.y) < 34) jx = -1;
              int px = int(p.x+8)/16;
              if ((int(p.y)+45 > g.gamemap.realheight[px]) &&
                  (int(p.y)+45 > g.gamemap.realheight[px+1])) jx = 1;
            }
            if (jx == 1) p.coms = 7;
            if (jx == -1) p.coms = 8;
            if (p.d == 13) p.coms = 0;
            break;

          case 3: // Go left
            if (p.d < 5) jx = -1;
            if ((p.d > 5) && (p.d < 11)) jx = 1;
            if (p.d > 10){
              jx = -1;
              if (p.s < 4.0 * GAME_SPEED) jx = 1;
              if (int(p.y) < 34) jx = 1;
              int px = int(p.x+8)/16;
              if ((int(p.y)+45 > g.gamemap.realheight[px]) &&
                  (int(p.y)+45 > g.gamemap.realheight[px+1])) jx = -1;
            }
            if (jx == 1) p.coms = 9;
            if (jx == -1) p.coms = 10;
            if (p.d == 5) p.coms = 0;
            break;

          case 4: // Level off
            if (p.d < 8) jx = -1;
            if (p.d > 10) jx = 1;
            if (p.d == 1) jx = int(drand()*2.0)*2 -1;
            if ((p.d > 3) && (p.d < 15)) p.coms = 0;
            break;

          case 5: // Go down
            if (p.d < 9) jx = -1;
            if (p.d > 9) jx = 1;
            if (p.d == 1) jx = int(drand()*2.0)*2 -1;
            if ((p.d == 9) && (p.s > 3.0 * GAME_SPEED)) p.coms = 0;
            break;

          case 6: // Go upwards lots
            if ((p.d > 1) && (p.d < 9)) jx = 1;
            if (p.d > 9) jx = -1;
            if (p.d == 9){
              if (int(p.x) < 100) jx = -1;
              if (int(p.x) > GAME_WIDTH - 116) jx = 1;
              if (jx == 0){
                int px = int(p.x+8)/16 - 1;
                jx = sign(g.gamemap.smoothheight[px] - g.gamemap.smoothheight[px+3]);
                if (jx == 0) jx = int(drand()*2.0)*2 -1;
              }
            }
            p.coms = 0;
            break;

          case 7: // Go right +ve
            if (p.d != 13){
              jx = 1;
            }else{
              p.coms = 0;
            }
            break;

          case 8: // Go right -ve
            if (p.d != 13){
              jx = -1;
            }else{
              p.coms = 0;
            }
            break;

          case 9: // Go left +ve
            if (p.d != 5){
              jx = 1;
            }else{
              p.coms = 0;
            }
            break;

          case 10: // Go left -ve
            if (p.d != 5){
              jx = -1;
            }else{
              p.coms = 0;
            }
            break;

          case 11: // Landing
            if (p.d == 6) jx = -1;
            if (p.d == 12) jx = 1;
            int dx = int(p.x) - 32*g.base[p.side].mapx - g.base[p.side].runwayx;
            if ((dx < 10) || (dx > -10 + g.base[p.side].runwaylength)) p.coms = 0;
            break;

        }
// Wiggle to avoid gunfire
        if ((p.gunthreat > 0) && (jx == 0) && (!p.hide)) jx = int(drand()*2.0)*2 - 1;
      }
// Afterburner for jet if speed slow
      if ((p.burner) && (p.s < 5.0 * GAME_SPEED)) jy = -1;
// Stealth ability
      if (p.stealth) jy = -1;
      if (p.shotdelay == 0){
// Launch bomb
        if (p.targetx > 0){
          double rx = p.x - double(16*p.targetx) + 8.0 + p.xs;
          double ry = p.y - double(p.targety) + 14.0 + p.ys;
          double a = (p.ys + 2.0*GAME_SPEED)*(p.ys + 2.0*GAME_SPEED)
			          - (ry*0.2) * GAME_SPEED * GAME_SPEED;
          if (a > 0){
            if (abs(int(rx + p.xs*10.0/(GAME_SPEED*GAME_SPEED)*(-p.ys-2.0*GAME_SPEED+sqrt(a)))) < 6) jy = 1;
          }
        }
// Fire shot
        if ((p.s < 8.0*GAME_SPEED) && (p.ammo > 0)){
          g.dp.reset();
          while (g.dp.next()){
            if ((g.dp().side != p.side) && (g.dp().state < 2) &&
                (!g.dp().hide)){
              double rx = p.x - g.dp().x;
              double ry = p.y - g.dp().y;
              double d = sqrt(rx*rx + ry*ry);
              if (d < 100.0){
                double smartsine = ((ry*g.dp().xs - rx*g.dp().ys)/(8.0*GAME_SPEED)/d);
                smartsine = dlimit(smartsine, -1.0, 1.0);
                double smartangle = asin(smartsine);
                double angle = atan2(rx,ry);
                int bogied = wrap((int(((angle-smartangle)/PI*8.0)+1.5)), 1, 17);
                if (bogied == p.d) jb = true;
              }
            }
          } 
        }
      }
      break;
  }

}
