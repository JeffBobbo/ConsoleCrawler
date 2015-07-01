#ifndef EVENT_H_INCLUDE
#define EVENT_H_INCLUDE

#include <list>
#include <functional>

class Event
{
public:
  enum EVENT_TYPE
  {
    UNKNOWN = -1,
    RESIZE,
    KEYBOARD
  };

  enum KEY
  {
    ESC = 27,
#ifdef _WIN32
    L_ARROW = 37,
    U_ARROW,
    R_ARROW,
    D_ARROW,
    L_SQ_BRACKET = 219,
    R_SQ_BRACKET = 221,
#else
    U_ARROW = 65,
    D_ARROW,
    R_ARROW,
    L_ARROW,
    L_SQ_BRACKET = 91,
    R_SQ_BRACKET = 93,
#endif
    K_0 = 48,
    K_1,
    K_2,
    K_3,
    K_4,
    K_5,
    K_6,
    K_7,
    K_8,
    K_9,
    A = 65,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

  };


  Event();
  virtual ~Event();

  EVENT_TYPE eType;
  int32_t eKey;
  bool eKeyState : 1;
};

class Receiver
{
public:
  Receiver();
  Receiver(std::function<bool(Event&)> eventcb);
  virtual ~Receiver();

  virtual bool postEvent(Event& event);
  virtual bool onEvent();
  virtual bool pollEvents(); // collects events from other sources, then calls OnEvent
private:
  std::list<Event > events;
  std::function<bool(Event&)> process;
};

extern Receiver* receiver;
#endif
