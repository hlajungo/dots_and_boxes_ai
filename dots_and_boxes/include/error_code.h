#pragma once
//#include <string>
//#include <unordered_map>
//#include <functional>
//#include <mutex>
#include <magic_enum/magic_enum.hpp>
#include <stdint.h>

enum class ERR : uint32_t
{
  OK = 0,
  UNCATEGORIZE,
  INVALID_POSITION,
  INVAILD_EDGE
};

// 將 enum 轉 string
inline std::string
err2str (ERR e)
{
  // magic_enum::enum_name 回傳 string_view
  auto name = magic_enum::enum_name (e);
  if (name.empty ())
  {
    return "UNKNOWN_ERROR(" + std::to_string (static_cast<uint32_t> (e)) + ")";
  }
  return std::string{ name };
}
