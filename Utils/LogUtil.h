#pragma once

#include "..\stdafx.h"

#include "..\Globals.h"
#include "StringUtil.h"

namespace LogUtil {
  using std::wstring;
  using std::string;

  void ToFile(wstring message);
  void ToScreen(wstring message);
  void ToFile(string message);
  void ToScreen(string message);
  void ToFile(const char* message);
  void ToScreen(const char* message);
  void LastExceptionToFile(string source);
}
