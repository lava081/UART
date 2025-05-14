#include <iostream>
#include <codecvt>
#include "user_string.h"

uint16_t utf8_utf16BE(char *dist, const char *src)
{
  // 设置 locale 并创建 codecvt 转换器
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> utf8_ucs2_cvt;

  // 将 UTF-8 转换为 UTF-16 的宽字符字符串
  std::u16string utf16_str = utf8_ucs2_cvt.from_bytes(src);

  // 写入 UTF-16 BE 到目标缓冲区
  for (char16_t c : utf16_str)
  {
    // uint16_t u16 = static_cast<uint16_t>(c);
    *dist++ = static_cast<char>((c >> 8) & 0xFF); // 高字节
    *dist++ = static_cast<char>(c & 0xFF);        // 低字节
  }

  // 添加 UTF-16 BE 的空终止符
  *dist++ = 0x00;
  *dist = 0x00;
  return static_cast<uint16_t>(utf16_str.length() * 2); // 返回转换后的长度
}
