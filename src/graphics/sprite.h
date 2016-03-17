#include <vector>
#include <GL/glew.h>
#include "../../glm/glm/glm.hpp"

namespace stella { namespace graphics { 
  struct VertexData
  {
    glm::vec3 vertex;
    unsigned int color;
  };

  class Sprite {
    public:
      Sprite(GLint x, GLint y, GLint w, GLint h, std::vector<GLuint> frames);
      Sprite(GLint x, GLint y, GLint w, GLint h, GLuint frame);
      ~Sprite();

      // Getters
      inline const glm::vec2 GetPos() const { return Pos; }
      inline const glm::vec2 GetDimensions() const { return Dimensions; }
      inline const glm::vec4 GetColor() const { return Color; }
      inline const GLuint GetCurrentFrame() const { return Frame; }
      inline const GLboolean IsVisible() const { return Visible; }

      // Setters
      void SetPos(const GLint& x, const GLint& y) { Pos.x = x; Pos.y = y; }
      void SetDimensions(const GLint& w, const GLint& h, const GLint& d) { Dimensions.x = w; Dimensions.y = h; }
      void SetColor(int r, int g, int b) { Color.x = r/255.0f; Color.y = g/255.0f; Color.z = b/255.0f; }
      void SetFrame(GLuint frame);
      void SetVisible(GLboolean vis) { Visible = vis; }
      void ToggleVisible() { Visible = !Visible; }

    private:
      glm::vec2 Pos, Dimensions;
      glm::vec4 Color;
      std::vector<GLuint> Frames;
      GLuint Frame, NumberOfFrames;
      GLboolean Visible;
  };
} }

