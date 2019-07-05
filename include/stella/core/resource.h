#include <memory>
#include <unordered_map>
#include <iostream>

namespace stella
{
namespace core
{

  class Resource
  {
    public:
      Resource(const std::string &path)
      {
        this->load(path);
      }
      virtual ~Resource() {}
      virtual void load(const std::string &path);
  };

  class ResourceManager
  {
    private:
      std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;

    public:
      template <typename T>
      std::shared_ptr<T> load(const std::string &path)
      {
        static_assert(std::is_base_of<Resource, T>::value, "T must inherit from Resource");
        auto res = m_resources[path];
        if (!res)
        {
          m_resources[path] = res = std::make_shared<T>(path);
        }

        auto value = std::dynamic_pointer_cast<T>(res);
        if (!value)
        {
          std::cout << "Resource " << path << " is already loaded as another type\n";
          return nullptr;
        }
        return value;
      }

  };
}
}

