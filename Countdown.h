#pragma once

#include "stdafx.h"
#include "Globals.h"
#include "RBR/RBR.h"
#include "RBR/D3D9Helpers.h"
#include "Utils/LogUtil.h"
#include "Utils/StringUtil.h"
#include "Utils/INIUtil.h"

namespace Countdown {
  IMAGE_TEXTURE countdownTex[6];
  IMAGE_TEXTURE dot_tex;

  int countdownNumberTexSize = 256;

  INIUtil::INIManager ini(Globals::PluginFolder + "\\RBRCountdown.ini");
  bool iniCentered = true;
  int iniX = 0;
  int iniY = 0;
  float iniScale = 1.0f;

  void DrawCountdownNumbers(float countdown, int centerX, int centerY) {
    int i = (int)std::ceil(countdown);
    i = std::clamp(i, 0, 5);
    IMAGE_TEXTURE tex = countdownTex[i];

    float multiplier = 1 - std::abs(countdown - std::floor(countdown));
    //int size = 256 + (int)(multiplier * 100);
    int size = (int)std::round(countdownNumberTexSize * iniScale);

    int x = centerX - size / 2;
    int y = centerY - size;

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
    int size = (int)std::round(16 * iniScale);
    int dotDistance = (int)std::round(10 * iniScale);
    int y = (int)std::round(50 * iniScale);

    for(int i = 0; i < dots; i++) {
      int x = i * (size + dotDistance);

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
    float countdown = g_pRBRCarInfo->stageStartCountdown;
    int centerX = 0;
    int centerY = 0;

    if(iniCentered) {
      RBRAPI_MapRBRPointToScreenPoint(320, 240, &centerX, &centerY);
    }

    centerX += iniX;
    centerY += iniY;

    // Get this number by taking the font height (=388) divide
    // by 2 (=194) then divide by image height (=194/1024=0.189453125)
    // This will center the text on screen.
    centerY += 0.1894531f * countdownNumberTexSize * iniScale;

    DrawCountdownNumbers(countdown, centerX, centerY);
    DrawCountdownDots(countdown, centerX, centerY);
  }

  void LoadINI() {
    try {
      iniCentered = ini.Get("Settings", "Centered", true);
      iniX = ini.Get("Settings", "XOffset", 0);
      iniY = ini.Get("Settings", "YOffset", 0);
      {
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        RBRAPI_MapRBRPointToScreenPoint(0, 0, &x1, &y1);
        RBRAPI_MapRBRPointToScreenPoint(640, 480, &x2, &y2);

        // 1440 pixels is the size when screen size is 1920
        iniScale = (float)std::abs(x2 - x1) / 1440.0f;
        iniScale *= ini.Get("Settings", "Scale", 1.0f);
      }

      ini.Save();
    } catch(...) {
      LogUtil::LastExceptionToFile("Failed loading INI.");
    }
  }

  void InitCountdown() {
    LoadINI();

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
