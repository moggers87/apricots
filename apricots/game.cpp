// Apricots game routine
// Author: M.D.Snellgrove
// Date: 24/3/2002
// History:

// Changes by M Harman for Windows version, June 2003:
//   Changes for input stuff.
//   Changes to main loop.

// Changes by M Snellgrove 7/7/2003
// Keyboard input tidied up and controls for player 2 re-activated

// Changed by M Snellgrove 30/7/2003
// GAME_SPEED bugfix in smoke timings and bomb y-velocity

// Changed by M Snellgrove 3/8/2003
// Delay loop now fixed (thanks M Harman)

#include "apricots.h"

// Fire shot

void fire_shot(plane &p, linkedlist<shottype> &shot, sampleio &sound, double xmove[17], double ymove[17]) {

  shottype bullet;
  bullet.x = p.x + 6.0 + 12.0 * xmove[p.d] + p.xs;
  bullet.y = p.y + 6.0 + 12.0 * ymove[p.d] + p.ys;
  bullet.xs = 8.0 * xmove[p.d] * GAME_SPEED;
  bullet.ys = 8.0 * ymove[p.d] * GAME_SPEED;
  bullet.side = p.side;
  bullet.time = int(40 / GAME_SPEED);
  shot.add(bullet);
  p.ammo--;
  p.shotdelay = int(3 / GAME_SPEED);
  sound.play(SOUND_SHOT);
}

// Drop bomb

void drop_bomb(plane &p, linkedlist<falltype> &fall, sampleio &sound, int bombimage[17]) {

  falltype bomb;
  bomb.x = p.x + 5.0 + p.xs;
  bomb.y = p.y + 16.0 + p.ys;
  bomb.xs = p.xs;
  bomb.ys = p.ys + (2.0 * GAME_SPEED);
  bomb.image = bombimage[p.d];
  bomb.side = p.side;
  bomb.type = 3;
  switch (p.d) {
  case 0:
    break;
  case 1:
  case 9:
    bomb.bombrotate = 0;
    break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
    bomb.bombrotate = -1;
    break;
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
    bomb.bombrotate = 1;
    break;
  default:
    switch_bad_default("plane.d", __FILE__, __LINE__);
    break;
  }
  bomb.rotatedelay = 0;
  fall.add(bomb);
  p.bombs--;

  p.shotdelay = int(5 / GAME_SPEED);
  sound.play(SOUND_BOMB);
}

// Main plane action routine

void act(gamedata &g, int jx, int jy, bool jb) {

  // Reset hidden/afterburner status
  g.p().hide = false;
  g.p().boost = false;
  g.p().gunthreat = 0;

  switch (g.p().state) {
  case 0: // OK planes

    switch (g.p().land) {
    case 0: // Stationary on runway
      // Check for mission 0 and mission 1 wins
      if (((g.mission == 0) || (g.mission == 1)) && (!g.p().drak) && (g.p().score >= g.targetscore)) {
        g.winner = g.p().side;
      }
      // Start Engine
      if (jy == -1) {
        g.p().s = 0.3 * GAME_SPEED * GAME_SPEED;
        g.p().land = plane::LandingState::TAKING_OFF;
        if ((g.p().control) > 0) {
          g.sound.volume(g.p().control - 1, 0.0);
          g.sound.loop(g.p().control - 1, g.p().enginesample);
        }
      }
      break;

    case 1: // Taking off plane
      if (jy == -1) {
        g.p().s = clamp(g.p().s + 0.3 * GAME_SPEED * GAME_SPEED, 0.0, 6.0 * GAME_SPEED);
      }
      // Take off plane
      if ((jx == -1) && (g.p().s > 2.0 * GAME_SPEED) && (g.base[g.p().side].planed == 13)) {
        g.p().d++;
        g.p().rotate = g.p().maxrotate;
        g.p().land = plane::LandingState::FLYING;
      }
      if ((jx == 1) && (g.p().s > 2.0 * GAME_SPEED) && (g.base[g.p().side].planed == 5)) {
        g.p().d--;
        g.p().rotate = g.p().maxrotate;
        g.p().land = plane::LandingState::FLYING;
      }
      // Off end of runway
      if (abs(int(g.p().x - g.base[g.p().side].planex)) > g.base[g.p().side].runwaylength) {
        g.p().land = plane::LandingState::FLYING;
      }
      break;

    case 2: // flying
      // Navigate plane
      if ((g.p().rotate == 0) && (jx != 0)) {
        g.p().d = wrap(g.p().d - jx, 1, 17);
        g.p().rotate = g.p().maxrotate;
      } else {
        if (g.p().rotate > 0) {
          g.p().rotate--;
        }
      }
      // Acceleration / Afterburner Controls
      {
        double acceleration = g.accel[g.p().d] * GAME_SPEED * GAME_SPEED;
        if (g.p().burner) {
          if (jy == -1) {
            acceleration += 0.3 * GAME_SPEED * GAME_SPEED;
            g.p().boost = true;
          }
          if ((g.p().s > 6.0 * GAME_SPEED) && (jy != -1)) {
            acceleration -= 0.3 * GAME_SPEED * GAME_SPEED;
          }
          g.p().s = clamp(g.p().s + acceleration, 0.0, 12.0 * GAME_SPEED);
        } else {
          g.p().s = clamp(g.p().s + acceleration, 0.0, 6.0 * GAME_SPEED);
        }
      }
      // Stealth Controls
      if ((jy == -1) && (jx == 0) && (!jb) && (g.p().stealth)) {
        g.p().hide = true;
      }
      // Check for shotfire
      if (g.p().shotdelay == 0) {
        if ((jb) && (g.p().ammo > 0)) {
          fire_shot(g.p(), g.shot, g.sound, g.xmove, g.ymove);
        }
        // Check for bombdrop
        if ((jy == 1) && (g.p().bombs > 0)) {
          drop_bomb(g.p(), g.fall, g.sound, g.bombimage);
        }
      } else {
        g.p().shotdelay--;
      }
      break;

    case plane::LandingState::LANDING:
      if ((((g.p().x - g.base[g.p().side].planex) < 2.0) && (g.base[g.p().side].planed == 13)) ||
          (((g.p().x - g.base[g.p().side].planex) > -2.0) && (g.base[g.p().side].planed == 5))) {
        g.p().land = plane::LandingState::LANDED;
        g.p().x = g.base[g.p().side].planex;
        g.p().y = g.base[g.p().side].planey;
        g.p().d = g.base[g.p().side].planed;
        g.p().xs = 0;
        g.p().ys = 0;
        g.p().s = 0;
        g.p().ammo = g.p().maxammo;
        g.p().bombs = g.p().maxbombs;
        g.p().coms = plane::Coms::ACTION;
        g.p().targetx = 0;
        g.p().targety = 0;
        g.p().cruiseheight = 0;
      }
      break;
    default:
      switch_bad_default("plane.land", __FILE__, __LINE__);
      break;
    }
    // Set speed for planes
    g.p().xs = g.p().s * g.xmove[g.p().d];
    g.p().ys = g.p().s * g.ymove[g.p().d];
    // Check for stall
    if ((g.p().s < 1.0 * GAME_SPEED) && (g.p().land == 2)) {
      g.p().state = 1;
      if ((g.p().control) > 0) {
        g.sound.stop(g.p().control - 1);
        g.sound.play(SOUND_STALL);
      }
    }
    if (g.p().y < 0) {
      g.p().y = 0;
      g.p().ys = 0;
      g.p().state = 1;
      if ((g.p().control) > 0) {
        g.sound.stop(g.p().control - 1);
        g.sound.play(SOUND_STALL);
      }
    }
    break;

  case 1: // Stalling planes
    // Navigate plane
    if ((g.p().rotate == 0) && (jx != 0)) {
      g.p().d = wrap(g.p().d - jx, 1, 17);
      g.p().rotate = g.p().maxrotate;
    } else {
      if (g.p().rotate > 0) {
        g.p().rotate--;
      }
    }
    // Check for shotfire
    if (g.p().shotdelay == 0) {
      if ((jb) && (g.p().ammo > 0)) {
        fire_shot(g.p(), g.shot, g.sound, g.xmove, g.ymove);
      }
      // Check for bombdrop
      if ((jy == 1) && (g.p().bombs > 0)) {
        drop_bomb(g.p(), g.fall, g.sound, g.bombimage);
      }
    } else {
      g.p().shotdelay--;
    }
    // Gravity and drag
    g.p().ys += 0.1 * GAME_SPEED * GAME_SPEED;
    if (fabs(g.p().xs) > 0.02 * GAME_SPEED * GAME_SPEED) {
      g.p().xs -= g.p().xs / fabs(g.p().xs) * 0.02 * GAME_SPEED * GAME_SPEED;
    }
    // Recover from Stall
    if ((g.p().ys > 3.0 * GAME_SPEED) && (g.p().d == 9)) {
      g.p().s = clamp(g.p().ys, 3.0 * GAME_SPEED, 6.0 * GAME_SPEED);
      g.p().state = 0;
      g.p().xs = g.p().s * g.xmove[g.p().d];
      g.p().ys = g.p().s * g.ymove[g.p().d];
      g.p().coms = plane::Coms::ACTION;
      if ((g.p().control) > 0) {
        double volume = g.p().s / (6.0 * GAME_SPEED);
        g.sound.volume(g.p().control - 1, volume * 0.5);
        g.sound.loop(g.p().control - 1, g.p().enginesample);
      }
    }
    break;

  case 2: // Dead planes
    // Gravity and drag
    g.p().ys += 0.1 * GAME_SPEED * GAME_SPEED;
    if (fabs(g.p().xs) > 0.02 * GAME_SPEED * GAME_SPEED) {
      g.p().xs -= g.p().xs / fabs(g.p().xs) * 0.02 * GAME_SPEED * GAME_SPEED;
    }
    // Smoking plane
    g.p().crash++;
    if (g.p().crash == int(5 / GAME_SPEED)) {
      g.p().crash = 0;
      smoketype newsmoke;
      newsmoke.x = int(g.p().x);
      newsmoke.y = g.p().y;
      newsmoke.time = 0;
      g.smoke.add(newsmoke);
    }
    break;

  case 3: // Crashed planes
    g.p().crash++;
    if (g.p().crash == int(70 / GAME_SPEED)) {
      if (!g.p().drak) {
        // Respawn plane to runway
        g.p().land = plane::LandingState::LANDED;
        g.p().state = 0;
        g.p().rotate = 0;
        g.p().crash = 0;
        g.p().x = g.base[g.p().side].planex;
        g.p().y = g.base[g.p().side].planey;
        g.p().d = g.base[g.p().side].planed;
        g.p().xs = 0;
        g.p().ys = 0;
        g.p().s = 0;
        g.p().ammo = g.p().maxammo;
        g.p().bombs = g.p().maxbombs;
        g.p().coms = plane::Coms::ACTION;
        g.p().targetx = 0;
        g.p().targety = 0;
        g.p().cruiseheight = 0;
      } else {
        // Expunge drak
        g.p().state = 4;
      }
    }
    break;
  default:
    switch_bad_default("plane.state", __FILE__, __LINE__);
    break;
  }
  // Move the planes
  g.p().x += g.p().xs;
  g.p().y += g.p().ys;

  // Control Engine Volume
  if ((g.p().control) > 0) {
    if ((g.p().state == 0) && (g.p().land > 0)) {
      double volume = g.p().s / (6.0 * GAME_SPEED);
      g.sound.volume(g.p().control - 1, volume * 0.5);
      if ((g.p().boost) && (g.p().enginesample == SOUND_JET)) {
        g.p().enginesample = SOUND_BURNER;
        g.sound.loop(g.p().control - 1, SOUND_BURNER);
      }
      if ((!g.p().boost) && (g.p().enginesample == SOUND_BURNER)) {
        g.p().enginesample = SOUND_JET;
        g.sound.loop(g.p().control - 1, SOUND_JET);
      }
    } else {
      g.sound.stop(g.p().control - 1);
    }
  }

  if (g.p().state < 3)
    detect_collisions(g);
}

// Actual keyboard detection

void keyboard(const Uint8 *keys, int &jx, int &jy, bool &jb, SDL_Scancode up, SDL_Scancode down, SDL_Scancode left,
              SDL_Scancode right, SDL_Scancode fire) {

  if (keys[left]) {
    jx = -1;
  }
  if (keys[right]) {
    jx = 1;
  }
  if (keys[up]) {
    jy = -1;
  }
  if (keys[down]) {
    jy = 1;
  }
  if (keys[fire]) {
    jb = true;
  }
}

// Plane control routine
// Returns control in arguments jx,jy,jb

void control(gamedata &g, const Uint8 *keys, int &jx, int &jy, bool &jb) {

  switch (g.p().control) {
    case 1: // Player 1
      keyboard(keys, jx, jy, jb, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
               SDL_SCANCODE_RETURN);
      break;
    case 2: // Player 2
      keyboard(keys, jx, jy, jb, SDL_SCANCODE_S, SDL_SCANCODE_X, SDL_SCANCODE_Z, SDL_SCANCODE_C, SDL_SCANCODE_LCTRL);
      break;
    default: // Computer controlled
      computer_ai(g, g.p(), jx, jy, jb);
      break;
  }
}

// Calculate time pause to stabilize framerate

Uint32 time_left(Uint32 next_time) {

  Uint32 now = SDL_GetTicks();
  if (next_time <= now)
    return 0;
  else
    return next_time - now;
}

// Main game loop

void game(gamedata &g) {

  SDL_Event event;
  bool quit = false;

  // Set first tick interval
  Uint32 next_time = SDL_GetTicks() + TICK_INTERVAL;

  while (!quit && (g.winner == 0)) {

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE] == SDL_PRESSED || (SDL_PollEvent(&event) == 1 && event.type == SDL_QUIT)) {
      printf("Quit requested, quitting.\n");
      quit = true;
    }

    // Go through each plane individually
    g.p.reset();
    while (g.p.next()) {

      // Setup virtual joystick
      int jx = 0;

      int jy = 0;
      bool jb = false;

      if (g.p().state < 2) {
        control(g, keys, jx, jy, jb);
      }
      // Then move the plane
      act(g, jx, jy, jb);
      // Remove expunged planes
      if (g.p().state == 4) {
        g.drakms.fighter[g.p().id - 7] = false;
        g.drakms.fightersout--;
        g.dp.reset();
        while (g.dp.next()) {
          if (g.dp().id == g.p().id)
            g.dp.kill();
        }
        g.p.kill();
      }
    }

    // Then do everything else
    all(g);
    drawall(g);

    // Delay for time remaining in TICK_INTERVAL
    SDL_Delay(time_left(next_time));
    next_time = SDL_GetTicks() + TICK_INTERVAL;
  }
}
