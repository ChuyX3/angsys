#include "pch.h"
#include "vulkan\vk_driver.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::vulkan;


#if defined WINDOWS_PLATFORM
#define ang_load_proc_address(lib, name) GetProcAddress((HMODULE)(lib), (name))
#define ang_unload_library(lib) FreeLibrary((HMODULE)(lib))
#elif defined LINUX_PLATFORM
#define ang_load_proc_address dlsym
#define ang_unload_library dlclose
#endif


ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkFreeFunction(void* pUserData, void* pMemory);
ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkInternalAllocationNotification(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope) {}
ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkVoidFunction(void) {}

ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
	//auto allocator = static_cast<memory::iraw_allocator*>(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
	auto allocator = reinterpret_cast<memory::iraw_allocator*>(pUserData);

	switch (allocationScope)
	{
#ifdef _DEBUG
	case VK_SYSTEM_ALLOCATION_SCOPE_COMMAND:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_COMMAND", VK_SYSTEM_ALLOCATION_SCOPE_COMMAND);
	case VK_SYSTEM_ALLOCATION_SCOPE_OBJECT:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_OBJECT", VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
	case VK_SYSTEM_ALLOCATION_SCOPE_CACHE:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_CACHE", VK_SYSTEM_ALLOCATION_SCOPE_CACHE);
	case VK_SYSTEM_ALLOCATION_SCOPE_DEVICE:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_DEVICE", VK_SYSTEM_ALLOCATION_SCOPE_DEVICE);
	case VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE", VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE);
	case VK_SYSTEM_ALLOCATION_SCOPE_RANGE_SIZE:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_RANGE_SIZE", VK_SYSTEM_ALLOCATION_SCOPE_RANGE_SIZE);
	case VK_SYSTEM_ALLOCATION_SCOPE_MAX_ENUM:
		return allocator->aligned_memory_alloc(size, alignment, "VK_SYSTEM_ALLOCATION_SCOPE_MAX_ENUM", VK_SYSTEM_ALLOCATION_SCOPE_MAX_ENUM);
#endif//_DEBUG
	default:
		return allocator->aligned_memory_alloc(size, alignment);
	}
}

ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
	ang_graphics_vulkan_vkFreeFunction(pUserData, pOriginal);
	return ang_graphics_vulkan_vkAllocationFunction(pUserData, size, alignment, allocationScope);
}

ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkFreeFunction(void* pUserData, void* pMemory)
{
	auto allocator = reinterpret_cast<memory::iraw_allocator*>(pUserData);
	allocator->memory_release(pMemory);
}

VulkanDriver::VulkanDriver()
	: m_hLibrary(NULL)
	, m_vkInstance(null)
	, m_vkDevice(null)
{
	Initialize(null);
}

VulkanDriver::VulkanDriver(pointer native_window)
	: m_hLibrary(NULL)
	, m_vkInstance(null)
	, m_vkDevice(null)
{
	Initialize(native_window);
}

VulkanDriver::~VulkanDriver()
{
	Terminate();
}

void VulkanDriver::Initialize(pointer native_window)
{
	m_hLibrary = LoadLibraryA("vulkan-1.dll");
	if (m_hLibrary == null)
		throw_exception(except_code::unsupported);
	vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress((HMODULE)m_hLibrary, "vkGetInstanceProcAddr");


#define VK_GLOBAL_FUNCTION(_FUNC_NAME) _FUNC_NAME.function = GetGlobalProcAddr<PFN_##_FUNC_NAME>(#_FUNC_NAME);
#define VK_INSTANCE_FUNCTION(_FUNC_NAME) _FUNC_NAME.function = GetInstanceProcAddr<PFN_##_FUNC_NAME>(#_FUNC_NAME);
#define VK_DEVICE_FUNCTION(_FUNC_NAME)_FUNC_NAME.function = GetDeviceProcAddr<PFN_##_FUNC_NAME>(#_FUNC_NAME);

	VK_GLOBAL_FUNCTION(vkCreateInstance);
	VK_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties);
	VK_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties);

	uint extensions_count = 0;
	if ((vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr) != VK_SUCCESS) || (extensions_count == 0))
		throw_exception(except_code::unsupported);

	scope_array<VkExtensionProperties> available_extensions(extensions_count);

	if (vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]) != VK_SUCCESS)\
		throw_exception(except_code::unsupported);

	const char* extensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XCB_KHR)
		VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#endif
	};

	auto CheckExtensionAvailability = [&](const char* extension_name) ->bool
	{
		for (windex i = 0; i < available_extensions.size(); ++i)
			if (text::encoder<text::encoding::ascii>::compare(available_extensions[i].extensionName, extension_name) == 0)
				return true;
		return false;
	};

	for (windex i = 0; i < algorithms::array_size(extensions); ++i)
	{
		if (!CheckExtensionAvailability(extensions[i]))
			throw_exception(except_code::unsupported);
	}

	available_extensions.clear();

	VkApplicationInfo application_info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,             // VkStructureType            sType
		nullptr,                                        // const void                *pNext
		"ang::graphics::vulkan",					    // const char                *pApplicationName
		VK_MAKE_VERSION(1, 0, 0),                       // uint32_t                   applicationVersion
		"ang::graphics::vulkan",                        // const char                *pEngineName
		VK_MAKE_VERSION(1, 0, 0),                       // uint32_t                   engineVersion
		VK_API_VERSION_1_0
	};

	VkInstanceCreateInfo instance_create_info = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,         // VkStructureType            sType
		nullptr,                                        // const void*                pNext
		0,                                              // VkInstanceCreateFlags      flags
		&application_info,                              // const VkApplicationInfo   *pApplicationInfo
		0,                                              // uint32_t                   enabledLayerCount
		nullptr,                                        // const char * const        *ppEnabledLayerNames
		algorithms::array_size(extensions),             // uint32_t                   enabledExtensionCount
		extensions                                      // const char * const        *ppEnabledExtensionNames
	};

	if (vkCreateInstance(&instance_create_info, null, &m_vkInstance) != VK_SUCCESS)
		throw_exception(except_code::unsupported);

	VK_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
	VK_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties);
	VK_INSTANCE_FUNCTION(vkCreateDevice);
	VK_INSTANCE_FUNCTION(vkGetDeviceProcAddr);
	VK_INSTANCE_FUNCTION(vkDestroyInstance);

#if defined(USE_SWAPCHAIN_EXTENSIONS)
	VK_INSTANCE_FUNCTION(vkDestroySurfaceKHR);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
	VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	VK_INSTANCE_FUNCTION(vkCreateWin32SurfaceKHR);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	VK_INSTANCE_FUNCTION(vkCreateXcbSurfaceKHR);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	VK_INSTANCE_FUNCTION(vkCreateXlibSurfaceKHR);
#endif//PLATFORM
#endif//USE_SWAPCHAIN_EXTENSIONS


#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION	
#undef VK_DEVICE_FUNCTION
}

void VulkanDriver::Terminate()
{
	vkDestroyInstance(m_vkInstance, null);
	FreeLibrary((HMODULE)m_hLibrary);
}



