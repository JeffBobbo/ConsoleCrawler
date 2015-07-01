#include "event.h"

#include <iostream>

Receiver* receiver = nullptr;

void setupEvents();
void closeEvents();

Event::Event()
{
  this->eType = EVENT_TYPE::UNKNOWN;
  this->eKey = -1;
  this->eKeyState = false;
}

Event::~Event()
{
}

Receiver::Receiver()
{
  setupEvents();
}

Receiver::Receiver(std::function<bool(Event&)> eventcb)
{
  setupEvents();
  if (eventcb)
    this->process = eventcb;
}

Receiver::~Receiver()
{
  closeEvents();
}

bool Receiver::postEvent(Event& event)
{
  this->events.push_front(event);
  return true;
}


bool Receiver::onEvent()
{
  while (!this->events.empty())
  {
    Event event = this->events.front();

    if (this->process)
    {
      if (this->process(event) == false)
        return false;
    }
    else
    {
      std::cerr << "poop" << std::flush;
    }

    this->events.pop_front();
  }
  return true;
}

#include "input.cpp" // note, changes here means need to make clean

#ifdef _WIN32
#include <windows.h>
#endif
bool Receiver::pollEvents()
{
#ifdef _WIN32
  INPUT_RECORD ibuf[128];
  DWORD n;
  if (ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ibuf, 128, &n))
  {
    for (uint32_t i = 0; i < n; ++i)
    {
      Event e;
      bool caught = true;
      switch (ibuf[i].EventType)
      {
        case WINDOW_BUFFER_SIZE_EVENT:
          e.eType = Event::EVENT_TYPE::RESIZE;
        break;

        case KEY_EVENT: // keyboard input
          e.eType = Event::EVENT_TYPE::KEYBOARD;
//          int32_t key = ibuf[i].Event.KeyEvent.wVirtualKeyCode;
          e.eKey = ibuf[i].Event.KeyEvent.wVirtualKeyCode;
          e.eKeyState = ibuf[i].Event.KeyEvent.bKeyDown;
        break;

        default:
          caught = false;
        break;
      }
      if (caught)
        postEvent(e);
    }
  }
#else
  int32_t key = getKeyboardInput();
  if (key >= 0)
  {
    Event e;
    e.eType = Event::EVENT_TYPE::KEYBOARD;
    if (key == Event::KEY::ESC)
    {
      int32_t bracket = getKeyboardInput(); // get the [
      if (bracket == Event::KEY::L_SQ_BRACKET) // if it is [, get the actual key
      {
        key = getKeyboardInput();

      }
      else if (bracket >= 0)
      {
        key = bracket; // there's a character, so push that in
      }
    }
    e.eKey = key;
    e.eKeyState = true; // linux doesn't emit key up events
    postEvent(e);
  }
#endif
//#endif
  return this->onEvent();
}

#ifdef _WIN32
DWORD cMode; // storage for terminal settings to restore after execution
#else
struct termios oldT; // storage for terminal settings to restore after execution
struct termios newT;
#endif
void setupEvents()
{
#ifdef _WIN32
  HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(h, &cMode); // save old mode
  SetConsoleMode(h, ENABLE_WINDOW_INPUT); // set new mode
#else
  tcgetattr(fileno(stdin), &oldT); // store the 'current' terminal settings for when we exit
  tcgetattr(fileno(stdin), &newT); // advised thing is to get stuff, make required changes only
  cfmakeraw(&newT); // convenience function provided by termios.h to set a bunch of modes
  tcsetattr(fileno(stdin), TCSANOW, &newT); // do change immediately

  sigwinchHandler(0);
#endif
}

void closeEvents()
{
#ifdef _WIN32
  HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
  SetConsoleMode(h, cMode); // restore
#else
  tcsetattr(fileno(stdin), TCSANOW, &oldT); // restore the old settings for exit
#endif
}
