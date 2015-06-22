#include "console.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h> // console size stuff
#endif

void clearConsole()
{
#ifdef _WIN32
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO cinfo;
  GetConsoleScreenBufferInfo(handle, &cinfo);
  COORD c = {0, 0};
  ConsoleVec s = getConsoleSize();
  DWORD junk; // junk because I don't need this, but FillConsoleOutputCharacter fails if you pass a null pointer
  FillConsoleOutputCharacter(handle, ' ', s.x * s.y, c, &junk);

  FillConsoleOutputAttribute(handle, cinfo.wAttributes, s.x * s.y, c, &junk); // not sure why this is here fully, but it was on MSDN
#else
  std::cout << "\033[2J"; // clear
#endif
  setCursorPosition(0, 0);
}

TColour operator|(TColour c1, TColour c2)
{
  return TColour(uint8_t(c1)|uint8_t(c2)); // cast then cast back, otherwise stack overflow
  // bit ugly, but oh well, I guess that's the cost of over engineering for prettiness
}

/*
std::ostream& operator<<(std::ostream& stream, TColour col)
{
#ifdef _WIN32
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, WORD(col));
  return stream;
#else
  uint8_t fg = (col & 0x0F);
  uint8_t bg = col >= BG_RED ? (col & 0xF0) / 16 : 0;

  return stream << "\033[" << (fg + 30) << ";" << (bg + 40) << *//*(col & FG_INTENSE ? ";1" : ";24") <<*//* "m";
#endif
  return stream;
}
*/

void setColour(TColour col)
{
#ifdef _WIN32
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, WORD(col));
#else
  uint8_t fg = (col & 0x0F);
  uint8_t bg = col >= BG_RED ? (col & 0xF0) / 16 : 0;

  std::cout << "\033[" << (fg + 30) << ";" << (bg + 40) << /*(col & FG_INTENSE ? ";1" : ";24") <<*/ "m";
#endif
}


ConsoleVec getConsoleSize()
{
  ConsoleVec size;

#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO cinfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cinfo);
  size.x = cinfo.srWindow.Right - cinfo.srWindow.Left + 1; // ugly hack: reports 1 less than actual value
  size.y = cinfo.srWindow.Bottom - cinfo.srWindow.Top + 1; // ditto
#else
  struct winsize ws;
  ioctl(0, TIOCGWINSZ, &ws);

  size.x = ws.ws_col;
  size.y = ws.ws_row;
#endif
  return size;
}

void setConsoleSize(const ConsoleVec& size)
{
  setConsoleSize(size.x, size.y);
}

void setConsoleSize(const uint16_t w /* = 80 */, const uint16_t h /* = 24*/)
{
#ifdef _WIN32
  const COORD c = {static_cast<int16_t>(w) , static_cast<int16_t>(h)}; //change the values
  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
  struct winsize ws;
  ws.ws_row = h;
  ws.ws_col = w;
  ioctl(0, TIOCSWINSZ, &ws);
#endif
}

void setCursorPosition(const ConsoleVec& pos)
{
  setCursorPosition(pos.x, pos.y);
}

void setCursorPosition(const int16_t& x, const int16_t& y)
{
#ifdef _WIN32
  COORD c = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
  std::cout << "\033[" << y+1 << ";" << x+1 << "H";
#endif
}

void output(const std::string& str, const int16_t x /* = -1 */, const int16_t y /*= -1*/, const TColour c /* = -1 */)
{
  output(str.c_str(), x, y, c);
}

void output(const char* const str, const int16_t x /* = -1 */, const int16_t y /*= -1*/, const TColour c /* = -1 */)
{
  if (c >= 0)
    setColour(c);
  if (x >= 0 && y >= 0)
    setCursorPosition(x, y);
  std::cout << str;
}
