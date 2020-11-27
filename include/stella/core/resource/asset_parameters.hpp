#include <string>

namespace stella::core
{
  // Asset types
  const std::string ASSET_TYPE_TEXTURE = "texture";
  const std::string ASSET_TYPE_TEXTURE_ATLAS = "texture-atlas";
  const std::string ASSET_TYPE_SHADER = "shader";
  const std::string ASSET_TYPE_MODEL = "model";

  // JSON objects
  const std::string ASSET_OBJECT_ASSETS = "assets";

  // JSON parameters
  const std::string ASSET_PARAMETER_ID = "id";
  const std::string ASSET_PARAMETER_TYPE = "type";
  const std::string ASSET_PARAMETER_PATH = "path";

  const std::string ASSET_PARAMETER_UNIFORM = "uniform";
  const std::string ASSET_PARAMETER_HORIZONTAL_FRAMES = "horizontalFrames";
  const std::string ASSET_PARAMETER_VERTICAL_FRAMES = "verticalFrames";
}
