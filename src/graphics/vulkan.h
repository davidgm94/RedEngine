#pragma once
#include <volk.h>
#include "../red_os.h"
#ifndef VK_KHR_VALIDATION_LAYER_NAME
#define VK_KHR_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"
#endif

static inline void printVkResult(VkResult result, const char* file, int line)
{
#define PRINT_VKRESULT(resultType)\
case (resultType): { char buffer[256];\
platform_Sprintf(buffer, "[VKRESULT] %s in file: %s, line: %i.\n", #resultType, file, line); \
platform_DebugInfo(buffer); break; }
    switch (result)
    {
		PRINT_VKRESULT(VK_SUCCESS)
		PRINT_VKRESULT(VK_NOT_READY)
		PRINT_VKRESULT(VK_TIMEOUT)
		PRINT_VKRESULT(VK_EVENT_SET)
		PRINT_VKRESULT(VK_EVENT_RESET)
		PRINT_VKRESULT(VK_INCOMPLETE)
		PRINT_VKRESULT(VK_ERROR_OUT_OF_HOST_MEMORY)
		PRINT_VKRESULT(VK_ERROR_OUT_OF_DEVICE_MEMORY)
		PRINT_VKRESULT(VK_ERROR_INITIALIZATION_FAILED)
		PRINT_VKRESULT(VK_ERROR_DEVICE_LOST)
		PRINT_VKRESULT(VK_ERROR_MEMORY_MAP_FAILED)
		PRINT_VKRESULT(VK_ERROR_LAYER_NOT_PRESENT)
		PRINT_VKRESULT(VK_ERROR_EXTENSION_NOT_PRESENT)
		PRINT_VKRESULT(VK_ERROR_FEATURE_NOT_PRESENT)
		PRINT_VKRESULT(VK_ERROR_INCOMPATIBLE_DRIVER)
		PRINT_VKRESULT(VK_ERROR_TOO_MANY_OBJECTS)
		PRINT_VKRESULT(VK_ERROR_FORMAT_NOT_SUPPORTED)
		PRINT_VKRESULT(VK_ERROR_FRAGMENTED_POOL)
		PRINT_VKRESULT(VK_ERROR_OUT_OF_POOL_MEMORY)
		PRINT_VKRESULT(VK_ERROR_INVALID_EXTERNAL_HANDLE)
		PRINT_VKRESULT(VK_ERROR_SURFACE_LOST_KHR)
		PRINT_VKRESULT(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)
		PRINT_VKRESULT(VK_SUBOPTIMAL_KHR)
		PRINT_VKRESULT(VK_ERROR_OUT_OF_DATE_KHR)
		PRINT_VKRESULT(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)
		PRINT_VKRESULT(VK_ERROR_VALIDATION_FAILED_EXT)
		PRINT_VKRESULT(VK_ERROR_INVALID_SHADER_NV)
		PRINT_VKRESULT(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT)
		PRINT_VKRESULT(VK_ERROR_FRAGMENTATION_EXT)
		PRINT_VKRESULT(VK_ERROR_NOT_PERMITTED_EXT)
		PRINT_VKRESULT(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT)
		PRINT_VKRESULT(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT)
		//PRINT_VKRESULT(VK_ERROR_OUT_OF_POOL_MEMORY_KHR)	    // repeated
		//PRINT_VKRESULT(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR) // repeated
		// PRINT_VKRESULT(VK_RESULT_BEGIN_RANGE)
		//PRINT_VKRESULT(VK_RESULT_END_RANGE)
		PRINT_VKRESULT(VK_RESULT_RANGE_SIZE)
		PRINT_VKRESULT(VK_RESULT_MAX_ENUM)

		invalid_default_case;
    }
#undef PRINT_VKRESULT
}

#ifndef NDEBUG
#define VKCHECK(call)								\
{												\
    VkResult result_ = call; 						\
    if (result_ != VK_SUCCESS)						\
    {										 \
	   printVkResult(result_, __FILE__, __LINE__);					\
    }										 \
	//msg_assert(result_ == VK_SUCCESS, call);				\
}
#else
#define VKCHECK(call) (call)
#endif

// TODO: WARNING: Magical constants to make development easier. Modify later
#define QUEUE_FAMILY_PROPERTY_COUNT 34
#define QUEUE_FAMILY_INDEX_COUNT 3
#define SURFACE_FORMAT_COUNT 2
#define PRESENT_MODE_COUNT 3
#define IMAGE_COUNT 3

typedef uint queue_family_indices[QUEUE_FAMILY_INDEX_COUNT];

enum Vk_QueueFamilyIndex
{
    VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS,
    VULKAN_QUEUE_FAMILY_INDEX_COMPUTE,
    VULKAN_QUEUE_FAMILY_INDEX_TRANSFER,
};

#if VK_USE_PLATFORM_WIN32_KHR
typedef struct win32_os_vk_surface
{
    HINSTANCE instance;
    HWND window;
} win32_os_vk_surface;
#else
#endif

typedef struct
{
    VkQueueFamilyProperties properties[QUEUE_FAMILY_PROPERTY_COUNT];
    size_t propertyCount;
    queue_family_indices indices;
} queue_family;

typedef struct
{
    queue_family queueFamily;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VkSurfaceFormatKHR surfaceFormats[SURFACE_FORMAT_COUNT];
	VkPresentModeKHR presentModes[PRESENT_MODE_COUNT];
} swapchain_properties;

typedef struct
{
    VkAllocationCallbacks allocator;
    VkInstance instance;
#if _DEBUG
    VkDebugReportCallbackEXT debugCallback;
#endif
    swapchain_properties swapchainProperties;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
	VkQueue queues[QUEUE_FAMILY_INDEX_COUNT];
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
	VkImage swapchainImages[IMAGE_COUNT];
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffers[IMAGE_COUNT];
} vulkan_renderer;

static inline VkInstance vk_createInstance(VkAllocationCallbacks* allocator)
{
    VkApplicationInfo applicationInfo;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = "Red Engine";
    applicationInfo.applicationVersion = 1;
    applicationInfo.pEngineName = "Red";
    applicationInfo.engineVersion = 1;
    applicationInfo.apiVersion = 0;
	    
#if 1
    u32 instanceLayerCount;
    VkLayerProperties instanceLayerProperties;
    VKCHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, &instanceLayerProperties));
    assert(instanceLayerCount > 0);

    VkLayerProperties instanceLayers[instanceLayerCount];
    VKCHECK(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayers));

    u32 instanceExtensionCount = 0;
    VKCHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr));
    assert(instanceExtensionCount > 0);
    VkExtensionProperties extensionProperties[instanceExtensionCount];
    VKCHECK(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, extensionProperties));
#else
#endif

	//const char* enabledLayers[] =
    //{
		
#if _DEBUG
	  //VK_KHR_VALIDATION_LAYER_NAME,
#endif // _DEBUG
    //};
    const char* enabledExtensions[] =
    {
	   VK_KHR_SURFACE_EXTENSION_NAME,
#if _DEBUG
	   //VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
#if VK_USE_PLATFORM_WIN32_KHR
	   VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#else
#endif
    };

    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledLayerCount = 0;//ArrayCount(enabledLayers);
	createInfo.ppEnabledLayerNames = 0;//enabledLayers;
	createInfo.enabledExtensionCount = ArrayCount(enabledExtensions);//ArrayCount(enabledExtensions);
	createInfo.ppEnabledExtensionNames = enabledExtensions; // enabledExtensions;

    VkInstance instance;
	VKCHECK(vkCreateInstance(&createInfo, allocator, &instance));

    return instance;
}

#ifdef _DEBUG
VkBool32 vk_debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, u64 object,
    size_t location, i32 messageCode, const char* pLayerPrefix, const char* pMessage,
    void* pUserData)
{
	objectType = 5;
	object = 5;
	location = 5;
	pUserData = 0;
    // Select prefix depending on flags passed to the callback
		    // Note that multiple flags may be set for a single validation message
    char prefix[20];

    // Error that may result in undefined behaviour
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
	   strcpy(prefix, "ERROR:");
    };
    // Warnings may hint at unexpected / non-spec API usage
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
	   strcpy(prefix, "WARNING:");
    };
    // May indicate sub-optimal usage of the API
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
	   strcpy(prefix, "PERFORMANCE:");
    };
    // Informal messages that may become handy during debugging
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
    {
	   strcpy(prefix, "INFO:");
    }
    // Diagnostic info from the Vulkan loader and layers
    // Usually not helpful in terms of API usage, but may help to debug layer and loader problems 
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
    {
	   strcpy(prefix, "DEBUG:");
    }

    // Display message to default output (console/logcat)
    char debugMessage[4096];
    platform_Sprintf(debugMessage, "%s [%s] Code %d:%s", prefix, pLayerPrefix, messageCode, pMessage);


    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
	   platform_Fprintf(stderr, "%s\n", debugMessage);
    }
    else
    {
	   platform_Printf("%s\n", debugMessage);
    }

#ifdef _WIN64
    OutputDebugStringA(debugMessage);
    OutputDebugStringA("\n");
#endif

    fflush(stdout);
    // The return value of this callback controls wether the Vulkan call that caused
    // the validation message will be aborted or not
    // We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message 
    // (and return a VkResult) to abort
    // If you instead want to have calls abort, pass in VK_TRUE and the function will 
    // return VK_ERROR_VALIDATION_FAILED_EXT 
    return VK_FALSE;
}

static inline VkDebugReportCallbackEXT vk_createDebugCallback(VkAllocationCallbacks* allocator, VkInstance instance, VkDebugReportFlagsEXT flags, PFN_vkDebugReportCallbackEXT callback)
{
    VkDebugReportCallbackCreateInfoEXT createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.pNext = nullptr;
    createInfo.flags = flags;
    createInfo.pfnCallback = callback;
    createInfo.pUserData = nullptr;

    VkDebugReportCallbackEXT callbackHandle = nullptr;
    VKCHECK(vkCreateDebugReportCallbackEXT(instance, &createInfo, allocator, &callbackHandle));
    return callbackHandle;
}
#endif

static const char* vk_physicalDeviceTypeString(VkPhysicalDeviceType type)
{
	switch (type)
	{
#define _LOCAL_TOSTRING(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
		_LOCAL_TOSTRING(OTHER);
		_LOCAL_TOSTRING(INTEGRATED_GPU);
		_LOCAL_TOSTRING(DISCRETE_GPU);
		_LOCAL_TOSTRING(VIRTUAL_GPU);
#undef _LOCAL_TOSTRING
		default: return "UNKNOWN_DEVICE";
	}
}

static inline VkPhysicalDevice vk_pickPhysicalDevice(VkInstance instance)
{
    u32 physicalDeviceCount = 0;
    VKCHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));
    assert(physicalDeviceCount > 0);
    VkPhysicalDevice physicalDevices[physicalDeviceCount];
    VKCHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices));
	VkPhysicalDeviceProperties properties[physicalDeviceCount];
	VkPhysicalDeviceFeatures features[physicalDeviceCount];

    // TODO: formalize
	platform_Printf("Listing available GPUs\n----------------------\n");

    for (u32 i = 0; i < physicalDeviceCount; i++)
    {
		vkGetPhysicalDeviceProperties(physicalDevices[i], &properties[i]);
		vkGetPhysicalDeviceFeatures(physicalDevices[i], &features[i]);

		platform_Printf("Device %u: %s\n", i, properties[i].deviceName);
		platform_Printf("\tType: %s\n", vk_physicalDeviceTypeString(properties[i].deviceType));
		platform_Printf("\tAPI: %u.%u.%u\n",
			VK_VERSION_MAJOR(properties[i].apiVersion),
			VK_VERSION_MINOR(properties[i].apiVersion),
			VK_VERSION_PATCH(properties[i].apiVersion));
	}

    return physicalDevices[0];
}

static inline u32 vk_getQueueFamilyIndex(VkQueueFlags queueFlags, const VkQueueFamilyProperties* queueFamilyProperties, u32 queueFamilyPropertyArraysize)
{
    // Dedicated queue for compute
    // Try to find a queue family index that supports compute but not graphics
    if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	   for (u32 i = 0; i < queueFamilyPropertyArraysize; i++)
		  if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			 return i;

    // Dedicated queue for transfer
    // Try to find a queue family index that supports transfer but not graphics and compute
    if (queueFlags & VK_QUEUE_TRANSFER_BIT)
	   for (u32 i = 0; i < queueFamilyPropertyArraysize; i++)
		  if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			 return i;

    // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
    for (u32 i = 0; i < queueFamilyPropertyArraysize; i++)
	   if (queueFamilyProperties[i].queueFlags & queueFlags)
		  return i;

    assert(!"Couldn't find a matching queue");
    return 0;
}

// WARNING: pQueuePriorities might go out of scope!
static inline void vk_setupQueueCreation(VkDeviceQueueCreateInfo* queueCreateInfoArray, queue_family* queueFamily, VkQueueFlags requestedQueueTypes, const float* defaultQueuePriorities)
{
    u32 currentIndex = 0;
    
    // Graphics queue
    if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
    {
	   queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS] =
		  vk_getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT, queueFamily->properties, queueFamily->propertyCount);
	   VkDeviceQueueCreateInfo queueInfo;
	   queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	   queueInfo.pNext = nullptr;
	   queueInfo.flags = 0;
	   queueInfo.queueFamilyIndex = queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS];
	   queueInfo.queueCount = 1;
	   queueInfo.pQueuePriorities = defaultQueuePriorities;
	   platform_Memcpy(&queueCreateInfoArray[currentIndex++], &queueInfo, sizeof(queueInfo));
    }
    else
    {
	   queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS] = VK_NULL_HANDLE;
    }

    // Compute queue
    if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
    {
	   queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_COMPUTE] =
		  vk_getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT, queueFamily->properties, queueFamily->propertyCount);
	   if (queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_COMPUTE] != queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS])
	   {
		  VkDeviceQueueCreateInfo queueInfo;
		  queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		  queueInfo.pNext = nullptr;
		  queueInfo.flags = 0;
		  queueInfo.queueFamilyIndex = queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_COMPUTE];
		  queueInfo.queueCount = 1;
		  queueInfo.pQueuePriorities = defaultQueuePriorities;
		  platform_Memcpy(&queueCreateInfoArray[currentIndex++], &queueInfo, sizeof(queueInfo));
	   }
    }
    else
    {
	   queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_COMPUTE] = queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS];
    }

    // Transfer queue
    if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
    {
	   queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_TRANSFER] =
		  vk_getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT, queueFamily->properties, queueFamily->propertyCount);
	   if ((queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_TRANSFER] != queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS])
		  && (queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_TRANSFER] != queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_COMPUTE]))
	   {
		  VkDeviceQueueCreateInfo queueInfo;
		  queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		  queueInfo.pNext = nullptr;
		  queueInfo.flags = 0;
		  queueInfo.queueFamilyIndex = queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_TRANSFER];
		  queueInfo.queueCount = 1;
		  queueInfo.pQueuePriorities = defaultQueuePriorities;
		  platform_Memcpy(&queueCreateInfoArray[currentIndex++], &queueInfo, sizeof(queueInfo));
	   }
    }
    else
    {
	   queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_TRANSFER] = queueFamily->indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS];
    }
}

static inline VkDevice vk_createDevice(VkAllocationCallbacks* allocator, VkPhysicalDevice physicalDevice, swapchain_properties* swapchainProperties, VkQueueFlags queuesToCreate, u32 queueCount)
{    
    VkDeviceQueueCreateInfo queues[queueCount];
	float defaultQueuePriorities[] = { 1.0f };
	vk_setupQueueCreation(queues, &swapchainProperties->queueFamily, VK_QUEUE_GRAPHICS_BIT, defaultQueuePriorities);

    //const char* enabledLayers[] =
    //{
////#if _DEBUG
		//VK_KHR_VALIDATION_LAYER_NAME,
//#endif // _DEBUG
    //};

    const char* enabledExtensions[] =
	{	 
	   VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

	VkDeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.pNext = 0;
	const float priority[] = { 1.0f };
	queueCreateInfo.pQueuePriorities = priority;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.queueFamilyIndex = 0;
	queueCreateInfo.flags = 0;

	VkDeviceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.queueCreateInfoCount = 1;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.enabledLayerCount = 0; 
	createInfo.ppEnabledLayerNames = 0;
	createInfo.enabledExtensionCount = ArrayCount(enabledExtensions);
	createInfo.ppEnabledExtensionNames = enabledExtensions;
	createInfo.pEnabledFeatures = &swapchainProperties->features;

    VkDevice device = nullptr;
    VKCHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

    return device;
}

static inline void vk_getQueues(VkQueue* queueArray, VkDevice device, queue_family* queueFamily)
{	
	// TODO: this is now getting all three types of queues. Modify to widen use
	for (enum Vk_QueueFamilyIndex familyIndex = 0; familyIndex < QUEUE_FAMILY_INDEX_COUNT; familyIndex++)
	{
		VkQueue queue;
		vkGetDeviceQueue(device, queueFamily->indices[familyIndex], 0, &queue); queueArray[familyIndex] = queue;
	}
}

static inline VkSurfaceFormatKHR vk_pickSurfaceFormat(const VkSurfaceFormatKHR* surfaceFormats, u32 surfaceFormatCount)
{
    VkSurfaceFormatKHR pickedSurfaceFormat = {0};
    if (surfaceFormatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
    {
	   pickedSurfaceFormat.format = VK_FORMAT_UNDEFINED;
	   pickedSurfaceFormat.colorSpace = surfaceFormats[0].colorSpace;
	   return pickedSurfaceFormat;
    }
    else
    {
	   const VkFormat desiredFormat = VK_FORMAT_B8G8R8A8_UNORM;
	   for (u32 i = 0; i < surfaceFormatCount; i++)
	   {
		  VkSurfaceFormatKHR surfaceFormat = surfaceFormats[i];
		  if (surfaceFormat.format == desiredFormat)
		  {
			 pickedSurfaceFormat = surfaceFormat;
			 return pickedSurfaceFormat;
		  }
	   }
    }

    // Simply pick the first one
    pickedSurfaceFormat = surfaceFormats[0];
    return pickedSurfaceFormat;
}

static inline VkSurfaceKHR vk_createSurface(VkAllocationCallbacks* allocator, VkInstance instance, platform_dependencies* window)
{
#if VK_USE_PLATFORM_WIN32_KHR
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.hinstance = window->anotherOSHandle;
    createInfo.hwnd = window->windowHandle;

        VkSurfaceKHR surface;
    VKCHECK(vkCreateWin32SurfaceKHR(instance, &createInfo, allocator, &surface));

#else
#include <GLFW/glfw3.h>
    VkSurfaceKHR surface;
    GLFWwindow* window2 = null;
    glfwCreateWindowSurface(instance, window2, allocator, &surface);
#endif


    return surface;
}

static inline void vk_fillSwapchainProperties(swapchain_properties* swapchainProperties, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    u32 queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);
    assert(queueFamilyPropertyCount > 0);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, swapchainProperties->queueFamily.properties);
    swapchainProperties->queueFamily.propertyCount = queueFamilyPropertyCount;

    vkGetPhysicalDeviceFeatures(physicalDevice, &swapchainProperties->features);
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &swapchainProperties->memoryProperties);
    vkGetPhysicalDeviceProperties(physicalDevice, &swapchainProperties->physicalDeviceProperties);

    VkBool32 surfaceSupportsPresentation;
    VKCHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &surfaceSupportsPresentation));
    assert(surfaceSupportsPresentation);

    VKCHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapchainProperties->surfaceCapabilities));

    u32 surfaceFormatCount = 0;
    VKCHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr));
    assert(surfaceFormatCount > 0);
    //DEBUG_COUNTER("surfaceFormatCount:", surfaceFormatCount);
    VKCHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, swapchainProperties->surfaceFormats));

    u32 presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    assert(presentModeCount > 0);
    //DEBUG_COUNTER("presentModeCount:", presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapchainProperties->presentModes);
}

static inline VkPresentModeKHR vk_pickPresentMode(const VkPresentModeKHR* presentModes, u32 presentModeCount)
{
    VkPresentModeKHR pickedPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    for (u32 i = 0; i < presentModeCount; i++)
    {
	   VkPresentModeKHR presentMode = presentModes[i];
	   if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
	   {
		  pickedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		  return pickedPresentMode;
	   }
	   if ((pickedPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR))
	   {
		  pickedPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	   }
    }

    return pickedPresentMode;
}

static inline u32 vk_pickImageCount(VkSurfaceCapabilitiesKHR* surfaceCapabilities)
{
    u32 imageCount = surfaceCapabilities->minImageCount + 1;

    if (surfaceCapabilities->maxImageCount > 0 &&
	   imageCount > surfaceCapabilities->maxImageCount)
    {
	   imageCount = surfaceCapabilities->maxImageCount;
    }

    return imageCount;
}

static inline VkExtent2D vk_pickImageExtent(VkSurfaceCapabilitiesKHR* surfaceCapabilities, VkExtent2D* desiredExtent)
{
    VkExtent2D currentExtent = surfaceCapabilities->currentExtent;

    // If the current extent is defined
    if (currentExtent.width != UINT32_MAX)
    {
	   VkExtent2D imageExtent = currentExtent;
	   return imageExtent;
    }
    else
    {
	   VkExtent2D imageExtent;
	   imageExtent.width = desiredExtent->width;
	   imageExtent.height = desiredExtent->height;

	   VkExtent2D minImageExtent = surfaceCapabilities->minImageExtent;
	   VkExtent2D maxImageExtent = surfaceCapabilities->maxImageExtent;

	   if (imageExtent.width < minImageExtent.width)
	   {
		  imageExtent.width = minImageExtent.width;
	   }
	   if (imageExtent.width > maxImageExtent.width)
	   {
		  imageExtent.width = maxImageExtent.width;
	   }
	   if (imageExtent.height < minImageExtent.height)
	   {
		  imageExtent.height = minImageExtent.height;
	   }
	   if (imageExtent.height > maxImageExtent.height)
	   {
		  imageExtent.height = maxImageExtent.height;
	   }

	   return imageExtent;
    }
}

static inline VkImageUsageFlags vk_getImageUsage(VkImageUsageFlags supportedImageUsageFlags, VkImageUsageFlags desiredUsages)
{
    VkImageUsageFlags imageUsage = desiredUsages & supportedImageUsageFlags;
    assert(desiredUsages == imageUsage);
    return imageUsage;
}

static inline VkSurfaceTransformFlagsKHR vk_pickImageTransformation(VkSurfaceCapabilitiesKHR* surfaceCapabilities, VkSurfaceTransformFlagsKHR desiredTransform)
{
    VkSurfaceTransformFlagsKHR surfaceTransform;
    if (surfaceCapabilities->supportedTransforms & desiredTransform)
    {
	   surfaceTransform = desiredTransform;
    }
    else
    {
	   surfaceTransform = surfaceCapabilities->currentTransform;
    }

    return surfaceTransform;
}

typedef struct
{
    VkSurfaceFormatKHR surfaceFormat;
    VkExtent2D imageExtent;
    VkPresentModeKHR presentMode;
    VkImageUsageFlags imageUsage;
    VkSurfaceTransformFlagsKHR surfaceTransform;
    u32 minImageCount;
}
swapchain_requirements;

static inline void vk_fillSwapchainRequirements(swapchain_requirements* swapchainRequirements, swapchain_properties* swapchainProperties, VkExtent2D* desiredSwapchainExtent)
{
    swapchainRequirements->surfaceFormat = vk_pickSurfaceFormat(swapchainProperties->surfaceFormats, ArrayCount(swapchainProperties->surfaceFormats));
    swapchainRequirements->imageExtent = vk_pickImageExtent(&swapchainProperties->surfaceCapabilities, desiredSwapchainExtent);
    swapchainRequirements->presentMode = vk_pickPresentMode(swapchainProperties->presentModes, ArrayCount(swapchainProperties->presentModes));
    swapchainRequirements->imageUsage = vk_getImageUsage(swapchainProperties->surfaceCapabilities.supportedUsageFlags, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    swapchainRequirements->surfaceTransform = vk_pickImageTransformation(&swapchainProperties->surfaceCapabilities, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR); // TODO: investigate further
    swapchainRequirements->minImageCount = vk_pickImageCount(&swapchainProperties->surfaceCapabilities);
}

static inline VkSwapchainKHR vk_createSwapchain(VkAllocationCallbacks* allocator, VkDevice device, VkSurfaceKHR surface, swapchain_requirements* swapchainRequirements, VkSwapchainKHR oldSwapchain)
{
    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.surface = surface;
    createInfo.minImageCount = swapchainRequirements->minImageCount;
    createInfo.imageFormat = swapchainRequirements->surfaceFormat.format;
    createInfo.imageColorSpace = swapchainRequirements->surfaceFormat.colorSpace;
    createInfo.imageExtent = swapchainRequirements->imageExtent;
    createInfo.imageArrayLayers = 1; // TODO: investigate further
    createInfo.imageUsage = swapchainRequirements->imageUsage;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // TODO: investigate further
    createInfo.queueFamilyIndexCount = 0; // TODO: investigate further
    createInfo.pQueueFamilyIndices = nullptr; // TODO: investigate further
    createInfo.preTransform = swapchainRequirements->surfaceTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // TODO: investigate further
    createInfo.presentMode = swapchainRequirements->presentMode;
    createInfo.clipped = VK_TRUE; // TODO: investigate further
    createInfo.oldSwapchain = oldSwapchain;
    
    VkSwapchainKHR swapchain;
    VKCHECK(vkCreateSwapchainKHR(device, &createInfo, allocator, &swapchain));

    return swapchain;
}

static inline void vk_getSwapchainImages(VkImage* images, VkDevice device, VkSwapchainKHR swapchain)
{
	u32 imageCount = 0;
	VKCHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
	assert(imageCount > 0);
	VKCHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images));
}

static inline u32 vk_acquireNextImage(VkDevice device, VkSwapchainKHR swapchain, VkSemaphore semaphore)
{
	u32 imageIndex;
	VKCHECK(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, semaphore, nullptr, &imageIndex));
	return imageIndex;
}

static inline VkCommandPool vk_createCommandPool(VkAllocationCallbacks* allocator, VkDevice device, u32 queueFamilyIndex)
{
	// TODO: maybe take a look at these options
	VkCommandPoolCreateFlags commandPoolCreateFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VkCommandPoolCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = commandPoolCreateFlags;
	createInfo.queueFamilyIndex = queueFamilyIndex;

	VkCommandPool commandPool;
	VKCHECK(vkCreateCommandPool(device, &createInfo, allocator, &commandPool));

	return commandPool;
}

static inline void vk_createCommandBuffers(VkDevice device, VkCommandPool commandPool, u32 commandBufferCount, VkCommandBuffer* commandBufferArray)
{
	// TODO: maybe take a look at this
	VkCommandBufferLevel cmdBufferLvl = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkCommandBufferAllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = cmdBufferLvl;
	commandBufferAllocateInfo.commandBufferCount = commandBufferCount;

	VkCommandBuffer commandBuffersLocal[commandBufferCount];
	VKCHECK(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffersLocal));
	platform_Memcpy(commandBufferArray, commandBuffersLocal, sizeof(VkCommandBuffer) * commandBufferCount);
}

static inline void vk_beginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags commandBufferUsage, VkRenderPass renderPass, u32 subpass, VkFramebuffer framebuffer, VkQueryPipelineStatisticFlags pipelineStatistics)
{
	VkCommandBufferInheritanceInfo cbInheritanceInfo;
	cbInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	cbInheritanceInfo.pNext = nullptr;
	cbInheritanceInfo.renderPass = renderPass;		// can be nullptr
	cbInheritanceInfo.subpass = subpass;			// ignored if renderPass == nullptr
	cbInheritanceInfo.framebuffer = framebuffer;	// can be nullptr
	cbInheritanceInfo.occlusionQueryEnable = false; // TODO: check this?
	cbInheritanceInfo.queryFlags = 0;				// TODO: maybe error?
	cbInheritanceInfo.pipelineStatistics = pipelineStatistics;

	VkCommandBufferBeginInfo cbBeginInfo;
	cbBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cbBeginInfo.pNext = nullptr;
	cbBeginInfo.flags = commandBufferUsage;
	cbBeginInfo.pInheritanceInfo = &cbInheritanceInfo;

	VKCHECK(vkBeginCommandBuffer(commandBuffer, &cbBeginInfo));
}

static inline void vk_endCommandBuffer(VkCommandBuffer commandBuffer)
{
	VKCHECK(vkEndCommandBuffer(commandBuffer));
}

static inline void vk_resetCommandBuffer(VkCommandBuffer commandBuffer)
{
	// TODO: actually not releasing the memory back to the command pool
	VkCommandBufferResetFlags resetFlags = 0;
	VKCHECK(vkResetCommandBuffer(commandBuffer, resetFlags));
}

static inline void vk_resetCommandPool(VkCommandPool commandPool, VkDevice device)
{
	// TODO: actually not releasing the memory 
	VkCommandPoolResetFlags resetFlags = 0;
	VKCHECK(vkResetCommandPool(device, commandPool, resetFlags));
}

static inline VkSemaphore vk_createSemaphoreSignaled(VkAllocationCallbacks* allocator, VkDevice device)
{
	VkSemaphoreCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	createInfo.pNext = nullptr;
	VkSemaphore semaphore;
	VKCHECK(vkCreateSemaphore(device, &createInfo, allocator, &semaphore));

	return semaphore;
}

static inline VkSemaphore vk_createSemaphoreUnsignaled(VkAllocationCallbacks* allocator, VkDevice device)
{
	VkSemaphoreCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pNext = nullptr;
	VkSemaphore semaphore;
	VKCHECK(vkCreateSemaphore(device, &createInfo, allocator, &semaphore));

	return semaphore;
}

static inline VkFence vk_createFenceSignaled(VkAllocationCallbacks* allocator, VkDevice device)
{
	VkFenceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	VkFence fence;
	VKCHECK(vkCreateFence(device, &createInfo, allocator, &fence));

	return fence;
}

static inline VkFence vk_createFenceUnsignaled(VkAllocationCallbacks* allocator, VkDevice device)
{
	VkFenceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	VkFence fence;
	VKCHECK(vkCreateFence(device, &createInfo, allocator, &fence));

	return fence;
}

static inline void vk_waitForFences(VkDevice device, VkFence* fenceArray, u32 fenceCount, u64 timeout, VkBool32 waitForAll)
{
	// TODO: better handle return value	(VK_TIMEOUT in case of error)
	VKCHECK(vkWaitForFences(device, fenceCount, fenceArray, waitForAll, timeout));
}

// Semaphores automatically are reset. Fences are not
static inline void vk_resetFences(VkDevice device, VkFence* fenceArray, u32 fenceCount)
{
	VKCHECK(vkResetFences(device, fenceCount, fenceArray));
}

// WARNING: Make sure that none of these command buffers is currently processed by the device, or were recorded with a VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT flag.
static inline void vk_submitCommandsToQueue(VkSemaphore* waitSemaphoreArray, u32 waitSemaphoreCount, VkPipelineStageFlags* pipelineStageArray,
	VkQueue queue, VkCommandBuffer* commandBufferArray, u32 commandBufferCount,
	VkSemaphore* signalSemaphoreArray, u32 signalSemaphoreCount, VkFence fence) // fence can be nullptr
{
	// TODO: I have based all the computation in the premise that the element count of all elements
	// are the same, stored in the variable semaphoreCount. This may be wrong. Check.
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = waitSemaphoreCount;
	submitInfo.pWaitSemaphores = waitSemaphoreArray;
	submitInfo.pWaitDstStageMask = pipelineStageArray;
	submitInfo.commandBufferCount = commandBufferCount;
	submitInfo.pCommandBuffers = commandBufferArray;
	submitInfo.signalSemaphoreCount = signalSemaphoreCount;
	submitInfo.pSignalSemaphores = signalSemaphoreArray;

	// TODO: Maybe handle the magical constant submitCount to do a more asynchronous work
#define SUBMITCOUNT 1
	VKCHECK(vkQueueSubmit(queue, SUBMITCOUNT, &submitInfo, fence));
}

// In line with TODO comments in previous function, we should batch command submission to queues: HOW TO?

//	We shouldn't submit command buffers if they were already submitted and their execution
//	hasn't ended yet. We can do this only when command buffers were recorded with a
//	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT flag, but we should avoid using
//	this flag for performance reasons.
static inline void vk_synchronizeTwoCommandBuffers(VkSemaphore* semaphoreArray1, u32 semaphoreArray1Count, VkPipelineStageFlags* pipelineStageArray1,
	VkQueue queue1, VkCommandBuffer* commandBufferArray1, u32 commandBufferCount1,
	VkSemaphore* synchronizingSemaphoreArray, u32 synchronizingSemaphoreCount, VkPipelineStageFlags* synchronizingPipelineStageArray,
	VkQueue queue2, VkCommandBuffer* commandBufferArray2, u32 commandBufferArray2Count,
	VkSemaphore* signalSemaphoreArray, u32 signalSemaphoreCount, VkFence fence)
{

	// TODO: maybe these three lines are unnecessary? Remove if so.
	u32 firstSignalSemaphoreCount = synchronizingSemaphoreCount;
	VkSemaphore firstSignalSemaphoreArray[firstSignalSemaphoreCount];
	platform_Memcpy(firstSignalSemaphoreArray, synchronizingSemaphoreArray, sizeof(VkSemaphore) * firstSignalSemaphoreCount);

	vk_submitCommandsToQueue(semaphoreArray1, semaphoreArray1Count, pipelineStageArray1,
		queue1, commandBufferArray1, commandBufferCount1,
		firstSignalSemaphoreArray, firstSignalSemaphoreCount, VK_NULL_HANDLE);

	vk_submitCommandsToQueue(synchronizingSemaphoreArray, synchronizingSemaphoreCount, synchronizingPipelineStageArray,
		queue2, commandBufferArray2, commandBufferArray2Count,
		signalSemaphoreArray, signalSemaphoreCount, fence);
}

static inline void vk_waitForCommands(VkDevice device,
	VkSemaphore* waitSemaphoreArray, u32 waitSemaphoreCount, VkPipelineStageFlags* pipelineStageArray,
	VkQueue queue, VkCommandBuffer* commandBufferArray, u32 commandBufferCount,
	VkSemaphore* signalSemaphoreArray, u32 signalSemaphoreCount,
	VkFence fence, u64 timeout)
{
	vk_submitCommandsToQueue(waitSemaphoreArray, waitSemaphoreCount, pipelineStageArray,
		queue, commandBufferArray, commandBufferCount,
		signalSemaphoreArray, signalSemaphoreCount, fence);

	// don't wait for all
	vk_waitForFences(device, &fence, 1, timeout, false);
}

// TODO: change return value if needed
static inline void waitForQueue(VkQueue queue)
{
	VKCHECK(vkQueueWaitIdle(queue));
}

static inline void waitForDevice(VkDevice device)
{
	VKCHECK(vkDeviceWaitIdle(device));
}

// TODO: change it to work with different queues
static inline VkBuffer vk_createBuffer(VkAllocationCallbacks* allocator, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage)
{
	VkBufferCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0; // TODO: take a look at this!
	createInfo.size = size;
	createInfo.usage = usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // TODO: change in the future to support multiple queues!
	createInfo.queueFamilyIndexCount = 0; // TODO """
	createInfo.pQueueFamilyIndices = nullptr; // TODO """

	VkBuffer buffer;
	VKCHECK(vkCreateBuffer(device, &createInfo, allocator, &buffer));

	return buffer;
}

static inline u32 vk_findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDeviceMemoryProperties* memoryProperties)
{
	u32 memoryTypeCount = memoryProperties->memoryTypeCount;
	for (u32 i = 0; i < memoryTypeCount; i++)
		if (typeFilter & (1 << i) && (memoryProperties->memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	assert(!"Error");
	return ~0u;
}

static inline VkDeviceMemory vk_allocateAndBindToBuffer(VkAllocationCallbacks* allocator, VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags memoryProperties, VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties)
{
	VkMemoryRequirements bufferMemoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &bufferMemoryRequirements);

	VkDeviceMemory bufferMemory;

	u32 memoryTypeIndex = vk_findMemoryType(bufferMemoryRequirements.memoryTypeBits, memoryProperties, physicalDeviceMemoryProperties);

	VkMemoryAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = bufferMemoryRequirements.size;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;

	// TODO: maybe allocate a unique big chunk of memory instead of a piece every time we create a buffer
	VKCHECK(vkAllocateMemory(device, &allocateInfo, allocator, &bufferMemory));

	VKCHECK(vkBindBufferMemory(device, buffer, bufferMemory, 0));

	return bufferMemory;
}

static inline void vk_bufferMemoryBarrier( /* { */
	VkBuffer* bufferArray, u32 bufferCount,
	VkAccessFlags* currentAccessFlagArray, VkAccessFlags* newAccessFlagArray,
	u32* currentQueueFamilyArray, u32* newQueueFamilyArray,
	/* } */
	VkCommandBuffer commandBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages)
{
	VkBufferMemoryBarrier memoryBarrierArray[bufferCount];

	for (u32 i = 0; i < bufferCount; i++)
	{
		memoryBarrierArray[i] = (VkBufferMemoryBarrier)
		{
			VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			nullptr,
			currentAccessFlagArray[i],
			newAccessFlagArray[i],
			currentQueueFamilyArray[i],
			newQueueFamilyArray[i],
			bufferArray[i],
			0,
			VK_WHOLE_SIZE
		};
	}

	// TODO: check buffer memory barrier array count
	vkCmdPipelineBarrier(commandBuffer, generatingStages, consumingStages, 0, 0, nullptr, bufferCount, memoryBarrierArray, 0, nullptr);
}

static inline VkBufferView vk_createBufferView(VkAllocationCallbacks* allocator, VkDevice device, VkBuffer buffer, VkFormat bufferFormat, VkDeviceSize memoryOffset, VkDeviceSize memoryRange)
{
	VkBufferViewCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0; // TODO: take a look at this
	createInfo.buffer = buffer;
	createInfo.format = bufferFormat;
	createInfo.offset = memoryOffset;
	createInfo.range = memoryRange;
	
	VkBufferView bufferView;
	VKCHECK(vkCreateBufferView(device, &createInfo, allocator, &bufferView));

	return bufferView;
}

static inline VkImage vk_createImage(VkAllocationCallbacks* allocator, VkDevice device,
	VkImageType type, VkFormat format, VkExtent3D* extent, u32 mipmapLevelCount, u32 layerCount, VkSampleCountFlagBits samples, VkImageUsageFlags usage, bool renderCube)
{
	VkImageCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = renderCube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0; // TODO: look at this in the future
	createInfo.imageType = type;
	createInfo.format = format;
	createInfo.extent = *extent;
	createInfo.mipLevels = mipmapLevelCount;
	createInfo.arrayLayers = layerCount;
	createInfo.samples = samples;
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL; // TODO ????? is this all we want? PS: optimal is the best option for performance (but what if we want to use linear for some reason?)
	createInfo.usage = usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	// TODO: obviously take a look at this into the future to support multiple queue family indices
	createInfo.queueFamilyIndexCount = 0;				// TODO: obviously take a look at this into the future to support multiple queue family indices
	createInfo.pQueueFamilyIndices = nullptr;			// TODO: obviously take a look at this into the future to support multiple queue family indices
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // ?? TODO: maybe this can be a hit on performance?

	VkImage image;
	VKCHECK(vkCreateImage(device, &createInfo, allocator, &image));

	return image;
}


// TODO:
/*
	Similarly to binding memory objects to buffers, we should allocate bigger memory objects
	and bind parts of them to multiple images.This way, we perform fewer memory allocations
	and the driver has to track a smaller number of memory objects.This may improve the
	performance of our application.It may also allow us to save some memory, as each
	allocation may require more memory than requested during allocation(in other words, its
	size may always be rounded up to a multiple of the memory page size).Allocating bigger
	memory objects and reusing parts of them for multiple images spares us the wasted area.
*/
static inline VkDeviceMemory vk_allocateAndBindToImage(VkAllocationCallbacks* allocator, VkDevice device, VkImage image,
	VkMemoryPropertyFlags memoryPropertyFlags, VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties)
{
	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(device, image, &memoryRequirements);

	VkDeviceMemory imageMemory;
	u32 memoryTypeIndex = vk_findMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags, physicalDeviceMemoryProperties);

	VkMemoryAllocateInfo allocateInfo;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;

	VKCHECK(vkAllocateMemory(device, &allocateInfo, allocator, &imageMemory));
	VKCHECK(vkBindImageMemory(device, image, imageMemory, 0));

	return imageMemory;
}

static inline void vk_imageMemoryBarrier( /* { */
	VkImage* imageArray, u32 imageCount,
	VkAccessFlags* currentAccessFlagArray, VkAccessFlags* newAccessFlagArray,
	VkImageLayout* currentLayoutArray, VkImageLayout* newLayoutArray,
	u32* currentQueueFamilyArray, u32* newQueueFamilyArray,
	VkImageAspectFlags* aspectArray,
	/* } */
	VkCommandBuffer commandBuffer, VkPipelineStageFlags generatingStages, VkPipelineStageFlags consumingStages)
{
	VkImageMemoryBarrier memoryBarrierArray[imageCount];

	for (u32 i = 0; i < imageCount; i++)
	{
		memoryBarrierArray[i] = (VkImageMemoryBarrier)
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			currentAccessFlagArray[i],
			newAccessFlagArray[i],
			currentLayoutArray[i],
			newLayoutArray[i],
			currentQueueFamilyArray[i],
			newQueueFamilyArray[i],
			imageArray[i],
		(VkImageSubresourceRange){aspectArray[i], 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS }// TODO:  modify???
		};
	}

	// TODO: check image memory barrier array count
	vkCmdPipelineBarrier(commandBuffer, generatingStages, consumingStages, 0, 0, nullptr, 0, nullptr, imageCount, memoryBarrierArray);
}

static inline VkImageView vk_createImageView(VkAllocationCallbacks* allocator, VkDevice device, VkImage image, VkImageViewType type, VkFormat format, VkImageAspectFlags aspect)
{
	VkImageViewCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0; // TODO: modify into the future?
	createInfo.image = image;
	createInfo.viewType = type;
	createInfo.format = format;
	// todo: take a look at these two last options
	createInfo.components = (VkComponentMapping){ VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	createInfo.subresourceRange = (VkImageSubresourceRange){ aspect, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS };

	VkImageView imageView;
	VKCHECK(vkCreateImageView(device, &createInfo, allocator, &imageView));

	return imageView;
}

typedef struct
{
	VkImage image;
	VkDeviceMemory memory;
	VkImageView view;
} VulkanImage;

static inline VulkanImage vk_create2DImage(VkAllocationCallbacks* allocator, VkDevice device, VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties,
	VkFormat format, VkExtent2D* extent2D, u32 mipmapLevelCount, u32 layerCount, VkSampleCountFlagBits sampleCount, VkImageUsageFlags usage, VkImageAspectFlags aspect, bool renderCube)
{
	VkExtent3D extent = (VkExtent3D){ extent2D->width, extent2D->height, 1 };
	VkImage image = vk_createImage(allocator, device, VK_IMAGE_TYPE_2D, format, &extent, mipmapLevelCount, layerCount, sampleCount, usage, renderCube);
	VkDeviceMemory memory = vk_allocateAndBindToImage(allocator, device, image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDeviceMemoryProperties);
	VkImageView imageView = vk_createImageView(allocator, device, image, VK_IMAGE_VIEW_TYPE_2D, format, aspect);

	return (VulkanImage) { image, memory, imageView };
}

static inline VulkanImage vk_createLayered2DImageWithCubemapView(VkAllocationCallbacks* allocator, VkDevice device, VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties,
	u32 sideExtent, u32 mipmapLevelCount, VkImageUsageFlags usage, VkImageAspectFlags aspect)
{
	VkExtent3D extent = (VkExtent3D){ sideExtent, sideExtent, 1 };
	VkImage image = vk_createImage(allocator, device, VK_IMAGE_TYPE_2D, VK_FORMAT_R8G8B8A8_UNORM, &extent, mipmapLevelCount, 6, VK_SAMPLE_COUNT_1_BIT, usage, true);
	VkDeviceMemory memory = vk_allocateAndBindToImage(allocator, device, image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDeviceMemoryProperties);
	VkImageView imageView = vk_createImageView(allocator, device, image, VK_IMAGE_VIEW_TYPE_CUBE, VK_FORMAT_R8G8B8A8_UNORM, aspect);

	return (VulkanImage) { image, memory, imageView };
}

static inline void vk_mapAndCopyMemoryToDevice(VkDevice device, VkDeviceMemory hostVisibleMemory, VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap, void** pointerToDeviceLocalMemory)
{
	void* pointerToMemory;
	// VkMemoryMapFlags memoryMapFlags = 0; // TODO: take a look at this in the future
	VKCHECK(vkMapMemory(device, hostVisibleMemory, offset, dataSize, 0, &pointerToMemory));
	platform_Memcpy(pointerToMemory, data, dataSize);

	VkMappedMemoryRange memoryRanges[] =
	{
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			hostVisibleMemory,
			offset,
			VK_WHOLE_SIZE
		}
	};

	VKCHECK(vkFlushMappedMemoryRanges(device, ArrayCount(memoryRanges), memoryRanges));

	if (unmap)
	{
		vkUnmapMemory(device, hostVisibleMemory);
	}
	else if (pointerToDeviceLocalMemory != nullptr)
	{
		*pointerToDeviceLocalMemory = pointerToMemory;
	}
}

// WARNING: Command buffer must be in recording state.
static inline void vk_copyDataBetweenBuffers(VkCommandBuffer commandBuffer,
	VkBuffer dstBuffer, VkBuffer srcBuffer,
	u32 bufferRegionCount, VkBufferCopy* bufferRegionArray)
{
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, bufferRegionCount, bufferRegionArray);
}

// WARNING: Command buffer must be in recording state.
static inline void vk_copyDataFromBufferToImage(VkCommandBuffer commandBuffer,
	VkBuffer buffer, VkImage image,	VkImageLayout currentImageLayout,
	u32 bufferRegionCount, VkBufferImageCopy* bufferRegionArray)
{
	vkCmdCopyBufferToImage(commandBuffer, buffer, image, currentImageLayout, bufferRegionCount, bufferRegionArray);
}

// WARNING: Command buffer must be in recording state.
static inline void vk_copyDataFromImageToBuffer(VkCommandBuffer commandBuffer,
	VkImage image, VkImageLayout currentImageLayout, VkBuffer buffer,	
	u32 copyRegionCount, VkBufferImageCopy* regionArray)
{
	vkCmdCopyImageToBuffer(commandBuffer, image, currentImageLayout, buffer, copyRegionCount, regionArray);
}

/*
	In real-life scenarios, we should use an existing buffer and reuse it as a staging buffer as
	many times as possible to avoid unnecessary buffer creation and destruction operations.
	This way, we also avoid waiting on a fence.
*/	

// BIG TODO: CLEANUP POINTER CASTING
static inline void vk_updateDeviceLocalMemoryBufferThroughStagingBuffer(VkAllocationCallbacks* allocator, VkDevice device, VkPhysicalDeviceMemoryProperties* memoryProperties,
	VkCommandBuffer commandBuffer, VkQueue queue, VkBuffer dstBuffer,
	VkDeviceSize dstOffset, VkDeviceSize dataSize, void* data,
	VkAccessFlags dstBufferCurrentAccess, VkAccessFlags dstBufferNewAccess,
	VkPipelineStageFlags dstBufferGeneratingStages, VkPipelineStageFlags dstBufferConsumingStages,
	VkSemaphore* signalSemaphoreArray, u32 signalSemaphoreCount)
{
	VkBuffer stagingBuffer = vk_createBuffer(allocator, device, dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	VkDeviceMemory stagingBufferMemory = vk_allocateAndBindToBuffer(allocator, device, stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memoryProperties);
	vk_mapAndCopyMemoryToDevice(device, stagingBufferMemory, 0, dataSize, data, true, nullptr);
	vk_beginCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr, 0, nullptr, 0);
	
	vk_bufferMemoryBarrier(&dstBuffer, 1, &dstBufferCurrentAccess,
		&(VkAccessFlags) {VK_ACCESS_TRANSFER_WRITE_BIT}, &(u32) { VK_QUEUE_FAMILY_IGNORED }, &(u32) { VK_QUEUE_FAMILY_IGNORED },
		commandBuffer, dstBufferGeneratingStages, VK_PIPELINE_STAGE_TRANSFER_BIT);

	VkBufferCopy bufferCopy = (VkBufferCopy){0, dstOffset, dataSize};
	vk_copyDataBetweenBuffers(commandBuffer, dstBuffer, stagingBuffer, 1, &bufferCopy);

	vk_bufferMemoryBarrier(&dstBuffer, 1,
		&(VkAccessFlags) { VK_ACCESS_TRANSFER_WRITE_BIT }, &dstBufferNewAccess,
		&(u32) { VK_QUEUE_FAMILY_IGNORED }, &(u32) { VK_QUEUE_FAMILY_IGNORED },
		commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, dstBufferConsumingStages);
	
	vk_endCommandBuffer(commandBuffer);

	VkFence fence = vk_createFenceUnsignaled(allocator, device);

	vk_submitCommandsToQueue(nullptr, 0, 0, queue, &(VkCommandBuffer) { commandBuffer }, 1, signalSemaphoreArray, signalSemaphoreCount, fence);

	vk_waitForFences(device, &(VkFence) { fence }, 1, UINT64_MAX, false);

	vkDestroyFence(device, fence, allocator);
	vkFreeMemory(device, stagingBufferMemory, allocator);
	vkDestroyBuffer(device, stagingBuffer, allocator);
}

static inline void vk_updateDeviceLocalMemoryImageThroughStagingBuffer(VkAllocationCallbacks* allocator, VkDevice device, VkPhysicalDeviceMemoryProperties* memoryProperties,
	VkCommandBuffer commandBuffer, VkQueue queue, VkImage dstImage,
	/*VkDevice dstOffset,*/ VkDeviceSize dataSize, void* data,
	VkImageLayout dstImageCurrentLayout, VkImageLayout dstImageNewLayout,
	VkAccessFlags dstImageCurrentAccess, VkAccessFlags dstImageNewAccess,
	VkPipelineStageFlags dstImageGeneratingStages, VkPipelineStageFlags dstImageConsumingStages,
	VkImageAspectFlags dstImageAspect,
	VkImageSubresourceLayers* dstImageSubresource , VkOffset3D* dstImageOffset, VkExtent3D* dstImageExtent,
	VkSemaphore* signalSemaphoreArray, u32 signalSemaphoreCount)
{
	VkBuffer stagingBuffer = vk_createBuffer(allocator, device, dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	VkDeviceMemory stagingBufferMemory = vk_allocateAndBindToBuffer(allocator, device, stagingBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memoryProperties);
	vk_mapAndCopyMemoryToDevice(device, stagingBufferMemory, 0, dataSize, data, true, nullptr);
	vk_beginCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, nullptr, 0, nullptr, 0);

	vk_imageMemoryBarrier(&dstImage, 1,
		&dstImageCurrentAccess, &(VkAccessFlags) { VK_ACCESS_TRANSFER_WRITE_BIT },
		& dstImageCurrentLayout, &(VkImageLayout) { VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL },
		&(u32) { VK_QUEUE_FAMILY_IGNORED }, &(u32) { VK_QUEUE_FAMILY_IGNORED },
		& dstImageAspect,
		commandBuffer,
		dstImageGeneratingStages, VK_PIPELINE_STAGE_TRANSFER_BIT);

	VkBufferImageCopy imageCopy = { 0, 0, 0, *dstImageSubresource, *dstImageOffset, *dstImageExtent };
	vk_copyDataFromBufferToImage(commandBuffer, stagingBuffer, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

	vk_imageMemoryBarrier(&dstImage, 1,
		&(VkAccessFlags) { VK_ACCESS_TRANSFER_WRITE_BIT }, &dstImageNewAccess,
		(VkImageLayout*) VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &dstImageNewLayout,
		&(u32) { VK_QUEUE_FAMILY_IGNORED }, &(u32) { VK_QUEUE_FAMILY_IGNORED },
		&dstImageAspect,
		commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, dstImageConsumingStages);
	
	vk_endCommandBuffer(commandBuffer);

	VkFence fence = vk_createFenceUnsignaled(allocator, device);

	vk_submitCommandsToQueue(nullptr, 0, 0, queue, &(VkCommandBuffer) { commandBuffer }, 1, signalSemaphoreArray, signalSemaphoreCount, fence);

	vk_waitForFences(device, &(VkFence) { fence }, 1, UINT64_MAX, false);

	vkDestroyFence(device, fence, allocator);
	vkFreeMemory(device, stagingBufferMemory, allocator);
	vkDestroyBuffer(device, stagingBuffer, allocator);
}

/*
	Descriptors are opaque data structures that represent shader resources.
	They are organized
	into groups or sets and their contents are specified by descriptor set layouts. To provide
	resources to shaders, we bind descriptor sets to pipelines. We can bind multiple sets at once.
	To access resources from within shaders, we need to specify from which set and from which
	location within a set (called a binding) the given resource is acquired.
*/

static inline VkSampler vk_createSampler(VkAllocationCallbacks* allocator, VkDevice device,
	VkFilter magFilter, VkFilter minFilter, VkSamplerMipmapMode mipmapMode,
	VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w,
	float miploadBias, bool anisotropyEnable, float maxAnisotropy, 
	bool compareEnable, VkCompareOp compareOp,
	float minLOD, float maxLOD,
	VkBorderColor borderColor, bool unnormalizedCoordinates)
{
	VkSamplerCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0; // TODO
	createInfo.magFilter = magFilter;
	createInfo.minFilter = minFilter;
	createInfo.mipmapMode = mipmapMode;
	createInfo.addressModeU = u;
	createInfo.addressModeV = v;
	createInfo.addressModeW = w;
	createInfo.mipLodBias = miploadBias;
	createInfo.anisotropyEnable = anisotropyEnable;
	createInfo.maxAnisotropy = maxAnisotropy;
	createInfo.compareEnable = compareEnable;
	createInfo.compareOp = compareOp;
	createInfo.minLod = minLOD;
	createInfo.maxLod = maxLOD;
	createInfo.borderColor = borderColor;
	createInfo.unnormalizedCoordinates = unnormalizedCoordinates;

	VkSampler sampler;
	VKCHECK(vkCreateSampler(device, &createInfo, allocator, &sampler));

	return sampler;
}


static inline void vk_presentImage(VkQueue graphicsQueue, VkSemaphore* semaphoreArray, u32 semaphoreCount, VkSwapchainKHR* swapchainArray, u32 swapchainCount, u32* imageIndexArray)
{
	VkResult result;
	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.waitSemaphoreCount = semaphoreCount;
	presentInfo.pWaitSemaphores = semaphoreArray;
	presentInfo.swapchainCount = swapchainCount;
	presentInfo.pSwapchains = swapchainArray;
	presentInfo.pImageIndices = imageIndexArray;
	presentInfo.pResults = &result;

	VKCHECK(vkQueuePresentKHR(graphicsQueue, &presentInfo));
}

void vk_load(vulkan_renderer* vk, platform_dependencies* window)
{
    VkAllocationCallbacks* allocator = nullptr;

    VkQueueFlags availableQueues[] = { VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT };
    VkQueueFlags queuesToCreate = 0;
    u32 availableQueueCount = ArrayCount(availableQueues);
    for (u32 i = 0; i < availableQueueCount; i++)
    {
	   queuesToCreate |= availableQueues[i];
    }
	
    // Using volk code to load Vulkan function pointers
    VKCHECK(volkInitialize());
    vk->instance = vk_createInstance(allocator);
    volkLoadInstance(vk->instance);
    // Here ends all Volk code usage
#if _DEBUG
	VkDebugReportFlagsEXT debugCallbackFlags =
		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		
    //vk->debugCallback = vk_createDebugCallback(allocator, vk->instance, debugCallbackFlags, vk_debugCallback);
#endif
    vk->physicalDevice = vk_pickPhysicalDevice(vk->instance);
	
     vk->surface = vk_createSurface(allocator, vk->instance, window);
    vk_fillSwapchainProperties(&vk->swapchainProperties, vk->physicalDevice, vk->surface);
    vk->device = vk_createDevice(allocator, vk->physicalDevice, &vk->swapchainProperties, queuesToCreate, availableQueueCount);
//	vk_getQueues(vk->queues, vk->device, &vk->swapchainProperties.queueFamily);
//    swapchain_requirements swapchainRequirements;
//    vk_fillSwapchainRequirements(&swapchainRequirements, &vk->swapchainProperties, surfaceExtent);
//    vk->swapchain = vk_createSwapchain(allocator, vk->device, vk->surface, &swapchainRequirements, nullptr /* TODO: investigate this further*/);
//	vk_getSwapchainImages(vk->swapchainImages, vk->device, vk->swapchain);
//	VkSemaphore imageAcquireSemaphore = vk_createSemaphoreUnsignaled(allocator, vk->device);
//	vk->commandPool = vk_createCommandPool(allocator, vk->device, vk->swapchainProperties.queueFamily.indices[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS]);
//	vk_createCommandBuffers(vk->device, vk->commandPool, IMAGE_COUNT, vk->commandBuffers);
//	u32 currentImageIndex = vk_acquireNextImage(vk->device, vk->swapchain, imageAcquireSemaphore);
//
//	vk_presentImage(vk->queues[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS], &imageAcquireSemaphore, 1, &vk->swapchain, 1, &currentImageIndex);
//
//
//	// TODO: TEMPORAL STUFF TO BE MOVED AWAY
//	vkDestroySemaphore(vk->device, imageAcquireSemaphore, allocator);
}

void vk_destroy(vulkan_renderer* vk)
{
    VkAllocationCallbacks* allocator = nullptr;
	vkFreeCommandBuffers(vk->device, vk->commandPool, IMAGE_COUNT, vk->commandBuffers);
	vkDestroyCommandPool(vk->device, vk->commandPool, allocator);
    vkDestroySwapchainKHR(vk->device, vk->swapchain, allocator);
    vkDestroyDevice(vk->device, nullptr);
    vkDestroySurfaceKHR(vk->instance, vk->surface, allocator);
#if _DEBUG
    vkDestroyDebugReportCallbackEXT(vk->instance, vk->debugCallback, allocator);
#endif
    vkDestroyInstance(vk->instance, allocator);
}
