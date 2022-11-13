
#pragma once

#include "Globals.h"
#include "RBR/RBR.h"
#include "Lib/Detourxs/detourxs.h"
#include "Countdown.h"

tRBRDirectXEndScene Func_OrigRBRDirectXEndScene = nullptr;

HRESULT __fastcall CustomRBRDirectXEndScene(void* objPointer) {
  // We need code at the beginning of this function that is not a call to another function.
  // If we just added "Countdown::DrawCountdown();" then hooking EndScene from another 
  // plugin will fail. This is because of some technicality of how the hooking works.

  // If we are not in the car, do nothing.
  if(g_pRBRGameMode->gameMode != 0x01) { return ::Func_OrigRBRDirectXEndScene(objPointer); }
  float countdown = g_pRBRCarInfo->stageStartCountdown;
  if(countdown > 5 || countdown < -1) { return ::Func_OrigRBRDirectXEndScene(objPointer); }

  Countdown::DrawCountdown();

  return ::Func_OrigRBRDirectXEndScene(objPointer);
}

class Plugin : public IPlugin {
private:
  IRBRGame* m_pGame;

public:
  Plugin(IRBRGame* pGame) : m_pGame(pGame) {
    LogUtil::ToFile("Creating Plugin " + Globals::PluginName + ".");

    if(CreateDirectory(Globals::PluginFolder.c_str(), NULL) ||
      ERROR_ALREADY_EXISTS == GetLastError()
      ) {
      // success
    } else {
      LogUtil::ToFile("Failed creating plugin folder.");
    }
  }

  virtual ~Plugin(void) {
    LogUtil::ToFile("Destroying Plugin " + Globals::PluginName + ".");
  }

  virtual const char* GetName(void) {
    if(Func_OrigRBRDirectXEndScene == nullptr) {
      // Do the initialization and texture creation only once because RBR may call GetName several times
      LogUtil::ToFile("Initializing the plugin");
      RBRAPI_InitializeObjReferences();
      Countdown::InitCountdown();

      auto gtcDirect3DEndScene = new DetourXS((LPVOID)0x0040E890, ::CustomRBRDirectXEndScene, TRUE);
      Func_OrigRBRDirectXEndScene = (tRBRDirectXEndScene)gtcDirect3DEndScene->GetTrampoline();
    }

    return Globals::PluginName.c_str();
  }

  virtual void DrawResultsUI(void) {
    // Do nothing
  }

  virtual void DrawFrontEndPage(void) {
    // Do nothing
  }

  virtual void HandleFrontEndEvents(char txtKeyboard, bool bUp, bool bDown, bool bLeft, bool bRight, bool bSelect) {
    // Do nothing
  }

  virtual void TickFrontEndPage(float fTimeDelta) {
    // Do nothing
  }

  // Is called when the player timer starts (after GO! or in case of a false start)
  virtual void StageStarted(int iMap, const char* ptxtPlayerName, bool bWasFalseStart) {
    // Do nothing
  }

  // Is called when player finishes stage (fFinishTime is 0.0f if player failed the stage)
  virtual void HandleResults(
    float fCheckPoint1, float fCheckPoint2, float fFinishTime, const char* ptxtPlayerName
  ) {
    // Do nothing
  }

  // Is called when a player passed a checkpoint 
  virtual void CheckPoint(float fCheckPointTime, int iCheckPointID, const char* ptxtPlayerName) {
    // Do nothing
  }
};
