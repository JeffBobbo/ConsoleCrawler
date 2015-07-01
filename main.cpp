#include <iostream>
#include <cstdint>
#include <sstream>
#include <algorithm>  // std::max
#include <functional> // eventHandler
#include <locale>     // eventHandler

#include <thread>
#include <chrono>

#include "event.h"
#include "soul.h"
#include "player.h"
#include "console.h"
#include "misc.h"


enum OPTIONS
{
  OPT_PROG = 1,
  OPT_HEAL,
  OPT_ATK,
  OPT_STR,
  OPT_DEF
};

void presentOptions(Player* p, Soul* s)
{
  output(p->getName() + "'s status:", 0, 0);
  output("Level: " + toString(p->getLevel()) + " - " + toString(p->getXP()) + "XP, " + toString(p->getSP()) + "SP" + (getTurn() ? ". Turn: " + toString(getTurn()) : ""), 0, 1);
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
    output("[" + toString(OPT_PROG) + "] Attack", 0, opt++);
  else
    output("[" + toString(OPT_PROG) + "] Walk forwards", 0, opt++);
  if (p->getHPPerc() < 1.0)
  output("[" + toString(OPT_HEAL) + "] Heal", 0, opt++);
  if (p->getSP())
  {
    output("[" + toString(OPT_ATK) + "] Increase atk", 0, opt++);
    output("[" + toString(OPT_STR) + "] Increase str", 0, opt++);
    output("[" + toString(OPT_DEF) + "] Increase def", 0, opt++);
  }
  output("[q] quit", 0, opt++);
  output("> ", 0, opt++);

  int oldPos = opt - 1;
  std::string m;
  while (popMessage(m))
    output(m, 0, opt++);
  setCursorPosition(2, oldPos);
}

int32_t currentKey = 0;
bool eventHandler(const Event& event)
{
  std::locale loc;
  switch (event.eType)
  {
    case Event::EVENT_TYPE::KEYBOARD:
    {
      if (event.eKeyState == false)
        break;

      currentKey = event.eKey;
      if (std::toupper(static_cast<char >(event.eKey), loc) == 'Q') // Q, 113 for q
        return false;
    }
    break;
    default:
    break;
  }
  return true;
}

int main(int argc, char** argv)
{
  for (int i = 1; i < argc; ++i)
    argv[i] = argv[i];

  seedMT();

  clearConsole();
  std::cout << "Welcome to ConsoleCrawler!" << std::endl << std::endl << "Name yourself, explorer!" << std::endl << "> ";

  Player* player = nullptr;
  while (player == nullptr)
  {
    std::string name;
    std::getline(std::cin, name);

    if (name.length() > 20)
    {
      std::cout << "That's far too long for me to remember!" << std::endl << "> ";
      continue;
    }
    if (cleanName(name))
      player = new Player(100, 100, 1, 1, 1, name); // yes I'm a horrible person who doesn't use c++11's memory management
    else
      std::cout << "Sorry, I didn't get that, try something else?" << std::endl << "> ";
  }

  receiver = new Receiver(std::function<bool(Event&)>(eventHandler));
  clearConsole();
  Soul* gremlin = nullptr;
  bool run = true;
  while (receiver->pollEvents() && run)
  {
    if (currentKey == -1)
      continue;
    clearConsole();
    presentOptions(player, gremlin);

    int32_t opt = currentKey - Event::KEY::K_0;
    bool goodTurn = false;
    switch (opt)
    {
      case OPT_PROG:
      {
        if (!gremlin)
        {
          int16_t pLevel = player->getLevel();
          int16_t pointsToSpend = pLevel * 3;
          int16_t pAtk = pointsToSpend * randRange(0.25, 0.5);
          int16_t pStr = pointsToSpend * randRange(0.25, 0.5);
          int16_t pDef = pointsToSpend - (pAtk + pStr);
          gremlin = new Soul(9 + pLevel, 9 + pLevel, pAtk, pStr, pDef);
          addMessage("A wild gremlin appears!");
        }
        else
        {
          gremlin->makeImpact(player);
          if (player->getHP() == 0.0)
            run = false;
          player->makeImpact(gremlin);
          if (gremlin->getHP() == 0.0)
          {
            delete gremlin;
            gremlin = nullptr;
          }
        }
        goodTurn = true;
      }
      break;
      case OPT_HEAL:
      {
        if (getTurn() - player->getLastHeal() < TURNS_PER_HEAL)
        {
          uint64_t turns = TURNS_PER_HEAL - (getTurn() - player->getLastHeal());
          addMessage("You can't heal yourself right now (" + toString(turns) + " turn" + (turns > 1 ? "s" : "") + " until next).");
        }
        else
        {
          goodTurn = player->makeImpact(player, true);
        }
      }
      break;
      case OPT_ATK:
      case OPT_STR:
      case OPT_DEF:
      {
        player->upgradeSkill(opt - OPT_ATK);
      }
      break;
      default:
      {
      }
      break;
    }
    currentKey = -1;
    if (goodTurn)
      incTurn();
    std::cout << std::flush; // kinda outta place, but saves doing it every call to Output()
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 50));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  setColour(FG_DEFAULT|BG_DEFAULT);
  clearConsole();
  if (gremlin)
    delete gremlin;
  delete player;
  delete receiver;

  return 0;
}
