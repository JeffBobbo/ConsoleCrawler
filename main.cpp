#include <iostream>
#include <cstdint>
#include <sstream>

#include "soul.h"
#include "player.h"
#include "console.h"

bool cleanName(std::string str)
{
  const char* filter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890";
  return str.find_first_not_of(filter) == std::string::npos;
}

std::string presentOptions(Player* p, Soul* s)
{
  std::stringstream ss;
  ss << p->getName() << "'s status:\n";
  ss << "Level: " << p->getLevel() << "\n";
  ss << "Atk: " << p->getAtk() << ", Str: " << p->getStr() << ", Def: " << p->getDef() << "\n";
  ss << "HP: " << p->getHP() << "/" << p->getHPMax() << " (" << static_cast<int16_t>(p->getHPPerc() * 100.0) << "%)\n\n";

  if (s)
  {
    ss << "Gremlin's status:\n";
    ss << "Level: " << s->getLevel() << "\n";
    ss << "Atk: " << s->getAtk() << ", Str: " << s->getStr() << ", Def: " << s->getDef() << "\n";
    ss << "HP: " << s->getHP() << "/" << s->getHPMax() << " (" << static_cast<int16_t>(s->getHPPerc() * 100.0) << "%)\n\n";
  }

  ss << "Available actions:\n";
  if (s)
    ss << "[1] Attack\n";
  else
    ss << "[1] Walk forwards\n";
  ss << "[0] quit\n";
  ss << "[quit] quit\n";

  return ss.str();
}

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    argv[i] = argv[i];
  }

  std::cout << "Welcome to ConsoleCrawler!" << std::endl << std::endl << "Name yourself, explorer!" << std::endl << "> ";

  Player* player = nullptr;

  {
    while (player == nullptr)
    {
      std::string name;
      std::getline(std::cin, name);

      if (cleanName(name))
      {
        std::cout << "I hope you're ready, " << name << std::endl;
        player = new Player(100, 100, 1, 1, 1, name); // yes I'm a horrible person who doesn't use c++11 memory management
      }
      else
      {
        std::cout << "Sorry, I didn't get that, try something else?" << std::endl << "> ";
      }
    }
  }

  Soul* gremlin = nullptr;
  while (true)
  {
    std::cout << presentOptions(player, gremlin);
    std::cout << std::flush;
    std::string input;
    std::getline(std::cin, input);
    clearConsole();

    if (input == "quit")
      break;
    if (input == "1")
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
  }

  if (gremlin)
    delete gremlin;
  delete player;

  return 0;
}
