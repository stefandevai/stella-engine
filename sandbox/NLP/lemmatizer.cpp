#include "lemmatizer.h"
#include "../../include/stella/fileutils.h"

namespace stella
{
namespace nlp
{
    Lemmatizer::Lemmatizer(const std::string& lemma_lookup_path)
    {
        auto file_str = FileUtils::read_file(lemma_lookup_path.c_str());
        m_build_lookup_table (file_str);
    }

    std::wstring Lemmatizer::m_lemmatize(const std::wstring& str)
    {
        std::wstring lemma = str;
        std::string sstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
        if (m_lemmas.find(sstr) != m_lemmas.end())
        {
            sstr = m_lemmas[sstr];
            lemma = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(sstr);
        }
        return lemma;
    }

    void Lemmatizer::m_build_lookup_table (const std::string& file_str)
    {
        std::stringstream ss (file_str);
        std::string key_value;

        while (std::getline(ss, key_value, ','))
        {
            const auto it = key_value.find(' ');
            std::string key = key_value.substr(0, it);
            std::string value = key_value.substr(it+1);
            m_lemmas.insert(std::make_pair(key, value));
        }
    }

}
}