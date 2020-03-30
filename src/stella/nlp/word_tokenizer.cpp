#include "stella/nlp/word_tokenizer.hpp"
#include "stella/core/string_utils.hpp"

namespace stella
{
namespace nlp
{
  WordTokenizer::WordTokenizer() {}

  std::vector<std::wstring> WordTokenizer::m_tokenize (const std::wstring& str)
  {
    std::vector<std::wstring> tokens;
    auto tks_begin = std::wsregex_iterator (str.begin(), str.end(), m_sent_regex);
    auto tks_end   = std::wsregex_iterator();

    for (auto it = tks_begin; it != tks_end; ++it)
    {
      std::wstring token = (*it).str();
      size_t suffix_pos  = m_has_suffix (token);
      if (suffix_pos != std::string::npos)
      {
        tokens.push_back (token.substr (0, suffix_pos));
        tokens.push_back (token.substr (suffix_pos));
      }
      else
      {
        tokens.push_back (token);
      }
    }

    return tokens;
  }

  size_t WordTokenizer::m_has_suffix (const std::wstring& token)
  {
    for (const auto& suffix : SUFFIXES)
    {
      const size_t pos = StringUtils::suffix (token, suffix);
      if (pos != std::string::npos)
      {
        return pos;
      }
    }
    return std::string::npos;
  }
} // namespace nlp
} // namespace stella