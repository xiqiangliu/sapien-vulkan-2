#pragma once
#include "svulkan2/core/image.h"
#include <string>
#include <vector>

namespace svulkan2 {
namespace shader {

std::unique_ptr<core::Image>
generateBRDFLUT(uint32_t size);

void prefilterCubemap(core::Image &image);

} // namespace shader
} // namespace svulkan2
