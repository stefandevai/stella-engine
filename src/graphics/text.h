#pragma once

#include "renderable.h"

#include <string>

namespace stella { namespace graphics{
  class Text : public Renderable
  {
    public:
      Text(std::string text);
      ~Text();

      void Render();

    private:
      std::string TextString;
  };
} }

