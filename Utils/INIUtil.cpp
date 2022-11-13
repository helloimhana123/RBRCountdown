
#include "INIUtil.h"

#include <fstream>

namespace INIUtil {
  using std::string;

  INIManager::INIManager(std::string _filePath) : filePath(_filePath) {
    {
      // create if doesn't exist
      std::ofstream file(filePath, std::ios_base::app);
      file << "";
    }

    defaultOptionsSet = false;
    ini.SetUnicode();
    SI_Error iniResult = ini.LoadFile(filePath.c_str());

    if(iniResult != SI_OK) {
      LogUtil::ToFile("Error loading ini from: " + filePath);
      LogUtil::ToFile("Error loading ini with error: " + std::to_string(iniResult));
      if(iniResult == SI_FILE) {
        LogUtil::ToFile("Errno: " + LogUtil::ErrNoToString(errno));
      }
      return;
    }
  }

  INIManager::~INIManager() {
  }

  string INIManager::Get(const string& section, const string& name, const string& defaultValue) {
    string value = string(ini.GetValue(section.c_str(), name.c_str(), defaultValue.c_str()));

    // Set value to apply defaults and flag "ini file content modified" status if the default was inserted as a new option
    if(ini.SetValue(section.c_str(), name.c_str(), value.c_str()) == SI_INSERTED)
    {
      defaultOptionsSet = true;
    }

    return value;
  }

  int INIManager::Get(const string& section, const string& name, int defaultValue) {
    string value = Get(section, name, std::to_string(defaultValue));
    int intValue = defaultValue;
    try {
      intValue = std::stoi(value);
    } catch(...) {
      LogUtil::LastExceptionToFile(
        "Failed getting INI value for section: " + section +
        ", name: " + name
      );
    }
    return intValue;
  }

  float INIManager::Get(const string& section, const string& name, float defaultValue) {
    string value = Get(section, name, std::to_string(defaultValue));
    float floatValue = defaultValue;
    try {
      // Decimal separator always as '.' in the source string representing a float value regardless of the default PC localizaation
      _locale_t convLocaleUS = _create_locale(LC_NUMERIC, "en-US");
      floatValue = static_cast<float>(_atof_l(value.c_str(), convLocaleUS));
    } catch(...) {
      LogUtil::LastExceptionToFile(
        "Failed getting INI value for section: " + section +
        ", name: " + name
      );
    }
    return floatValue;
  }

  bool INIManager::Get(const string& section, const string& name, bool defaultValue) {
    string defaultString = defaultValue ? "True" : "False";
    string value = Get(section, name, defaultString);
    bool boolValue = defaultValue;

    if(_strnicmp(value.c_str(), "true", 4) == 0 || value == "1") {
      boolValue = true;
    } else if(_strnicmp(value.c_str(), "false", 5) == 0 || value == "0") {
      boolValue = false;
    } else {
      LogUtil::ToFile("Error getting bool for section: " +
        section + ", name: " + name + ". Got invalid value: " + value
      );
    }

    return boolValue;
  }

  void INIManager::Save(bool forceSave) {
    if(defaultOptionsSet || forceSave) {
      SI_Error saveResult = ini.SaveFile(filePath.c_str());

      if(saveResult != SI_OK) {
        LogUtil::ToFile("Error saving ini to: " + filePath);
        LogUtil::ToFile("Error saving ini with error: " + std::to_string(saveResult));
        if(saveResult == SI_FILE) {
          LogUtil::ToFile("Error reason: " + LogUtil::ErrNoToString(errno));
        }
      }
    }
  }
}
