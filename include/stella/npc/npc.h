#pragma once

#include <string>
#include <stack>
#include <vector>

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
        int m_engaging_try_index = 0;
        double m_attention_timer = 0.0f;
        std::wstring m_context{L"NON_ATTACHED"};

    public:
        explicit NPC (const std::wstring& name);
        std::wstring request (const std::wstring& req);

    protected:
        void update_current_action();
        std::wstring process_when_none (const std::wstring& req);
        std::wstring process_when_greeted (const std::wstring& req);
        std::wstring process_nlp(const std::wstring& req);
    };
}
}
