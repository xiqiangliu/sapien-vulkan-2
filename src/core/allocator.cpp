#include "svulkan2/core/allocator.h"
#include "svulkan2/core/buffer.h"
#include "svulkan2/core/context.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#define VMA_BUFFER_DEVICE_ADDRESS 1
#define VMA_EXTERNAL_MEMORY 1
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_IMPLEMENTATION
#include "svulkan2/common/vk_mem_alloc.h"
#pragma GCC diagnostic pop

namespace svulkan2 {
namespace core {

Allocator::Allocator(VmaAllocatorCreateInfo const &info) {
  if (vmaCreateAllocator(&info, &mMemoryAllocator) != VK_SUCCESS) {
    throw std::runtime_error("failed to create VmaAllocator");
  }

  // find GPU memory suitable for external buffer
  // VkPhysicalDeviceMemoryProperties properties;
  vk::PhysicalDeviceMemoryProperties properties;
  vk::PhysicalDevice(info.physicalDevice).getMemoryProperties(&properties);

  vk::BufferCreateInfo bufferInfo({}, 64,
                                  vk::BufferUsageFlagBits::eVertexBuffer |
                                      vk::BufferUsageFlagBits::eIndexBuffer |
                                      vk::BufferUsageFlagBits::eTransferDst);
  vk::ExternalMemoryBufferCreateInfo externalMemoryBufferInfo(
      vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd);
  bufferInfo.setPNext(&externalMemoryBufferInfo);

  vk::MemoryRequirements memReq;
  {
    auto buffer = Context::Get()->getDevice().createBufferUnique(bufferInfo);
    memReq =
        Context::Get()->getDevice().getBufferMemoryRequirements(buffer.get());
  }

  // Context::Get()->getDevice().getBufferMemoryRequirements2({&bufferInfo},
  // &req);

  int index = -1;
  for (uint32_t i = 0; i < properties.memoryTypeCount; ++i) {
    if ((memReq.memoryTypeBits & (1 << i)) &&
        (properties.memoryTypes[i].propertyFlags &
         vk::MemoryPropertyFlagBits::eDeviceLocal)) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    throw std::runtime_error(
        "Failed to find a suitable memory type for external memory pool");
  }

  VmaPoolCreateInfo poolInfo{};
  poolInfo.memoryTypeIndex = static_cast<uint32_t>(index);
  mExternalAllocInfo = vk::ExportMemoryAllocateInfo(
      vk::ExternalMemoryHandleTypeFlagBits::eOpaqueFd);
  poolInfo.pMemoryAllocateNext = &mExternalAllocInfo;

  vmaCreatePool(mMemoryAllocator, &poolInfo, &mExternalMemoryPool);
}

Allocator::~Allocator() {
  vmaDestroyPool(mMemoryAllocator, mExternalMemoryPool);
  vmaDestroyAllocator(mMemoryAllocator);
}

std::unique_ptr<Buffer> Allocator::allocateStagingBuffer(vk::DeviceSize size,
                                                         bool readback) {
  if (readback) {
    return std::make_unique<Buffer>(size,
                                    vk::BufferUsageFlagBits::eTransferSrc |
                                        vk::BufferUsageFlagBits::eTransferDst,
                                    VMA_MEMORY_USAGE_GPU_TO_CPU);
  }
  return std::make_unique<Buffer>(size,
                                  vk::BufferUsageFlagBits::eTransferSrc |
                                      vk::BufferUsageFlagBits::eTransferDst,
                                  VMA_MEMORY_USAGE_CPU_ONLY);
}

std::unique_ptr<class Buffer>
Allocator::allocateUniformBuffer(vk::DeviceSize size, bool deviceOnly) {
  if (deviceOnly) {
    return std::make_unique<Buffer>(size,
                                    vk::BufferUsageFlagBits::eUniformBuffer,
                                    VMA_MEMORY_USAGE_GPU_ONLY);
  } else {
    return std::make_unique<Buffer>(size,
                                    vk::BufferUsageFlagBits::eUniformBuffer,
                                    VMA_MEMORY_USAGE_CPU_TO_GPU);
  }
}

} // namespace core
} // namespace svulkan2
