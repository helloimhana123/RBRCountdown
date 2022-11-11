
#include "INIUtil.h"

namespace INIUtil {
  using std::string;

  INIManager::INIManager(std::string _filePath) : filePath(_filePath) {
    {
      // create if doesn't exist
      std::ofstream file(filePath, std::ios_base::app);
      file << "";
    }

    ini.SetUnicode();
    SI_Error iniResult = ini.LoadFile(filePath.c_str());

    if(iniResult != SI_OK) {
      LogUtil::ToFile("Error loading ini from: " + filePath);
      LogUtil::ToFile("Error loading ini with error: " + std::to_string(iniResult));
      if(iniResult == SI_FILE) {
        LogUtil::ToFile("Errno: " + std::string(std::strerror(errno)));
      }
      return;
    }
  }

  INIManager::~INIManager() {
  }

  string INIManager::Get(string section, string name, string default) {
    string value = string(ini.GetValue(section.c_str(), name.c_str(), default.c_str()));
    // Set value to apply defaults
    ini.SetValue(section.c_str(), name.c_str(), value.c_str());
    return value;
  }

  int INIManager::Get(string section, string name, int default) {
    string value = Get(section, name, std::to_string(default));
    int intValue = default;
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

  bool INIManager::Get(string section, string name, bool default) {
    string defaultString = default ? "True" : "False";
    string value = Get(section, name, defaultString);
    bool boolValue = default;

    if(value == "True") {
      boolValue = true;
    } else if(value == "False") {
      boolValue = false;
    } else {
      LogUtil::ToFile("Error getting bool for section: " +
        section + ", name: " + name + ". Got invalid value: " + value
      );
    }

    return boolValue;
  }

  void INIManager::Save() {
    SI_Error saveResult = ini.SaveFile(filePath.c_str());

    if(saveResult != SI_OK) {
      LogUtil::ToFile("Error saving ini to: " + filePath);
      LogUtil::ToFile("Error saving ini with error: " + std::to_string(saveResult));
      if(saveResult == SI_FILE) {
        LogUtil::ToFile("Error reason: " + std::string(std::strerror(errno)));
      }
    }
  }
}
