
#include "LogUtil.h"

#include <fstream>

namespace LogUtil {
  using std::wstring;
  using std::string;

  std::string lastExceptionString(const std::exception_ptr& eptr = std::current_exception())
  {
    if(!eptr) {
      return "Bad Exception";
      //throw std::bad_exception();
    }

    try {
      std::rethrow_exception(eptr);
    } catch(const std::exception& e) {
      return e.what();
    } catch(const std::string& e) {
      return e;
    } catch(const char* e) {
      return e;
    } catch(...) {
      return "Unkown Exception.";
    }
  }

  // will automatically close when program closes
  std::wofstream output(
    Globals::PluginFolder + "\\" + Globals::PluginName + ".log"
  );

  void ToFile(wstring message) {
    output << message << std::endl;
    output.flush();
  }

  void ToScreen(wstring message) {
    ToFile(L"To Screen: " + message);
  }

  void ToFile(string message) {
    ToFile(StringUtil::string_to_wide_string(message));
  }

  void ToScreen(string message) {
    ToScreen(StringUtil::string_to_wide_string(message));
  }

  void ToFile(const char* message) {
    ToFile(string(message));
  }

  void ToScreen(const char* message) {
    ToScreen(string(message));
  }

  void LastExceptionToFile(string source) {
    ToFile(source + " Exception: " + lastExceptionString());
  }

  std::string ErrNoToString(int errorno) {
    char szErrorMsg[256] = { 0 };
    strerror_s(szErrorMsg, errno);
    return std::string(szErrorMsg) + " (" + std::to_string(errorno) + ")";
  }
}
