#pragma once

#include "LogUtil.h"
#include "..\Lib\SimpleINI\SimpleIni.h"

namespace INIUtil {
  using std::string;

  class INIManager {
  public:
    INIManager(std::string filePath);
    ~INIManager();

    string Get(const string& section, const string& name, const string& defaultValue);
    int Get(const string& section, const string& name, int defaultValue);
    float Get(const string& section, const string& name, float defaultValue);
    bool Get(const string& section, const string& name, bool defaultValue);

    void Save(bool forceSave = false);

  private:
    bool defaultOptionsSet;
    string filePath;
    CSimpleIniA ini;
  };
}
