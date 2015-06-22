#ifndef RENDER_H_INCLUDE
#define RENDER_H_INCLUDE

#ifdef _WIN32
#else
#endif

#include <iostream>
#include <cstdint>

// some reference information
// http://en.wikipedia.org/wiki/ANSI_escape_code#CSI_codes


struct ConsoleVec
{
  uint16_t x;
  uint16_t y;
  ConsoleVec() { x = y = uint16_t(0); };
  ConsoleVec(uint16_t a, uint16_t b) { x = a; y = b; };
  ConsoleVec(const ConsoleVec& o) { x = o.x; y = o.y; };
};

void clearConsole();

enum TColour
{
  NONE = -0x01,
#ifdef _WIN32
// windows colours
  FG_BLUE   = 0x01,
  FG_GREEN  = 0x02,
  FG_RED    = 0x04,
  BG_BLUE   = 0x10,
  BG_GREEN  = 0x20,
  BG_RED    = 0x40,
  FG_DEFAULT = 0x00,
  BG_DEFAULT = 0x0F,
#else
// linux colours
  FG_RED     = 0x01,
  FG_GREEN   = 0x02,
  FG_BLUE    = 0x04,

  BG_RED     = 0x10,
  BG_GREEN   = 0x20,
  BG_BLUE    = 0x40,

  FG_DEFAULT = 0x09,
  BG_DEFAULT = 0x90,
#endif
  //FG_INTENSE= 0x08,
  //BG_INTENSE= 0x80,
  // convenience
  FG_CYAN    = FG_BLUE|FG_GREEN,
  FG_YELLOW  = FG_GREEN|FG_RED,
  FG_MAGENTA = FG_BLUE|FG_RED,
  FG_WHITE   = FG_BLUE|FG_GREEN|FG_RED,

  BG_CYAN    = BG_BLUE|BG_GREEN,
  BG_YELLOW  = BG_GREEN|BG_RED,
  BG_MAGENTA = BG_BLUE|BG_RED,
  BG_WHITE   = BG_BLUE|BG_GREEN|BG_RED
};

TColour operator|(TColour c1, TColour c2);
//std::ostream& operator<<(std::ostream& stream, TColour col);

void setColour(TColour col);

ConsoleVec getConsoleSize();

// these don't work, poopy
void setConsoleSize(const ConsoleVec& size);
void setConsoleSize(const uint16_t w = 80, const uint16_t h = 24);

void setCursorPosition(const ConsoleVec& pos);
void setCursorPosition(const int16_t& x, const int16_t& y);

void output(const std::string& str, const int16_t x = -1, const int16_t y = -1, const TColour c = NONE);
void output(const char* const str, const int16_t x = -1, const int16_t y = -1, const TColour c = NONE);

#endif
