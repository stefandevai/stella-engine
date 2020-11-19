#include "stella/npc/npc.hpp"
#include "stella/npc/speech.hpp"
#include <algorithm>
#include <cwctype>

namespace stella
{
namespace npc
{
  NPC::NPC (const std::string& script_path)
  {
    m_lua.script_file (script_path);
    m_name = m_lua.get<std::wstring> ("name");
    m_state_stack.push (npc::State::NONE);
  }

  void NPC::update_current_action() { /*std:: << "*" << m_name.c_str() << " is crying in a dark corner...\n";*/ }

  std::wstring NPC::request (const std::wstring& req)
  {
    std::wstring response{};
    std::wstring lower_req = req;
    std::transform (lower_req.begin(), lower_req.end(), lower_req.begin(), towlower);

    switch (m_state_stack.top())
    {
      case npc::State::NONE:
        response = this->process_when_none (lower_req);
        break;

      case npc::State::GREETED:
        response = this->process_when_greeted (lower_req);
        break;

      default:
        break;
    }

    return response;
  }

  std::wstring NPC::process_when_none (const std::wstring& req)
  {
    std::wstring response{};
    if (std::find (Speech::GREETINGS.begin(), Speech::GREETINGS.end(), req) != Speech::GREETINGS.end())
    {
      m_state_stack.push (npc::State::GREETED);
      response = L"Hi, Nikte...";
    }
    else if (m_engaging_try_index < m_engaging_tries.size())
    {
      response = m_engaging_tries.at (m_engaging_try_index);
      ++m_engaging_try_index;
      m_attention_timer = 100.0;
    }
    else
    {
      this->update_current_action();
    }
    return response;
  }

  std::wstring NPC::process_when_greeted (const std::wstring& req)
  {
    std::wstring response{};

    bool clicked_on_phrase = false;
    if (clicked_on_phrase)
    {
      // TODO: GUI with phrase suggestions
      goto return_response;
    }
    // Check if player is closing the conversation
    else if (std::find (Speech::FAREWELLS.begin(), Speech::FAREWELLS.end(), req) != Speech::FAREWELLS.end())
    {
      m_state_stack.pop();
      m_engaging_try_index = 0;
      m_context            = "NON_ATTACHED";
      response             = L"Bye!";
    }
    // Check if input is any of the keywords
    else
    {
      sol::table context = m_lua["database"][m_context];
      for (auto& speech_pair : context)
      {
        const sol::table& speech   = speech_pair.second;
        const sol::table& keywords = speech["keywords"];

        for (unsigned int i = 1; i <= keywords.size(); ++i)
        {
          const std::wstring& kw = keywords[i];
          if (req == kw)
          {
            // Necessary to avoid a compilation error on MSVC
            std::wstring t_response = speech["responses"][1];
            std::string t_context   = speech["next_speech"];
            response                = std::move (t_response);
            m_context               = std::move (t_context);
            goto return_response;
          }
        }
      }
    }

    // If there was no answer until now, process NLP
    // if (response.empty())
    //{
    ////response = process_nlp (req);
    // response = L"No more NLP module";
    //}

  return_response:
    return response;
  }
} // namespace npc
} // namespace stella
