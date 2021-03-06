#include "soul.h"

#include "player.h"
#include "misc.h"

#include <sstream>
#include <cmath>

const uint64_t TURNS_PER_HEAL = 10;

bool Soul::doImpact(const int16_t damage, Soul* source)
{
  if (damage == 0)
    return true; // no hit done, but it's not because of failure
  Player* const me = dynamic_cast<Player* const>(this);
  if (damage < 0)
  {
    if (this == source)
      lastHeal = getTurn();
    const int16_t actualHeal = std::min(-damage, max - hp);
    if (me)
      addMessage("You were healed for " + toString(actualHeal) + "HP.");
    else
      addMessage("The gremlin healed himself for " + toString(actualHeal) + "!");
  }
  else
  {
    if (me)
      addMessage("You were hit for " + toString(damage) + "HP.");
    else
      addMessage("You hit the gremlin for " + toString(damage) + "HP.");
  }
  hp -= damage;
  if (hp <= 0.0)
    doDeath(source);
  if (hp >= max)
    hp = max;
  return true;
}

void Soul::doDeath(Soul* source)
{
  if (source == this)
    addMessage("The gremlin killed themself?");
  if (source->isPlayer())
  {
    Player* p = static_cast<Player*>(source);
    addMessage("You killed a gremlin!");

    // give some xp
    double victimLevel = getLevel();
    double killerLevel = p->getLevel();

    int64_t xp = 250.0 * std::pow((victimLevel + 1.0) / killerLevel, 0.9); // 250 XP if you're one level greater than the enemy
    int64_t levelled = p->deltaXP(xp);
    std::stringstream ss;
    ss << "You gained " << xp << "XP";
    if (levelled)
    {
      ss << " and gained ";
      if (levelled > 1)
        ss << levelled << " skill points";
      else
        ss << "a skill point";
      ss << "!";
    }
    else
    {
      ss << ".";
    }
    ss << " " << (XP_PER_LEVEL - p->getXP()) << "XP until next skill point.";
    addMessage(ss.str());
  }
  if (!source)
    addMessage("The gremlin died... But whom killed it?");

  hp = 0;
}

bool Soul::makeImpact(Soul* target, bool heal /* = false */)
{
  if (!target)
    return false;
  return target->doImpact(heal ? calcHeal() : calcDamage(), this);
}

int16_t Soul::calcDamage()
{
  double atk = static_cast<double>(getAtk());
  double str = static_cast<double>(getStr());
  double accuracy = randRange(0.0, 1.0 / atk) * std::pow(atk, 1.05);
  return str * accuracy + (accuracy > 0.3 ? 1 : 0); // give them a bit of free damage
}

int16_t Soul::calcHeal()
{
  double def = static_cast<double>(getDef());
  double str = static_cast<double>(getStr());
  double accuracy = randRange(0.0, 1.0 / def) * std::pow(def, 1.025);
  return -(str * accuracy + 5);
}
