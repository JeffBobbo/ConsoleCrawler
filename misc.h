#ifndef MISC_H_INCLIDE
#define MISC_H_INCLIDE

#include <chrono>
#include <cstdint>
#include <string>

inline uint64_t tickCount() { return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1); };

void seedMT(uint64_t s = 0);

double randRange(double min, double max);
int64_t randRange(int64_t min, int64_t max);
inline int32_t randRange(int32_t min, int32_t max) { return randRange(static_cast<int64_t>(min), static_cast<int64_t>(max)); };

int64_t toInt(const std::string& s, bool* const success = nullptr);

template <typename T>
std::string toString(const T& t, bool* const success = nullptr)
{
  try
  {
    if (success)
      *success = true;
    return std::to_string(t);
  }
  catch (...)
  {
    if (success)
      *success = false;
    return std::string();
  }
}

void toUpper(std::string& s);
void toLower(std::string& s);

bool cleanName(const std::string& n);

void addMessage(const std::string& m);
bool popMessage(std::string& m);

#endif
