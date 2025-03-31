#pragma once

#include"../InterfaceType.h"

namespace drhi
{
	typedef struct Semaphores
	{
		// Swap chain image presentation
		VkSemaphore presentComplete;
		// Command buffer submission and execution
		VkSemaphore renderComplete;
	} Semaphores;

	void createSemaphore(Semaphores* semaphores, VkDevice* device);
}