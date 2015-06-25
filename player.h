#ifndef PLAYER_H_INCLUDE
#define PLAYER_H_INCLUDE

#include <iostream>

#include "soul.h"

const extern int64_t XP_PER_LEVEL;

class Player : public Soul
{
private:
  std::string name;
  int64_t xp; // experience
  int64_t sp; // skill points for atk/def/str
//  int up;
public:
  Player(double h, double m, int16_t a, int16_t s, int16_t d, std::string n);
  virtual ~Player() {};

  virtual inline bool isPlayer() const { return true; };

  inline std::string getName() const { return name; };

  virtual void doImpact(const double damage, Soul* source);
  virtual void doDeath(Soul* source);
  virtual void makeImpact(Soul* target, bool heal = false);

  inline int64_t getXP() const { return xp; };
  virtual int64_t deltaXP(const int64_t d); // return number of levels up
  inline int64_t getSP() const { return sp; };

  virtual void upgradeSkill(const uint8_t skill);
};

#endif
