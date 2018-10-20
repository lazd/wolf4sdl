////////////////////////////////////////////////////////////////////
//
// WL_MENU.C
// by John Romero (C) 1992 Id Software, Inc.
//
////////////////////////////////////////////////////////////////////

#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
#endif

#include "wl_def.h"
#pragma hdrstop

extern int lastgamemusicoffset;
extern int numEpisodesMissing;

//
// PRIVATE PROTOTYPES
//
int CP_ReadThis (int);

#ifdef SPEAR
#define STARTITEM       newgame

#else
#ifdef GOODTIMES
#define STARTITEM       newgame

#else
#define STARTITEM       readthis
#endif
#endif

// ENDSTRx constants are defined in foreign.h
char endStrings[9][80] = {
    ENDSTR1,
    ENDSTR2,
    ENDSTR3,
    ENDSTR4,
    ENDSTR5,
    ENDSTR6,
    ENDSTR7,
    ENDSTR8,
    ENDSTR9
};

CP_itemtype MainMenu[] = {
#ifdef JAPAN
    {1, "", CP_NewGame},
    {1, "", CP_Sound},
    {1, "", CP_Control},
    {1, "", CP_LoadGame},
    {0, "", CP_SaveGame},
    {1, "", CP_ChangeView},
    {2, "", CP_ReadThis},
    {1, "", CP_ViewScores},
    {1, "", 0},
    {1, "", 0}
#else

    {1, STR_NG, CP_NewGame},
    {1, STR_SD, CP_Sound},
    {1, STR_CL, CP_Control},
    {1, STR_LG, CP_LoadGame},
    {0, STR_SG, CP_SaveGame},
    {1, STR_CV, CP_ChangeView},

#ifndef GOODTIMES
#ifndef SPEAR

#ifdef SPANISH
    {2, "Ve esto!", CP_ReadThis},
#else
    {2, "Read This!", CP_ReadThis},
#endif

#endif
#endif

    {1, STR_VS, CP_ViewScores},
    {1, STR_BD, 0},
    {1, STR_QT, 0}
#endif
};

CP_itemtype SndMenu[] = {
#ifdef JAPAN
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {0, "", 0},
    {0, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {0, "", 0},
    {0, "", 0},
    {1, "", 0},
    {1, "", 0},
#else
    {1, STR_NONE, 0},
    {0, STR_PC, 0},
    {1, STR_ALSB, 0},
    {0, "", 0},
    {0, "", 0},
    {1, STR_NONE, 0},
    {0, STR_DISNEY, 0},
    {1, STR_SB, 0},
    {0, "", 0},
    {0, "", 0},
    {1, STR_NONE, 0},
    {1, STR_ALSB, 0}
#endif
};

#ifdef JAPAN
enum { CTL_MOUSEENABLE, CTL_MOVEWITHMOUSE, CTL_JOYENABLE, CTL_JOY2BUTTONUNKNOWN, CTL_GAMEPADUNKONWN, CTL_MOUSESENS, CTL_JOYSENS, CTL_CUSTOMIZEKB, CTL_CUSTOMIZEJOY };
#else
enum { CTL_MOUSEENABLE, CTL_MOVEWITHMOUSE, CTL_MOUSESENS, CTL_CUSTOMIZEKB, CTL_JOYENABLE, CTL_JOYSENS, CTL_CUSTOMIZEJOY };
#endif

CP_itemtype CtlMenu[] = {
#ifdef JAPAN
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", MouseSensitivity},
    {0, "", JoySensitivity},
    {1, "", CustomControls},
    {0, "", CustomGamepadControls}
#else
    {0, STR_MOUSEEN, 0},
    {0, STR_MOVEWITHMOUSE, 0},
    {0, STR_SENS, MouseSensitivity},
    {1, STR_CUSTOM, CustomControls},
    {0, STR_JOYEN, 0},
    {0, STR_JOYSENS, JoySensitivity},
    {0, STR_CUSTOMJOY, CustomGamepadControls}
#endif
};

#ifndef SPEAR
CP_itemtype NewEmenu[] = {
#ifdef JAPAN
#ifdef JAPDEMO
    {1, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
#else
    {1, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0}
#endif
#else
#ifdef SPANISH
    {1, "Episodio 1\n" "Fuga desde Wolfenstein", 0},
    {0, "", 0},
    {3, "Episodio 2\n" "Operacion Eisenfaust", 0},
    {0, "", 0},
    {3, "Episodio 3\n" "Muere, Fuhrer, Muere!", 0},
    {0, "", 0},
    {3, "Episodio 4\n" "Un Negro Secreto", 0},
    {0, "", 0},
    {3, "Episodio 5\n" "Huellas del Loco", 0},
    {0, "", 0},
    {3, "Episodio 6\n" "Confrontacion", 0}
#else
    {1, "Episode 1\n" "Escape from Wolfenstein", 0},
    {0, "", 0},
    {3, "Episode 2\n" "Operation: Eisenfaust", 0},
    {0, "", 0},
    {3, "Episode 3\n" "Die, Fuhrer, Die!", 0},
    {0, "", 0},
    {3, "Episode 4\n" "A Dark Secret", 0},
    {0, "", 0},
    {3, "Episode 5\n" "Trail of the Madman", 0},
    {0, "", 0},
    {3, "Episode 6\n" "Confrontation", 0}
#endif
#endif
};
#endif


CP_itemtype NewMenu[] = {
#ifdef JAPAN
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0}
#else
    {1, STR_DADDY, 0},
    {1, STR_HURTME, 0},
    {1, STR_BRINGEM, 0},
    {1, STR_DEATH, 0}
#endif
};

CP_itemtype LSMenu[] = {
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0},
    {1, "", 0}
};

CP_itemtype CusMenu[] = {
    {1, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0},
    {1, "", 0}
};

CP_itemtype CusJoyMenu[] = {
    {1, "", 0},
    {0, "", 0},
    {0, "", 0},
    {1, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0},
    {0, "", 0}
};

// CP_iteminfo struct format: short x, y, amount, curpos, indent;
CP_iteminfo MainItems = { MENU_X, MENU_Y, lengthof(MainMenu), STARTITEM, 24 },
            SndItems  = { SM_X, SM_Y1, lengthof(SndMenu), 0, 52 },
            LSItems   = { LSM_X, LSM_Y, lengthof(LSMenu), 0, 24 },
            CtlItems  = { CTL_X, CTL_Y, lengthof(CtlMenu), -1, 56 },
            CusItems  = { 8, CST_Y + 13 * 2, lengthof(CusMenu), -1, 0},
            CusJoyItems  = { 8, CST_Y + 13 * 2, lengthof(CusJoyMenu), -1, 0},
#ifndef SPEAR
            NewEitems = { NE_X, NE_Y, lengthof(NewEmenu), 0, 88 },
#endif
            NewItems  = { NM_X, NM_Y, lengthof(NewMenu), 2, 24 };

int color_hlite[] = {
    DEACTIVE,
    HIGHLIGHT,
    READHCOLOR,
    0x67
};

int color_norml[] = {
    DEACTIVE,
    TEXTCOLOR,
    READCOLOR,
    0x6b
};

int EpisodeSelect[6] = { 1 };


static int SaveGamesAvail[10];
static int StartGame;
static int SoundStatus = 1;
static int pickquick;
static char SaveGameNames[10][32];
static char SaveName[13] = "savegam?.";

#define MENUSPACING     13
#define MENUPADDING     5
#define RADIOOFFSET     3

////////////////////////////////////////////////////////////////////
//
// INPUT MANAGER SCANCODE TABLES
//
////////////////////////////////////////////////////////////////////

#if 0
static const char *ScanNames[] =      // Scan code names with single chars
{
    "?", "?", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "+", "?", "?",
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "|", "?", "A", "S",
    "D", "F", "G", "H", "J", "K", "L", ";", "\"", "?", "?", "?", "Z", "X", "C", "V",
    "B", "N", "M", ",", ".", "/", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "\xf", "?", "-", "\x15", "5", "\x11", "+", "?",
    "\x13", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?"
};                              // DEBUG - consolidate these
static ScanCode ExtScanCodes[] =        // Scan codes with >1 char names
{
    1, 0xe, 0xf, 0x1d, 0x2a, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
    0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x57, 0x59, 0x46, 0x1c, 0x36,
    0x37, 0x38, 0x47, 0x49, 0x4f, 0x51, 0x52, 0x53, 0x45, 0x48,
    0x50, 0x4b, 0x4d, 0x00
};
static const char *ExtScanNames[] =   // Names corresponding to ExtScanCodes
{
    "Esc", "BkSp", "Tab", "Ctrl", "LShft", "Space", "CapsLk", "F1", "F2", "F3", "F4",
    "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "ScrlLk", "Enter", "RShft",
    "PrtSc", "Alt", "Home", "PgUp", "End", "PgDn", "Ins", "Del", "NumLk", "Up",
    "Down", "Left", "Right", ""
};

/*#pragma warning 737 9
static byte
                                        *ScanNames[] =          // Scan code names with single chars
                                        {
        "?","?","1","2","3","4","5","6","7","8","9","0","-","+","?","?",
        "Q","W","E","R","T","Y","U","I","O","P","[","]","|","?","A","S",
        "D","F","G","H","J","K","L",";","\"","?","?","?","Z","X","C","V",
        "B","N","M",",",".","/","?","?","?","?","?","?","?","?","?","?",
        "?","?","?","?","?","?","?","?","\xf","?","-","\x15","5","\x11","+","?",
        "\x13","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?",
        "?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?",
        "?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?"
                                        };      // DEBUG - consolidate these
static byte ExtScanCodes[] =    // Scan codes with >1 char names
                                        {
        1,0xe,0xf,0x1d,0x2a,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,
        0x3f,0x40,0x41,0x42,0x43,0x44,0x57,0x59,0x46,0x1c,0x36,
        0x37,0x38,0x47,0x49,0x4f,0x51,0x52,0x53,0x45,0x48,
        0x50,0x4b,0x4d,0x00
                                        };
static byte *ExtScanNames[] =   // Names corresponding to ExtScanCodes
                                        {
        "Esc","BkSp","Tab","Ctrl","LShft","Space","CapsLk","F1","F2","F3","F4",
        "F5","F6","F7","F8","F9","F10","F11","F12","ScrlLk","Enter","RShft",
        "PrtSc","Alt","Home","PgUp","End","PgDn","Ins","Del","NumLk","Up",
        "Down","Left","Right",""
                                        };*/

#else
static std::map<ScanCode, const char *> ScanNames;
//static const char* const ScanNames[SDLK_LAST] =
//    {
//        "?","?","?","?","?","?","?","?",                                //   0
//        "BkSp","Tab","?","?","?","Return","?","?",                      //   8
//        "?","?","?","Pause","?","?","?","?",                            //  16
//        "?","?","?","Esc","?","?","?","?",                              //  24
//        "Space","!","\"","#","$","?","&","'",                           //  32
//        "(",")","*","+",",","-",".","/",                                //  40
//        "0","1","2","3","4","5","6","7",                                //  48
//        "8","9",":",";","<","=",">","?",                                //  56
//        "@","A","B","C","D","E","F","G",                                //  64
//        "H","I","J","K","L","M","N","O",                                //  72
//        "P","Q","R","S","T","U","V","W",                                //  80
//        "X","Y","Z","[","\\","]","^","_",                               //  88
//        "`","a","b","c","d","e","f","h",                                //  96
//        "h","i","j","k","l","m","n","o",                                // 104
//        "p","q","r","s","t","u","v","w",                                // 112
//        "x","y","z","{","|","}","~","?",                                // 120
//        "?","?","?","?","?","?","?","?",                                // 128
//        "?","?","?","?","?","?","?","?",                                // 136
//        "?","?","?","?","?","?","?","?",                                // 144
//        "?","?","?","?","?","?","?","?",                                // 152
//        "?","?","?","?","?","?","?","?",                                // 160
//        "?","?","?","?","?","?","?","?",                                // 168
//        "?","?","?","?","?","?","?","?",                                // 176
//        "?","?","?","?","?","?","?","?",                                // 184
//        "?","?","?","?","?","?","?","?",                                // 192
//        "?","?","?","?","?","?","?","?",                                // 200
//        "?","?","?","?","?","?","?","?",                                // 208
//        "?","?","?","?","?","?","?","?",                                // 216
//        "?","?","?","?","?","?","?","?",                                // 224
//        "?","?","?","?","?","?","?","?",                                // 232
//        "?","?","?","?","?","?","?","?",                                // 240
//        "?","?","?","?","?","?","?","?",                                // 248
//        "?","?","?","?","?","?","?","?",                                // 256
//        "?","?","?","?","?","?","?","Enter",                            // 264
//        "?","Up","Down","Right","Left","Ins","Home","End",              // 272
//        "PgUp","PgDn","F1","F2","F3","F4","F5","F6",                    // 280
//        "F7","F8","F9","F10","F11","F12","?","?",                       // 288
//        "?","?","?","?","NumLk","CapsLk","ScrlLk","RShft",              // 296
//        "Shift","RCtrl","Ctrl","RAlt","Alt","?","?","?",                // 304
//        "?","?","?","?","PrtSc","?","?","?",                            // 312
//        "?","?"                                                         // 320
//    };

void US_SetScanNames()
{
    ScanNames[sc_Enter] = "Enter";
    ScanNames[SDLK_UP] = "Up";
    ScanNames[SDLK_DOWN] = "Down";
    ScanNames[SDLK_RIGHT] = "Right";
    ScanNames[SDLK_LEFT] = "Left";
    ScanNames[SDLK_INSERT] = "Ins";
    ScanNames[SDLK_HOME] = "Home";
    ScanNames[SDLK_END] = "End";
    ScanNames[SDLK_PAGEUP] = "PgUp";
    ScanNames[SDLK_PAGEDOWN] = "PgDn";
    ScanNames[SDLK_F1] = "F1";
    ScanNames[SDLK_F2] = "F2";
    ScanNames[SDLK_F3] = "F3";
    ScanNames[SDLK_F4] = "F4";
    ScanNames[SDLK_F5] = "F5";
    ScanNames[SDLK_F6] = "F6";
    ScanNames[SDLK_F7] = "F7";
    ScanNames[SDLK_F8] = "F8";
    ScanNames[SDLK_F9] = "F9";
    ScanNames[SDLK_F10] = "F10";
    ScanNames[SDLK_F11] = "F11";
    ScanNames[SDLK_F12] = "F12";
    ScanNames[SDLK_F13] = "F13";
    ScanNames[SDLK_F14] = "F14";
    ScanNames[SDLK_F15] = "F15";
    ScanNames[SDLK_F16] = "F16";
    ScanNames[SDLK_F17] = "F17";
    ScanNames[SDLK_F18] = "F18";
    ScanNames[SDLK_F19] = "F19";
    ScanNames[SDLK_NUMLOCKCLEAR] = "NumLk";
    ScanNames[SDLK_CAPSLOCK] = "CapsLk";
    ScanNames[SDLK_SCROLLLOCK] = "ScrlLk";
    ScanNames[SDLK_RSHIFT] = "RShft";
    ScanNames[SDLK_LSHIFT] = "Shift";
    ScanNames[SDLK_RCTRL] = "RCtrl";
    ScanNames[SDLK_LCTRL] = "LCtrl";
    ScanNames[SDLK_RALT] = "RAlt";
    ScanNames[SDLK_LALT] = "Alt";
    ScanNames[SDLK_RGUI] = "RMeta";
    ScanNames[SDLK_LGUI] = "LMeta";
    ScanNames[SDLK_PRINTSCREEN] = "PrtSc";
    ScanNames[sc_BackSpace] = "BkSp";
    ScanNames[sc_Tab] = "Tab";
    ScanNames[sc_Return] = "Return";
    ScanNames[SDLK_PAUSE] = "Pause";
    ScanNames[sc_Escape] = "Esc";
    ScanNames[sc_Space] = "Space";
    ScanNames[SDLK_EXCLAIM] = "!";
    ScanNames[SDLK_QUOTEDBL] = "\"";
    ScanNames[SDLK_HASH] = "#";
    ScanNames[SDLK_DOLLAR] = "$";
    ScanNames[SDLK_QUESTION] = "?";
    ScanNames[SDLK_AMPERSAND] = "&";
    ScanNames[SDLK_QUOTE] = "'";
    ScanNames[SDLK_LEFTPAREN] = "(";
    ScanNames[SDLK_RIGHTPAREN] = ")";
    ScanNames[SDLK_ASTERISK] = "*";
    ScanNames[SDLK_PLUS] = "+";
    ScanNames[SDLK_COMMA] = ",";
    ScanNames[SDLK_MINUS] = "-";
    ScanNames[SDLK_PERIOD] = ".";
    ScanNames[SDLK_SLASH] = "/";
    ScanNames[SDLK_0] = "0";
    ScanNames[SDLK_1] = "1";
    ScanNames[SDLK_2] = "2";
    ScanNames[SDLK_3] = "3";
    ScanNames[SDLK_4] = "4";
    ScanNames[SDLK_5] = "5";
    ScanNames[SDLK_6] = "6";
    ScanNames[SDLK_7] = "7";
    ScanNames[SDLK_8] = "8";
    ScanNames[SDLK_9] = "9";
    ScanNames[SDLK_COLON] = ":";
    ScanNames[SDLK_SEMICOLON] = ";";
    ScanNames[SDLK_LESS] = "<";
    ScanNames[SDLK_EQUALS] = "=";
    ScanNames[SDLK_GREATER] = ">";
    ScanNames[SDLK_AT] = "@";
    ScanNames[SDLK_a] = "A";
    ScanNames[SDLK_b] = "B";
    ScanNames[SDLK_c] = "C";
    ScanNames[SDLK_d] = "D";
    ScanNames[SDLK_e] = "E";
    ScanNames[SDLK_f] = "F";
    ScanNames[SDLK_g] = "G";
    ScanNames[SDLK_h] = "H";
    ScanNames[SDLK_i] = "I";
    ScanNames[SDLK_j] = "J";
    ScanNames[SDLK_k] = "K";
    ScanNames[SDLK_l] = "L";
    ScanNames[SDLK_m] = "M";
    ScanNames[SDLK_n] = "N";
    ScanNames[SDLK_o] = "O";
    ScanNames[SDLK_p] = "P";
    ScanNames[SDLK_q] = "Q";
    ScanNames[SDLK_r] = "R";
    ScanNames[SDLK_s] = "S";
    ScanNames[SDLK_t] = "T";
    ScanNames[SDLK_u] = "U";
    ScanNames[SDLK_v] = "V";
    ScanNames[SDLK_w] = "W";
    ScanNames[SDLK_x] = "X";
    ScanNames[SDLK_y] = "Y";
    ScanNames[SDLK_z] = "Z";
    ScanNames[SDLK_LEFTBRACKET] = "[";
    ScanNames[SDLK_BACKSLASH] = "\\";
    ScanNames[SDLK_RIGHTBRACKET] = "]";
}


#endif

////////////////////////////////////////////////////////////////////
//
// Wolfenstein Control Panel!  Ta Da!
//
////////////////////////////////////////////////////////////////////
void
US_ControlPanel (ScanCode scancode)
{
    int which;

#ifdef _arch_dreamcast
    DC_StatusClearLCD();
#endif

    if (ingame)
    {
        if (CP_CheckQuick (scancode))
            return;
        lastgamemusicoffset = StartCPMusic (MENUSONG);
    }
    else
        StartCPMusic (MENUSONG);
    SetupControlPanel ();

    //
    // F-KEYS FROM WITHIN GAME
    //
    switch (scancode)
    {
        case sc_F1:
#ifdef SPEAR
            BossKey ();
#else
#ifdef GOODTIMES
            BossKey ();
#else
            HelpScreens ();
#endif
#endif
            goto finishup;

        case sc_F2:
            CP_SaveGame (0);
            goto finishup;

        case sc_F3:
            CP_LoadGame (0);
            goto finishup;

        case sc_F4:
            CP_Sound (0);
            goto finishup;

        case sc_F5:
            CP_ChangeView (0);
            goto finishup;

        case sc_F6:
            CP_Control (0);
            goto finishup;

        finishup:
            CleanupControlPanel ();
#ifdef SPEAR
            UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
            return;
    }

#ifdef SPEAR
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

    DrawMainMenu ();
    MenuFadeIn ();
    StartGame = 0;

    //
    // MAIN MENU LOOP
    //
    do
    {
        which = HandleMenu (&MainItems, &MainMenu[0], NULL);

#ifdef SPEAR
#ifndef SPEARDEMO
        IN_ProcessEvents();

        //
        // EASTER EGG FOR SPEAR OF DESTINY!
        //
        if (Keyboard[sc_I] && Keyboard[sc_D])
        {
            VW_FadeOut ();
            StartCPMusic (XJAZNAZI_MUS);
            UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
            UnCacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
            ClearMemory ();


            CA_CacheGrChunk (IDGUYS1PIC);
            VWB_DrawPic (0, 0, IDGUYS1PIC);
            UNCACHEGRCHUNK (IDGUYS1PIC);

            CA_CacheGrChunk (IDGUYS2PIC);
            VWB_DrawPic (0, 80, IDGUYS2PIC);
            UNCACHEGRCHUNK (IDGUYS2PIC);

            VW_UpdateScreen ();

            SDL_Color pal[256];
            CA_CacheGrChunk (IDGUYSPALETTE);
            VL_ConvertPalette(grsegs[IDGUYSPALETTE], pal, 256);
            VL_FadeIn (0, 255, pal, 30);
            UNCACHEGRCHUNK (IDGUYSPALETTE);

            while (Keyboard[sc_I] || Keyboard[sc_D])
                IN_WaitAndProcessEvents();
            IN_ClearKeysDown ();
            IN_Ack ();

            VW_FadeOut ();

            CacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
            CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
            DrawMainMenu ();
            StartCPMusic (MENUSONG);
            MenuFadeIn ();
        }
#endif
#endif

        switch (which)
        {
            case viewscores:
                if (MainMenu[viewscores].routine == NULL)
                {
                    if (CP_EndGame (0))
                        StartGame = 1;
                }
                else
                {
                    DrawMainMenu();
                    MenuFadeIn ();
                }
                break;

            case backtodemo:
                StartGame = 1;
                if (!ingame)
                    StartCPMusic (INTROSONG);
                VL_FadeOut (0, 255, 0, 0, 0, 10);
                break;

            case -1:
            case quit:
                CP_Quit (0);
                break;

            default:
                if (!StartGame)
                {
                    DrawMainMenu ();
                    MenuFadeIn ();
                }
        }

        //
        // "EXIT OPTIONS" OR "NEW GAME" EXITS
        //
    }
    while (!StartGame);

    //
    // DEALLOCATE EVERYTHING
    //
    CleanupControlPanel ();

    //
    // CHANGE MAINMENU ITEM
    //
    if (startgame || loadedgame)
        EnableEndGameMenuItem();

    // RETURN/START GAME EXECUTION

#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
}

void EnableEndGameMenuItem()
{
    MainMenu[viewscores].routine = NULL;
#ifndef JAPAN
    strcpy (MainMenu[viewscores].string, STR_EG);
#endif
}

////////////////////////
//
// DRAW MAIN MENU SCREEN
//
void
DrawMainMenu (void)
{
#ifdef JAPAN
    CA_CacheScreen (S_OPTIONSPIC);
#else
    ClearMScreen ();

    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);
    DrawStripes (10);
    VWB_DrawPic (84, 0, C_OPTIONSPIC);

#ifdef SPANISH
    DrawWindow (MENU_X - 8, MENU_Y - 3, MENU_W + 8, MENU_H, BKGDCOLOR);
#else
    DrawWindow (MENU_X - 8, MENU_Y - 3, MENU_W, MENU_H, BKGDCOLOR);
#endif
#endif

    //
    // CHANGE "GAME" AND "DEMO"
    //
    if (ingame)
    {
#ifndef JAPAN

#ifdef SPANISH
        strcpy (&MainMenu[backtodemo].string, STR_GAME);
#else
        strcpy (&MainMenu[backtodemo].string[8], STR_GAME);
#endif

#else
        CA_CacheGrChunk (C_MRETGAMEPIC);
        VWB_DrawPic (12 * 8, 20 * 8, C_MRETGAMEPIC);
        UNCACHEGRCHUNK (C_MRETGAMEPIC);
        CA_CacheGrChunk (C_MENDGAMEPIC);
        VWB_DrawPic (12 * 8, 18 * 8, C_MENDGAMEPIC);
        UNCACHEGRCHUNK (C_MENDGAMEPIC);
#endif
        MainMenu[backtodemo].active = 2;
    }
    else
    {
#ifndef JAPAN
#ifdef SPANISH
        strcpy (&MainMenu[backtodemo].string, STR_BD);
#else
        strcpy (&MainMenu[backtodemo].string[8], STR_DEMO);
#endif
#else
        CA_CacheGrChunk (C_MRETDEMOPIC);
        VWB_DrawPic (12 * 8, 20 * 8, C_MRETDEMOPIC);
        UNCACHEGRCHUNK (C_MRETDEMOPIC);
        CA_CacheGrChunk (C_MSCORESPIC);
        VWB_DrawPic (12 * 8, 18 * 8, C_MSCORESPIC);
        UNCACHEGRCHUNK (C_MSCORESPIC);
#endif
        MainMenu[backtodemo].active = 1;
    }

    DrawMenu (&MainItems, &MainMenu[0]);
    VW_UpdateScreen ();
}

#ifndef GOODTIMES
#ifndef SPEAR
////////////////////////////////////////////////////////////////////
//
// READ THIS!
//
////////////////////////////////////////////////////////////////////
int
CP_ReadThis (int)
{
    StartCPMusic (CORNER_MUS);
    HelpScreens ();
    StartCPMusic (MENUSONG);
    return true;
}
#endif
#endif


#ifdef GOODTIMES
////////////////////////////////////////////////////////////////////
//
// BOSS KEY
//
////////////////////////////////////////////////////////////////////
void
BossKey (void)
{
#ifdef NOTYET
    byte palette1[256][3];
    SD_MusicOff ();
/*       _AX = 3;
        geninterrupt(0x10); */
    _asm
    {
    mov eax, 3 int 0x10}
    puts ("C>");
    SetTextCursor (2, 0);
//      while (!Keyboard[sc_Escape])
    IN_Ack ();
    IN_ClearKeysDown ();

    SD_MusicOn ();
    VL_SetVGAPlaneMode ();
    for (int i = 0; i < 768; i++)
        palette1[0][i] = 0;

    VL_SetPalette (&palette1[0][0]);
    LoadLatchMem ();
#endif
}
#else
#ifdef SPEAR
void
BossKey (void)
{
#ifdef NOTYET
    byte palette1[256][3];
    SD_MusicOff ();
/*       _AX = 3;
        geninterrupt(0x10); */
    _asm
    {
    mov eax, 3 int 0x10}
    puts ("C>");
    SetTextCursor (2, 0);
//      while (!Keyboard[sc_Escape])
    IN_Ack ();
    IN_ClearKeysDown ();

    SD_MusicOn ();
    VL_SetVGAPlaneMode ();
    for (int i = 0; i < 768; i++)
        palette1[0][i] = 0;

    VL_SetPalette (&palette1[0][0]);
    LoadLatchMem ();
#endif
}
#endif
#endif


////////////////////////////////////////////////////////////////////
//
// CHECK QUICK-KEYS & QUIT (WHILE IN A GAME)
//
////////////////////////////////////////////////////////////////////
int
CP_CheckQuick (ScanCode scancode)
{
    switch (scancode)
    {
        //
        // END GAME
        //
        case sc_F7:
            CA_CacheGrChunk (STARTFONT + 1);

            WindowH = 160;
#ifdef JAPAN
            if (GetYorN (7, 8, C_JAPQUITPIC))
#else
            if (Confirm (ENDGAMESTR))
#endif
            {
                playstate = ex_died;
                killerobj = NULL;
                pickquick = gamestate.lives = 0;
            }

            WindowH = 200;
            fontnumber = 0;
            MainMenu[savegame].active = 0;
            return 1;

        //
        // QUICKSAVE
        //
        case sc_F8:
            if (SaveGamesAvail[LSItems.curpos] && pickquick)
            {
                CA_CacheGrChunk (STARTFONT + 1);
                fontnumber = 1;
                Message (STR_SAVING "...");
                CP_SaveGame (1);
                fontnumber = 0;
            }
            else
            {
#ifndef SPEAR
                CA_CacheGrChunk (STARTFONT + 1);
                CA_CacheGrChunk (C_CURSOR1PIC);
                CA_CacheGrChunk (C_CURSOR2PIC);
                CA_CacheGrChunk (C_DISKLOADING1PIC);
                CA_CacheGrChunk (C_DISKLOADING2PIC);
                CA_CacheGrChunk (C_SAVEGAMEPIC);
                CA_CacheGrChunk (C_MOUSELBACKPIC);
#else
                CacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
                CA_CacheGrChunk (C_CURSOR1PIC);
#endif

                VW_FadeOut ();
                if(screenHeight % 200 != 0)
                    VL_ClearScreen(0);

                lastgamemusicoffset = StartCPMusic (MENUSONG);
                pickquick = CP_SaveGame (0);

                SETFONTCOLOR (0, 15);
                IN_ClearKeysDown ();
                VW_FadeOut();
                if(viewsize != 21)
                    DrawPlayScreen ();

                if (!startgame && !loadedgame)
                    ContinueMusic (lastgamemusicoffset);

                if (loadedgame)
                    playstate = ex_abort;
                lasttimecount = GetTimeCount ();

                if (MousePresent && IN_IsInputGrabbed())
                    IN_CenterMouse();     // Clear accumulated mouse movement

#ifndef SPEAR
                UNCACHEGRCHUNK (C_CURSOR1PIC);
                UNCACHEGRCHUNK (C_CURSOR2PIC);
                UNCACHEGRCHUNK (C_DISKLOADING1PIC);
                UNCACHEGRCHUNK (C_DISKLOADING2PIC);
                UNCACHEGRCHUNK (C_SAVEGAMEPIC);
                UNCACHEGRCHUNK (C_MOUSELBACKPIC);
#else
                UnCacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif
            }
            return 1;

        //
        // QUICKLOAD
        //
        case sc_F9:
            if (SaveGamesAvail[LSItems.curpos] && pickquick)
            {
                char string[100] = STR_LGC;


                CA_CacheGrChunk (STARTFONT + 1);
                fontnumber = 1;

                strcat (string, SaveGameNames[LSItems.curpos]);
                strcat (string, "\"?");

                if (Confirm (string))
                    CP_LoadGame (1);

                fontnumber = 0;
            }
            else
            {
#ifndef SPEAR
                CA_CacheGrChunk (STARTFONT + 1);
                CA_CacheGrChunk (C_CURSOR1PIC);
                CA_CacheGrChunk (C_CURSOR2PIC);
                CA_CacheGrChunk (C_DISKLOADING1PIC);
                CA_CacheGrChunk (C_DISKLOADING2PIC);
                CA_CacheGrChunk (C_LOADGAMEPIC);
                CA_CacheGrChunk (C_MOUSELBACKPIC);
#else
                CA_CacheGrChunk (C_CURSOR1PIC);
                CacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif

                VW_FadeOut ();
                if(screenHeight % 200 != 0)
                    VL_ClearScreen(0);

                lastgamemusicoffset = StartCPMusic (MENUSONG);
                pickquick = CP_LoadGame (0);    // loads lastgamemusicoffs

                SETFONTCOLOR (0, 15);
                IN_ClearKeysDown ();
                VW_FadeOut();
                if(viewsize != 21)
                    DrawPlayScreen ();

                if (!startgame && !loadedgame)
                    ContinueMusic (lastgamemusicoffset);

                if (loadedgame)
                    playstate = ex_abort;

                lasttimecount = GetTimeCount ();

                if (MousePresent && IN_IsInputGrabbed())
                    IN_CenterMouse();     // Clear accumulated mouse movement

#ifndef SPEAR
                UNCACHEGRCHUNK (C_CURSOR1PIC);
                UNCACHEGRCHUNK (C_CURSOR2PIC);
                UNCACHEGRCHUNK (C_DISKLOADING1PIC);
                UNCACHEGRCHUNK (C_DISKLOADING2PIC);
                UNCACHEGRCHUNK (C_LOADGAMEPIC);
                UNCACHEGRCHUNK (C_MOUSELBACKPIC);
#else
                UnCacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif
            }
            return 1;

        //
        // QUIT
        //
        case sc_F10:
            CA_CacheGrChunk (STARTFONT + 1);

            WindowX = WindowY = 0;
            WindowW = 320;
            WindowH = 160;
#ifdef JAPAN
            if (GetYorN (7, 8, C_QUITMSGPIC))
#else
#ifdef SPANISH
            if (Confirm (ENDGAMESTR))
#else
            if (Confirm (endStrings[US_RndT () & 0x7 + (US_RndT () & 1)]))
#endif
#endif
            {
                VW_UpdateScreen ();
                SD_MusicOff ();
                SD_StopSound ();
                MenuFadeOut ();

                Quit (NULL);
            }

            DrawPlayBorder ();
            WindowH = 200;
            fontnumber = 0;
            return 1;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////
//
// END THE CURRENT GAME
//
////////////////////////////////////////////////////////////////////
int
CP_EndGame (int)
{
    int res;
#ifdef JAPAN
    res = GetYorN (7, 8, C_JAPQUITPIC);
#else
    res = Confirm (ENDGAMESTR);
#endif
    DrawMainMenu();
    if(!res) return 0;

    pickquick = gamestate.lives = 0;
    playstate = ex_died;
    killerobj = NULL;

    MainMenu[savegame].active = 0;
    MainMenu[viewscores].routine = CP_ViewScores;
#ifndef JAPAN
    strcpy (MainMenu[viewscores].string, STR_VS);
#endif

    return 1;
}


////////////////////////////////////////////////////////////////////
//
// VIEW THE HIGH SCORES
//
////////////////////////////////////////////////////////////////////
int
CP_ViewScores (int)
{
    fontnumber = 0;

#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
    StartCPMusic (XAWARD_MUS);
#else
    StartCPMusic (ROSTER_MUS);
#endif

    DrawHighScores ();
    VW_UpdateScreen ();
    MenuFadeIn ();
    fontnumber = 1;

    IN_Ack ();

    StartCPMusic (MENUSONG);
    MenuFadeOut ();

#ifdef SPEAR
    CacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
    return 0;
}


////////////////////////////////////////////////////////////////////
//
// START A NEW GAME
//
////////////////////////////////////////////////////////////////////
int
CP_NewGame (int)
{
    int which, episode;

#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif


#ifndef SPEAR
  firstpart:

    DrawNewEpisode ();
    do
    {
        which = HandleMenu (&NewEitems, &NewEmenu[0], NULL);
        switch (which)
        {
            case -1:
                MenuFadeOut ();
                return 0;

            default:
                if (!EpisodeSelect[which / 2])
                {
                    SD_PlaySound (NOWAYSND);
                    Message ("Please select \"Read This!\"\n"
                             "from the Options menu to\n"
                             "find out how to order this\n" "episode from Apogee.");
                    IN_ClearKeysDown ();
                    IN_Ack ();
                    DrawNewEpisode ();
                    which = 0;
                }
                else
                {
                    episode = which / 2;
                    which = 1;
                }
                break;
        }

    }
    while (!which);

    ShootSnd ();

    //
    // ALREADY IN A GAME?
    //
    if (ingame)
#ifdef JAPAN
        if (!GetYorN (7, 8, C_JAPNEWGAMEPIC))
#else
        if (!Confirm (CURGAME))
#endif
        {
            MenuFadeOut ();
            return 0;
        }

    MenuFadeOut ();

#else
    episode = 0;

    //
    // ALREADY IN A GAME?
    //
    CacheLump (NEWGAME_LUMP_START, NEWGAME_LUMP_END);
    DrawNewGame ();
    if (ingame)
        if (!Confirm (CURGAME))
        {
            MenuFadeOut ();
            UnCacheLump (NEWGAME_LUMP_START, NEWGAME_LUMP_END);
            CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
            return 0;
        }

#endif

    DrawNewGame ();
    which = HandleMenu (&NewItems, &NewMenu[0], DrawNewGameDiff);
    if (which < 0)
    {
        MenuFadeOut ();
#ifndef SPEAR
        goto firstpart;
#else
        UnCacheLump (NEWGAME_LUMP_START, NEWGAME_LUMP_END);
        CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
        return 0;
#endif
    }

    ShootSnd ();
    NewGame (which, episode);
    StartGame = 1;
    MenuFadeOut ();

    //
    // CHANGE "READ THIS!" TO NORMAL COLOR
    //
#ifndef SPEAR
#ifndef GOODTIMES
    MainMenu[readthis].active = 1;
#endif
#endif

    pickquick = 0;

#ifdef SPEAR
    UnCacheLump (NEWGAME_LUMP_START, NEWGAME_LUMP_END);
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

    return 0;
}


#ifndef SPEAR
/////////////////////
//
// DRAW NEW EPISODE MENU
//
void
DrawNewEpisode (void)
{
    int i;

#ifdef JAPAN
    CA_CacheScreen (S_EPISODEPIC);
#else
    ClearMScreen ();
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);

    DrawWindow (NE_X - 4, NE_Y - 4, NE_W + 8, NE_H + 8, BKGDCOLOR);
    SETFONTCOLOR (READHCOLOR, BKGDCOLOR);
    PrintY = 2;
    WindowX = 0;
#ifdef SPANISH
    US_CPrint ("Cual episodio jugar?");
#else
    US_CPrint ("Which episode to play?");
#endif
#endif

    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
    DrawMenu (&NewEitems, &NewEmenu[0]);

    for (i = 0; i < 6; i++)
        VWB_DrawPic (NE_X + 32, NE_Y + i * 26, C_EPISODE1PIC + i);

    VW_UpdateScreen ();
    MenuFadeIn ();
    WaitKeyUp ();
}
#endif

/////////////////////
//
// DRAW NEW GAME MENU
//
void
DrawNewGame (void)
{
#ifdef JAPAN
    CA_CacheScreen (S_SKILLPIC);
#else
    ClearMScreen ();
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);

    SETFONTCOLOR (READHCOLOR, BKGDCOLOR);
    PrintX = NM_X + 20;
    PrintY = NM_Y - 32;

#ifndef SPEAR
#ifdef SPANISH
    US_Print ("Eres macho?");
#else
    US_Print ("How tough are you?");
#endif
#else
    VWB_DrawPic (PrintX, PrintY, C_HOWTOUGHPIC);
#endif

    DrawWindow (NM_X - 5, NM_Y - 10, NM_W, NM_H, BKGDCOLOR);
#endif

    DrawMenu (&NewItems, &NewMenu[0]);
    DrawNewGameDiff (NewItems.curpos);
    VW_UpdateScreen ();
    MenuFadeIn ();
    WaitKeyUp ();
}


////////////////////////
//
// DRAW NEW GAME GRAPHIC
//
void
DrawNewGameDiff (int w)
{
    VWB_DrawPic (NM_X + 185, NM_Y + 7, w + C_BABYMODEPIC);
}


////////////////////////////////////////////////////////////////////
//
// HANDLE SOUND MENU
//
////////////////////////////////////////////////////////////////////
int
CP_Sound (int)
{
    int which;


#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
    CacheLump (SOUND_LUMP_START, SOUND_LUMP_END);
#endif

    DrawSoundMenu ();
    MenuFadeIn ();
    WaitKeyUp ();

    do
    {
        which = HandleMenu (&SndItems, &SndMenu[0], NULL);
        //
        // HANDLE MENU CHOICES
        //
        switch (which)
        {
                //
                // SOUND EFFECTS
                //
            case 0:
                if (SoundMode != sdm_Off)
                {
                    SD_WaitSoundDone ();
                    SD_SetSoundMode (sdm_Off);
                    DrawSoundMenu ();
                }
                break;
            case 1:
                if (SoundMode != sdm_PC)
                {
                    SD_WaitSoundDone ();
                    SD_SetSoundMode (sdm_PC);
                    CA_LoadAllSounds ();
                    DrawSoundMenu ();
                    ShootSnd ();
                }
                break;
            case 2:
                if (SoundMode != sdm_AdLib)
                {
                    SD_WaitSoundDone ();
                    SD_SetSoundMode (sdm_AdLib);
                    CA_LoadAllSounds ();
                    DrawSoundMenu ();
                    ShootSnd ();
                }
                break;

                //
                // DIGITIZED SOUND
                //
            case 5:
                if (DigiMode != sds_Off)
                {
                    SD_SetDigiDevice (sds_Off);
                    DrawSoundMenu ();
                }
                break;
            case 6:
/*                if (DigiMode != sds_SoundSource)
                {
                    SD_SetDigiDevice (sds_SoundSource);
                    DrawSoundMenu ();
                    ShootSnd ();
                }*/
                break;
            case 7:
                if (DigiMode != sds_SoundBlaster)
                {
                    SD_SetDigiDevice (sds_SoundBlaster);
                    DrawSoundMenu ();
                    ShootSnd ();
                }
                break;

                //
                // MUSIC
                //
            case 10:
                if (MusicMode != smm_Off)
                {
                    SD_SetMusicMode (smm_Off);
                    DrawSoundMenu ();
                    ShootSnd ();
                }
                break;
            case 11:
                if (MusicMode != smm_AdLib)
                {
                    SD_SetMusicMode (smm_AdLib);
                    DrawSoundMenu ();
                    ShootSnd ();
                    StartCPMusic (MENUSONG);
                }
                break;
        }
    }
    while (which >= 0);

    MenuFadeOut ();

#ifdef SPEAR
    UnCacheLump (SOUND_LUMP_START, SOUND_LUMP_END);
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
    return 0;
}


//////////////////////
//
// DRAW THE SOUND MENU
//
void
DrawSoundMenu (void)
{
    int i, on;


#ifdef JAPAN
    CA_CacheScreen (S_SOUNDPIC);
#else
    //
    // DRAW SOUND MENU
    //
    ClearMScreen ();
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);

    DrawWindow (SM_X - 8, SM_Y1 - 3, SM_W, SM_H1, BKGDCOLOR);
    DrawWindow (SM_X - 8, SM_Y2 - 3, SM_W, SM_H2, BKGDCOLOR);
    DrawWindow (SM_X - 8, SM_Y3 - 3, SM_W, SM_H3, BKGDCOLOR);
#endif

    //
    // IF NO ADLIB, NON-CHOOSENESS!
    //
    if (!AdLibPresent && !SoundBlasterPresent)
    {
        SndMenu[2].active = SndMenu[10].active = SndMenu[11].active = 0;
    }

    if (!SoundBlasterPresent)
        SndMenu[7].active = 0;

    if (!SoundBlasterPresent)
        SndMenu[5].active = 0;

    DrawMenu (&SndItems, &SndMenu[0]);
#ifndef JAPAN
    VWB_DrawPic (100, SM_Y1 - 20, C_FXTITLEPIC);
    VWB_DrawPic (100, SM_Y2 - 20, C_DIGITITLEPIC);
    VWB_DrawPic (100, SM_Y3 - 20, C_MUSICTITLEPIC);
#endif

    for (i = 0; i < SndItems.amount; i++)
#ifdef JAPAN
        if (i != 3 && i != 4 && i != 8 && i != 9)
#else
        if (SndMenu[i].string[0])
#endif
        {
            //
            // DRAW SELECTED/NOT SELECTED GRAPHIC BUTTONS
            //
            on = 0;
            switch (i)
            {
                    //
                    // SOUND EFFECTS
                    //
                case 0:
                    if (SoundMode == sdm_Off)
                        on = 1;
                    break;
                case 1:
                    if (SoundMode == sdm_PC)
                        on = 1;
                    break;
                case 2:
                    if (SoundMode == sdm_AdLib)
                        on = 1;
                    break;

                    //
                    // DIGITIZED SOUND
                    //
                case 5:
                    if (DigiMode == sds_Off)
                        on = 1;
                    break;
                case 6:
//                    if (DigiMode == sds_SoundSource)
//                        on = 1;
                    break;
                case 7:
                    if (DigiMode == sds_SoundBlaster)
                        on = 1;
                    break;

                    //
                    // MUSIC
                    //
                case 10:
                    if (MusicMode == smm_Off)
                        on = 1;
                    break;
                case 11:
                    if (MusicMode == smm_AdLib)
                        on = 1;
                    break;
            }

            if (on)
                VWB_DrawPic (SM_X + 24, SM_Y1 + i * 13 + 2, C_SELECTEDPIC);
            else
                VWB_DrawPic (SM_X + 24, SM_Y1 + i * 13 + 2, C_NOTSELECTEDPIC);
        }

    DrawMenuGun (&SndItems);
    VW_UpdateScreen ();
}


//
// DRAW LOAD/SAVE IN PROGRESS
//
void
DrawLSAction (int which)
{
#define LSA_X   96
#define LSA_Y   80
#define LSA_W   130
#define LSA_H   42

    DrawWindow (LSA_X, LSA_Y, LSA_W, LSA_H, TEXTCOLOR);
    DrawOutline (LSA_X, LSA_Y, LSA_W, LSA_H, 0, HIGHLIGHT);
    VWB_DrawPic (LSA_X + 8, LSA_Y + 5, C_DISKLOADING1PIC);

    fontnumber = 1;
    SETFONTCOLOR (0, TEXTCOLOR);
    PrintX = LSA_X + 46;
    PrintY = LSA_Y + 13;

    if (!which)
        US_Print (STR_LOADING "...");
    else
        US_Print (STR_SAVING "...");

    VW_UpdateScreen ();
}


////////////////////////////////////////////////////////////////////
//
// LOAD SAVED GAMES
//
////////////////////////////////////////////////////////////////////
int
CP_LoadGame (int quick)
{
    FILE *file;
    int which, exit = 0;
    char name[13];
    char loadpath[300];

    strcpy (name, SaveName);

    //
    // QUICKLOAD?
    //
    if (quick)
    {
        which = LSItems.curpos;

        if (SaveGamesAvail[which])
        {
            name[7] = which + '0';

#ifdef _arch_dreamcast
            DC_LoadFromVMU(name);
#endif

            if(configdir[0])
                snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, name);
            else
                strcpy(loadpath, name);

            file = fopen (loadpath, "rb");
            fseek (file, 32, SEEK_SET);
            loadedgame = true;
            LoadTheGame (file, 0, 0);
            loadedgame = false;
            fclose (file);

            DrawFace ();
            DrawHealth ();
            DrawLives ();
            DrawLevel ();
            DrawAmmo ();
            DrawKeys ();
            DrawWeapon ();
            DrawScore ();
            ContinueMusic (lastgamemusicoffset);
            return 1;
        }
    }


#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
    CacheLump (LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
#endif

    DrawLoadSaveScreen (0);

    do
    {
        which = HandleMenu (&LSItems, &LSMenu[0], TrackWhichGame);
        if (which >= 0 && SaveGamesAvail[which])
        {
            ShootSnd ();
            name[7] = which + '0';

#ifdef _arch_dreamcast
            DC_LoadFromVMU(name);
#endif

            if(configdir[0])
                snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, name);
            else
                strcpy(loadpath, name);

            file = fopen (loadpath, "rb");
            fseek (file, 32, SEEK_SET);

            DrawLSAction (0);
            loadedgame = true;

            LoadTheGame (file, LSA_X + 8, LSA_Y + 5);
            fclose (file);

            StartGame = 1;
            ShootSnd ();
            //
            // CHANGE "READ THIS!" TO NORMAL COLOR
            //

#ifndef SPEAR
#ifndef GOODTIMES
            MainMenu[readthis].active = 1;
#endif
#endif

            exit = 1;
            break;
        }

    }
    while (which >= 0);

    MenuFadeOut ();

#ifdef SPEAR
    UnCacheLump (LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

    return exit;
}


///////////////////////////////////
//
// HIGHLIGHT CURRENT SELECTED ENTRY
//
void
TrackWhichGame (int w)
{
    static int lastgameon = 0;

    PrintLSEntry (lastgameon, TEXTCOLOR);
    PrintLSEntry (w, HIGHLIGHT);

    lastgameon = w;
}


////////////////////////////
//
// DRAW THE LOAD/SAVE SCREEN
//
void
DrawLoadSaveScreen (int loadsave)
{
#define DISKX   100
#define DISKY   0

    int i;


    ClearMScreen ();
    fontnumber = 1;
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);
    DrawWindow (LSM_X - 10, LSM_Y - 5, LSM_W, LSM_H, BKGDCOLOR);
    DrawStripes (10);

    if (!loadsave)
        VWB_DrawPic (60, 0, C_LOADGAMEPIC);
    else
        VWB_DrawPic (60, 0, C_SAVEGAMEPIC);

    for (i = 0; i < 10; i++)
        PrintLSEntry (i, TEXTCOLOR);

    DrawMenu (&LSItems, &LSMenu[0]);
    VW_UpdateScreen ();
    MenuFadeIn ();
    WaitKeyUp ();
}


///////////////////////////////////////////
//
// PRINT LOAD/SAVE GAME ENTRY W/BOX OUTLINE
//
void
PrintLSEntry (int w, int color)
{
    SETFONTCOLOR (color, BKGDCOLOR);
    DrawOutline (LSM_X + LSItems.indent, LSM_Y + w * 13, LSM_W - LSItems.indent - 15, 11, color,
                 color);
    PrintX = LSM_X + LSItems.indent + 2;
    PrintY = LSM_Y + w * 13 + 1;
    fontnumber = 0;

    if (SaveGamesAvail[w])
        US_Print (SaveGameNames[w]);
    else
        US_Print ("      - " STR_EMPTY " -");

    fontnumber = 1;
}


////////////////////////////////////////////////////////////////////
//
// SAVE CURRENT GAME
//
////////////////////////////////////////////////////////////////////
int
CP_SaveGame (int quick)
{
    int which, exit = 0;
    FILE *file;
    char name[13];
    char savepath[300];
    char input[32];

    strcpy (name, SaveName);

    //
    // QUICKSAVE?
    //
    if (quick)
    {
        which = LSItems.curpos;

        if (SaveGamesAvail[which])
        {
            name[7] = which + '0';

            if(configdir[0])
                snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
            else
                strcpy(savepath, name);

            unlink (savepath);
            file = fopen (savepath, "wb");

            strcpy (input, &SaveGameNames[which][0]);

            fwrite (input, 1, 32, file);
            fseek (file, 32, SEEK_SET);
            SaveTheGame (file, 0, 0);
            fclose (file);

#ifdef _arch_dreamcast
            DC_SaveToVMU(name, input);
#endif

            return 1;
        }
    }


#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
    CacheLump (LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
#endif

    DrawLoadSaveScreen (1);

    do
    {
        which = HandleMenu (&LSItems, &LSMenu[0], TrackWhichGame);
        if (which >= 0)
        {
            //
            // OVERWRITE EXISTING SAVEGAME?
            //
            if (SaveGamesAvail[which])
            {
#ifdef JAPAN
                if (!GetYorN (7, 8, C_JAPSAVEOVERPIC))
#else
                if (!Confirm (GAMESVD))
#endif
                {
                    DrawLoadSaveScreen (1);
                    continue;
                }
                else
                {
                    DrawLoadSaveScreen (1);
                    PrintLSEntry (which, HIGHLIGHT);
                    VW_UpdateScreen ();
                }
            }

            ShootSnd ();

            strcpy (input, &SaveGameNames[which][0]);
            name[7] = which + '0';

            fontnumber = 0;
            if (!SaveGamesAvail[which])
                VWB_Bar (LSM_X + LSItems.indent + 1, LSM_Y + which * 13 + 1,
                         LSM_W - LSItems.indent - 16, 10, BKGDCOLOR);
            VW_UpdateScreen ();

            if (US_LineInput
                (LSM_X + LSItems.indent + 2, LSM_Y + which * 13 + 1, input, input, true, 31,
                 LSM_W - LSItems.indent - 30))
            {
                SaveGamesAvail[which] = 1;
                strcpy (&SaveGameNames[which][0], input);

                if(configdir[0])
                    snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
                else
                    strcpy(savepath, name);

                unlink (savepath);
                file = fopen (savepath, "wb");
                fwrite (input, 32, 1, file);
                fseek (file, 32, SEEK_SET);

                DrawLSAction (1);
                SaveTheGame (file, LSA_X + 8, LSA_Y + 5);

                fclose (file);

#ifdef _arch_dreamcast
                DC_SaveToVMU(name, input);
#endif

                ShootSnd ();
                exit = 1;
            }
            else
            {
                VWB_Bar (LSM_X + LSItems.indent + 1, LSM_Y + which * 13 + 1,
                         LSM_W - LSItems.indent - 16, 10, BKGDCOLOR);
                PrintLSEntry (which, HIGHLIGHT);
                VW_UpdateScreen ();
                SD_PlaySound (ESCPRESSEDSND);
                continue;
            }

            fontnumber = 1;
            break;
        }

    }
    while (which >= 0);

    MenuFadeOut ();

#ifdef SPEAR
    UnCacheLump (LOADSAVE_LUMP_START, LOADSAVE_LUMP_END);
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif

    return exit;
}

////////////////////////////////////////////////////////////////////
//
// DEFINE CONTROLS
//
////////////////////////////////////////////////////////////////////
int
CP_Control (int)
{
    int which;

#ifdef SPEAR
    UnCacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
    CacheLump (CONTROL_LUMP_START, CONTROL_LUMP_END);
#endif

    DrawCtlScreen ();
    MenuFadeIn ();
    WaitKeyUp ();

    do
    {
        which = HandleMenu (&CtlItems, CtlMenu, NULL);
        switch (which)
        {
            case CTL_MOUSEENABLE:
                mouseenabled ^= 1;
                if(IN_IsInputGrabbed())
                    IN_CenterMouse();
                DrawCtlScreen ();
                CusItems.curpos = -1;
                ShootSnd ();
                break;

            case CTL_MOVEWITHMOUSE:
                movewithmouse ^= 1;
                DrawCtlScreen ();
                CusItems.curpos = -1;
                ShootSnd ();
                break;

            case CTL_JOYENABLE:
                joystickenabled ^= 1;
                DrawCtlScreen ();
                CusItems.curpos = -1;
                ShootSnd ();
                break;

            case CTL_MOUSESENS:
            case CTL_JOYSENS:
            case CTL_CUSTOMIZEJOY:
            case CTL_CUSTOMIZEKB:
                DrawCtlScreen ();
                MenuFadeIn ();
                WaitKeyUp ();
                break;
        }
    }
    while (which >= 0);

    MenuFadeOut ();

#ifdef SPEAR
    UnCacheLump (CONTROL_LUMP_START, CONTROL_LUMP_END);
    CacheLump (OPTIONS_LUMP_START, OPTIONS_LUMP_END);
#endif
    return 0;
}


////////////////////////////////
//
// DRAW MOUSE SENSITIVITY SCREEN
//
void
DrawSlider (int value, const char *sorg)
{
#ifdef JAPAN
    CA_CacheScreen (S_MOUSESENSPIC);
#else
    ClearMScreen ();
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);
#ifdef SPANISH
    DrawWindow (10, 80, 300, 43, BKGDCOLOR);
#else
    DrawWindow (10, 80, 300, 30, BKGDCOLOR);
#endif

    WindowX = 0;
    WindowW = 320;
    PrintY = 82;
    SETFONTCOLOR (READCOLOR, BKGDCOLOR);
    US_CPrint (sorg);

    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
    PrintX = 14;
    PrintY = 95 + 13;
    US_Print (STR_SLOW);
    PrintX = 252;
    US_Print (STR_FAST);
#else
    PrintX = 14;
    PrintY = 95;
    US_Print (STR_SLOW);
    PrintX = 269;
    US_Print (STR_FAST);
#endif
#endif

    VWB_Bar (60, 97, 200, 10, TEXTCOLOR);
    DrawOutline (60, 97, 200, 10, 0, HIGHLIGHT);
    DrawOutline (60 + 20 * value, 97, 20, 10, 0, READCOLOR);
    VWB_Bar (61 + 20 * value, 98, 19, 9, READHCOLOR);

    VW_UpdateScreen ();
    MenuFadeIn ();
}

///////////////////////////
//
// ADJUST MOUSE SENSITIVITY
//
int
JoySensitivity (int)
{
    ControlInfo ci;
    int exit = 0, oldJA;


    oldJA = joyadjustment;
    DrawSlider (joyadjustment, STR_JOYADJ);
    do
    {
        SDL_Delay(5);
        ReadAnyControl (&ci);
        switch (ci.dir)
        {
            case dir_North:
            case dir_West:
                if (joyadjustment)
                {
                    joyadjustment--;
                    VWB_Bar (60, 97, 200, 10, TEXTCOLOR);
                    DrawOutline (60, 97, 200, 10, 0, HIGHLIGHT);
                    DrawOutline (60 + 20 * joyadjustment, 97, 20, 10, 0, READCOLOR);
                    VWB_Bar (61 + 20 * joyadjustment, 98, 19, 9, READHCOLOR);
                    VW_UpdateScreen ();
                    SD_PlaySound (MOVEGUN1SND);
                    TicDelay(20);
                }
                break;

            case dir_South:
            case dir_East:
                if (joyadjustment < 9)
                {
                    joyadjustment++;
                    VWB_Bar (60, 97, 200, 10, TEXTCOLOR);
                    DrawOutline (60, 97, 200, 10, 0, HIGHLIGHT);
                    DrawOutline (60 + 20 * joyadjustment, 97, 20, 10, 0, READCOLOR);
                    VWB_Bar (61 + 20 * joyadjustment, 98, 19, 9, READHCOLOR);
                    VW_UpdateScreen ();
                    SD_PlaySound (MOVEGUN1SND);
                    TicDelay(20);
                }
                break;
            default:
                break;
        }

        if (ci.button0 || Keyboard[sc_Space] || Keyboard[sc_Enter])
            exit = 1;
        else if (ci.button1 || Keyboard[sc_Escape])
            exit = 2;

    }
    while (!exit);

    if (exit == 2)
    {
        joyadjustment = oldJA;
        SD_PlaySound (ESCPRESSEDSND);
    }
    else
        SD_PlaySound (SHOOTSND);

    WaitKeyUp ();
    MenuFadeOut ();

    return 0;
}

///////////////////////////
//
// ADJUST MOUSE SENSITIVITY
//
int
MouseSensitivity (int)
{
    ControlInfo ci;
    int exit = 0, oldMA;


    oldMA = mouseadjustment;
    DrawSlider (mouseadjustment, STR_MOUSEADJ);
    do
    {
        SDL_Delay(5);
        ReadAnyControl (&ci);
        switch (ci.dir)
        {
            case dir_North:
            case dir_West:
                if (mouseadjustment)
                {
                    mouseadjustment--;
                    VWB_Bar (60, 97, 200, 10, TEXTCOLOR);
                    DrawOutline (60, 97, 200, 10, 0, HIGHLIGHT);
                    DrawOutline (60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
                    VWB_Bar (61 + 20 * mouseadjustment, 98, 19, 9, READHCOLOR);
                    VW_UpdateScreen ();
                    SD_PlaySound (MOVEGUN1SND);
                    TicDelay(20);
                }
                break;

            case dir_South:
            case dir_East:
                if (mouseadjustment < 9)
                {
                    mouseadjustment++;
                    VWB_Bar (60, 97, 200, 10, TEXTCOLOR);
                    DrawOutline (60, 97, 200, 10, 0, HIGHLIGHT);
                    DrawOutline (60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
                    VWB_Bar (61 + 20 * mouseadjustment, 98, 19, 9, READHCOLOR);
                    VW_UpdateScreen ();
                    SD_PlaySound (MOVEGUN1SND);
                    TicDelay(20);
                }
                break;
            default:
                break;
        }

        if (ci.button0 || Keyboard[sc_Space] || Keyboard[sc_Enter])
            exit = 1;
        else if (ci.button1 || Keyboard[sc_Escape])
            exit = 2;

    }
    while (!exit);

    if (exit == 2)
    {
        mouseadjustment = oldMA;
        SD_PlaySound (ESCPRESSEDSND);
    }
    else
        SD_PlaySound (SHOOTSND);

    WaitKeyUp ();
    MenuFadeOut ();

    return 0;
}


///////////////////////////
//
// DRAW CONTROL MENU SCREEN
//
void
DrawCtlScreen (void)
{
    int i, x, y;

#ifdef JAPAN
    CA_CacheScreen (S_CONTROLPIC);
#else
    ClearMScreen ();
    DrawStripes (10);
    VWB_DrawPic (80, 0, C_CONTROLPIC);
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);
    DrawWindow (CTL_X - 8, CTL_Y - MENUPADDING, CTL_W, lengthof(CtlMenu) * MENUSPACING + MENUPADDING * 2, BKGDCOLOR);
#endif
    WindowX = 0;
    WindowW = 320;
    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);

    if (IN_JoyPresent())
    {
        CtlMenu[CTL_JOYENABLE].active = 1;
    }

    CtlMenu[CTL_CUSTOMIZEJOY].active = joystickenabled;
    CtlMenu[CTL_JOYSENS].active = joystickenabled;


    if (MousePresent)
    {
        CtlMenu[CTL_MOUSESENS].active = CtlMenu[CTL_MOUSEENABLE].active = 1;
    }

    CtlMenu[CTL_MOUSESENS].active = mouseenabled;
    CtlMenu[CTL_MOVEWITHMOUSE].active = mouseenabled;


    DrawMenu (&CtlItems, CtlMenu);

    x = CTL_X + CtlItems.indent - 24;
    y = CTL_Y + RADIOOFFSET;
    if (mouseenabled)
        VWB_DrawPic (x, y, C_SELECTEDPIC);
    else
        VWB_DrawPic (x, y, C_NOTSELECTEDPIC);

    y = CTL_Y + MENUSPACING + RADIOOFFSET;
    if (movewithmouse)
        VWB_DrawPic (x, y, C_SELECTEDPIC);
    else
        VWB_DrawPic (x, y, C_NOTSELECTEDPIC);

    y = CTL_Y + (MENUSPACING * 4) + RADIOOFFSET;
    if (joystickenabled)
        VWB_DrawPic (x, y, C_SELECTEDPIC);
    else
        VWB_DrawPic (x, y, C_NOTSELECTEDPIC);

    //
    // PICK FIRST AVAILABLE SPOT
    //
    if (CtlItems.curpos < 0 || !CtlMenu[CtlItems.curpos].active)
    {
        for (i = 0; i < CtlItems.amount; i++)
        {
            if (CtlMenu[i].active)
            {
                CtlItems.curpos = i;
                break;
            }
        }
    }

    DrawMenuGun (&CtlItems);
    VW_UpdateScreen ();
}


////////////////////////////////////////////////////////////////////
//
// CUSTOMIZE CONTROLS
//
////////////////////////////////////////////////////////////////////
char joyarray[13][3] = { "A ", "B ", "X ", "Y ", "L1", "R1", "L2", "R2", "LS", "RS", "BK", "GD", "XB" };
char mbarray[3][2] = { "L", "R", "M", };
int8_t order[4] = { bt_run, bt_use, bt_attack, bt_strafe };
int8_t order2[4] = { bt_pause, bt_esc, bt_prevweapon, bt_nextweapon };


int
CustomGamepadControls (int)
{
    int which;

    DrawCustomJoyScreen ();
    do
    {
        which = HandleMenu (&CusJoyItems, &CusJoyMenu[0], FixupCustom);
        switch (which)
        {
            case 0:
                DefineJoy2Btns ();
                DrawCustJoy2 (1);
                break;
            case 3:
                DefineJoyBtns ();
                DrawCustJoy (0);
                break;
        }
    }
    while (which >= 0);

    MenuFadeOut ();

    return 0;
}


int
CustomControls (int)
{
    int which;

    DrawCustomScreen ();
    do
    {
        which = HandleMenu (&CusItems, &CusMenu[0], FixupCustom);
        switch (which)
        {
            case 0:
                DefineMouseBtns ();
                DrawCustMouse (1);
                break;
            case 6:
                DefineKeyBtns ();
                DrawCustKeybd (0);
                break;
            case 8:
                DefineKeyMove ();
                DrawCustKeys (0);
        }
    }
    while (which >= 0);

    MenuFadeOut ();

    return 0;
}


////////////////////////
//
// DEFINE THE MOUSE BUTTONS
//
void
DefineMouseBtns (void)
{
    CustomCtrls mouseallowed = { 0, 1, 1, 1 };
    EnterCtrlData (2, &mouseallowed, DrawCustMouse, PrintCustMouse, MOUSE);
}


////////////////////////
//
// DEFINE THE MOUSE BUTTONS
//
void
DefineJoy2Btns (void)
{
    CustomCtrls joyallowed = { 1, 1, 1, 1 };
    EnterCtrlData (2, &joyallowed, DrawCustJoy2, PrintCustJoy2, JOYSTICK2);
}


////////////////////////
//
// DEFINE THE JOYSTICK BUTTONS
//
void
DefineJoyBtns (void)
{
    CustomCtrls joyallowed = { 1, 1, 1, 1 };
    EnterCtrlData (5, &joyallowed, DrawCustJoy, PrintCustJoy, JOYSTICK);
}


////////////////////////
//
// DEFINE THE KEYBOARD BUTTONS
//
void
DefineKeyBtns (void)
{
    CustomCtrls keyallowed = { 1, 1, 1, 1 };
    EnterCtrlData (8, &keyallowed, DrawCustKeybd, PrintCustKeybd, KEYBOARDBTNS);
}


////////////////////////
//
// DEFINE THE KEYBOARD BUTTONS
//
void
DefineKeyMove (void)
{
    CustomCtrls keyallowed = { 1, 1, 1, 1 };
    EnterCtrlData (10, &keyallowed, DrawCustKeys, PrintCustKeys, KEYBOARDMOVE);
}


////////////////////////
//
// ENTER CONTROL DATA FOR ANY TYPE OF CONTROL
//
enum
{ FWRD, RIGHT, BKWD, LEFT };
int moveorder[4] = { LEFT, RIGHT, FWRD, BKWD };

void
EnterCtrlData (int index, CustomCtrls * cust, void (*DrawRtn) (int), void (*PrintRtn) (int),
               int type)
{
    int j, exit, tick, redraw, which, x, picked, lastFlashTime;
    ControlInfo ci;


    ShootSnd ();
    PrintY = CST_Y + 13 * index;
    IN_ClearKeysDown ();
    exit = 0;
    redraw = 1;
    //
    // FIND FIRST SPOT IN ALLOWED ARRAY
    //
    for (j = 0; j < 4; j++)
        if (cust->allowed[j])
        {
            which = j;
            break;
        }

    do
    {
        if (redraw)
        {
            x = CST_START + CST_SPC * which;

            // Clear existing text
            DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);

            // Redraw the other joystick options that could have been overwritten
            if (type == JOYSTICK)
            {
                DrawCustJoy2(0);
            }
            else if (type == JOYSTICK2) {
                DrawCustJoy(0);
            }

            DrawRtn (1);

            // Draw the grey box on the highlighted item
            DrawWindow (x - 2, PrintY, CST_SPC, 11, TEXTCOLOR);
            DrawOutline (x - 2, PrintY, CST_SPC, 11, 0, HIGHLIGHT);

            SETFONTCOLOR (0, TEXTCOLOR);
            PrintRtn (which);
            PrintX = x;
            SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
            VW_UpdateScreen ();
            WaitKeyUp ();
            redraw = 0;
        }

        SDL_Delay(5);
        ReadAnyControl (&ci);

        if (type == MOUSE || type == JOYSTICK || type == JOYSTICK2)
            if (IN_KeyDown (sc_Enter) || IN_KeyDown (sc_Control) || IN_KeyDown (sc_Alt))
            {
                IN_ClearKeysDown ();
                ci.button0 = ci.button1 = false;
            }
        //
        // CHANGE BUTTON VALUE?
        //
        if (((type != KEYBOARDBTNS && type != KEYBOARDMOVE) && (ci.button0 || ci.button1 || ci.button2 || ci.button3 || ci.button4 || ci.button5 || ci.button6 || ci.button7 || ci.button8 || ci.button9 || ci.button10 || ci.button11 || ci.button12)) ||
            ((type == KEYBOARDBTNS || type == KEYBOARDMOVE) && LastScan == sc_Enter))
        {
            lastFlashTime = GetTimeCount();
            tick = picked = 0;
            SETFONTCOLOR (0, TEXTCOLOR);

            if (type == KEYBOARDBTNS || type == KEYBOARDMOVE)
                IN_ClearKeysDown ();

            while(1)
            {
                int button, result = 0;

                //
                // FLASH CURSOR
                //
                if (GetTimeCount() - lastFlashTime > 10)
                {
                    switch (tick)
                    {
                        case 0:
                            VWB_Bar (x, PrintY + 1, CST_SPC - 2, 10, TEXTCOLOR);
                            break;
                        case 1:
                            PrintX = x;
                            US_Print ("?");
                            SD_PlaySound (HITWALLSND);
                    }
                    tick ^= 1;
                    lastFlashTime = GetTimeCount();
                    VW_UpdateScreen ();
                }
                else SDL_Delay(5);

                //
                // WHICH TYPE OF INPUT DO WE PROCESS?
                //
                switch (type)
                {
                    case MOUSE:
                        button = IN_MouseButtons();
                        switch (button)
                        {
                            case 1:
                                result = 1;
                                break;
                            case 2:
                                result = 2;
                                break;
                            case 4:
                                result = 3;
                                break;
                        }

                        if (result)
                        {
                            for (int z = 0; z < 4; z++)
                                if (order[which] == buttonmouse[z])
                                {
                                    buttonmouse[z] = bt_nobutton;
                                    break;
                                }

                            buttonmouse[result - 1] = order[which];
                            picked = 1;
                            SD_PlaySound (SHOOTDOORSND);
                        }
                        break;

                    case JOYSTICK2:
                        if (ci.button0)
                            result = 1;
                        else if (ci.button1)
                            result = 2;
                        else if (ci.button2)
                            result = 3;
                        else if (ci.button3)
                            result = 4;
                        else if (ci.button4)
                            result = 5;
                        else if (ci.button5)
                            result = 6;
                        else if (ci.button6)
                            result = 7;
                        else if (ci.button7)
                            result = 8;
                        else if (ci.button8)
                            result = 9;
                        else if (ci.button9)
                            result = 10;
                        else if (ci.button10)
                            result = 11;
                        else if (ci.button11)
                            result = 12;
                        else if (ci.button12)
                            result = 13;

                        if (result)
                        {
                            /*
                            // Allow the same action to be bound to multiple buttons
                            for (int z = 0; z < 10; z++)
                            {
                                if (order[which] == buttonjoy[z])
                                {
                                    buttonjoy[z] = bt_nobutton;
                                    break;
                                }
                            }
                            */

                            buttonjoy[result - 1] = order2[which];
                            picked = 1;
                            SD_PlaySound (SHOOTDOORSND);
                        }
                        break;

                    case JOYSTICK:
                        if (ci.button0)
                            result = 1;
                        else if (ci.button1)
                            result = 2;
                        else if (ci.button2)
                            result = 3;
                        else if (ci.button3)
                            result = 4;
                        else if (ci.button4)
                            result = 5;
                        else if (ci.button5)
                            result = 6;
                        else if (ci.button6)
                            result = 7;
                        else if (ci.button7)
                            result = 8;
                        else if (ci.button8)
                            result = 9;
                        else if (ci.button9)
                            result = 10;
                        else if (ci.button10)
                            result = 11;
                        else if (ci.button11)
                            result = 12;
                        else if (ci.button12)
                            result = 13;

                        if (result)
                        {
                            /*
                            // Allow the same action to be bound to multiple buttons
                            for (int z = 0; z < 10; z++)
                            {
                                if (order[which] == buttonjoy[z])
                                {
                                    buttonjoy[z] = bt_nobutton;
                                    break;
                                }
                            }
                            */

                            buttonjoy[result - 1] = order[which];
                            picked = 1;
                            SD_PlaySound (SHOOTDOORSND);
                        }
                        break;

                    case KEYBOARDBTNS:
                        if (LastScan && LastScan != sc_Escape)
                        {
                            buttonscan[order[which]] = LastScan;
                            picked = 1;
                            ShootSnd ();
                            IN_ClearKeysDown ();
                        }
                        break;

                    case KEYBOARDMOVE:
                        if (LastScan && LastScan != sc_Escape)
                        {
                            dirscan[moveorder[which]] = LastScan;
                            picked = 1;
                            ShootSnd ();
                            IN_ClearKeysDown ();
                        }
                        break;
                }

                //
                // EXIT INPUT?
                //
                if (IN_KeyDown (sc_Escape) || (type != JOYSTICK && ci.button1))
                {
                    picked = 1;
                    SD_PlaySound (ESCPRESSEDSND);
                }

                if(picked) break;

                ReadAnyControl (&ci);
            }

            SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
            redraw = 1;
            WaitKeyUp ();
            continue;
        }

        if (ci.button1 || IN_KeyDown (sc_Escape))
            exit = 1;

        //
        // MOVE TO ANOTHER SPOT?
        //
        switch (ci.dir)
        {
            case dir_West:
                do
                {
                    which--;
                    if (which < 0)
                        which = 3;
                }
                while (!cust->allowed[which]);
                redraw = 1;
                SD_PlaySound (MOVEGUN1SND);
                while (ReadAnyControl (&ci), ci.dir != dir_None) SDL_Delay(5);
                IN_ClearKeysDown ();
                break;

            case dir_East:
                do
                {
                    which++;
                    if (which > 3)
                        which = 0;
                }
                while (!cust->allowed[which]);
                redraw = 1;
                SD_PlaySound (MOVEGUN1SND);
                while (ReadAnyControl (&ci), ci.dir != dir_None) SDL_Delay(5);
                IN_ClearKeysDown ();
                break;
            case dir_North:
            case dir_South:
                exit = 1;
            default:
                break;
        }
    }
    while (!exit);

    SD_PlaySound (ESCPRESSEDSND);
    WaitKeyUp ();
    DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);
}

static int lastwhich = -1;

////////////////////////
//
// FIXUP GUN CURSOR OVERDRAW SHIT
//
void
FixupCustom (int w)
{

    int y = CST_Y + 26 + w * 13;

    VWB_Hlin (7, 32, y - 1, DEACTIVE);
    VWB_Hlin (7, 32, y + 12, BORD2COLOR);
#ifndef SPEAR
    VWB_Hlin (7, 32, y - 2, BORDCOLOR);
    VWB_Hlin (7, 32, y + 13, BORDCOLOR);
#else
    VWB_Hlin (7, 32, y - 2, BORD2COLOR);
    VWB_Hlin (7, 32, y + 13, BORD2COLOR);
#endif

    if (lastwhich >= 0)
    {
        y = CST_Y + 26 + lastwhich * 13;
        VWB_Hlin (7, 32, y - 1, DEACTIVE);
        VWB_Hlin (7, 32, y + 12, BORD2COLOR);
#ifndef SPEAR
        VWB_Hlin (7, 32, y - 2, BORDCOLOR);
        VWB_Hlin (7, 32, y + 13, BORDCOLOR);
#else
        VWB_Hlin (7, 32, y - 2, BORD2COLOR);
        VWB_Hlin (7, 32, y + 13, BORD2COLOR);
#endif

    }

    lastwhich = w;
}

////////////////////////
//
// DRAW CUSTOMIZE SCREEN
//
void
DrawCustomJoyScreen (void)
{
    int i;

    // Reset last menu item
    lastwhich = -1;

#ifdef JAPAN
    CA_CacheScreen (S_CUSTOMPIC);
    fontnumber = 1;

    PrintX = CST_START;
    PrintY = CST_Y + 26;
    DrawCustJoy2 (0);

    PrintX = CST_START;
    US_Print ("\n\n\n");
    DrawCustJoy (0);

    PrintX = CST_START;
    US_Print ("\n\n\n");
    DrawCustKeybd (0);

    PrintX = CST_START;
    US_Print ("\n\n\n");
    DrawCustKeys (0);
#else
    ClearMScreen ();
    WindowX = 0;
    WindowW = 320;
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);
    DrawStripes (10);
    VWB_DrawPic (80, 0, C_CUSTOMIZEPIC);

    //
    // MOUSE
    //
    SETFONTCOLOR (READCOLOR, BKGDCOLOR);
    WindowX = 0;
    WindowW = 320;

#ifndef SPEAR
    PrintY = CST_Y;
    US_CPrint ("Gamepad\n");
#else
    PrintY = CST_Y + 13;
    VWB_DrawPic (128, 48, C_MOUSEPIC);
#endif

    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
    PrintX = CST_START - 16;
    US_Print (STR_CPAUSE);
    PrintX = CST_START - 16 + CST_SPC * 1;
    US_Print (STR_CESCAPE);
    PrintX = CST_START - 16 + CST_SPC * 2;
    US_Print (STR_CPREVWEP);
    PrintX = CST_START - 16 + CST_SPC * 3;
    US_Print (STR_CNEXTWEP "\n");
#else
    PrintX = CST_START;
    US_Print (STR_CPAUSE);
    PrintX = CST_START + CST_SPC * 1;
    US_Print (STR_CESCAPE);
    PrintX = CST_START + CST_SPC * 2;
    US_Print (STR_CPREVWEP);
    PrintX = CST_START + CST_SPC * 3;
    US_Print (STR_CNEXTWEP "\n");
#endif

    DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);
    DrawCustJoy2 (0);
    US_Print ("\n");


    //
    // JOYSTICK/PAD
    //
#ifndef SPEAR
    SETFONTCOLOR (READCOLOR, BKGDCOLOR);
    US_CPrint ("\n");
#else
    PrintY += 13;
    VWB_DrawPic (40, 88, C_JOYSTICKPIC);
#endif

#ifdef SPEAR
    VWB_DrawPic (112, 120, C_KEYBOARDPIC);
#endif

    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
    PrintX = CST_START - 16;
    US_Print (STR_CRUN);
    PrintX = CST_START - 16 + CST_SPC * 1;
    US_Print (STR_COPEN);
    PrintX = CST_START - 16 + CST_SPC * 2;
    US_Print (STR_CFIRE);
    PrintX = CST_START - 16 + CST_SPC * 3;
    US_Print (STR_CSTRAFE "\n");
#else
    PrintX = CST_START;
    US_Print (STR_CRUN);
    PrintX = CST_START + CST_SPC * 1;
    US_Print (STR_COPEN);
    PrintX = CST_START + CST_SPC * 2;
    US_Print (STR_CFIRE);
    PrintX = CST_START + CST_SPC * 3;
    US_Print (STR_CSTRAFE "\n");
#endif
    DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);
    DrawCustJoy (0);
    US_Print ("\n");
#endif
    //
    // PICK STARTING POINT IN MENU
    //
    if (CusJoyItems.curpos < 0)
        for (i = 0; i < CusJoyItems.amount; i++)
            if (CusJoyMenu[i].active)
            {
                CusJoyItems.curpos = i;
                break;
            }


    VW_UpdateScreen ();
    MenuFadeIn ();
}


////////////////////////
//
// DRAW CUSTOMIZE SCREEN
//
void
DrawCustomScreen (void)
{
    int i;

    // Reset last menu item
    lastwhich = -1;

#ifdef JAPAN
    CA_CacheScreen (S_CUSTOMPIC);
    fontnumber = 1;

    PrintX = CST_START;
    PrintY = CST_Y + 26;
    DrawCustMouse (0);

    PrintX = CST_START;
    US_Print ("\n\n\n");
    DrawCustKeybd (0);

    PrintX = CST_START;
    US_Print ("\n\n\n");
    DrawCustKeys (0);
#else
    ClearMScreen ();
    WindowX = 0;
    WindowW = 320;
    VWB_DrawPic (112, 184, C_MOUSELBACKPIC);
    DrawStripes (10);
    VWB_DrawPic (80, 0, C_CUSTOMIZEPIC);

    //
    // MOUSE
    //
    SETFONTCOLOR (READCOLOR, BKGDCOLOR);
    WindowX = 0;
    WindowW = 320;

#ifndef SPEAR
    PrintY = CST_Y;
    US_CPrint ("Mouse\n");
#else
    PrintY = CST_Y + 13;
    VWB_DrawPic (128, 48, C_MOUSEPIC);
#endif

    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
    PrintX = CST_START - 16;
    US_Print (STR_CRUN);
    PrintX = CST_START - 16 + CST_SPC * 1;
    US_Print (STR_COPEN);
    PrintX = CST_START - 16 + CST_SPC * 2;
    US_Print (STR_CFIRE);
    PrintX = CST_START - 16 + CST_SPC * 3;
    US_Print (STR_CSTRAFE "\n");
#else
    PrintX = CST_START;
    US_Print (STR_CRUN);
    PrintX = CST_START + CST_SPC * 1;
    US_Print (STR_COPEN);
    PrintX = CST_START + CST_SPC * 2;
    US_Print (STR_CFIRE);
    PrintX = CST_START + CST_SPC * 3;
    US_Print (STR_CSTRAFE "\n");
#endif

    DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);
    DrawCustMouse (0);
    US_Print ("\n");
    US_Print ("\n");
    US_Print ("\n");
    PrintY += 13;

    //
    // KEYBOARD
    //
#ifndef SPEAR
    SETFONTCOLOR (READCOLOR, BKGDCOLOR);
    US_CPrint ("Keyboard\n");
#else
    PrintY += 13;
#endif
    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
    PrintX = CST_START - 16;
    US_Print (STR_CRUN);
    PrintX = CST_START - 16 + CST_SPC * 1;
    US_Print (STR_COPEN);
    PrintX = CST_START - 16 + CST_SPC * 2;
    US_Print (STR_CFIRE);
    PrintX = CST_START - 16 + CST_SPC * 3;
    US_Print (STR_CSTRAFE "\n");
#else
    PrintX = CST_START;
    US_Print (STR_CRUN);
    PrintX = CST_START + CST_SPC * 1;
    US_Print (STR_COPEN);
    PrintX = CST_START + CST_SPC * 2;
    US_Print (STR_CFIRE);
    PrintX = CST_START + CST_SPC * 3;
    US_Print (STR_CSTRAFE "\n");
#endif
    DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);
    DrawCustKeybd (0);
    US_Print ("\n");


    //
    // KEYBOARD MOVE KEYS
    //
    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
    PrintX = 4;
    US_Print (STR_LEFT);
    US_Print ("/");
    US_Print (STR_RIGHT);
    US_Print ("/");
    US_Print (STR_FRWD);
    US_Print ("/");
    US_Print (STR_BKWD "\n");
#else
    PrintX = CST_START;
    US_Print (STR_LEFT);
    PrintX = CST_START + CST_SPC * 1;
    US_Print (STR_RIGHT);
    PrintX = CST_START + CST_SPC * 2;
    US_Print (STR_FRWD);
    PrintX = CST_START + CST_SPC * 3;
    US_Print (STR_BKWD "\n");
#endif
    DrawWindow (5, PrintY - 1, 310, 13, BKGDCOLOR);
    DrawCustKeys (0);
#endif
    //
    // PICK STARTING POINT IN MENU
    //
    if (CusItems.curpos < 0)
        for (i = 0; i < CusItems.amount; i++)
            if (CusMenu[i].active)
            {
                CusItems.curpos = i;
                break;
            }


    VW_UpdateScreen ();
    MenuFadeIn ();
}


void
PrintCustMouse (int i)
{
    int j;

    for (j = 0; j < 4; j++)
        if (order[i] == buttonmouse[j])
        {
            PrintX = CST_START + CST_SPC * i;
            US_Print (mbarray[j]);
            break;
        }
}

void
DrawCustMouse (int hilight)
{
    int i, color;


    color = TEXTCOLOR;
    if (hilight)
        color = HIGHLIGHT;
    SETFONTCOLOR (color, BKGDCOLOR);

    if (!mouseenabled)
    {
        SETFONTCOLOR (DEACTIVE, BKGDCOLOR);
        CusMenu[0].active = 0;
    }
    else
        CusMenu[0].active = 1;

    PrintY = CST_Y + 13 * 2;
    for (i = 0; i < 4; i++)
        PrintCustMouse (i);
}

void
PrintCustJoy2 (int i)
{
    int j;

    for (int j = 0; j < 13; j++)
    {
        if (order2[i] == buttonjoy[j])
        {
            PrintX = CST_START + CST_SPC * i;
            US_Print (joyarray[j]);
            break;
        }
    }
}

void
DrawCustJoy2 (int hilight)
{
    int i, color;

    color = TEXTCOLOR;
    if (hilight)
        color = HIGHLIGHT;
    SETFONTCOLOR (color, BKGDCOLOR);

    if (!joystickenabled)
    {
        SETFONTCOLOR (DEACTIVE, BKGDCOLOR);
        CusJoyMenu[0].active = 0;
    }
    else
        CusJoyMenu[0].active = 1;

    // Clear text
    VWB_Bar (60, CST_Y + 13 * 2, 250, 12, BKGDCOLOR);

    PrintY = CST_Y + 13 * 2;
    for (i = 0; i < 4; i++)
        PrintCustJoy2 (i);
}

void
PrintCustJoy (int i)
{
    for (int j = 0; j < 10; j++)
    {
        if (order[i] == buttonjoy[j])
        {
            PrintX = CST_START + CST_SPC * i;
            US_Print (joyarray[j]);
            break;
        }
    }
}

void
DrawCustJoy (int hilight)
{
    int i, color;

    color = TEXTCOLOR;
    if (hilight)
        color = HIGHLIGHT;
    SETFONTCOLOR (color, BKGDCOLOR);

    if (!joystickenabled)
    {
        SETFONTCOLOR (DEACTIVE, BKGDCOLOR);
        CusJoyMenu[3].active = 0;
    }
    else
        CusJoyMenu[3].active = 1;

    // Clear text
    VWB_Bar (60, CST_Y + 13 * 5, 250, 12, BKGDCOLOR);

    PrintY = CST_Y + 13 * 5;
    for (i = 0; i < 4; i++)
        PrintCustJoy (i);
}


void
PrintCustKeybd (int i)
{
    PrintX = CST_START + CST_SPC * i;
    US_Print ((const char *) IN_GetScanName (buttonscan[order[i]]));
}

void
DrawCustKeybd (int hilight)
{
    int i, color;


    color = TEXTCOLOR;
    if (hilight)
        color = HIGHLIGHT;
    SETFONTCOLOR (color, BKGDCOLOR);

    PrintY = CST_Y + 13 * 8;
    for (i = 0; i < 4; i++)
        PrintCustKeybd (i);
}

void
PrintCustKeys (int i)
{
    PrintX = CST_START + CST_SPC * i;
    US_Print ((const char *) IN_GetScanName (dirscan[moveorder[i]]));
}

void
DrawCustKeys (int hilight)
{
    int i, color;


    color = TEXTCOLOR;
    if (hilight)
        color = HIGHLIGHT;
    SETFONTCOLOR (color, BKGDCOLOR);

    PrintY = CST_Y + 13 * 10;
    for (i = 0; i < 4; i++)
        PrintCustKeys (i);
}


////////////////////////////////////////////////////////////////////
//
// CHANGE SCREEN VIEWING SIZE
//
////////////////////////////////////////////////////////////////////
int
CP_ChangeView (int)
{
    int exit = 0, oldview, newview;
    ControlInfo ci;

    WindowX = WindowY = 0;
    WindowW = 320;
    WindowH = 200;
    newview = oldview = viewsize;
    DrawChangeView (oldview);
    MenuFadeIn ();

    do
    {
        CheckPause ();
        SDL_Delay(5);
        ReadAnyControl (&ci);
        switch (ci.dir)
        {
            case dir_South:
            case dir_West:
                newview--;
                if (newview < 4)
                    newview = 4;
                if(newview >= 19) DrawChangeView(newview);
                else ShowViewSize (newview);
                VW_UpdateScreen ();
                SD_PlaySound (HITWALLSND);
                TicDelay (10);
                break;

            case dir_North:
            case dir_East:
                newview++;
                if (newview >= 21)
                {
                    newview = 21;
                    DrawChangeView(newview);
                }
                else ShowViewSize (newview);
                VW_UpdateScreen ();
                SD_PlaySound (HITWALLSND);
                TicDelay (10);
                break;
            default:
                break;
        }

        if (ci.button0 || Keyboard[sc_Enter])
            exit = 1;
        else if (ci.button1 || Keyboard[sc_Escape])
        {
            SD_PlaySound (ESCPRESSEDSND);
            MenuFadeOut ();
            if(screenHeight % 200 != 0)
                VL_ClearScreen(0);
            return 0;
        }
    }
    while (!exit);

    if (oldview != newview)
    {
        SD_PlaySound (SHOOTSND);
        Message (STR_THINK "...");
        NewViewSize (newview);
    }

    ShootSnd ();
    MenuFadeOut ();
    if(screenHeight % 200 != 0)
        VL_ClearScreen(0);

    return 0;
}


/////////////////////////////
//
// DRAW THE CHANGEVIEW SCREEN
//
void
DrawChangeView (int view)
{
    int rescaledHeight = screenHeight / scaleFactor;
    if(view != 21) VWB_Bar (0, rescaledHeight - 40, 320, 40, bordercol);

#ifdef JAPAN
    CA_CacheScreen (S_CHANGEPIC);

    ShowViewSize (view);
#else
    ShowViewSize (view);

    PrintY = (screenHeight / scaleFactor) - 39;
    WindowX = 0;
    WindowY = 320;                                  // TODO: Check this!
    SETFONTCOLOR (HIGHLIGHT, BKGDCOLOR);

    US_CPrint (STR_SIZE1 "\n");
    US_CPrint (STR_SIZE2 "\n");
    US_CPrint (STR_SIZE3);
#endif
    VW_UpdateScreen ();
}


////////////////////////////////////////////////////////////////////
//
// QUIT THIS INFERNAL GAME!
//
////////////////////////////////////////////////////////////////////
int
CP_Quit (int)
{
#ifdef JAPAN
    if (GetYorN (7, 11, C_QUITMSGPIC))
#else

#ifdef SPANISH
    if (Confirm (ENDGAMESTR))
#else
    if (Confirm (endStrings[US_RndT () & 0x7 + (US_RndT () & 1)]))
#endif

#endif
    {
        VW_UpdateScreen ();
        SD_MusicOff ();
        SD_StopSound ();
        MenuFadeOut ();
        Quit (NULL);
    }

    DrawMainMenu ();
    return 0;
}


////////////////////////////////////////////////////////////////////
//
// HANDLE INTRO SCREEN (SYSTEM CONFIG)
//
////////////////////////////////////////////////////////////////////
void
IntroScreen (void)
{
#ifdef SPEAR

#define MAINCOLOR       0x4f
#define EMSCOLOR        0x4f
#define XMSCOLOR        0x4f

#else

#define MAINCOLOR       0x6c
#define EMSCOLOR        0x6c    // 0x4f
#define XMSCOLOR        0x6c    // 0x7f

#endif
#define FILLCOLOR       14

//      long memory;
//      long emshere,xmshere;
    int i;
/*      int ems[10]={100,200,300,400,500,600,700,800,900,1000},
                xms[10]={100,200,300,400,500,600,700,800,900,1000};
        int main[10]={32,64,96,128,160,192,224,256,288,320};*/


    //
    // DRAW MAIN MEMORY
    //
#ifdef ABCAUS
    memory = (1023l + mminfo.nearheap + mminfo.farheap) / 1024l;
    for (i = 0; i < 10; i++)
        if (memory >= main[i])
            VWB_Bar (49, 163 - 8 * i, 6, 5, MAINCOLOR - i);

    //
    // DRAW EMS MEMORY
    //
    if (EMSPresent)
    {
        emshere = 4l * EMSPagesAvail;
        for (i = 0; i < 10; i++)
            if (emshere >= ems[i])
                VWB_Bar (89, 163 - 8 * i, 6, 5, EMSCOLOR - i);
    }

    //
    // DRAW XMS MEMORY
    //
    if (XMSPresent)
    {
        xmshere = 4l * XMSPagesAvail;
        for (i = 0; i < 10; i++)
            if (xmshere >= xms[i])
                VWB_Bar (129, 163 - 8 * i, 6, 5, XMSCOLOR - i);
    }
#else
    for (i = 0; i < 10; i++)
        VWB_Bar (49, 163 - 8 * i, 6, 5, MAINCOLOR - i);
    for (i = 0; i < 10; i++)
        VWB_Bar (89, 163 - 8 * i, 6, 5, EMSCOLOR - i);
    for (i = 0; i < 10; i++)
        VWB_Bar (129, 163 - 8 * i, 6, 5, XMSCOLOR - i);
#endif


    //
    // FILL BOXES
    //
    if (MousePresent)
        VWB_Bar (164, 82, 12, 2, FILLCOLOR);

    if (IN_JoyPresent())
        VWB_Bar (164, 105, 12, 2, FILLCOLOR);

    if (AdLibPresent && !SoundBlasterPresent)
        VWB_Bar (164, 128, 12, 2, FILLCOLOR);

    if (SoundBlasterPresent)
        VWB_Bar (164, 151, 12, 2, FILLCOLOR);

//    if (SoundSourcePresent)
//        VWB_Bar (164, 174, 12, 2, FILLCOLOR);
}


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
// SUPPORT ROUTINES
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
// Clear Menu screens to dark red
//
////////////////////////////////////////////////////////////////////
void
ClearMScreen (void)
{
#ifndef SPEAR
    VWB_Bar (0, 0, 320, 200, BORDCOLOR);
#else
    VWB_DrawPic (0, 0, C_BACKDROPPIC);
#endif
}


////////////////////////////////////////////////////////////////////
//
// Un/Cache a LUMP of graphics
//
////////////////////////////////////////////////////////////////////
void
CacheLump (int lumpstart, int lumpend)
{
    int i;

    for (i = lumpstart; i <= lumpend; i++)
        CA_CacheGrChunk (i);
}


void
UnCacheLump (int lumpstart, int lumpend)
{
    int i;

    for (i = lumpstart; i <= lumpend; i++)
        if (grsegs[i])
            UNCACHEGRCHUNK (i);
}


////////////////////////////////////////////////////////////////////
//
// Draw a window for a menu
//
////////////////////////////////////////////////////////////////////
void
DrawWindow (int x, int y, int w, int h, int wcolor)
{
    VWB_Bar (x, y, w, h, wcolor);
    DrawOutline (x, y, w, h, BORD2COLOR, DEACTIVE);
}


void
DrawOutline (int x, int y, int w, int h, int color1, int color2)
{
    VWB_Hlin (x, x + w, y, color2);
    VWB_Vlin (y, y + h, x, color2);
    VWB_Hlin (x, x + w, y + h, color1);
    VWB_Vlin (y, y + h, x + w, color1);
}


////////////////////////////////////////////////////////////////////
//
// Setup Control Panel stuff - graphics, etc.
//
////////////////////////////////////////////////////////////////////
void
SetupControlPanel (void)
{
    //
    // CACHE GRAPHICS & SOUNDS
    //
    CA_CacheGrChunk (STARTFONT + 1);
#ifndef SPEAR
    CacheLump (CONTROLS_LUMP_START, CONTROLS_LUMP_END);
#else
    CacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif

    SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
    fontnumber = 1;
    WindowH = 200;
    if(screenHeight % 200 != 0)
        VL_ClearScreen(0);

    if (!ingame)
        CA_LoadAllSounds ();
    else
        MainMenu[savegame].active = 1;

    //
    // CENTER MOUSE
    //
    if(IN_IsInputGrabbed())
        IN_CenterMouse();
}

////////////////////////////////////////////////////////////////////
//
// SEE WHICH SAVE GAME FILES ARE AVAILABLE & READ STRING IN
//
////////////////////////////////////////////////////////////////////
void SetupSaveGames()
{
    char name[13];
    char savepath[300];

    strcpy(name, SaveName);
    for(int i = 0; i < 10; i++)
    {
        name[7] = '0' + i;
#ifdef _arch_dreamcast
        // Try to unpack file
        if(DC_LoadFromVMU(name))
        {
#endif
            if(configdir[0])
                snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
            else
                strcpy(savepath, name);

            const int handle = open(savepath, O_RDONLY | O_BINARY);
            if(handle >= 0)
            {
                char temp[32];

                SaveGamesAvail[i] = 1;
                read(handle, temp, 32);
                close(handle);
                strcpy(&SaveGameNames[i][0], temp);
            }
#ifdef _arch_dreamcast
            // Remove unpacked version of file
            fs_unlink(name);
        }
#endif
    }
}

////////////////////////////////////////////////////////////////////
//
// Clean up all the Control Panel stuff
//
////////////////////////////////////////////////////////////////////
void
CleanupControlPanel (void)
{
#ifndef SPEAR
    UnCacheLump (CONTROLS_LUMP_START, CONTROLS_LUMP_END);
#else
    UnCacheLump (BACKDROP_LUMP_START, BACKDROP_LUMP_END);
#endif

    fontnumber = 0;
}


////////////////////////////////////////////////////////////////////
//
// Handle moving gun around a menu
//
////////////////////////////////////////////////////////////////////
int
HandleMenu (CP_iteminfo * item_i, CP_itemtype * items, void (*routine) (int w))
{
    char key;
    static int redrawitem = 1, lastitem = -1;
    int i, x, y, basey, exit, which, shape;
    int32_t lastBlinkTime, timer;
    ControlInfo ci;


    which = item_i->curpos;
    x = item_i->x & -8;
    basey = item_i->y - 2;
    y = basey + which * 13;

    VWB_DrawPic (x, y, C_CURSOR1PIC);
    SetTextColor (items + which, 1);
    if (redrawitem)
    {
        PrintX = item_i->x + item_i->indent;
        PrintY = item_i->y + which * 13;
        US_Print ((items + which)->string);
    }
    //
    // CALL CUSTOM ROUTINE IF IT IS NEEDED
    //
    if (routine)
        routine (which);
    VW_UpdateScreen ();

    shape = C_CURSOR1PIC;
    timer = 8;
    exit = 0;
    lastBlinkTime = GetTimeCount ();
    IN_ClearKeysDown ();


    do
    {
        //
        // CHANGE GUN SHAPE
        //
        if ((int32_t)GetTimeCount () - lastBlinkTime > timer)
        {
            lastBlinkTime = GetTimeCount ();
            if (shape == C_CURSOR1PIC)
            {
                shape = C_CURSOR2PIC;
                timer = 8;
            }
            else
            {
                shape = C_CURSOR1PIC;
                timer = 70;
            }
            VWB_DrawPic (x, y, shape);
            if (routine)
                routine (which);
            VW_UpdateScreen ();
        }
        else SDL_Delay(5);

        CheckPause ();

        //
        // SEE IF ANY KEYS ARE PRESSED FOR INITIAL CHAR FINDING
        //
        key = LastASCII;
        if (key)
        {
            int ok = 0;

            if (key >= 'a')
                key -= 'a' - 'A';

            for (i = which + 1; i < item_i->amount; i++)
                if ((items + i)->active && (items + i)->string[0] == key)
                {
                    EraseGun (item_i, items, x, y, which);
                    which = i;
                    DrawGun (item_i, items, x, &y, which, basey, routine);
                    ok = 1;
                    IN_ClearKeysDown ();
                    break;
                }

            //
            // DIDN'T FIND A MATCH FIRST TIME THRU. CHECK AGAIN.
            //
            if (!ok)
            {
                for (i = 0; i < which; i++)
                    if ((items + i)->active && (items + i)->string[0] == key)
                    {
                        EraseGun (item_i, items, x, y, which);
                        which = i;
                        DrawGun (item_i, items, x, &y, which, basey, routine);
                        IN_ClearKeysDown ();
                        break;
                    }
            }
        }

        //
        // GET INPUT
        //
        ReadAnyControl (&ci);
        switch (ci.dir)
        {
                ////////////////////////////////////////////////
                //
                // MOVE UP
                //
            case dir_North:

                EraseGun (item_i, items, x, y, which);

                //
                // ANIMATE HALF-STEP
                //
                if (which && (items + which - 1)->active)
                {
                    y -= 6;
                    DrawHalfStep (x, y);
                }

                //
                // MOVE TO NEXT AVAILABLE SPOT
                //
                do
                {
                    if (!which)
                        which = item_i->amount - 1;
                    else
                        which--;
                }
                while (!(items + which)->active);

                DrawGun (item_i, items, x, &y, which, basey, routine);
                //
                // WAIT FOR BUTTON-UP OR DELAY NEXT MOVE
                //
                TicDelay (20);
                break;

                ////////////////////////////////////////////////
                //
                // MOVE DOWN
                //
            case dir_South:

                EraseGun (item_i, items, x, y, which);
                //
                // ANIMATE HALF-STEP
                //
                if (which != item_i->amount - 1 && (items + which + 1)->active)
                {
                    y += 6;
                    DrawHalfStep (x, y);
                }

                do
                {
                    if (which == item_i->amount - 1)
                        which = 0;
                    else
                        which++;
                }
                while (!(items + which)->active);

                DrawGun (item_i, items, x, &y, which, basey, routine);

                //
                // WAIT FOR BUTTON-UP OR DELAY NEXT MOVE
                //
                TicDelay (20);
                break;
            default:
                break;
        }

        if (ci.button0 || Keyboard[sc_Space] || Keyboard[sc_Enter])
            exit = 1;

        if ((ci.button1 && !Keyboard[sc_Alt]) || Keyboard[sc_Escape])
            exit = 2;

    }
    while (!exit);


    IN_ClearKeysDown ();

    //
    // ERASE EVERYTHING
    //
    if (lastitem != which)
    {
        VWB_Bar (x - 1, y, 25, 16, BKGDCOLOR);
        PrintX = item_i->x + item_i->indent;
        PrintY = item_i->y + which * 13;
        US_Print ((items + which)->string);
        redrawitem = 1;
    }
    else
        redrawitem = 0;

    if (routine)
        routine (which);
    VW_UpdateScreen ();

    item_i->curpos = which;

    lastitem = which;
    switch (exit)
    {
        case 1:
            //
            // CALL THE ROUTINE
            //
            if ((items + which)->routine != NULL)
            {
                ShootSnd ();
                MenuFadeOut ();
                (items + which)->routine (0);
            }
            return which;

        case 2:
            SD_PlaySound (ESCPRESSEDSND);
            return -1;
    }

    return 0;                   // JUST TO SHUT UP THE ERROR MESSAGES!
}


//
// ERASE GUN & DE-HIGHLIGHT STRING
//
void
EraseGun (CP_iteminfo * item_i, CP_itemtype * items, int x, int y, int which)
{
    VWB_Bar (x - 1, y, 25, 16, BKGDCOLOR);
    SetTextColor (items + which, 0);

    PrintX = item_i->x + item_i->indent;
    PrintY = item_i->y + which * 13;
    US_Print ((items + which)->string);
    VW_UpdateScreen ();
}


//
// DRAW HALF STEP OF GUN TO NEXT POSITION
//
void
DrawHalfStep (int x, int y)
{
    VWB_DrawPic (x, y, C_CURSOR1PIC);
    VW_UpdateScreen ();
    SD_PlaySound (MOVEGUN1SND);
    SDL_Delay (8 * 100 / 7);
}


//
// DRAW GUN AT NEW POSITION
//
void
DrawGun (CP_iteminfo * item_i, CP_itemtype * items, int x, int *y, int which, int basey,
         void (*routine) (int w))
{
    VWB_Bar (x - 1, *y, 25, 16, BKGDCOLOR);
    *y = basey + which * 13;
    VWB_DrawPic (x, *y, C_CURSOR1PIC);
    SetTextColor (items + which, 1);

    PrintX = item_i->x + item_i->indent;
    PrintY = item_i->y + which * 13;
    US_Print ((items + which)->string);

    //
    // CALL CUSTOM ROUTINE IF IT IS NEEDED
    //
    if (routine)
        routine (which);
    VW_UpdateScreen ();
    SD_PlaySound (MOVEGUN2SND);
}

////////////////////////////////////////////////////////////////////
//
// DELAY FOR AN AMOUNT OF TICS OR UNTIL CONTROLS ARE INACTIVE
//
////////////////////////////////////////////////////////////////////
void
TicDelay (int count)
{
    ControlInfo ci;

    int32_t startTime = GetTimeCount ();
    do
    {
        SDL_Delay(5);
        ReadAnyControl (&ci);
    }
    while ((int32_t) GetTimeCount () - startTime < count && ci.dir != dir_None);
}


////////////////////////////////////////////////////////////////////
//
// Draw a menu
//
////////////////////////////////////////////////////////////////////
void
DrawMenu (CP_iteminfo * item_i, CP_itemtype * items)
{
    int i, which = item_i->curpos;


    WindowX = PrintX = item_i->x + item_i->indent;
    WindowY = PrintY = item_i->y;
    WindowW = 320;
    WindowH = 200;

    for (i = 0; i < item_i->amount; i++)
    {
        SetTextColor (items + i, which == i);

        PrintY = item_i->y + i * 13;
        if ((items + i)->active)
            US_Print ((items + i)->string);
        else
        {
            SETFONTCOLOR (DEACTIVE, BKGDCOLOR);
            US_Print ((items + i)->string);
            SETFONTCOLOR (TEXTCOLOR, BKGDCOLOR);
        }

        US_Print ("\n");
    }
}


////////////////////////////////////////////////////////////////////
//
// SET TEXT COLOR (HIGHLIGHT OR NO)
//
////////////////////////////////////////////////////////////////////
void
SetTextColor (CP_itemtype * items, int hlight)
{
    if (hlight)
    {
        SETFONTCOLOR (color_hlite[items->active], BKGDCOLOR);
    }
    else
    {
        SETFONTCOLOR (color_norml[items->active], BKGDCOLOR);
    }
}


////////////////////////////////////////////////////////////////////
//
// WAIT FOR CTRLKEY-UP OR BUTTON-UP
//
////////////////////////////////////////////////////////////////////
void
WaitKeyUp (void)
{
    ControlInfo ci;
    while (ReadAnyControl (&ci), ci.button0 |
           ci.button1 | ci.button2 | ci.button3 | ci.button4 | ci.button5 | ci.button6 | ci.button7 | ci.button8 | ci.button9 | ci.button10 | ci.button11 | ci.button12 |
           Keyboard[sc_Space] | Keyboard[sc_Enter] | Keyboard[sc_Escape])
    {
        IN_WaitAndProcessEvents();
    }
}


////////////////////////////////////////////////////////////////////
//
// READ KEYBOARD, JOYSTICK AND MOUSE FOR INPUT
//
////////////////////////////////////////////////////////////////////
void
ReadAnyControl (ControlInfo * ci)
{
    int mouseactive = 0;

    IN_ReadControl (0, ci);

    if (mouseenabled && IN_IsInputGrabbed())
    {
        int mousex, mousey, buttons;
        buttons = SDL_GetMouseState(&mousex, &mousey);
        int middlePressed = buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
        int rightPressed = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        buttons &= ~(SDL_BUTTON(SDL_BUTTON_MIDDLE) | SDL_BUTTON(SDL_BUTTON_RIGHT));
        if(middlePressed) buttons |= 1 << 2;
        if(rightPressed) buttons |= 1 << 1;

        if(mousey - CENTERY < -SENSITIVE)
        {
            ci->dir = dir_North;
            mouseactive = 1;
        }
        else if(mousey - CENTERY > SENSITIVE)
        {
            ci->dir = dir_South;
            mouseactive = 1;
        }

        if(mousex - CENTERX < -SENSITIVE)
        {
            ci->dir = dir_West;
            mouseactive = 1;
        }
        else if(mousex - CENTERX > SENSITIVE)
        {
            ci->dir = dir_East;
            mouseactive = 1;
        }

        if(mouseactive)
            IN_CenterMouse();

        if (buttons)
        {
            ci->button0 = buttons & 1;
            ci->button1 = buttons & 2;
            ci->button2 = buttons & 4;
            ci->button3 = false;
            mouseactive = 1;
        }
    }

    if (joystickenabled && !mouseactive)
    {
        int jx, jy, jb;

        IN_GetJoyDelta (&jx, &jy, SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY);
        if (jy < -SENSITIVE)
            ci->dir = dir_North;
        else if (jy > SENSITIVE)
            ci->dir = dir_South;

        if (jx < -SENSITIVE)
            ci->dir = dir_West;
        else if (jx > SENSITIVE)
            ci->dir = dir_East;

        jb = IN_JoyButtons ();
        if (jb)
        {
            ci->button0 = (jb & (1 << 0)) > 0;
            ci->button1 = (jb & (1 << 1)) > 0;
            ci->button2 = (jb & (1 << 2)) > 0;
            ci->button3 = (jb & (1 << 3)) > 0;
            ci->button4 = (jb & (1 << 4)) > 0;
            ci->button5 = (jb & (1 << 5)) > 0;
            ci->button6 = (jb & (1 << 6)) > 0;
            ci->button7 = (jb & (1 << 7)) > 0;
            ci->button8 = (jb & (1 << 8)) > 0;
            ci->button9 = (jb & (1 << 9)) > 0;
            ci->button10 = (jb & (1 << 10)) > 0;
            ci->button11 = (jb & (1 << 11)) > 0;
            ci->button12 = (jb & (1 << 12)) > 0;
        }
    }
}


////////////////////////////////////////////////////////////////////
//
// DRAW DIALOG AND CONFIRM YES OR NO TO QUESTION
//
////////////////////////////////////////////////////////////////////
int
Confirm (const char *string)
{
    int xit = 0, x, y, tick = 0, lastBlinkTime;
    int whichsnd[2] = { ESCPRESSEDSND, SHOOTSND };
    ControlInfo ci;

    Message (string);
    IN_ClearKeysDown ();
    WaitKeyUp ();

    //
    // BLINK CURSOR
    //
    x = PrintX;
    y = PrintY;
    lastBlinkTime = GetTimeCount();

    do
    {
        ReadAnyControl(&ci);

        if (GetTimeCount() - lastBlinkTime >= 10)
        {
            switch (tick)
            {
                case 0:
                    VWB_Bar (x, y, 8, 13, TEXTCOLOR);
                    break;
                case 1:
                    PrintX = x;
                    PrintY = y;
                    US_Print ("_");
            }
            VW_UpdateScreen ();
            tick ^= 1;
            lastBlinkTime = GetTimeCount();
        }
        else SDL_Delay(5);

#ifdef SPANISH
    }
    while (!Keyboard[sc_S] && !Keyboard[sc_N] && !Keyboard[sc_Escape]);
#else
    }
    while (!Keyboard[sc_Y] && !Keyboard[sc_N] && !Keyboard[sc_Escape] && !ci.button0 && !ci.button1);
#endif

#ifdef SPANISH
    if (Keyboard[sc_S] || ci.button0)
    {
        xit = 1;
        ShootSnd ();
    }
#else
    if (Keyboard[sc_Y] || ci.button0)
    {
        xit = 1;
        ShootSnd ();
    }
#endif

    IN_ClearKeysDown ();
    WaitKeyUp ();

    SD_PlaySound ((soundnames) whichsnd[xit]);

    return xit;
}

#ifdef JAPAN
////////////////////////////////////////////////////////////////////
//
// DRAW MESSAGE & GET Y OR N
//
////////////////////////////////////////////////////////////////////
int
GetYorN (int x, int y, int pic)
{
    int xit = 0;
    soundnames whichsnd[2] = { ESCPRESSEDSND, SHOOTSND };


    CA_CacheGrChunk (pic);
    VWB_DrawPic (x * 8, y * 8, pic);
    UNCACHEGRCHUNK (pic);
    VW_UpdateScreen ();
    IN_ClearKeysDown ();

    do
    {
        IN_WaitAndProcessEvents();
    }
#ifdef SPANISH
    while (!Keyboard[sc_S] && !Keyboard[sc_N] && !Keyboard[sc_Escape]);
#else
    while (!Keyboard[sc_Y] && !Keyboard[sc_N] && !Keyboard[sc_Escape]);
#endif

#ifdef SPANISH
    if (Keyboard[sc_S])
    {
        xit = 1;
        ShootSnd ();
    }

    while (Keyboard[sc_S] || Keyboard[sc_N] || Keyboard[sc_Escape])
        IN_WaitAndProcessEvents();

#else

    if (Keyboard[sc_Y])
    {
        xit = 1;
        ShootSnd ();
    }

    while (Keyboard[sc_Y] || Keyboard[sc_N] || Keyboard[sc_Escape])
        IN_WaitAndProcessEvents();
#endif

    IN_ClearKeysDown ();
    SD_PlaySound (whichsnd[xit]);
    return xit;
}
#endif


////////////////////////////////////////////////////////////////////
//
// PRINT A MESSAGE IN A WINDOW
//
////////////////////////////////////////////////////////////////////
void
Message (const char *string)
{
    int h = 0, w = 0, mw = 0, i, len = (int) strlen(string);
    fontstruct *font;


    CA_CacheGrChunk (STARTFONT + 1);
    fontnumber = 1;
    font = (fontstruct *) grsegs[STARTFONT + fontnumber];
    h = font->height;
    for (i = 0; i < len; i++)
    {
        if (string[i] == '\n')
        {
            if (w > mw)
                mw = w;
            w = 0;
            h += font->height;
        }
        else
            w += font->width[string[i]];
    }

    if (w + 10 > mw)
        mw = w + 10;

    PrintY = (WindowH / 2) - h / 2;
    PrintX = WindowX = 160 - mw / 2;

    DrawWindow (WindowX - 5, PrintY - 5, mw + 10, h + 10, TEXTCOLOR);
    DrawOutline (WindowX - 5, PrintY - 5, mw + 10, h + 10, 0, HIGHLIGHT);
    SETFONTCOLOR (0, TEXTCOLOR);
    US_Print (string);
    VW_UpdateScreen ();
}

////////////////////////////////////////////////////////////////////
//
// THIS MAY BE FIXED A LITTLE LATER...
//
////////////////////////////////////////////////////////////////////
static int lastmusic;

int
StartCPMusic (int song)
{
    int lastoffs;

    lastmusic = song;
    lastoffs = SD_MusicOff ();
    UNCACHEAUDIOCHUNK (STARTMUSIC + lastmusic);

    SD_StartMusic(STARTMUSIC + song);
    return lastoffs;
}

void
FreeMusic (void)
{
    UNCACHEAUDIOCHUNK (STARTMUSIC + lastmusic);
}


///////////////////////////////////////////////////////////////////////////
//
//      IN_GetScanName() - Returns a string containing the name of the
//              specified scan code
//
///////////////////////////////////////////////////////////////////////////
const char *
IN_GetScanName (ScanCode scan)
{
/*    const char **p;
    ScanCode *s;

    for (s = ExtScanCodes, p = ExtScanNames; *s; p++, s++)
        if (*s == scan)
            return (*p);*/
    std::map<ScanCode, const char*>::iterator it = ScanNames.find(scan);
    if(it == ScanNames.end())
        return "?";
    return it->second;
}


///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR PAUSE KEY (FOR MUSIC ONLY)
//
///////////////////////////////////////////////////////////////////////////
void
CheckPause (void)
{
    if (Paused)
    {
        switch (SoundStatus)
        {
            case 0:
                SD_MusicOn ();
                break;
            case 1:
                SD_MusicOff ();
                break;
        }

        SoundStatus ^= 1;
        VW_WaitVBL (3);
        IN_ClearKeysDown ();
        Paused = false;
    }
}


///////////////////////////////////////////////////////////////////////////
//
// DRAW GUN CURSOR AT CORRECT POSITION IN MENU
//
///////////////////////////////////////////////////////////////////////////
void
DrawMenuGun (CP_iteminfo * iteminfo)
{
    int x, y;


    x = iteminfo->x;
    y = iteminfo->y + iteminfo->curpos * 13 - 2;
    VWB_DrawPic (x, y, C_CURSOR1PIC);
}


///////////////////////////////////////////////////////////////////////////
//
// DRAW SCREEN TITLE STRIPES
//
///////////////////////////////////////////////////////////////////////////
void
DrawStripes (int y)
{
#ifndef SPEAR
    VWB_Bar (0, y, 320, 24, 0);
    VWB_Hlin (0, 319, y + 22, STRIPE);
#else
    VWB_Bar (0, y, 320, 22, 0);
    VWB_Hlin (0, 319, y + 23, 0);
#endif
}

void
ShootSnd (void)
{
    SD_PlaySound (SHOOTSND);
}


///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR EPISODES
//
///////////////////////////////////////////////////////////////////////////
void
CheckForEpisodes (void)
{
    struct stat statbuf;

    // On Linux like systems, the configdir defaults to $HOME/.wolf4sdl
#if !defined(_WIN32) && !defined(_arch_dreamcast)
    if(configdir[0] == 0)
    {
        // Set config location to home directory for multi-user support
        char *homedir = getenv("HOME");
        if(homedir == NULL)
        {
            Quit("Your $HOME directory is not defined. You must set this before playing.");
        }
        #define WOLFDIR "/.wolf4sdl"
        if(strlen(homedir) + sizeof(WOLFDIR) > sizeof(configdir))
        {
            Quit("Your $HOME directory path is too long. It cannot be used for saving games.");
        }
        snprintf(configdir, sizeof(configdir), "%s" WOLFDIR, homedir);
    }
#endif

    if(configdir[0] != 0)
    {
        // Ensure config directory exists and create if necessary
        if(stat(configdir, &statbuf) != 0)
        {
#ifdef _WIN32
            if(_mkdir(configdir) != 0)
#else
            if(mkdir(configdir, 0755) != 0)
#endif
            {
                Quit("The configuration directory \"%s\" could not be created.", configdir);
            }
        }
    }

//
// JAPANESE VERSION
//
#ifdef JAPAN
#ifdef JAPDEMO
    if(!stat(DATADIR "vswap.wj1", &statbuf))
    {
        strcpy (extension, "wj1");
        numEpisodesMissing = 5;
#else
    if(!stat(DATADIR "vswap.wj6", &statbuf))
    {
        strcpy (extension, "wj6");
#endif
        strcat (configname, extension);
        strcat (SaveName, extension);
        strcat (demoname, extension);
        EpisodeSelect[1] =
            EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
    }
    else
        Quit ("NO JAPANESE WOLFENSTEIN 3-D DATA FILES to be found!");
    strcpy (graphext, extension);
    strcpy (audioext, extension);
#else

//
// ENGLISH
//
#ifdef UPLOAD
    if(!stat(DATADIR "vswap.wl1", &statbuf))
    {
        strcpy (extension, "wl1");
        numEpisodesMissing = 5;
    }
    else
        Quit ("NO WOLFENSTEIN 3-D DATA FILES to be found!");
#else
#ifndef SPEAR
    if(!stat(DATADIR "vswap.wl6", &statbuf))
    {
        strcpy (extension, "wl6");
        NewEmenu[2].active =
            NewEmenu[4].active =
            NewEmenu[6].active =
            NewEmenu[8].active =
            NewEmenu[10].active =
            EpisodeSelect[1] =
            EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
    }
    else
    {
        if(!stat(DATADIR "vswap.wl3", &statbuf))
        {
            strcpy (extension, "wl3");
            numEpisodesMissing = 3;
            NewEmenu[2].active = NewEmenu[4].active = EpisodeSelect[1] = EpisodeSelect[2] = 1;
        }
        else
        {
            if(!stat(DATADIR "vswap.wl1", &statbuf))
            {
                strcpy (extension, "wl1");
                numEpisodesMissing = 5;
            }
            else
                Quit ("NO WOLFENSTEIN 3-D DATA FILES to be found!");
        }
    }
#endif
#endif


#ifdef SPEAR
#ifndef SPEARDEMO
    if(param_mission == 0)
    {
        if(!stat(DATADIR "vswap.sod", &statbuf))
            strcpy (extension, "sod");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 1)
    {
        if(!stat(DATADIR "vswap.sd1", &statbuf))
            strcpy (extension, "sd1");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 2)
    {
        if(!stat(DATADIR "vswap.sd2", &statbuf))
            strcpy (extension, "sd2");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else if(param_mission == 3)
    {
        if(!stat(DATADIR "vswap.sd3", &statbuf))
            strcpy (extension, "sd3");
        else
            Quit ("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
    }
    else
        Quit ("UNSUPPORTED MISSION!");
    strcpy (graphext, "sod");
    strcpy (audioext, "sod");
#else
    if(!stat(DATADIR "vswap.sdm", &statbuf))
    {
        strcpy (extension, "sdm");
    }
    else
        Quit ("NO SPEAR OF DESTINY DEMO DATA FILES TO BE FOUND!");
    strcpy (graphext, "sdm");
    strcpy (audioext, "sdm");
#endif
#else
    strcpy (graphext, extension);
    strcpy (audioext, extension);
#endif

    strcat (configname, extension);
    strcat (SaveName, extension);
    strcat (demoname, extension);

#ifndef SPEAR
#ifndef GOODTIMES
    strcat (helpfilename, extension);
#endif
    strcat (endfilename, extension);
#endif
#endif
}
