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

#include "SDLfont.h"
#include "linkedlist.h"
#include "sampleio.h"
#include "shape.h"
#include <SDL.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
using namespace std;

// Global constants

const int TILE_SIZE = 32;
const int GAME_WIDTH = 2400;
const int GAME_HEIGHT = 320;
const int MAP_W = GAME_WIDTH / TILE_SIZE;
const int MAP_H = GAME_HEIGHT / TILE_SIZE;
const double GAME_SPEED = 0.5;
const int TICK_INTERVAL = 20;
const double PI = 3.141592;
const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 640;

// Datatypes

struct building {
  enum Type {
    NONE,
    TREE,
    TOWER,
    BUILDING,
    RADAR,
    GUN,
  };
  Type type;
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

const building NB = {building::Type::NONE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // null building
const building CONTROLTOWER = {building::Type::BUILDING, 0, 0, 0, 69, 73, 240, 0, 0, 140, 225};
const building FUEL = {building::Type::BUILDING, 0, 0, 0, 71, 75, 60, 0, 0, 140, 140};
const building HANGAR = {building::Type::BUILDING, 0, 0, 0, 70, 74, 160, 0, 0, 228, 140};
const building GUN = {building::Type::GUN, 0, 0, 0, 166, 170, 200, 0, 0, 140, 140};
const building RADAR = {building::Type::RADAR, 0, 0, 0, 238, 246, 120, 0, 0, 140, 225};
const building TOWER = {building::Type::TOWER, 0, 0, 0, 0, 0, 0, 0, 0, 140, 140};
const building POWERSTATION = {building::Type::BUILDING, 0, 0, 0, 112, 113, 80, 0, 0, 140, 140};
const building COOLINGTOWER_LEFT = {building::Type::BUILDING, 0, 0, 0, 108, 111, 60, 0, 0, 140, 225};
const building COOLINGTOWER_MIDDLE = {building::Type::BUILDING, 0, 0, 0, 109, 111, 60, 0, 0, 140, 225};
const building COOLINGTOWER_RIGHT = {building::Type::BUILDING, 0, 0, 0, 110, 111, 60, 0, 0, 140, 225};
const building FACTORY = {building::Type::BUILDING, 0, 0, 0, 72, 76, 100, 0, 0, 140, 140};
const building CIVILIAN_1 = {building::Type::BUILDING, 0, 0, 0, 194, 195, -30, 0, 0, 222, 222};
const building CIVILIAN_2 = {building::Type::BUILDING, 0, 0, 0, 200, 201, -60, 0, 0, 225, 219};
const building CIVILIAN_3 = {building::Type::BUILDING, 0, 0, 0, 202, 203, -40, 0, 0, 219, 140};
const building FIRTREE = {building::Type::TREE, 0, 0, 0, 258, 259, -10, 0, 0, 143, 143};
const building PALMTREE = {building::Type::TREE, 0, 0, 0, 192, 193, -10, 0, 0, 143, 222};

struct firetype {
  enum Type {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
  };
  int x;
  int y;
  int time = 0;
  Type type = ZERO;
};

struct smoketype {
  int x;
  double y;
  int time;
};

struct lasertype {
  int x;
  int y;
  int image;
  int time;
};

struct radartype {
  int x;
  int y;
  int image;
  int rotate;
  int xpos;
  int id;
};

struct guntype {
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

struct falltype {
  enum Type {
    TREEBITS,
    SHRAPNEL,
    LARGE_BITS,
    BOMB,
  };
  double x;
  double y;
  double xs;
  double ys;
  int image;
  int side = 0;
  Type type;
  int bombrotate = 0;
  int rotatedelay = 0;
};

struct shottype {
  double x;
  double y;
  double xs;
  double ys;
  int side;
  int time;
};

struct map {
  int image[MAP_W][MAP_H];
  int groundheight[MAP_W];
  building b[MAP_W * 2];
  int realheight[MAP_W * 2 + 1];
  int smoothheight[MAP_W * 2 + 1];
  int steepheight[MAP_W * 2 + 1];
  shape ground;
};

struct drakmstype {
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

struct drakguntype {
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

struct airbase {
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
  airbase() {}

  airbase(int rx, int rlength, int bsize, int ppos, int pd, int mx, int px, int py, building b0 = NB, building b1 = NB,
          building b2 = NB, building b3 = NB, building b4 = NB, building b5 = NB, building b6 = NB, building b7 = NB,
          building b8 = NB, building b9 = NB, building b10 = NB, building b11 = NB, building b12 = NB,
          building b13 = NB, building b14 = NB) {
    this->runwayx = rx;
    this->runwaylength = rlength;
    this->size = bsize;
    this->planepos = ppos;
    this->planed = pd;
    this->mapx = mx;
    this->planex = px;
    this->planey = py;
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

struct plane {
  enum LandingState {
    LANDED,
    TAKING_OFF,
    FLYING,
    LANDING,
  };
  enum Coms {
    ACTION,
    GO_UP,
    GO_RIGHT,
    GO_LEFT,
    LEVEL_OFF,
    GO_DOWN,
    GO_UP_LOTS,
    GO_RIGHT_PLUS,
    GO_RIGHT_MINUS,
    GO_LEFT_PLUS,
    GO_LEFT_MINUS,
    START_LANDING,
  };
  enum Control {
    COMPUTER,
    PLAYER_ONE,
    PLAYER_TWO,
  };
  double x;
  double y;
  double xs;
  double ys;
  double s;
  int d;
  Control control;
  LandingState land;
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
  Coms coms;        //
  int targetx;      //
  int targety;      // Computer AI stuff
  int cruiseheight; //
  int gunthreat;    //
};

struct planeclone {
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

struct info {
  int planetype;
  int basetype;
  plane::Control control;
};

struct gamedata {
  int planes;
  int towers;
  int guns;
  int buildings;
  int trees;
  int players;
  int targetscore;
  int mission;
  int winner;
  float volume;
  plane *player1;
  plane *player2;
  airbase base[7];
  info planeinfo[7];
  SDL_Surface *virtualscreen;
  SDL_Surface *gamescreen;
  SDL_Renderer *renderer;
  shape images[319];
  map gamemap;
  linkedlist<radartype> radar;
  linkedlist<guntype> gun;
  linkedlist<plane> p;
  linkedlist<planeclone> dp;
  linkedlist<firetype> explosion;
  linkedlist<firetype> flame;
  linkedlist<smoketype> smoke;
  linkedlist<falltype> fall;
  linkedlist<shottype> shot;
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
  linkedlist<drakguntype> drakgun;
  linkedlist<lasertype> laser;
};

// Airbase definitions

const airbase EMPTY_AIRBASE(0, 0, 0, 0, 0, 0, 0, 0);
const airbase STANDARD_AIRBASE(48, 80, 4, 48, 13, 0, 0, 0, CONTROLTOWER, RADAR, GUN, NB, NB, NB, NB, NB, NB, FUEL,
                               HANGAR);
const airbase REVERSED_AIRBASE(32, 80, 4, 96, 5, 0, 0, 0, HANGAR, FUEL, NB, NB, NB, NB, NB, NB, GUN, RADAR,
                               CONTROLTOWER);
const airbase LITTLE_AIRBASE(16, 80, 2, 16, 13, 0, 0, 0, CONTROLTOWER, NB, NB, NB, NB, NB, NB);
const airbase LONG_AIRBASE(16, 192, 6, 16, 13, 0, 0, 0, CONTROLTOWER, NB, NB, NB, NB, NB, NB, NB, NB, NB, NB, NB, NB,
                           NB, HANGAR);
const airbase ORIGINAL_AIRBASE(32, 80, 3, 32, 13, 0, 0, 0, CONTROLTOWER, RADAR, NB, NB, NB, NB, NB, NB, HANGAR);
const airbase SHOOTY_AIRBASE(80, 80, 6, 80, 13, 0, 0, 0, GUN, FUEL, GUN, CONTROLTOWER, RADAR, NB, NB, NB, NB, NB, NB,
                             HANGAR, GUN, FUEL, GUN);
const airbase TWOGUN_AIRBASE(48, 96, 5, 128, 5, 0, 0, 0, RADAR, GUN, HANGAR, NB, NB, NB, NB, NB, NB, NB, CONTROLTOWER,
                             GUN, RADAR);
const array<airbase, 8> AIRBASES = {
    EMPTY_AIRBASE, STANDARD_AIRBASE, REVERSED_AIRBASE, LITTLE_AIRBASE,
    LONG_AIRBASE,  ORIGINAL_AIRBASE, SHOOTY_AIRBASE,   TWOGUN_AIRBASE,
};

// Sample definitions
enum Sounds {
  SOUND_ENGINE,
  SOUND_JET,
  SOUND_EXPLODE,
  SOUND_GROUNDHIT,
  SOUND_FUELEXPLODE,
  SOUND_SHOT,
  SOUND_GUNSHOT,
  SOUND_BOMB,
  SOUND_SPLASH,
  SOUND_LASER,
  SOUND_STALL,
  SOUND_GUNSHOT2,
  SOUND_BURNER,
  SOUND_FINISH,
};
const int SOUNDS_COUNT = 14;

inline const array<const char *, SOUNDS_COUNT> SOUND_NAMES = {
    "engine.wav", "jet.wav",    "explode.wav", "groundhit.wav", "fuelexplode.wav", "shot.wav",        "gunshot.wav",
    "bomb.wav",   "splash.wav", "laser.wav",   "stall.wav",     "gunshot2.wav",    "afterburner.wav", "finish.wav",
};

// Plane definitions

const plane NULLPLANE = {0.0,
                         0.0,
                         0.0,
                         0.0,
                         0.0,
                         0,
                         plane::Control::COMPUTER,
                         plane::LandingState::LANDED,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         false,
                         false,
                         false,
                         false,
                         0,
                         0,
                         0,
                         0,
                         0,
                         0,
                         SOUND_ENGINE,
                         false,
                         0,
                         0,
                         plane::Coms::ACTION,
                         0,
                         0,
                         0,
                         0};
const plane SPITFIRE = {0.0,
                        0.0,
                        0.0,
                        0.0,
                        0.0,
                        0,
                        plane::Control::COMPUTER,
                        plane::LandingState::LANDED,
                        0,
                        0,
                        0,
                        0,
                        122,
                        0,
                        int(3 / GAME_SPEED) - 1,
                        false,
                        false,
                        false,
                        false,
                        0,
                        8,
                        8,
                        4,
                        4,
                        143,
                        SOUND_ENGINE,
                        false,
                        0,
                        0,
                        plane::Coms::ACTION,
                        0,
                        0,
                        0,
                        0};
const plane JET = {0.0,
                   0.0,
                   0.0,
                   0.0,
                   0.0,
                   0,
                   plane::Control::COMPUTER,
                   plane::LandingState::LANDED,
                   0,
                   0,
                   0,
                   0,
                   76,
                   0,
                   int(2 / GAME_SPEED) - 1,
                   false,
                   true,
                   false,
                   false,
                   0,
                   12,
                   12,
                   5,
                   5,
                   140,
                   SOUND_JET,
                   false,
                   0,
                   0,
                   plane::Coms::ACTION,
                   0,
                   0,
                   0,
                   0};
const plane STEALTH = {0.0,
                       0.0,
                       0.0,
                       0.0,
                       0.0,
                       0,
                       plane::Control::COMPUTER,
                       plane::LandingState::LANDED,
                       0,
                       0,
                       0,
                       0,
                       174,
                       0,
                       int(4 / GAME_SPEED) - 1,
                       false,
                       false,
                       false,
                       true,
                       0,
                       3,
                       3,
                       6,
                       6,
                       235,
                       SOUND_JET,
                       false,
                       0,
                       0,
                       plane::Coms::ACTION,
                       0,
                       0,
                       0,
                       0};
const plane DRAK_FIGHTER = {0.0,
                            0.0,
                            0.0,
                            2.0 * GAME_SPEED,
                            2.0 * GAME_SPEED,
                            9,
                            plane::Control::COMPUTER,
                            plane::LandingState::FLYING,
                            1,
                            0,
                            0,
                            0,
                            259,
                            0,
                            int(1 / GAME_SPEED) - 1,
                            false,
                            true,
                            false,
                            false,
                            0,
                            1000,
                            1000,
                            0,
                            0,
                            235,
                            SOUND_JET,
                            true,
                            0,
                            0,
                            plane::Coms::ACTION,
                            0,
                            0,
                            0,
                            0};

const array<plane, 5> PLANES = {
    NULLPLANE, SPITFIRE, JET, STEALTH, DRAK_FIGHTER,
};

// Drak mothership initial state

const drakmstype DRAKMSINIT = {
    0.0, -32.0, 0.0, 0, 0, 0, 0, int(200 / GAME_SPEED), true, true, 0, 0, {false, false, false}};

const drakguntype DGUN_LASER_LEFT = {-1, 24, 27, 0, 0, 0, 0, 0, {305, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
const drakguntype DGUN_LASER_RIGHT = {1, 56, 27, 0, 0, 0, 0, 0, {305, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
const drakguntype DGUN_TOP_LEFT = {
    0, 2, -3, 1, 0, 0, 0, 0, {0, 281, 280, 279, 278, 277, 0, 0, 0, 0, 0, 0, 0, 0, 284, 283, 282}};
const drakguntype DGUN_TOP_RIGHT = {
    0, 78, -3, 1, 0, 0, 0, 0, {0, 281, 280, 279, 278, 0, 0, 0, 0, 0, 0, 0, 0, 285, 284, 283, 282}};
const drakguntype DGUN_SIDE_LEFT = {
    0, -4, 20, 7, 0, 0, 0, 0, {0, 0, 0, 0, 287, 288, 289, 290, 291, 292, 293, 294, 0, 0, 0, 0, 0}};
const drakguntype DGUN_SIDE_RIGHT = {
    0, 84, 20, 11, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 303, 302, 301, 300, 299, 298, 297, 296, 0, 0}};

// Land types
enum LandType {
  HILAND,
  LOLAND,
  BEACH,
  PORTLEFT,
  PORTRIGHT,
  SEA,
};

// Function prototypes

Uint32 time_left(Uint32);
bool fall_collision(gamedata &, falltype &);
double drand();
int getConfig(string, string, int, int, int);
LandType randomland(LandType, LandType);
int sign(int);
int wrap(int, int, int);
string find_config_file();
string getConfig(string, string, string);
void act(gamedata &, int, int, bool);
void all(gamedata &);
void animate_explosions(linkedlist<firetype> &);
void animate_flames(linkedlist<firetype> &, linkedlist<smoketype> &);
void animate_smoke(linkedlist<smoketype> &);
void clone_planes(linkedlist<plane> &, linkedlist<planeclone> &, int, int &);
void computer_ai(gamedata &, plane &, int &, int &, bool &);
void control(gamedata &, const Uint8 *, int &, int &, bool &);
void create_airbases(airbase *, info *, int);
void decay_lasers(linkedlist<lasertype> &);
void detect_collisions(gamedata &);
void display_playinfo(SDL_Surface *, int, plane &, int, shape *, int, int, SDLfont &, SDLfont &);
void display_score(SDL_Surface *, plane &, int, int, int, int, SDLfont &, SDLfont &);
void drak_main(gamedata &);
void drakgunshoot(drakguntype &, drakmstype &, linkedlist<shottype> &, sampleio &, double *, double *);
void draw_buildings(building *, SDL_Surface *, shape *);
void draw_dither(SDL_Surface *, int, int, int, int);
void draw_map(int[MAP_W][MAP_H], SDL_Surface *, shape *, shape &);
void draw_mapblocks(int[MAP_W][MAP_H], SDL_Surface *, shape *);
void draw_runways(airbase *, int, int *, SDL_Surface *);
void drawall(gamedata &);
void drawblits(gamedata &, int, int);
void drop_bomb(plane &, linkedlist<falltype> &, sampleio &, int *);
void finish_game(gamedata &);
void fire_drakguns(gamedata &, linkedlist<drakguntype> &, drakmstype &, linkedlist<plane> &, sampleio &,
                   linkedlist<lasertype> &, building *, linkedlist<shottype> &, int *, double *, double *);
void fire_guns(linkedlist<guntype> &, linkedlist<plane> &, sampleio &, building[MAP_W * 2], linkedlist<shottype> &,
               double[17], double[17]);
void fire_laser(gamedata &, drakguntype &, drakmstype &, sampleio &, building *, linkedlist<lasertype> &, int *);
void fire_shot(plane &, linkedlist<shottype> &, sampleio &, double *, double *);
void followtarget(plane &, int &, int &, int, int, bool);
void game(gamedata &);
void gunshoot(guntype &, linkedlist<shottype> &, sampleio &, double[17], double[17]);
void init_data(gamedata &);
void init_gameconstants(gamedata &);
void init_gamedata(gamedata &);
void init_sound(gamedata &);
void keyboard(const Uint8 *, int &, int &, bool &, SDL_Scancode, SDL_Scancode, SDL_Scancode, SDL_Scancode,
              SDL_Scancode);
void killbuilding(gamedata &, building &);
void killtower(gamedata &, building &, double, double, int, int);
void launch_drakfighter(drakmstype &, linkedlist<plane> &, linkedlist<planeclone> &);
void load_font(SDL_Surface *, SDLfont &, SDLfont &);
void load_shapes(gamedata &, shape *);
void move_falls(gamedata &);
void move_shots(linkedlist<shottype> &, shape &, shape &, shape &, drakmstype &);
void plane_collisions(gamedata &);
void rotate_radars(linkedlist<radartype> &);
void setup_buildings(gamedata &, bool *);
void setup_display(gamedata &);
void setup_draks(drakmstype &, linkedlist<drakguntype> &);
void setup_game(gamedata &);
void setup_intelligence(map &);
void setup_map(map &, int, airbase *, bool *);
void setup_planes(linkedlist<plane> &, linkedlist<planeclone> &, airbase *, int, info *, plane *&, plane *&);
void switch_bad_default(const char *, const char *, int);
void winnerbox(gamedata &, int, int, int, int);
