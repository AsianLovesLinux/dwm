#include <X11/XF86keysym.h>
static const unsigned int borderpx = 4;
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "AdwaitaMono Nerd Font:size=16" };
static const char dmenufont[]       = "AdwaitaMono Nerd Font:size=16";
static const char col_gray1[]       = "#2e3440";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#d8dee9";
static const char col_gray4[]       = "#d8dee9";
static const char col_cyan[]        = "#81a1c1";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, "#5e81ac",  col_cyan  },
};
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *roficmd[] = { "rofi", "-show", "drun", NULL }; 
static const char *wallpaper[] = { "rofi", "-show", "wallpaper", "-modi", "'wallpaper:~/.config/rofi/wallpaper.sh'", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *bravecmd[] = { "brave-browser-stable", NULL };
static const char *volup[]   = { "wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "0.1+", NULL };
static const char *voldown[] = { "wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "0.1-", NULL };
static const char *volmute[] = { "wpctl", "set-mute",   "@DEFAULT_AUDIO_SINK@", "toggle", NULL };
static const char *micmute[] = { "wpctl", "set-mute",   "@DEFAULT_AUDIO_SOURCE@", "toggle", NULL };
static const char *exitrofi[] = { "bash", "/home/ryan/.config/rofi/exit-menu.sh", NULL };
static const char *flameshot[] = { "flameshot", "gui", NULL };
static const char *files[] = { "nemo", NULL };
static const Key keys[] = {
	/* logout menu */
	{ MODKEY|ShiftMask, XK_e, spawn, {.v = exitrofi } },
	/* push down/up */
	{ MODKEY|ShiftMask,           XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,           XK_k,      pushup,         {0} },
	/* Wallpaper switcher */
	{ Mod1Mask,                       XK_w,      spawn,          {.v = wallpaper } },
	/* volume adjustment binds I use for my Aula F75 */	
	{ 0, XF86XK_AudioRaiseVolume, spawn, {.v = volup } },
	{ 0, XF86XK_AudioLowerVolume, spawn, {.v = voldown } },
	{ 0, XF86XK_AudioMute,        spawn, {.v = volmute } },
	{ 0, XF86XK_AudioMicMute,     spawn, {.v = micmute } },
	/* My binds for spawning apps */	
	{ MODKEY,                       XK_p,      spawn,          {.v = flameshot } },
	{ MODKEY,                       XK_w,      spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = bravecmd } },
	{ MODKEY,             		XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,			XK_y,	   spawn,	   {.v = files } },	
	/* toggling the dwm bar */	
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	/* moving between windows */	
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	/* I belive this is removing and adding master windows */	
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	/* increasing and decreasing the size of master windows */	
	{ MODKEY,                       XK_g,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	/* essientially swapping the master window with the window you are focused on */	
	{ MODKEY,                       XK_z, 	   zoom,           {0} },
	/* vanitygaps gap binds */	
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	/* Killing the focused window and whatever view is */	
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             		XK_q,      killclient,     {0} },	
	/* layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // Tiled
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} }, // Monocle
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[2]} }, // Spiral
	{ MODKEY,                       XK_d,      setlayout,      {.v = &layouts[3]} }, // Dwindle
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[7]} }, // grid 
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[8]} }, // nrowgrid
	{ MODKEY,                       XK_v,      setlayout,      {.v = &layouts[9]} }, // horizgrid
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[10]} }, // gapplessgrid
	{ MODKEY,                       XK_x,      setlayout,      {.v = &layouts[13]} }, // floating windows 
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} }, // Floating as you can tell
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	/* Monitor focus and moving windows between monitors (default DWM bindings) */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	/* moving between "TAGS" or workspaces basically */	
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* Killing dwm */	
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

