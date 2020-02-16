#include "stella/npc/npc.h"
#include <iostream>

namespace stella
{
namespace npc
{
    NPC::NPC (const std::wstring& name)
    : m_name(name)
    {
        m_state_stack.push (npc::State::NONE);
    }

    std::wstring NPC::request (const std::wstring& req)
    {
        std::wstring response{};

        switch (m_state_stack.top())
        {
        case npc::State::NONE:
            response = L"NONE";
            break;

        case npc::State::GREETED:
            response = L"GREETED";
            break;

        default:
            break;
        }

        return response;
    }

    void NPC::update_current_action()
    {

    }

    std::wstring NPC::process_when_none (const std::wstring& req)
    {
        std::wstring response{};
        return response;
    }

    std::wstring NPC::process_when_greeted (const std::wstring& req)
    {
        std::wstring response{};
        return response;
    }

    std::wstring NPC::process_nlp(const std::wstring& req)
    {
        std::wstring response{};
        return response;
    }
}
}