#include "misc.h"

#include <random>
#include <list>

std::list<std::string> mList;

#ifdef DEBUG
#include <iostream>
#endif

std::mt19937 mt;

void seedMT(uint64_t s /* = 0 */)
{
  if (!s)
    s = tickCount();
#ifdef DEBUG
  std::cout << "Seed: " << s << std::endl;
#endif
  mt.seed(s);
}

int64_t toInt(const std::string& s, bool* const success /* = nullptr */)
{
  try
  {
    if (success)
      (*success) = true;
    return std::stoll(s, nullptr, 0); // can throw
  }
  catch (...)
  {
    if (success)
      (*success) = false;
    return 0;
  }
}

double randRange(double min, double max)
{
  std::uniform_real_distribution<double> dist(min, max);
  return dist(mt);
}

int64_t randRange(int64_t min, int64_t max)
{
  std::uniform_int_distribution<int64_t> dist(min, max);
  return dist(mt);
}

void toUpper(std::string& s)
{
  for (std::string::iterator it = s.begin(); it != s.end(); ++it)
    *it = std::toupper(*it);
}

void toLower(std::string& s)
{
  for (std::string::iterator it = s.begin(); it != s.end(); ++it)
    *it = std::tolower(*it);
}


bool cleanName(const std::string& str)
{
  const char* filter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .,-1234567890";
  return str.find_first_not_of(filter) == std::string::npos;
}

void addMessage(const std::string& m)
{
  mList.push_front(m);
}

bool popMessage(std::string& m)
{
  m = "";
  if (mList.begin() == mList.end())
    return false;
  m = mList.back();
  mList.pop_back();
  return true;
}
