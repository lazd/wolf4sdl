//
//      ID Engine
//      ID_SD.h - Sound Manager Header
//      Version for Wolfenstein
//      By Jason Blochowiak
//

#ifndef __ID_SD__
#define __ID_SD__

#define alOut(n,b) YM3812Write(0, n, b)

#ifdef  __DEBUG__
#define __DEBUG_SoundMgr__
#endif

#define TickBase        70      // 70Hz per tick - used as a base for timer 0

typedef enum
{
    sdm_Off,
    sdm_PC,sdm_AdLib,
} SDMode;

typedef enum
{
    smm_Off,smm_AdLib
} SMMode;

typedef enum
{
    sds_Off,sds_PC,sds_SoundSource,sds_SoundBlaster
} SDSMode;

typedef struct
{
    longword        length;
    word            priority;
} SoundCommon;

//      PC Sound stuff
#define pcTimer         0x42
#define pcTAccess       0x43
#define pcSpeaker       0x61

#define pcSpkBits       3

typedef struct
{
    SoundCommon     common;
    byte            data[1];
} PCSound;

// Registers for the Sound Blaster card - needs to be offset by n0 (0x10,0x20,0x30,0x40,0x50,0x60)
#define sbReset         0x206   // W
#define sbFMStatus      0x208   // R
#define sbFMAddr        0x208   // W
#define sbFMData        0x209   // W
#define sbReadData      0x20a   // R
#define sbWriteCmd      0x20c   // W
#define sbWriteData     0x20c   // W
#define sbWriteStat     0x20c   // R
#define sbDataAvail     0x20e   // R

//      Registers for the Sound Blaster Pro card - needs to be offset by n0 (0x20 or 0x40)
#define sbpLFMStatus    0x200   // R
#define sbpLFMAddr      0x200   // W
#define sbpLFMData      0x201   // W
#define sbpRFMStatus    0x202   // R
#define sbpRFMAddr      0x202   // W
#define sbpRFMData      0x203   // W
#define sbpMixerAddr    0x204   // W
#define sbpMixerData    0x205   // RW
#define sbpCDData       0x210   // R
#define sbpCDCommand    0x210   // W
#define sbpCDStatus     0x211   // R
#define sbpCDReset      0x212   // W

//      SBPro Mixer addresses
#define sbpmReset       0x00
#define sbpmVoiceVol    0x04
#define sbpmMicMix      0x0a
#define sbpmFilterADC   0x0c
#define sbpmControl     0x0e
#define sbpmMasterVol   0x22
#define sbpmFMVol       0x26
#define sbpmCDVol       0x28
#define sbpmLineVol     0x2e

typedef struct
{
    SoundCommon     common;
    word            hertz;
    byte            bits,
                    reference;
    byte            data[1];
} SampledSound;

//      Registers for the AdLib card
#define alFMStatus      0x388   // R
#define alFMAddr        0x388   // W
#define alFMData        0x389   // W

//      Register addresses
// Operator stuff
#define alChar          0x20
#define alScale         0x40
#define alAttack        0x60
#define alSus           0x80
#define alWave          0xe0
// Channel stuff
#define alFreqL         0xa0
#define alFreqH         0xb0
#define alFeedCon       0xc0
// Global stuff
#define alEffects       0xbd

typedef struct
{
    byte    mChar,cChar,
            mScale,cScale,
            mAttack,cAttack,
            mSus,cSus,
            mWave,cWave,
            nConn,

            // These are only for Muse - these bytes are really unused
            voice,
            mode;
    byte    unused[3];
} Instrument;

typedef struct
{
    SoundCommon     common;
    Instrument      inst;
    byte            block;
    byte            data[1];
} AdLibSound;

//
//      Sequencing stuff
//
#define sqMaxTracks     10
#define sqMaxMoods      1       // DEBUG

#define sev_Null        0       // Does nothing
#define sev_NoteOff     1       // Turns a note off
#define sev_NoteOn      2       // Turns a note on
#define sev_NotePitch   3       // Sets the pitch of a currently playing note
#define sev_NewInst     4       // Installs a new instrument
#define sev_NewPerc     5       // Installs a new percussive instrument
#define sev_PercOn      6       // Turns a percussive note on
#define sev_PercOff     7       // Turns a percussive note off
#define sev_SeqEnd      -1      // Terminates a sequence

//      Flags for MusicGroup.flags
#define sf_Melodic              0
#define sf_Percussive   1

#if 1
typedef struct
{
    word    length;
    word    values[1];
} MusicGroup;
#else
typedef struct
{
    word    flags,
            count;
    word    offsets[1];
} MusicGroup;
#endif

typedef struct
{
    /* This part needs to be set up by the user */
    word        mood;
    word        *moods[sqMaxMoods];

    /* The rest is set up by the code */
    Instrument  inst;
    boolean     percussive;
    word        *seq;
    longword    nextevent;
} ActiveTrack;

#define sqmode_Normal           0
#define sqmode_FadeIn           1
#define sqmode_FadeOut          2

#define sqMaxFade               64      // DEBUG

typedef struct
{
    int valid;
    fixed globalsoundx, globalsoundy;
} globalsoundpos;

extern globalsoundpos channelSoundPos[];

// Global variables
extern  boolean         AdLibPresent,
                        SoundSourcePresent,
                        SoundBlasterPresent,
                        SoundPositioned;
//extern        boolean         NeedsMusic;                                     // For Caching Mgr
extern  SDMode          SoundMode;
extern  SDSMode         DigiMode;
extern  SMMode          MusicMode;
extern  boolean         DigiPlaying;
extern  int             DigiMap[];
extern  int             DigiChannel[];
//extern  volatile longword TimeCount;                                    // Global time in ticks
#define GetTimeCount()  ((SDL_GetTicks()*7)/100)
#define Delay(wolfticks) SDL_Delay(wolfticks * 100 / 7)

// Function prototypes
extern  void    SD_Startup(void),
                SD_Shutdown(void);
//extern  void SD_Default(boolean gotit,SDMode sd,SMMode sm);

extern  int     SD_GetChannelForDigi(int which);
extern  void    SD_PositionSound(int leftvol,int rightvol);
extern  boolean SD_PlaySound(soundnames sound);
extern  void    SD_SetPosition(int channel, int leftvol,int rightvol);
extern  void    SD_StopSound(void),
                SD_WaitSoundDone(void);

extern  void    SD_StartMusic(MusicGroup *music);
extern  void    SD_ContinueMusic(MusicGroup *music, int startoffs);
extern  void    SD_MusicOn(void),
                SD_FadeOutMusic(void);
extern  int     SD_MusicOff(void);

//extern  void SD_SetUserHook(void (*hook)(void));
extern  boolean SD_MusicPlaying(void);
extern  boolean SD_SetSoundMode(SDMode mode);
extern  boolean SD_SetMusicMode(SMMode mode);
extern  word    SD_SoundPlaying(void);

extern  void    SD_SetDigiDevice(SDSMode);
extern  void	SD_PrepareSound(int which);
extern  int     SD_PlayDigitized(word which,int leftpos,int rightpos);
extern  void    SD_StopDigitized(void),
                SD_Poll(void);

#ifdef  _MUSE_  // MUSE Goes directly to the lower level routines
extern  void    SDL_PCPlaySound(PCSound far *sound),
                SDL_PCStopSound(void),
                SDL_ALPlaySound(AdLibSound far *sound),
                SDL_ALStopSound(void);
#endif

#define SDL_LoadDigiSegment(x) (Pages+((PMSoundStart+(x))<<12))

#endif
