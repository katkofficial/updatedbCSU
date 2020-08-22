#pragma once

#include <string>

std::string toANSI(const std::wstring& unicodeString);

std::wstring toOEM866(const std::string& string);