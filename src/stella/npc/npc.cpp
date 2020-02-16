#include "stella/npc/npc.h"
#include "stella/npc/speech.h"
#include <iostream>
#include <algorithm>
#include <cwctype>

namespace stella
{
namespace npc
{
    NPC::NPC (const std::string& script_path)
    {
        m_lua.script_file(script_path);
        m_name = m_lua.get<std::wstring>("name");
        m_state_stack.push (npc::State::NONE);
    }

    void NPC::update_current_action()
    {
        std::cout << "*" << m_name.c_str() << " is crying in a dark corner...\n";
    }

    std::wstring NPC::request (const std::wstring& req)
    {
        std::wstring response{};
        std::wstring lower_req = req;
        std::transform(lower_req.begin(), lower_req.end(), lower_req.begin(), towlower);

        switch (m_state_stack.top())
        {
        case npc::State::NONE:
            response = this->process_when_none(lower_req);
            break;

        case npc::State::GREETED:
            response = this->process_when_greeted(lower_req);
            break;

        default:
            break;
        }

        return response;
    }

    std::wstring NPC::process_when_none (const std::wstring& req)
    {
        std::wstring response{};
        if (std::find(Speech::GREETINGS.begin(), Speech::GREETINGS.end(), req) != Speech::GREETINGS.end())
        {
            m_state_stack.push(npc::State::GREETED);
            response = L"Hi, Nikte... :'(";
        }
        else if (m_engaging_try_index < m_engaging_tries.size())
        {
            response = m_engaging_tries.at(m_engaging_try_index);
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
        if (std::find(Speech::FAREWELLS.begin(), Speech::FAREWELLS.end(), req) != Speech::FAREWELLS.end())
        {
            m_state_stack.pop();
            m_engaging_try_index = 0;
            m_context = "NON_ATTACHED";
            response = L"Bye!";
        }
        return response;
    }

    std::wstring NPC::process_nlp(const std::wstring& req)
    {
        std::wstring response{};
        return response;
    }
}
}