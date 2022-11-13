#pragma once

#include "LogUtil.h"
#include "..\Lib\SimpleINI\SimpleIni.h"

namespace INIUtil {
  using std::string;

  class INIManager {
  public:
    INIManager(std::string filePath);
    ~INIManager();

    string Get(string section, string name, string defaultValue);
    int Get(string section, string name, int defaultValue);
    float Get(string section, string name, float defaultValue);
    bool Get(string section, string name, bool defaultValue);

    void Save(bool forceSave = false);

  private:
    bool defaultOptionsSet;
    string filePath;
    CSimpleIniA ini;
  };
}
