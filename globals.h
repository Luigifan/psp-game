/**

defines various global variables to be reused in the lifetime of the game.

*/

#ifndef ___GLOBALS_H___
#define ___GLOBALS_H___

struct _player_t;
struct _camera_t;

#ifndef RGBA
#define RGBA(r,g,b,a) (((r)) | ((g) >> 8) | ((b) >> 16) | ((a) >> 24)) //TODO: is this correct?
#endif

#include <math.h>
#ifndef min
#define min(x, y) (fmin(x, y))
#endif

extern int kQuit;
extern int kLevelEditorMode;
extern int kUpdate; //whether or not to update the game (useful for pause screens or message boxes)
extern int kSkip; //mostly used for the PSP

extern struct _player_t* kPlayer;
//extern struct sprite_t* kTestEntity; //22x29 sprite on a 32x32 sheet
extern struct _sprite_t* kForest;
extern struct _sprite_t* kItems; //contains the sheet for the items
extern struct _sprite_t* kInventory; //contains the inventory sprites
extern struct _sprite_t* kButtons; //contains the button sprites
extern struct _camera_t* kCamera;
//extern struct input_t* kInput;
extern struct input_t* kInputLastFrame;

#ifdef PSP
extern struct OSL_FONT* kMainFont;
#endif

#ifdef SDL_VERS

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Window* kSdlWindow;
extern SDL_Renderer* kSdlRenderer;
extern SDL_Event kSdlEvent;
extern TTF_Font* kSdlFont;

extern float kDelta;

#endif

#endif //___GLOBALS_H___
