#include "player.h"

#include "misc.h"

const int64_t XP_PER_LEVEL = 1000;


Player::Player(double h, double m, int16_t a, int16_t s, int16_t d, std::string n) :
  Soul(h, m, a, s, d), name(n)
{
  xp = 0;
//  up = 0;
}

void Player::doImpact(const double damage, Soul* source)
{
  Soul::doImpact(damage, source);
}

void Player::doDeath(Soul* source)
{
  if (source == this)
    addMessage("You killed yourself!");
  if (source)
    addMessage("You were killed by a gremlin!");
  else
    addMessage("You were killed, but by whom?");
}

void Player::makeImpact(Soul* target, bool heal /* = false */)
{
  if (!target)
  {
    addMessage("No target");
    return;
  }
  Soul::makeImpact(target, heal);
}

int64_t Player::deltaXP(const int64_t d)
{
  xp += d;
  bool ret = 0;
  while (xp >= XP_PER_LEVEL)
  {
    sp++;
    xp -= XP_PER_LEVEL;
    ret++;
  }
  return ret;
}


void Player::upgradeSkill(const uint8_t skill)
{
  if (sp <= 0)
    return;

  switch (skill)
  {
    case 0:
      addMessage("Increased atk!");
      atk++;
    break;
    case 1:
      addMessage("Increased str!");
      str++;
    break;
    case 2:
      addMessage("Increased def!");
      def++;
    break;
  }
  sp--;
}
