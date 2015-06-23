#include <iostream>
#include <cstdint>
#include <sstream>

#include "soul.h"
#include "player.h"
#include "console.h"
#include "misc.h"

bool cleanName(std::string str)
{
  const char* filter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890";
  return str.find_first_not_of(filter) == std::string::npos;
}

enum OPTIONS
{
  PROG = 1,
  HEAL
};

void presentOptions(Player* p, Soul* s)
{
  output(p->getName() + "'s status:", 0, 0);
  output(std::string("Level: ") + toString(p->getLevel()), 0, 1);
  output(std::string("Atk: ") + toString(p->getAtk()) + ", Str: " + toString(p->getStr()) + ", Def: " + toString(p->getDef()), 0, 2);
  output(std::string("HP: ") + toString(p->getHP()) + "/" + toString(p->getHPMax()) + " (" + toString(static_cast<int16_t>(p->getHPPerc() * 100.0)) + "%)", 0, 3, (p->getHPPerc() >= 0.7 ? FG_GREEN : p->getHPPerc() >= 0.3 ? FG_YELLOW : FG_RED));

  if (s)
  {
    uint16_t x = getConsoleSize().x >> 1;
    output("Gremlin's status:", x, 0);
    output(std::string("Level: ") + toString(s->getLevel()), x, 1);
    output(std::string("Atk: ") + toString(s->getAtk()) + ", Str: " + toString(s->getStr()) + ", Def: " + toString(s->getDef()), x, 2);
    output(std::string("HP: ") + toString(s->getHP()) + "/" + toString(s->getHPMax()) + " (" + toString(static_cast<int16_t>(s->getHPPerc() * 100.0)) + "%)", x, 3, (s->getHPPerc() >= 0.7 ? FG_GREEN : s->getHPPerc() >= 0.3 ? FG_YELLOW : FG_RED));
  }

  output("Available actions:", 0, 5);
  uint16_t opt = 6;
  if (s)
    output("[" + toString(PROG) + "] Attack", 0, opt++);
  else
    output("[" + toString(PROG) + "] Walk forwards", 0, opt++);
  if (p->getHPPerc() < 1.0)
  output("[" + toString(HEAL) + "] Heal", 0, opt++);
  output("[quit] quit", 0, opt++);
  output("> ", 0, opt++);
}

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    argv[i] = argv[i];
  }

  clearConsole();
  std::cout << "Welcome to ConsoleCrawler!" << std::endl << std::endl << "Name yourself, explorer!" << std::endl << "> ";

  Player* player = nullptr;

  {
    while (player == nullptr)
    {
      std::string name;
      std::getline(std::cin, name);

      if (cleanName(name))
        player = new Player(100, 100, 1, 1, 1, name); // yes I'm a horrible person who doesn't use c++11 memory management
      else
        std::cout << "Sorry, I didn't get that, try something else?" << std::endl << "> ";
    }
  }

  clearConsole();
  Soul* gremlin = nullptr;
  while (true)
  {
    presentOptions(player, gremlin);
    std::cout << std::flush;
    std::string input;
    std::getline(std::cin, input);

    if (input == "quit")
      break;

    switch (toInt(input))
    {
      case PROG:
      {
        if (!gremlin)
        {
          gremlin = new Soul(10, 10, 1, 1, 1); // a weak gremlin
          std::cout << "A wild gremlin appears!" << std::endl;
        }
        else
        {
          gremlin->makeImpact(player);
          if (player->getHP() == 0.0)
            break;
          player->makeImpact(gremlin);
          if (gremlin->getHP() == 0.0)
          {
            delete gremlin;
            gremlin = nullptr;
          }
        }
      }
      break;
      case HEAL:
      {
        player->makeImpact(player, true);
      }
      break;
    }
    clearConsole();
  }

  if (gremlin)
    delete gremlin;
  delete player;

  return 0;
}
