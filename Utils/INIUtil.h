#pragma once

#include "..\stdafx.h"
#include "LogUtil.h"
#include "..\Lib\SimpleINI\SimpleIni.h"

namespace INIUtil {
  using std::string;

  class INIManager {
  public:
    INIManager(std::string filePath);
    ~INIManager();

    string Get(string section, string name, string default);
    int Get(string section, string name, int default);
    float Get(string section, string name, float default);
    bool Get(string section, string name, bool default);

    void Save();

  private:
    string filePath;
    CSimpleIniA ini;
  };
}
