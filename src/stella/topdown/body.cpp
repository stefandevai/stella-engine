#include "stella/topdown/body.h"

namespace stella {
namespace topdown {
Body::Body(glm::vec2 position, glm::vec2 dimension, glm::vec2 drag, bool collide_with_borders) : Position(position), Dimension(dimension), Drag(drag), CollideWithBorders(collide_with_borders) {

}

Body::~Body() {

}
}}
