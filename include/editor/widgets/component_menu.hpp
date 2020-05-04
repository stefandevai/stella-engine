#include "widget.hpp"
#include <unordered_map>
#include "stella/component_list.hpp"

namespace stella
{
namespace widget
{

  class ComponentMenu : public Widget
  {
    using ComponentMap = std::unordered_map<Components, bool>;

    public:
      ComponentMenu ();
      void render();
      void reset();
      inline const ComponentMap get_map() const { return m_selected_components; };

    private:
      ComponentMap m_selected_components;
  };

} // namespace widget
} // namespace stella

