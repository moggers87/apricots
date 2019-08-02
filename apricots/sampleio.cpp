// Wrapper Library for OpenAL
// Author: M.D.Snellgrove
// Date: 17/3/2002
// History:

// Changes by M Snellgrove 26/7/2003
//   Conditional Compilation using OpenAL

// Changes by Judebert 1/8/2003
//   Portability fix for WIN32 systems

// Changes by M Snellgrove 5/8/2003
//   Code cleanup

#include "sampleio.h"

// Empty Noaudio sound routines
#if AP_AUDIO==AUDIO_NOAUDIO

sampleio :: sampleio(){}
void sampleio :: init(int, char [][255], int, int){}
void sampleio :: close(){}
void sampleio :: update(){}
void sampleio :: channel(int, int){}
void sampleio :: loop(int, int){}
void sampleio :: play(int){}
void sampleio :: stop(int){}
void sampleio :: psource(int, int, bool){}
void sampleio :: volume(int, double){}
ALboolean sampleio :: sourceisplaying(ALuint){return false;}

#endif

// OpenAL sound routines
#if AP_AUDIO==AUDIO_OPENAL

// Constructor

sampleio :: sampleio(){

  initdone = false;

}

// Initialize OpenAL

void sampleio :: init(int nsamples, char filenames[][255], int nsources,
                      int npool){
  if (initdone){
    cerr << "sampleio: call to init when already in use" << endl;
    exit(1);
  }

  initdone = true;
  numsamples = nsamples;
  numsources = nsources;
  numpool = npool;
  samples = new ALuint[numsamples];
  sources = new ALuint[numsources+numpool];
  poolcount = numsources;

  // Initialize audio device
  alutInit(0, NULL);

  ALfloat zeroes[] = { 0.0f, 0.0f,  0.0f };
  ALfloat back[]   = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
  ALfloat front[]  = { 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f };
  ALfloat position[] = { 0.0f, 0.0f, -4.0f };
  ALsizei filelen;

  // Setup Listener
  alListenerfv(AL_POSITION, zeroes );
  alListenerfv(AL_VELOCITY, zeroes );
  alListenerfv(AL_ORIENTATION, front );

  // Load in samples
  ALvoid* data = malloc(5 * (512 * 3) * 1024);
  alGenBuffers(numsamples, samples);

  for (int i = 0; i < numsamples; i++){
    ALsizei freq;
    ALboolean fileok;
    // Evil OpenAL portability fix done here
#ifdef _WIN32
    ALenum format;
    ALboolean trash;
    alutLoadWAVFile(filenames[i],&format,&data,&filelen,&freq,&trash);
    fileok = (alGetError() == AL_NO_ERROR);
#else
    ALsizei format;
    ALsizei trash;
    fileok = alutLoadWAV(filenames[i],&data,&format,&filelen,&trash,&freq);
#endif
    if (!fileok){
      cerr << "sampleio: could not open " << filenames[i] << endl;
      exit(1);
    }         
    alBufferData(samples[i], format, data, filelen, freq);
  }

  // Generate Sources
  alGenSources(numsources+numpool, sources);

  for(int j=0;j<numsources+numpool;j++){
    alSourcefv(sources[j], AL_POSITION, position );
    alSourcefv(sources[j], AL_VELOCITY, zeroes );
    alSourcefv(sources[j], AL_ORIENTATION, back );
  }
  
  free(data);
  
}

// Clearup routine

void sampleio :: close(){

  if (initdone){
    delete [] samples;
    delete [] sources;
    initdone = false;
    alutExit();
  }

}

// Update method (blank)

void sampleio :: update(){

}

// Play a channel

void sampleio :: channel(int chan, int sample){
  if (!initdone){
    cerr << "sampleio: initialize before use" << endl;
    return;
  }
  if ((chan < 0) || (chan >= numsources)){
    cerr << "sampleio: attempt to play nonexistant source " << chan 
         << endl;
    return;
  }
  if ((sample < 0) || (sample >= numsamples)){
    cerr << "sampleio: attempt to play nonexistant sample " << sample 
         << endl;
    return;
  }

  psource(chan, sample, false);
}

// Loop a sample

void sampleio :: loop(int chan, int sample){

  if (!initdone){
    cerr << "sampleio: initialize before use" << endl;
    return;
  }
  if ((chan < 0) || (chan >= numsources)){
    cerr << "sampleio: attempt to play nonexistant source " << chan 
         << endl;
    return;
  }
  if ((sample < 0) || (sample >= numsamples)){
    cerr << "sampleio: attempt to play nonexistant sample " << sample 
         << endl;
    return;
  }
  psource(chan, sample, true);

}

// Play a sample

void sampleio :: play(int sample){

  if (!initdone){
    cerr << "sampleio: initialize before use" << endl;
    return;
  }
  if (numpool == 0){
    cerr << "sampleio: attempt to play nonexistant pool " << endl;
    return;
  }
  if ((sample < 0) || (sample >= numsamples)){
    cerr << "sampleio: attempt to play nonexistant sample " << sample 
         << endl;
    return;
  }
  poolcount++;
  if (poolcount == numsources + numpool) poolcount = numsources;
  psource(poolcount, sample, false);

}

// Stop current sample

void sampleio :: stop(int i){

  if (!initdone){
    cerr << "sampleio: initialize before use" << endl;
    return;
  }
  if ((i < 0) || (i >= numsources)){
    cerr << "sampleio: attempt to stop nonexistant source " << i
         << endl;
    return;
  }
  if (sourceisplaying(sources[i])){
    alSourceStop(sources[i]);
  }

}

// Actaully start a sample

void sampleio :: psource(int i, int sample, bool loop){

  // Stop current sample
  if (sourceisplaying(sources[i])){
    alSourceStop(sources[i]);
  }

  // Change to new sample
  alSourcei(sources[i], AL_BUFFER, samples[sample]);
  if (loop){
    alSourcei(sources[i], AL_LOOPING, AL_TRUE);
  }else{
    alSourcei(sources[i], AL_LOOPING, AL_FALSE);
  }

  // Play new sample
  alSourcePlay(sources[i]);

}

// Volume control function

void sampleio :: volume(int i, double vol){

  if (!initdone){
    cerr << "sampleio: initialize before use" << endl;
    return;
  }
  if ((i < 0) || (i >= numsources)){
    cerr << "sampleio: attempt to volume nonexistant source " << i 
         << endl;
    return;
  }
  ALfloat volf = ALfloat(vol);
  alSourcef(sources[i], AL_GAIN, volf);

}

// Check on playing sources function

ALboolean sampleio ::  sourceisplaying(ALuint sid) {

  ALint state;
  if(alIsSource(sid) == AL_FALSE){
    return AL_FALSE;
  }
  state = AL_INITIAL;

  // Evil OpenAL portability fix done here
#ifdef _WIN32
  alGetSourcei(sid, AL_SOURCE_STATE, &state);
#else
  alGetSourceiv(sid, AL_SOURCE_STATE, &state);
#endif

  switch(state) {
    case AL_PLAYING: case AL_PAUSED:
      return AL_TRUE;
    default:
      break;
  }
  return AL_FALSE;

}

#endif
