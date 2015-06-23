#ifndef SOUL_H_INCLUDE
#define SOUL_H_INCLUDE

#include <iostream>

/* soul class, belongs to AI and the player */
class Soul
{
protected:
  int16_t hp;
  int16_t max;
  int16_t atk;
  int16_t str;
  int16_t def;
public:
  Soul(int16_t h, int16_t m, int16_t a, int16_t s, int16_t d) : hp(h), max(m), atk(a), str(s), def(d) {};
  virtual ~Soul() {};

  virtual inline bool isPlayer() const { return false; };

  inline int16_t getHP() const { return hp; };
  inline int16_t getHPMax() const { return max; };
  inline double  getHPPerc() const { return static_cast<double>(hp) / static_cast<double>(max); };

  inline uint16_t getAtk() const { return atk; };
  inline uint16_t getStr() const { return str; };
  inline uint16_t getDef() const { return def; };
  virtual inline uint16_t getLevel() { return static_cast<double>(atk + str + def) / 2.0; };

  virtual void doImpact(const int16_t damage, Soul* source);
  virtual void doDeath(Soul* source);
  virtual void makeImpact(Soul* target, bool heal = false);

  virtual int16_t calcDamage();
  virtual int16_t calcHeal();
};

#endif
