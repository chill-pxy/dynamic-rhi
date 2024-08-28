#include "../../Include/InterfaceType.h"

namespace DRHI
{
    void pickPhysicalDevice(VkPhysicalDevice* physicalDevice, VkInstance* instance, unsigned int id);

    void pickGraphicQueueFamily(VkPhysicalDevice* physicalDevice, uint32_t queueFamily);
}