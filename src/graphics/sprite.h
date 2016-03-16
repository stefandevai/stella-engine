#include <vector>
#include <GL/glew.h>
#include "../../glm/glm/glm.hpp"

namespace stella { namespace graphics { 
  class Sprite {
    public:
      Sprite(GLint x, GLint y, std::vector<GLuint> frames);
      ~Sprite();

      glm::vec2 GetPos() { return Pos; }
      void SetPos(const GLfloat& x, const GLfloat& y) { Pos.x = x; Pos.y = y; }
      GLuint CurrentFrame() { return Frame; }
      void SetFrame(GLuint frame);

    private:
      glm::vec2 Pos;
      std::vector<GLuint> Frames;
      GLuint Frame, NumberOfFrames;
  };
} }

