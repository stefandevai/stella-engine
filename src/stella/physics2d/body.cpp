#include "stella/physics2d/body.h"

namespace stella {
namespace physics2d {
Body::Body(glm::vec2 position, glm::vec2 dimension, glm::vec2 drag) : Position(position), Dimension(dimension), Drag(drag) {

}

Body::~Body() {

}
}}

