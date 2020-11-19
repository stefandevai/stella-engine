#include "stella/npc/speech.hpp"

namespace stella
{
namespace npc
{
  const std::vector<std::wstring> Speech::GREETINGS{L"hi", L"hey", L"hello", L"heya", L"hiho", L"greetings"};
  const std::vector<std::wstring> Speech::FAREWELLS{
      L"bye", L"see ya", L"see you", L"goodbye", L"good bye", L"farewell"};
} // namespace npc
} // namespace stella
