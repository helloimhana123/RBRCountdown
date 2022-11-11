#pragma once

#include "stdafx.h"
#include "Globals.h"
#include "RBR/RBR.h"
#include "RBR/D3D9Helpers.h"
#include "Utils/LogUtil.h"
#include "Utils/StringUtil.h"

namespace Countdown {
  IMAGE_TEXTURE countdownTex[6];
  IMAGE_TEXTURE dot_tex;
  int countdownNumberTexSize = 256;

  void DrawCountdownNumbers(float countdown, int centerX, int centerY) {
    int i = (int)std::ceil(countdown);
    i = std::clamp(i, 0, 5);
    IMAGE_TEXTURE tex = countdownTex[i];

    float multiplier = 1 - std::abs(countdown - std::floor(countdown));
    //int size = 256 + (int)(multiplier * 100);
    int size = countdownNumberTexSize;

    int x = centerX - size / 2;
    int y = centerY - size / 2;

    HRESULT hResult = D3D9CreateVertexesForTex(
      &tex, (float)x, (float)y, (float)size, (float)size,
      0
    );

    if(SUCCEEDED(hResult)) {
      D3D9DrawVertexTex2D(g_pRBRIDirect3DDevice9, tex.pTexture, tex.vertexes2D);
    } else {
      LogUtil::ToFile("Failed creating vertexes.");
    }
  }

  void DrawCountdownDots(float countdown, int centerX, int centerY) {
    if(countdown < 0) { return; }

    IMAGE_TEXTURE tex = dot_tex;

    float decimal = std::abs(countdown - std::floor(countdown));
    int dots = (int)std::floor(decimal * 10); // floor
    int size = 16;
    int y = countdownNumberTexSize / 2 + 50;

    for(int i = 0; i < dots; i++) {
      int x = i * (size + 10);

      HRESULT hResult = D3D9CreateVertexesForTex(
        &tex, (float)(centerX + x - size / 2), (float)(centerY + y), (float)size, (float)size,
        0
      );

      if(SUCCEEDED(hResult)) {
        D3D9DrawVertexTex2D(g_pRBRIDirect3DDevice9, tex.pTexture, tex.vertexes2D);
      } else {
        LogUtil::ToFile("Failed creating vertexes for dots.");
      }

      hResult = D3D9CreateVertexesForTex(
        &tex, (float)(centerX - x - size / 2), (float)(centerY + y), (float)size, (float)size,
        0
      );

      if(SUCCEEDED(hResult)) {
        D3D9DrawVertexTex2D(g_pRBRIDirect3DDevice9, tex.pTexture, tex.vertexes2D);
      } else {
        LogUtil::ToFile("Failed creating vertexes for dots.");
      }
    }
  }

  void DrawCountdown() {
    // If we are not in the car, do nothing.
    if(g_pRBRGameMode->gameMode != 0x01) { return; }

    float countdown = g_pRBRCarInfo->stageStartCountdown;

    if(countdown > 5 || countdown < -1) { return; }

    int centerX = 0;
    int centerY = 0;
    RBRAPI_MapRBRPointToScreenPoint(320, 240, &centerX, &centerY);

    DrawCountdownNumbers(countdown, centerX, centerY);
    DrawCountdownDots(countdown, centerX, centerY);
  }

  void InitCountdown() {
    std::wstring countdownTexPath = StringUtil::string_to_wide_string(Globals::PluginFolder);

    for(int i = 0; i < 6; i++) {
      std::wstring numberStr = std::to_wstring(i);
      std::wstring path = countdownTexPath + L"\\countdown_000" + numberStr + L"_" + numberStr + L".png";

      HRESULT hResult =
        D3D9CreateRectangleVertexTexBufferFromFile(g_pRBRIDirect3DDevice9,
          path,
          0, 0, 0, 0,
          &countdownTex[i],
          0);

      if(SUCCEEDED(hResult)) {
        LogUtil::ToFile(L"Success loading TEX: " + path);
      } else {
        LogUtil::ToFile(L"Failed loading TEX: " + path);
      }
    }

    {
      std::wstring path = countdownTexPath + L"\\countdown_dot.png";

      HRESULT hResult =
        D3D9CreateRectangleVertexTexBufferFromFile(g_pRBRIDirect3DDevice9,
          path,
          0, 0, 0, 0,
          &dot_tex,
          0);

      if(SUCCEEDED(hResult)) {
        LogUtil::ToFile(L"Success loading TEX: " + path);
      } else {
        LogUtil::ToFile(L"Failed loading TEX: " + path);
      }
    }
  }
}
