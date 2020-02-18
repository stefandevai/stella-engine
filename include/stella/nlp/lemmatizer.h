#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace stella
{
namespace nlp
{
  class Lemmatizer
  {
  private:
    std::unordered_map<std::string, std::string> m_lemmas;

  public:
    Lemmatizer();
    std::wstring operator() (const std::wstring& token) { return m_lemmatize (token); }

  private:
    std::wstring m_lemmatize (const std::wstring& str);
    void m_build_lookup_table (const std::string& file_str);
  };
} // namespace nlp
} // namespace stella