#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace stella
{
namespace core
{
  class Resource
  {
  public:
    Resource (const std::string& path) : m_path(path) {}
    virtual ~Resource() {}

  private:
    std::string m_path;
  };

  template<class T, typename... Args>
  class ResourceManager
  {
  private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    std::vector<std::string> m_id_list{};

  public:
    ResourceManager(){};
    // std::shared_ptr<T> load(const std::string &name, const std::string &path)
    std::shared_ptr<T> load (const std::string& name, Args... args)
    {
      static_assert (std::is_base_of<Resource, T>::value, "T must inherit from Resource");
      auto res = m_resources[name];
      if (!res)
      {
        m_resources[name] = res = std::make_shared<T> (args...);
        m_id_list.push_back (name);
      }
      return std::dynamic_pointer_cast<T> (res);
    }

    std::shared_ptr<T> load (const std::string& name)
    {
      auto res = m_resources[name];
      if (!res)
      {
        std::cout << "There is no resource named " << name << '\n';
        return nullptr;
      }
      auto return_value = std::dynamic_pointer_cast<T> (res);
      return return_value;
    }

    std::vector<std::string> get_list() { return m_id_list; }
  };
} // namespace core
} // namespace stella
