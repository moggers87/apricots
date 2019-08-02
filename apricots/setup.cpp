// Apricots setup routine
// Author: M.D.Snellgrove
// Date: 24/3/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Changes for graphics related stuff.

#include "apricots.h"

// Create airbases

void create_airbases(airbase base[], info planeinfo[], int planes){

  base[0] = EMPTY_AIRBASE;
  for (int n=1;n<=planes;n++){
    switch(planeinfo[n].basetype){
      case 1:
        base[n] = STANDARD_AIRBASE;
        break;
      case 2:
        base[n] = REVERSED_AIRBASE;
        break;
      case 3:
        base[n] = LITTLE_AIRBASE;
        break;
      case 4:
        base[n] = LONG_AIRBASE;
        break;
      case 5:
        base[n] = ORIGINAL_AIRBASE;
        break;
      case 6:
        base[n] = SHOOTY_AIRBASE;
        break;
      case 7:
        base[n] = TWOGUN_AIRBASE;
        break;
    }
  }

}

// Land generation randomizer

int randomland(int landa, int landb){

  if (drand() > 0.5) return landa;
  return landb;

}

// Create the game map

void setup_map(map &gamemap, int planes, airbase base[], bool flatground[]){

// Arrange airbase flags (to pin out airbase locations)
  int airbaseflag[MAP_W];
  for (int x1=0;x1<MAP_W;x1++){
    airbaseflag[x1] = 0;
  }
// Pin out locations (in reverse order)
//   airbaseflag = 1 (airbase), 2 (1 space to left), 3 (2 spaces to left)
  for (int n=planes;n>=1;n--){
    if (planes > 1) base[n].mapx = 4 + (MAP_W-13)*(n-1)/(planes-1);
               else base[n].mapx = 4;
    airbaseflag[base[n].mapx-2] = 3;
    airbaseflag[base[n].mapx-1] = 2;
    for (int i=0;i<=base[n].size;i++){
      airbaseflag[base[n].mapx+i] = 1;
    }
  }

// Set ground default as unflat
  for (int i=0;i<MAP_W*2;i++){
    flatground[i] = false;
  }

// Clear gamemap
  for(int x2=0; x2<MAP_W; x2++){
    for(int y=0; y<MAP_H; y++){
      gamemap.image[x2][y] = 0;
    }
  }

// Setup landtypes
  const int HILAND = 0;
  const int LOLAND = 1;
  const int BEACH = 2;
  const int PORTLEFT = 3;
  const int PORTRIGHT = 4;
  const int SEA = 5;

  int height = 0;
  int land = 0;

// Do left of map
  int r;
  r = int(drand() * 4.0);
  int y1;
  switch(r){
    case 0: case 1: case 2:
      height = MAP_H - 1 - r;
      land = randomland(HILAND,LOLAND);
      for (y1=0;y1<height;y1++){
        gamemap.image[0][y1] = 2;
      }
      gamemap.image[0][height] = 14-(10*land);
      break;
    case 3:
      height = MAP_H - 1;
      land = SEA;
      for (y1=0; y1<height;y1++){
        gamemap.image[0][y1] = 2;
      }
      gamemap.image[0][height] = 28;
      break;
  }
  gamemap.groundheight[0] = 0;

// Do middle portion of map
  for (int x=1; x<MAP_W-1;x++){
    switch(land){
      case HILAND:
        gamemap.groundheight[x] = 32 * height + 9;
        flatground[x*2] = true;
        r = int(drand() * 6.0);
        if ((airbaseflag[x] == 2) && (height == MAP_H-3)) r = 5;
        if ((airbaseflag[x] == 2) && (height == MAP_H-2) &&
            ((r == 2) || (r == 4))) r = 5;
        if (airbaseflag[x] == 1) r = 0;
        if ((height == MAP_H-3) && ((r == 2) || (r == 4))) r = 0;
        if ((height == MAP_H-1) && (r == 5)) r = 0;
        switch(r){
          case 0:
            gamemap.image[x][height] = 9;
            flatground[x*2+1] = true;
            break;
          case 1:
            gamemap.image[x][height] = 8;
            land = LOLAND;
            break;
          case 2:
            gamemap.image[x][height] = 10;
            gamemap.image[x][height-1] = 11;
            height--;
            land = LOLAND;
            break;
          case 3:
            gamemap.image[x][height] = 20;
            land = LOLAND;
            break;
          case 4:
            gamemap.image[x][height] = 15;
            land = randomland(HILAND,LOLAND);
            gamemap.image[x][height-1] = 19-(2*land);
            height--;
            break;
          case 5:
            gamemap.image[x][height] = 18;
            if ((height != MAP_H-2) || (drand()>0.33) ||
                (airbaseflag[x] == 2)){
              land = randomland(HILAND,LOLAND);
              gamemap.image[x][height+1] = 14-(10*land);
              height++;
            }else{
              land = SEA;
              gamemap.image[x][height+1] = 28;
              height++;
            }
            break;
        }
        break;

      case LOLAND:
        gamemap.groundheight[x] = 32 * height + 22;
        flatground[x*2] = true;
        r = int(drand() * 6.0);
        if ((airbaseflag[x] == 2) && (height == MAP_H-3))
           r = 2 + 3 * int(drand() * 2.0);
        if ((airbaseflag[x] == 2) && (height == MAP_H-2) && (r == 4)) r = 5;
        if (airbaseflag[x] == 1) r = 0;
        if ((height == MAP_H-3) && (r == 4)) r = 0;
        if ((height == MAP_H-1) && (r == 5)) r = 2;
        if (((airbaseflag[x] == 2) || (x == MAP_W-2)) && (height == MAP_H-1) &&
            (r == 2)) r = 0;
        switch(r){
          case 0:
            gamemap.image[x][height] = 5;
            flatground[x*2+1] = true;
            break;
          case 1:
            gamemap.image[x][height] = 7;
            land = HILAND;
            break;
          case 2:
            if (height < MAP_H-1){
              gamemap.image[x][height] = 13;
              gamemap.image[x][height+1] = 12;
              height++;
              land = HILAND;
            }else{
              land = randomland(BEACH,PORTLEFT);
              if (airbaseflag[x] == 3) land = BEACH;
              gamemap.image[x][height] = 6+(8*land);
              if (land == PORTLEFT) flatground[x*2+1] = true;
            }
            break;
          case 3:
            gamemap.image[x][height] = 21;
            land = HILAND;
            break;
          case 4:
            gamemap.image[x][height] = 6;
            land = randomland(HILAND,LOLAND);
            gamemap.image[x][height-1] = 19-(2*land);
            height--;
            break;
          case 5:
            gamemap.image[x][height] = 16;
            if ((height != MAP_H-2) || (drand()>0.33) ||
                (airbaseflag[x] == 2)){
              land = randomland(HILAND,LOLAND);
              gamemap.image[x][height+1] = 14-(10*land);
              height++;
            }else{
              land = SEA;
              gamemap.image[x][height+1] = 28;
              height++;
            }
            break;
        }
        break;

      case BEACH:
        gamemap.groundheight[x] = GAME_HEIGHT - 4;
        flatground[x*2] = true;
        r = int(drand() * 3.0);
        if ((airbaseflag[x] > 1) || (x == MAP_W-2)) r = 1;
        switch(r){
          case 0:
            gamemap.image[x][height] = 24;
            flatground[x*2+1] = true;
            break;
          case 1:
            gamemap.image[x][height] = 23;
            land = LOLAND;
            break;
          case 2:
            gamemap.image[x][height] = 25;
            land = SEA;
            break;
        }
        break;

      case PORTLEFT:
        gamemap.groundheight[x] = GAME_HEIGHT - 10;
        flatground[x*2] = true;
        r = int(drand() * 3.0);
        if ((airbaseflag[x] > 1) || (x == MAP_W-2)) r = 1;
        switch(r){
          case 0:
            gamemap.image[x][height] = 32;
            flatground[x*2+1] = true;
            break;
          case 1: case 2:
            gamemap.image[x][height] = 33;
            land = SEA;
            break;
        }
        break;

      case PORTRIGHT:
        gamemap.groundheight[x] = GAME_HEIGHT - 10;
        flatground[x*2] = true;
        r = int(drand() * 3.0);
        if ((airbaseflag[x] > 1) || (x == MAP_W-2)) r = 1;
        switch(r){
          case 0:
            gamemap.image[x][height] = 32;
            flatground[x*2+1] = true;
            break;
          case 1: case 2:
            gamemap.image[x][height] = 31;
            land = LOLAND;
            flatground[x*2+1] = true;
            break;
        }
        break;

      case SEA:
        gamemap.groundheight[x] = GAME_HEIGHT - 2;
        r = int(drand() * 4.0);
        if (airbaseflag[x] == 2) r = 3;
        if (airbaseflag[x] == 3) r = 2;
        if ((r == 2) && (x == MAP_W-2)) r = 0;
        switch(r){
          case 0: case 1:
            gamemap.image[x][height] = 27;
            break;
          case 2:
            land = randomland(BEACH,PORTRIGHT);
            gamemap.image[x][height] = 18+(4*land);
            break;
          case 3:
            gamemap.image[x][height] = 29;
            land = randomland(HILAND,LOLAND);
            gamemap.image[x][height-1] = 19-(2*land);
            height--;
            break;
        }
        break;
    }
  }

// Do right of map
  switch(land){
    case HILAND:
      gamemap.image[MAP_W-1][height] = 15;
      gamemap.groundheight[MAP_W-1] = 32 * height + 9;
      flatground[(MAP_W-1)*2] = true;
      break;
    case LOLAND:
      gamemap.image[MAP_W-1][height] = 6;
      gamemap.groundheight[MAP_W-1] = 32 * height + 22;
      flatground[(MAP_W-1)*2] = true;
      break;
    case SEA:
      gamemap.image[MAP_W-1][height] = 29;
      gamemap.groundheight[MAP_W-1] = GAME_HEIGHT - 2;
      break;
  }
  for (int y=0;y<height;y++){
    gamemap.image[MAP_W-1][y] = 3;
  }

// Fill in ground
  for (int x3=0;x3<MAP_W;x3++){
    int y = MAP_H - 1;
    while (gamemap.image[x3][y] == 0){
      gamemap.image[x3][y] = 1;
      y--;
    }
  }

}

// Setup the buildings and airbases and scenery

void setup_buildings(gamedata &g, bool flatground[]){

// Clear building array
  for (int x=0;x<MAP_W*2;x++){
    g.gamemap.b[x].type = 0;
  }

// Place airbases
  int radarcount = 0;
  int guncount = 0;
  for (int n1=1;n1<=g.planes;n1++){
    g.base[n1].planex = 32 * g.base[n1].mapx + g.base[n1].planepos;
    g.base[n1].planey = g.gamemap.groundheight[g.base[n1].mapx] - 12;
    for(int x=0;x<=(g.base[n1].size+1)*2;x++){
      int px = x + g.base[n1].mapx*2;
      flatground[px] = false;
      if (g.base[n1].buildlist[x].type != 0){
        g.gamemap.b[px] = g.base[n1].buildlist[x];
        g.gamemap.b[px].x = px * 16 - 8;
        g.gamemap.b[px].y = g.gamemap.groundheight[g.base[n1].mapx] - 16;
        g.gamemap.b[px].side = n1;
        if (g.gamemap.b[px].type == 4){ //radar
          radarcount++;
          radartype newradar = {g.gamemap.b[px].x,g.gamemap.b[px].y,238,0,
                         px,radarcount};
          g.radar.add(newradar);
          g.gamemap.b[px].id = radarcount;
        }
        if (g.gamemap.b[px].type == 5){ //gun
          guncount++;
          guntype newgun = {g.gamemap.b[px].x,g.gamemap.b[px].y,4,3,0,n1,px,0,0,
                       guncount,0};
          g.gun.add(newgun);
          g.gamemap.b[px].id = guncount;
        }
      }
    }
  }

// Count spaces for buildings/trees etc
  int spaces = 0;
  for (int x1=0;x1<MAP_W*2;x1++){
    if (flatground[x1]){
      spaces++;
    }
  }

// Place towers
  int n = g.towers;
  while ((n>0) && (spaces>0)){
    int x;
    do{
      x = int(drand()*MAP_W*2.0);
    } while (!flatground[x]);
    flatground[x] = false;
    g.gamemap.b[x] = TOWER;
    g.gamemap.b[x].x = x * 16 - 8;
    g.gamemap.b[x].y = g.gamemap.groundheight[x/2] - 16;
    int towersize = int(drand()*4.0)+2;
    int heightexcess = towersize - (g.gamemap.groundheight[x/2]/16) +
                         MAP_H*2  - 8;
    if (heightexcess > 0) towersize -= heightexcess;
    g.gamemap.b[x].towersize = towersize;
    spaces--;
    n--;
  }

// Place guns
  n = g.guns;
  while ((n>0) && (spaces>0)){
    int x;
    do{
      x = int(drand()*MAP_W*2.0);
    } while (!flatground[x]);
    flatground[x] = false;
    g.gamemap.b[x] = GUN;
    g.gamemap.b[x].x = x * 16 - 8;
    g.gamemap.b[x].y = g.gamemap.groundheight[x/2] - 16;
    guncount++;
    guntype newgun = {g.gamemap.b[x].x,g.gamemap.b[x].y,4,3,0,0,x,0,0,
                       guncount,0};
    g.gun.add(newgun);
    g.gamemap.b[x].id = guncount;
    spaces--;
    n--;
  }



// Place buildings
  n = g.buildings;
  bool powerstation = false;
  while ((n>0) && (spaces>0)){
    int x;
    do{
      x = int(drand()*MAP_W*2.0);
    } while (!flatground[x]);
    if ((flatground[x-1]) && (flatground[x+1]) &&
        ((drand()<0.2) || (!powerstation))){ // powerstation
      bool bigpowerstation = false;
      if ((x>1) && (x<MAP_W*2-2)){ //bigpowerstation
        if ((flatground[x-2]) && (flatground[x+2])){
          bigpowerstation = true;
          g.gamemap.b[x-2] = COOLINGTOWER_LEFT;
          g.gamemap.b[x-1] = COOLINGTOWER_MIDDLE;
          g.gamemap.b[x] = POWERSTATION;
          g.gamemap.b[x+1] = COOLINGTOWER_MIDDLE;
          g.gamemap.b[x+2] = COOLINGTOWER_RIGHT;
          for(int px=x-2;px<=x+2;px++){
            g.gamemap.b[px].x = px * 16 - 8;
            g.gamemap.b[px].y = g.gamemap.groundheight[x/2] - 16;
            flatground[px] = false;
          }
          spaces -= 5;
          n--;
        }
      }
      if (!bigpowerstation){ //smallpowerstation
        g.gamemap.b[x-1] = COOLINGTOWER_LEFT;
        g.gamemap.b[x] = POWERSTATION;
        g.gamemap.b[x+1] = COOLINGTOWER_RIGHT;
        for(int px=x-1;px<=x+1;px++){
          g.gamemap.b[px].x = px * 16 - 8;
          g.gamemap.b[px].y = g.gamemap.groundheight[x/2] - 16;
          flatground[px] = false;
        }
        spaces -= 3;
        n--;
      }
    }else { //buildings
      int r = int(drand() * 6.0);
      switch(r){
        case 0:
          g.gamemap.b[x] = CIVILIAN_1;
          break;
        case 1:
          g.gamemap.b[x] = CIVILIAN_2;
          break;
        case 2:
          g.gamemap.b[x] = CIVILIAN_3;
          break;
        case 3: case 4: case 5:
          g.gamemap.b[x] = FACTORY;
          break;
      }
      g.gamemap.b[x].x = x * 16 - 8;
      g.gamemap.b[x].y = g.gamemap.groundheight[x/2] - 16;
      flatground[x] = false;
      spaces--;
      n--;
    }
  }

// Place trees
  n = g.trees;
  while ((n>0) && (spaces>0)){

    int x;
    do{
      x = int(drand()*MAP_W*2.0);
    } while (!flatground[x]);
    flatground[x] = false;
    int mapunderneath = g.gamemap.image[x/2][MAP_H-1];
    if ((mapunderneath<30) || ((x%2==0) && (mapunderneath==30))){
      if(g.gamemap.groundheight[x/2]<GAME_HEIGHT-5){ // firtree
        g.gamemap.b[x] = FIRTREE;
        g.gamemap.b[x].y = g.gamemap.groundheight[x/2] - 24;
      }else{ // palmtree
        g.gamemap.b[x] = PALMTREE;
        g.gamemap.b[x].y = g.gamemap.groundheight[x/2] - 16;
      }
      g.gamemap.b[x].x = x * 16 - 8;
      n--;
    }
    spaces--;
  }

}

// Draw mapblocks
void draw_mapblocks(int image[MAP_W][MAP_H], SDL_Surface *gamescreen,
                    shape images[]){

  //gl_setcontext(&gamescreen);
  for(int x=0; x<MAP_W; x++){
    for(int y=0; y<MAP_H; y++){
      if (image[x][y] > 0){
        images[image[x][y]].blit(gamescreen,x*32, y*32);
      }

    }
  }


}

// Draw the map onto the gamescreen

void draw_map(int image[MAP_W][MAP_H], SDL_Surface *gamescreen,
              shape images[], shape &ground){

  //gl_setcontext(&gamescreen);
// First setup the ground collision map
  SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = GAME_WIDTH;
    rect.h = GAME_HEIGHT;
  SDL_FillRect(gamescreen, &rect, 0);
  draw_mapblocks(image, gamescreen, images);
  ground.grab(gamescreen, 0, 0, GAME_WIDTH, GAME_HEIGHT);

// Now do the gamescreen properly
// Set the background sky blue (colour 16)
  SDL_FillRect(gamescreen, &rect, 16);
  draw_dither(gamescreen, 0, 0, GAME_WIDTH, GAME_HEIGHT);
  draw_mapblocks(image, gamescreen, images);


}

// Draw the buildings, trees and towers

void draw_buildings(building b[], SDL_Surface *gamescreen, shape images[]){

  //gl_setcontext(&gamescreen);
  for (int x=0;x<MAP_W*2;x++){
    if ((b[x].type == 1) || (b[x].type == 3)){ // buildings and trees
      //images[b[x].image].blit(gamescreen,b[x].x,b[x].y);
      images[b[x].image].blit(gamescreen,b[x].x,b[x].y);

    }
    if (b[x].type == 2){ // tower
      images[196].blit(gamescreen,b[x].x,b[x].y);
      for (int y=1;y<=b[x].towersize;y++){
         images[197].blit(gamescreen,b[x].x,b[x].y-16*y);
      }
    }
  }

}

// Draw the runways

void draw_runways(airbase base[], int planes, int groundheight[],
                 SDL_Surface *gamescreen){

  //gl_setcontext(&gamescreen);
  for(int n=1;n<=planes;n++){
    /*gl_fillbox(base[n].mapx*32+base[n].runwayx, groundheight[base[n].mapx]-1,
               base[n].runwaylength,2,3);*/
    SDL_Rect rect;
      rect.x = base[n].mapx*32+base[n].runwayx;
      rect.y = groundheight[base[n].mapx]-1;
      rect.w = base[n].runwaylength;
      rect.h = 2;
    SDL_FillRect(gamescreen, &rect, 3);
    for(int m=0;m<base[n].runwaylength/16;m++){
      /*gl_hline(base[n].mapx*32+base[n].runwayx+m*16+4,
               groundheight[base[n].mapx]-1,
               base[n].mapx*32+base[n].runwayx+m*16+12,1);*/
      SDL_Rect rect;
        rect.x = base[n].mapx*32+base[n].runwayx+m*16+4;
        rect.y = groundheight[base[n].mapx]-1;
        rect.w = 8;
        rect.h = 1;
      SDL_FillRect(gamescreen,&rect,1);
    }
  }

}

// Setup intelligence maps

void setup_intelligence(map &gamemap){

// Setup realheight map
  for(int x=0;x<=MAP_W*2;x++){
    if ((x>1) && (x<=MAP_W*2-2)){
      gamemap.realheight[x] = gamemap.groundheight[x/2] - 16;
      if (gamemap.b[x].type > 0) gamemap.realheight[x] -= 24;
      if (gamemap.b[x].type == 2){
        gamemap.realheight[x] -= (gamemap.b[x].towersize*16);
      }
      if (gamemap.realheight[x] < 0) gamemap.realheight[x] = 0;
    }else{
      gamemap.realheight[x] = GAME_HEIGHT;
    }
    gamemap.smoothheight[x] = gamemap.realheight[x];
    gamemap.steepheight[x] = gamemap.realheight[x];
  }
// Construct smoothmap
  int x1 = 1;
  do{
    while(gamemap.smoothheight[x1-1]-gamemap.smoothheight[x1] > 16){
      gamemap.smoothheight[x1-1] = gamemap.smoothheight[x1]+16;
      x1--;
    }
    if(gamemap.smoothheight[x1+1]-gamemap.smoothheight[x1] > 16){
      gamemap.smoothheight[x1+1] = gamemap.smoothheight[x1]+16;
    }
    x1++;
  }while (x1 < MAP_W*2);
// Construct steepmap
  x1 = 1;
  do{
    while(gamemap.steepheight[x1-1]-gamemap.steepheight[x1] > 32){
      gamemap.steepheight[x1-1] = gamemap.steepheight[x1]+32;
      x1--;
    }
    if(gamemap.steepheight[x1+1]-gamemap.steepheight[x1] > 32){
      gamemap.steepheight[x1+1] = gamemap.steepheight[x1]+32;
    }
    x1++;
  }while (x1 < MAP_W*2);

}

// Draw dither in specified box located at position x,y
// NB draws only on existing sky blue

void draw_dither(SDL_Surface *gamescreen,
                 int xbox, int ybox, int w, int h){

  //gl_setcontext(&gamescreen);

// Plot dither
  if( SDL_MUSTLOCK(gamescreen) != 0 )
    SDL_LockSurface(gamescreen);
  Uint8 *pixels = (Uint8 *)gamescreen->pixels;
  int xstart = limit(xbox, 0, GAME_WIDTH);
  int xend = limit(xbox+w, 0, GAME_WIDTH);
  int ystart = limit(ybox, 0, GAME_HEIGHT);
  int yend = limit(ybox+h, 0, GAME_HEIGHT);
  for (int x=xstart; x<xend;x++){
    for (int y=ystart;y<yend;y++){
      if( x < 0 || x >= GAME_WIDTH || y < 0 || y >= GAME_HEIGHT ) {
        printf("!\n");
      }
      int getcolour = pixels[y * gamescreen->pitch + x];
      if ((getcolour == 9) || (getcolour == 16)){ // If skyblue
        double ditherfactor = double(y) * 25.0 / (GAME_HEIGHT-32);
        int colour = 40 - int(ditherfactor);
        double chance = ditherfactor - int(ditherfactor);
        if (drand()<chance) colour--;
        if (colour < 16) colour = 16;
        pixels[y * gamescreen->pitch + x] = colour;        
      }
    }
  }
  if( SDL_MUSTLOCK(gamescreen) != 0 )
    SDL_UnlockSurface(gamescreen);

}

// Setup planes

void setup_planes(linkedlist <plane> &p, linkedlist <planeclone> &dp,
                  airbase base[], int planes, info planeinfo[],
                  plane* &player1, plane* &player2){

  p.reset();
  dp.reset();
  player1 = 0;
  player2 = 0;

  for (int n=1;n<=planes;n++){
// Create plane
    plane newplane;
    switch(planeinfo[n].planetype){
      case 1:
        newplane = SPITFIRE;
        break;
      case 2:
        newplane = JET;
        break;
      case 3:
        newplane = STEALTH;
        break;
    }
    newplane.x = base[n].planex;
    newplane.y = base[n].planey;
    newplane.d = base[n].planed;
    newplane.control = planeinfo[n].control;
    newplane.id = n;
    newplane.side = n;
    node <plane> * planenode = p.add(newplane);
// Create planeclone
    planeclone newclone;
    newclone.x = newplane.x;
    newclone.y = newplane.y;
    newclone.xs = newplane.xs;
    newclone.ys = newplane.ys;
    newclone.d = newplane.d;
    newclone.image = newplane.image;
    newclone.state = 0;
    newclone.hide = false;
    newclone.id = newplane.id;
    newclone.side = newplane.side;
    newclone.collide = false;
    newclone.scoreloss = 0;
    newclone.buildingwin = 0;

    for (int i=1;i<=planes;i++){
       if (i != n){
         for(int x=0;x<=(base[i].size+1)*2;x++){
           if (base[i].buildlist[x].type != 0){
             newclone.buildingwin ++;
           }
         }
       }
    }
    dp.add(newclone);
// Set player pointers
    if (planeinfo[n].control == 1) player1 = &(planenode->value);
    if (planeinfo[n].control == 2) player2 = &(planenode->value);
  }

}

// Main game setup routine

void setup_game(gamedata &g){

  create_airbases(g.base, g.planeinfo, g.planes);
  bool flatground[MAP_W*2];
  setup_map(g.gamemap, g.planes, g.base, flatground);
  setup_buildings(g, flatground);
  setup_intelligence(g.gamemap);

  draw_map(g.gamemap.image, g.gamescreen, g.images, g.gamemap.ground);
  draw_buildings(g.gamemap.b, g.gamescreen, g.images);
  draw_runways(g.base, g.planes, g.gamemap.groundheight, g.gamescreen);
  setup_planes(g.p, g.dp, g.base, g.planes, g.planeinfo, g.player1, g.player2);

// Drak options
  g.drak = ((g.drakoption == 2) || ((g.drakoption == 1) && (int(drand()*20) == 10)));
  if (g.drak){
    setup_draks(g.drakms, g.drakgun);
  }else{
    g.drakms.exist = 0;
  }

// Reset winner flag
  g.winner = 0;

}

