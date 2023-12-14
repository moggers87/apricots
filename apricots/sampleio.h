// 'sampleio' Class Header
// Author: M.D.Snellgrove
// Date: 17/3/2002
// History:

// Changes by M Snellgrove 25/7/2003
//   Conditional Compilation for OpenAL

// OpenAL includes
#include <AL/al.h>
#include <AL/alc.h>
#if defined(__APPLE__)
#include <OpenAL/alure.h>
#else
#include <AL/alure.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

// Class header

class sampleio {
private:
  int numsamples;
  int numsources;
  int numpool;
  ALuint *sources;
  bool initdone;
  ALuint *samples;
  int poolcount;
  void psource(int source, int sample, bool loop);
  ALboolean sourceisplaying(ALuint);
  float base_volume;

public:
  sampleio();
  void init(float, int, char[][255], int, int);
  void close();
  void update();
  void channel(int, int);
  void loop(int, int);
  void play(int);
  void stop(int);
  void volume(int, double);
};
