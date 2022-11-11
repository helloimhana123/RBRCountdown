
#pragma once

#include "..\stdafx.h"

namespace StringUtil {
  std::wstring string_to_wide_string(const std::string& string);
  std::string wide_string_to_string(const std::wstring& wide_string);
}
