// Apricots header file
// Author: M.D.Snellgrove
// Date: 26/3/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Necessary changes to function prototypes for various reasons.
//   I had to initialise the Airbase definitions with constructors; Borland
//     doesn't seem to like initialising structures with the nested {}.

// Changes by M Snellgrove 6/7/2003
//   Function drand48() renamed drand() and is now located in file all.cpp

// Changes by M Snellgrove 30/7/2003
//   smoketype object added, g.smoke is now a linkedlist <smoketype>

// Changes by M Snellgrove 3/8/2003
//   TICK_INTERVAL moved here and set to 20

#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include "shape.h"
#include "linkedlist.h"
#include "sampleio.h"
#include "SDLfont.h"
using namespace std;

// Global constants

const int GAME_WIDTH = 2400;
const int GAME_HEIGHT = 320;
const int MAP_W = GAME_WIDTH / 32;
const int MAP_H = GAME_HEIGHT / 32;
const double GAME_SPEED = 0.5;
const int TICK_INTERVAL = 20;
const double PI = 3.141592;

// Default data directory path (current directory)

#ifndef AP_PATH
#define AP_PATH "./"
#endif

// Datatypes

struct building{
  int type; // 0 = none, 1 = tree, 2 = tower, 3 = building, 4 = radar, 5 = gun
  int id;
  int x;
  int y;
  int image;
  int deadimage;
  int points;
  int side;
  int towersize;
  int shrapnelimage;
  int shrapnelimage2;
};

// Building definitions

const building NB = {0,0,0,0,0,0,0,0,0,0,0}; // null building
const building CONTROLTOWER = {3, 0, 0, 0, 69, 73, 240, 0, 0, 140, 225};
const building FUEL = {3, 0, 0, 0, 71, 75, 60, 0, 0, 140, 140};
const building HANGAR = {3, 0, 0, 0, 70, 74, 160, 0, 0, 228, 140};
const building GUN = {5, 0, 0, 0, 166, 170, 200, 0, 0, 140, 140};
const building RADAR = {4, 0, 0, 0, 238, 246, 120, 0, 0, 140, 225};
const building TOWER = {2, 0, 0, 0, 0, 0, 0, 0, 0, 140, 140};
const building POWERSTATION = {3, 0, 0, 0, 112, 113, 80, 0, 0, 140, 140};
const building COOLINGTOWER_LEFT = {3, 0, 0, 0, 108, 111, 60, 0, 0, 140, 225};
const building COOLINGTOWER_MIDDLE = {3, 0, 0, 0, 109, 111, 60, 0, 0, 140, 225};
const building COOLINGTOWER_RIGHT = {3, 0, 0, 0, 110, 111, 60, 0, 0, 140, 225};
const building FACTORY = {3, 0, 0, 0, 72, 76, 100, 0, 0, 140, 140};
const building CIVILIAN_1 = {3, 0, 0, 0, 194, 195, -30, 0, 0, 222, 222};
const building CIVILIAN_2 = {3, 0, 0, 0, 200, 201, -60, 0, 0, 225, 219};
const building CIVILIAN_3 = {3, 0, 0, 0, 202, 203, -40, 0, 0, 219, 140};
const building FIRTREE = {1, 0, 0, 0, 258, 259, -10, 0, 0, 143, 143};
const building PALMTREE = {1, 0, 0, 0, 192, 193, -10, 0, 0, 143, 222};

struct firetype{
  int x;
  int y;
  int time;
  int type;
};

struct smoketype{
  int x;
  double y;
  int time;
};

struct lasertype{
  int x;
  int y;
  int image;
  int time;
};

struct radartype{
  int x;
  int y;
  int image;
  int rotate;
  int xpos;
  int id;
};

struct guntype{
  int x;
  int y;
  int d;
  int ammo;
  int firedelay;
  int side;
  int xpos;
  int reload;
  int rotate;
  int id;
  int target;
};

struct falltype{
  double x;
  double y;
  double xs;
  double ys;
  int image;
  int side;
  int type; // 0 = treebits, 1 = shrapnel, 2 = large bits, 3 = bomb
  int bombrotate;
  int rotatedelay;
};

struct shottype{
  double x;
  double y;
  double xs;
  double ys;
  int side;
  int time;
};

struct map{
  int image[MAP_W][MAP_H];
  int groundheight[MAP_W];
  building b[MAP_W*2];
  int realheight[MAP_W*2+1];
  int smoothheight[MAP_W*2+1];
  int steepheight[MAP_W*2+1];
  shape ground;
};

struct info{
  int planetype;
  int basetype;
  int control;
};

struct drakmstype{
  double x;
  double y;
  double xs;
  int exist;
  int targetx;
  int damage;
  int launchdelay;
  int movedelay;
  bool lgun;
  bool rgun;
  int targetmod;
  int fightersout;
  bool fighter[3];
};

struct drakguntype{
  int type;
  int x;
  int y;
  int d;
  int time;
  int rotate;
  int reload;
  int target;
  int image[17];
};

struct airbase{
  int runwayx;
  int runwaylength;
  int size;
  int planepos;
  int planed;
  int mapx;
  int planex;
  int planey;
  building buildlist[15];

  // Explicit constructor required else Borland C++ Compiler gives errors
  airbase() {
  }

  airbase(int runwayx,int runwaylength,int size,int planepos,int planed,int mapx,int planex,int planey,
  	building b0 = NB,
  	building b1 = NB,
  	building b2 = NB,
  	building b3 = NB,
  	building b4 = NB,
  	building b5 = NB,
  	building b6 = NB,
  	building b7 = NB,
  	building b8 = NB,
  	building b9 = NB,
  	building b10 = NB,
  	building b11 = NB,
  	building b12 = NB,
  	building b13 = NB,
  	building b14 = NB
  	){
      this->runwayx = runwayx;
      this->runwaylength = runwaylength;
      this->size = size;
      this->planepos = planepos;
      this->planed = planed;
      this->mapx = mapx;
      this->planex = planex;
      this->planey = planey;
      this->buildlist[0] = b0;
      this->buildlist[1] = b1;
      this->buildlist[2] = b2;
      this->buildlist[3] = b3;
      this->buildlist[4] = b4;
      this->buildlist[5] = b5;
      this->buildlist[6] = b6;
      this->buildlist[7] = b7;
      this->buildlist[8] = b8;
      this->buildlist[9] = b9;
      this->buildlist[10] = b10;
      this->buildlist[11] = b11;
      this->buildlist[12] = b12;
      this->buildlist[13] = b13;
      this->buildlist[14] = b14;
  }
};

struct plane{
  double x;
  double y;
  double xs;
  double ys;
  double s;
  int d;
  int control;
  int land;
  int state;
  int crash;
  int id;
  int side;
  int image;
  int rotate;
  int maxrotate;
  bool boost;
  bool burner;
  bool hide;
  bool stealth;
  int shotdelay;
  int ammo;
  int maxammo;
  int bombs;
  int maxbombs;
  int shrapnelimage;
  int enginesample;
  bool drak;
  int score;
  int targetscore;
  int coms;         //
  int targetx;      //
  int targety;      // Computer AI stuff
  int cruiseheight; //
  int gunthreat;    //
};

struct planeclone{
  double x;
  double y;
  double xs;
  double ys;
  int d;
  int image;
  int state;
  bool hide;
  int id;
  int side;
  bool collide;
  int scoreloss;
  int buildingwin;
};

struct gamedata{
  int planes;
  int towers;
  int guns;
  int buildings;
  int trees;
  int players;
  int targetscore;
  int mission;
  int winner;
  plane* player1;
  plane* player2;
  airbase base[7];
  info planeinfo[7];
  SDL_Surface *virtualscreen;
  SDL_Surface *gamescreen;
  SDL_Renderer *renderer;
  shape images[319];
  map gamemap;
  linkedlist <radartype> radar;
  linkedlist <guntype> gun;
  linkedlist <plane> p;
  linkedlist <planeclone> dp;
  linkedlist <firetype> explosion;
  linkedlist <firetype> flame;
  linkedlist <smoketype> smoke;
  linkedlist <falltype> fall;
  linkedlist <shottype> shot;
  sampleio sound;
  SDLfont whitefont;
  SDLfont greenfont;
  double accel[17];
  double xmove[17];
  double ymove[17];
  int xboost[17];
  int yboost[17];
  int bombimage[17];
  int drakoption;
  bool drak;
  drakmstype drakms;
  linkedlist <drakguntype> drakgun;
  linkedlist <lasertype> laser;
};

// Airbase definitions

const airbase EMPTY_AIRBASE(0,0,0,0,0,0,0,0);
const airbase STANDARD_AIRBASE(48,80,4,48,13,0,0,0,CONTROLTOWER,RADAR,GUN,NB,NB,NB,NB,NB,NB,FUEL,HANGAR);
const airbase REVERSED_AIRBASE(32,80,4,96,5,0,0,0,HANGAR,FUEL,NB,NB,NB,NB,NB,
                                 NB,GUN,RADAR,CONTROLTOWER);
const airbase LITTLE_AIRBASE(16,80,2,16,13,0,0,0,CONTROLTOWER,NB,
                                 NB,NB,NB,NB,NB);
const airbase LONG_AIRBASE(16,192,6,16,13,0,0,0,CONTROLTOWER,NB,NB,NB,NB,NB,
                                 NB,NB,NB,NB,NB,NB,NB,NB,HANGAR);
const airbase ORIGINAL_AIRBASE(32,80,3,32,13,0,0,0,CONTROLTOWER,RADAR,NB,
                                 NB,NB,NB,NB,NB,HANGAR);
const airbase SHOOTY_AIRBASE(80,80,6,80,13,0,0,0,GUN,FUEL,GUN,CONTROLTOWER,
                                 RADAR,NB,NB,NB,NB,NB,NB,HANGAR,GUN,FUEL,GUN);
const airbase TWOGUN_AIRBASE(48,96,5,128,5,0,0,0,RADAR,GUN,HANGAR,NB,NB,
                                 NB,NB,NB,NB,NB,CONTROLTOWER,GUN,RADAR);

// Sample definitions

const int SOUND_ENGINE = 0;
const int SOUND_JET = 1;
const int SOUND_EXPLODE = 2;
const int SOUND_GROUNDHIT = 3;
const int SOUND_FUELEXPLODE = 4;
const int SOUND_SHOT = 5;
const int SOUND_GUNSHOT = 6;
const int SOUND_BOMB = 7;
const int SOUND_SPLASH = 8;
const int SOUND_LASER = 9;
const int SOUND_STALL = 10;
const int SOUND_GUNSHOT2 = 11;
const int SOUND_BURNER = 12;
const int SOUND_FINISH = 13;

// Plane definitions

const plane SPITFIRE = {0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0,
                        122, 0, int(3/GAME_SPEED)-1,
                        false, false, false, false, 0, 8, 8, 4, 4, 143,
                        SOUND_ENGINE, false, 0, 0, 0, 0, 0, 0, 0};
const plane JET = {0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0,
                        76, 0, int(2/GAME_SPEED)-1,
                        false, true, false, false, 0, 12, 12, 5, 5, 140,
                        SOUND_JET, false, 0, 0, 0, 0, 0, 0, 0};
const plane STEALTH = {0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0,
                        174, 0, int(4/GAME_SPEED)-1,
                        false, false, false, true, 0, 3, 3, 6, 6, 235,
                        SOUND_JET, false, 0, 0, 0, 0, 0, 0, 0};
const plane DRAK_FIGHTER = {0.0, 0.0, 0.0, 2.0*GAME_SPEED, 2.0*GAME_SPEED, 9, 0, 2, 1, 0, 0, 0,
                        259, 0, int(1/GAME_SPEED)-1,
                        false, true, false, false, 0, 1000, 1000, 0, 0, 235,
                        SOUND_JET, true, 0, 0, 0, 0, 0, 0, 0};


// Drak mothership initial state

const drakmstype DRAKMSINIT = {0.0, -32.0, 0.0, 0, 0, 0, 0, int(200/GAME_SPEED), true, true, 0, 0,
                           {false, false, false}};

const drakguntype DGUN_LASER_LEFT = {-1, 24, 27, 0, 0, 0, 0, 0,{305, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0}};
const drakguntype DGUN_LASER_RIGHT = {1, 56, 27, 0, 0, 0, 0, 0,{305, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0}};
const drakguntype DGUN_TOP_LEFT = {0, 2, -3, 1, 0, 0, 0, 0,{0, 281, 280, 279, 278, 277, 0, 0, 0,
                                   0, 0, 0, 0, 0, 284, 283, 282}};
const drakguntype DGUN_TOP_RIGHT = {0, 78, -3, 1, 0, 0, 0, 0,{0, 281, 280, 279, 278, 0, 0, 0, 0,
                                    0, 0, 0, 0, 285, 284, 283, 282}};
const drakguntype DGUN_SIDE_LEFT = {0, -4, 20, 7, 0, 0, 0, 0,{0, 0, 0, 0, 287, 288, 289, 290,
                                    291, 292, 293, 294, 0, 0, 0, 0, 0}};
const drakguntype DGUN_SIDE_RIGHT = {0, 84, 20, 11, 0, 0, 0, 0,{0, 0, 0, 0, 0, 0, 0, 303, 302,
                                     301, 300, 299, 298, 297, 296, 0, 0}};

// Function prototypes

void setup_game(gamedata &);

void init_data(gamedata &);

void drawall(gamedata &);

void game(gamedata &);

void all(gamedata &);

int wrap(int, int, int);

int limit(int, int, int);

double dlimit(double, double, double);

int sign(int);

void draw_dither(SDL_Surface *, int, int, int, int);

void computer_ai(gamedata &, plane &, int &, int &, bool &);

void detect_collisions(gamedata &);

void killbuilding(gamedata &, building &);

void killtower(gamedata &, building &, double, double, int, int);

void setup_intelligence(map &);

bool fall_collision(gamedata &, falltype &);

void gunshoot(guntype &, linkedlist <shottype> &, sampleio &, double[17],
              double[17]);

void finish_game(gamedata &);

void setup_draks(drakmstype &, linkedlist <drakguntype> &);

void drak_main(gamedata &);

double drand();
