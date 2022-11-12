
#pragma once

#include "Globals.h"
#include "RBR/RBR.h"
#include "Lib/Detourxs/detourxs.h"
#include "Countdown.h"

//tRBRDirectXBeginScene Func_OrigRBRDirectXBeginScene = nullptr;  // Re-routed built-in DX9 RBR function pointers
tRBRDirectXEndScene Func_OrigRBRDirectXEndScene = nullptr;

//HRESULT __fastcall CustomRBRDirectXBeginScene(void* objPointer)
//{
//  // Call the origial RBR BeginScene and let it to initialize the new D3D scene
//  HRESULT hResult = ::Func_OrigRBRDirectXBeginScene(objPointer);
//
//  return hResult;
//}

HRESULT __fastcall CustomRBRDirectXEndScene(void* objPointer) {
  Countdown::DrawCountdown();

  return ::Func_OrigRBRDirectXEndScene(objPointer);
}

class Plugin : public IPlugin {
private:
  IRBRGame* m_pGame;

public:
  Plugin(IRBRGame* pGame)
    : m_pGame(pGame)
  {
    LogUtil::ToFile("Creating Plugin " + Globals::PluginName + ".");
  }

  virtual ~Plugin(void) {
    LogUtil::ToFile("Destroying Plugin " + Globals::PluginName + ".");
  }

  virtual const char* GetName(void) {
    LogUtil::ToFile("GetName");

    RBRAPI_InitializeObjReferences();

    Countdown::InitCountdown();

    //auto gtcDirect3DBeginScene = new DetourXS((LPVOID)0x0040E880, ::CustomRBRDirectXBeginScene, TRUE);
    //Func_OrigRBRDirectXBeginScene = (tRBRDirectXBeginScene)gtcDirect3DBeginScene->GetTrampoline();

    if(Func_OrigRBRDirectXEndScene == nullptr) {
      auto gtcDirect3DEndScene = new DetourXS((LPVOID)0x0040E890, ::CustomRBRDirectXEndScene, TRUE);
      Func_OrigRBRDirectXEndScene = (tRBRDirectXEndScene)gtcDirect3DEndScene->GetTrampoline();
    }

    return Globals::PluginName.c_str();
  }

  //------------------------------------------------------------------------------------------------//
  virtual void DrawResultsUI(void) {
  }

  //------------------------------------------------------------------------------------------------//
  virtual void DrawFrontEndPage(void) {
  }

  //------------------------------------------------------------------------------------------------//
  virtual void HandleFrontEndEvents(char txtKeyboard, bool bUp, bool bDown, bool bLeft, bool bRight, bool bSelect) {
  }

  //------------------------------------------------------------------------------------------------//
  virtual void TickFrontEndPage(float fTimeDelta) {
    // Do nothing
  }

  //------------------------------------------------------------------------------------------------//
  /// Is called when the player timer starts (after GO! or in case of a false start)
  virtual void StageStarted(int iMap, const char* ptxtPlayerName, bool bWasFalseStart)
  {
    // Do nothing
  }

  //------------------------------------------------------------------------------------------------//
  /// Is called when player finishes stage ( fFinishTime is 0.0f if player failed the stage )
  virtual void HandleResults(float fCheckPoint1, float fCheckPoint2, float fFinishTime, const char* ptxtPlayerName)
  {
    // Do nothing
  }

  //------------------------------------------------------------------------------------------------//
  // Is called when a player passed a checkpoint 
  virtual void CheckPoint(float fCheckPointTime, int iCheckPointID, const char* ptxtPlayerName)
  {
    // Do nothing
  }
};
