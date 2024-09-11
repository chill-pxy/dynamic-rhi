#include<stdexcept>

#include "../../Include/InterfaceType.h"

namespace DRHI
{
    void pickPhysicalDevice(VkPhysicalDevice* physicalDevice, VkInstance* instance, unsigned int id);

    void pickGraphicQueueFamily(VkPhysicalDevice* physicalDevice, uint32_t queueFamily);

	/**
	* Get the index of a memory type that has all the requested property bits set
	*
	* @param typeBits Bit mask with bits set for each memory type supported by the resource to request for (from VkMemoryRequirements)
	* @param properties Bit mask of properties for the memory type to request
	* @param (Optional) memTypeFound Pointer to a bool that is set to true if a matching memory type has been found
	*
	* @return Index of the requested memory type
	*
	* @throw Throws an exception if memTypeFound is null and no memory type could be found that supports the requested properties
	*/
	uint32_t getMemoryType(VkPhysicalDevice* physicalDevice, uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound = (VkBool32*)nullptr);
}