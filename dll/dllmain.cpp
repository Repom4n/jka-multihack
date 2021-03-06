#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <GL\GL.h>

/// TODO
// add spinbot, nodbot

#include "input.h"

// -----------------
// Init global stuf
// -----------------
#include "globals.h"

// Base address of `jamp.exe`
char* JAMP_BASE = 0;
// offset for the camera angles
unsigned int ANGLE_X_OFFSET = 0x57DF8C;
unsigned int ANGLE_Z_OFFSET = 0x57DF88;
// offset for the enemy in the crosshair
unsigned int CROSSHAIR_ENT_OFFSET = 0x6FC464;

// --------
// Engine structs and types
// --------
DLLENTRY originalDLLEntry;
VMMain originalVMMain;

cgs_t* client_gameState;
cg_t* client_game;
gameState_t* gameState;
playerState_t* ps;

// --------------
// Global helpers
// --------------
centity_t* pPlayerEnt;
centity_t* pCurPushTarget;
centity_t* pCurAimTarget;
std::string curAimTargetName;

centity_t* pEntities[MAX_GENTITIES];
int idxCurEnt;

// Global CVars to control the hack via the in-game console
vmCvar_t WALLHACK;
vmCvar_t CHEATS;
vmCvar_t TRIGGERBOT;
vmCvar_t GLOW;
vmCvar_t ANTIGRIP;
vmCvar_t NOD;
vmCvar_t SPIN;
vmCvar_t ANTITRICK;
vmCvar_t AIM;

// --------
// Hooking
// --------
#include "mhook\mhook-lib\mhook.h"
#include "q3hook.h"

// ----------------------------------
// ----------------------------------

// The main method of the injected DLL
BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        // Initialize global stuff
        JAMP_BASE = (char*)GetModuleHandle(TEXT("jamp.exe"));

        if (!Mhook_SetHook((PVOID*)&originalGetProcAddress, hookGetProcAddress))
        {
            MessageBox(0, "Couldn't create hook", ":(", 0);
        }

        break;
    }
    return TRUE;
}