#include <csignal>
#include <cstdint>

#ifdef _WIN32
#include <winsock2.h> // timeval
#else
#include <termios.h>
#include <unistd.h>
#include <sys/time.h> //timeval

void sigwinchHandler(int32_t sig)
{
  signal(SIGWINCH, sigwinchHandler);

  if (sig)
  {
    Event e;
    e.eType = Event::EVENT_TYPE::RESIZE;
    receiver->postEvent(e);
  }
}

// while this does work on windows, it doesn't play nice
int32_t nonBlockInput()
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

// convenience wrapper
int32_t getKeyboardInput()
{
  int32_t ret = -1;
  if (nonBlockInput())
    ret = getchar(); // getchar is shorthand for fgetc(stdin)

  if (ret != EOF)
    return ret;
  return -1;
}

#endif
