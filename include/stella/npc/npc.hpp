#pragma once

#include <string>
#include <stack>
#include <vector>
#include <sol/sol.hpp>
#include "stella/nlp/nlp.h"

namespace stella
{
namespace npc
{
  enum class State
  {
    NONE,
    GREETED
  };

  class NPC
  {
  protected:
    std::wstring m_name;
    std::stack<npc::State> m_state_stack;
    std::vector<std::wstring> m_engaging_tries{L"What?", L"Did you talk to me?", L"..."};
    size_t m_engaging_try_index = 0;
    double m_attention_timer    = 0.0;
    std::string m_context{"NON_ATTACHED"};
    sol::state m_lua;
    // nlp::WordTokenizer tokenizer;
    nlp::NLP m_nlp{"./config/nlp/perceptron-tagger"};

  public:
    explicit NPC (const std::string& script_path);
    inline const npc::State get_state() const { return m_state_stack.top(); }
    std::wstring request (const std::wstring& req);

  protected:
    void update_current_action();
    std::wstring process_when_none (const std::wstring& req);
    std::wstring process_when_greeted (const std::wstring& req);
    std::wstring process_nlp (const std::wstring& req);
  };
} // namespace npc
} // namespace stella
