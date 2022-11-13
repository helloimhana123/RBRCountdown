#pragma once

#include <string>
#include "..\Globals.h"
#include "StringUtil.h"

namespace LogUtil {
  using std::wstring;
  using std::string;

  void ToFile(const wstring& message);
  void ToScreen(const wstring& message);
  void ToFile(const string& message);
  void ToScreen(const string& message);
  void ToFile(const char* message);
  void ToScreen(const char* message);
  void LastExceptionToFile(const string& source);
  std::string ErrNoToString(int errorno);
}
