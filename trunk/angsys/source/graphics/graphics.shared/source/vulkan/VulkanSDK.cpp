#include "pch.h"
#include "vulkan/VulkanSDK.hpp"

#if VULKAN_SUPPORT

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

class vulkan_sdk_internal
	: public object
	, public singleton<vulkan_sdk_internal>
{
private:
	friend singleton<vulkan_sdk_internal>;
	pointer _library;
	VkInstance _vk_instance;

public:
	pointer library()const { return _library; }
	VkInstance vk_instance()const { return _vk_instance; }
	void vk_instance(VkInstance value) { _vk_instance = value; }

private:
	vulkan_sdk_internal();
	virtual~vulkan_sdk_internal();
};

inline bool operator == (const VkDeviceQueueCreateInfo&, const VkDeviceQueueCreateInfo&) { return false; }
inline bool operator != (const VkDeviceQueueCreateInfo&, const VkDeviceQueueCreateInfo&) { return false; }

ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkReallocationFunction(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkFreeFunction(void* pUserData, void* pMemory);
ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkInternalAllocationNotification(void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope) {}
ANG_EXTERN void VKAPI_PTR ang_graphics_vulkan_vkVoidFunction(void) {}

ANG_EXTERN void* VKAPI_PTR ang_graphics_vulkan_vkAllocationFunction(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
{
	//auto allocator = static_cast<memory::ialigned_allocator*>(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
	auto allocator = reinterpret_cast<memory::ialigned_allocator*>(pUserData);

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
	auto allocator = reinterpret_cast<memory::ialigned_allocator*>(pUserData);
	allocator->memory_release(pMemory);
}




#define VK_EXPORTED_FUNCTION(_FUNC_NAME) \
	typename _vk_callback_type<VulkanSDK,PFN_##_FUNC_NAME>::type VulkanSDK::_FUNC_NAME; \
	static typename _vk_callback_type<VulkanSDK,PFN_##_FUNC_NAME>::full_type s_##_FUNC_NAME##_callback = null

#define VK_GLOBAL_FUNCTION(_FUNC_NAME) \
	typename _vk_callback_type<VulkanSDK,PFN_##_FUNC_NAME>::type VulkanSDK::_FUNC_NAME; \
	static typename _vk_callback_type<VulkanSDK,PFN_##_FUNC_NAME>::full_type s_##_FUNC_NAME##_callback = null

#define VK_INSTANCE_FUNCTION(_FUNC_NAME) \
	typename _vk_callback_type<VulkanSDK,PFN_##_FUNC_NAME>::type VulkanSDK::_FUNC_NAME; \
	static typename _vk_callback_type<VulkanSDK,PFN_##_FUNC_NAME>::full_type s_##_FUNC_NAME##_callback = null

#define VK_DEVICE_FUNCTION(_FUNC_NAME)

#include "vulkan/VulkanSDK.inl"

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION
#undef VK_DEVICE_FUNCTION


template<>
vulkan_sdk_internal* ang::singleton<vulkan_sdk_internal>::instance()
{
	if (_instance == null) {
		new vulkan_sdk_internal();
	}
	return static_cast<vulkan_sdk_internal*>(_instance);
}

template<>
bool ang::singleton<vulkan_sdk_internal>::release_instance()
{
	if (_instance && _instance->release() == 0)
		_instance = null;
	return true;
}


vulkan_sdk_internal::vulkan_sdk_internal()
	: _library(null)
	, _vk_instance(null)
{
#if defined WINDOWS_PLATFORM
	_library = LoadLibraryW(L"vulkan-1.dll");
#elif defined LINUX_PLATFORM
	static void* _library = dlopen("libvulkan.so", RTLD_NOW);
#endif

	if (_library == null)
		throw exception_t(except_code::unsupported);
}

vulkan_sdk_internal::~vulkan_sdk_internal()
{
	if (_vk_instance != null)
		VulkanSDK::vkDestroyInstance(_vk_instance, null);
	_vk_instance = null;

	if (_library != null)
		ang_unload_library(_library);
	_library = null;
}

VkInstance VulkanSDK::Instance()
{
	if (!vulkan_sdk_internal::is_instanced())
		return null;
	return vulkan_sdk_internal::instance()->vk_instance();
}


VkInstance VulkanSDK::Initialize()
{
	vulkan_sdk_internal* sdk = vulkan_sdk_internal::instance();
	sdk->add_ref();
	VkInstance _vk_instance = sdk->vk_instance();

	if (_vk_instance != null)
		return _vk_instance;
	try
	{
#define VK_EXPORTED_FUNCTION(_FUNC_NAME) _FUNC_NAME._callback = & s_##_FUNC_NAME##_callback;  s_##_FUNC_NAME##_callback.set((PFN_##_FUNC_NAME)ang_load_proc_address(sdk->library(), #_FUNC_NAME))
#define VK_GLOBAL_FUNCTION(_FUNC_NAME) _FUNC_NAME._callback  = & s_##_FUNC_NAME##_callback;  s_##_FUNC_NAME##_callback.set((PFN_##_FUNC_NAME)vkGetInstanceProcAddr(null, #_FUNC_NAME))
#define VK_INSTANCE_FUNCTION(_FUNC_NAME)
#define VK_DEVICE_FUNCTION(_FUNC_NAME)

#include "vulkan/VulkanSDK.inl"

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION
#undef VK_DEVICE_FUNCTION

		uint extensions_count = 0;
		if ((VulkanSDK::vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr) != VK_SUCCESS) || (extensions_count == 0))
			throw exception_t(except_code::unsupported);

		array_view<VkExtensionProperties> available_extensions = array_view<VkExtensionProperties>::new_array(extensions_count);

		if (VulkanSDK::vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]) != VK_SUCCESS)\
		{
			array_view<VkExtensionProperties>::delete_array(available_extensions);
			throw exception_t(except_code::unsupported);
		}

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
			for (index i = 0; i < available_extensions.size(); ++i)
				if (strings::algorithms::string_compare(available_extensions[i].extensionName, extension_name) == 0)
					return true;
			return false;
		};

		for (index i = 0; i < array_size(extensions); ++i)
		{
			if (!CheckExtensionAvailability(extensions[i]))
			{
				array_view<VkExtensionProperties>::delete_array(available_extensions);
				throw exception_t(except_code::unsupported);
			}
		}

		array_view<VkExtensionProperties>::delete_array(available_extensions);

		VkApplicationInfo application_info = {
			VK_STRUCTURE_TYPE_APPLICATION_INFO,             // VkStructureType            sType
			nullptr,                                        // const void                *pNext
			"ang::graphics::vulkan",					// const char                *pApplicationName
			VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   applicationVersion
			"ang::graphics::vulkan",                     // const char                *pEngineName
			VK_MAKE_VERSION(1, 0, 0),                     // uint32_t                   engineVersion
			VK_API_VERSION_1_0
		};

		VkInstanceCreateInfo instance_create_info = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,         // VkStructureType            sType
			nullptr,                                        // const void*                pNext
			0,                                              // VkInstanceCreateFlags      flags
			&application_info,                              // const VkApplicationInfo   *pApplicationInfo
			0,                                              // uint32_t                   enabledLayerCount
			nullptr,                                        // const char * const        *ppEnabledLayerNames
			array_size(extensions),                         // uint32_t                   enabledExtensionCount
			extensions                                      // const char * const        *ppEnabledExtensionNames
		};

		if (vkCreateInstance(&instance_create_info, null, &_vk_instance) != VK_SUCCESS)
			throw exception_t(except_code::unsupported);

		sdk->vk_instance(_vk_instance);

#define VK_EXPORTED_FUNCTION(_FUNC_NAME)
#define VK_GLOBAL_FUNCTION(_FUNC_NAME)
#define VK_INSTANCE_FUNCTION(_FUNC_NAME) _FUNC_NAME._callback = & s_##_FUNC_NAME##_callback;  s_##_FUNC_NAME##_callback.set((PFN_##_FUNC_NAME)vkGetInstanceProcAddr(_vk_instance, #_FUNC_NAME))
#define VK_DEVICE_FUNCTION(_FUNC_NAME)

#include "vulkan/VulkanSDK.inl"

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION
#undef VK_DEVICE_FUNCTION

		return _vk_instance;
	}
	catch (exception_t)
	{
		Release();
		return null;
	}
}

void VulkanSDK::Release()
{
	vulkan_sdk_internal::release_instance();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////



inline bool GetPhysicalDevice(VkInstance instance, VkPhysicalDevice& physical_device, uint& queue_family_index)
{
	physical_device = null;

	uint device_count = 0;
	if (VulkanSDK::vkEnumeratePhysicalDevices(instance, &device_count, null) != VK_SUCCESS) {
		return false;
	}

	array_view<VkPhysicalDevice> physical_devices = array_view<VkPhysicalDevice>::new_array(device_count);

	if (VulkanSDK::vkEnumeratePhysicalDevices(instance, &device_count, &physical_devices[0]) != VK_SUCCESS) {
		array_view<VkPhysicalDevice>::delete_array(physical_devices);
		return false;
	}

	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures   device_features;


	for (index i = 0; i < device_count; ++i)
	{
		VulkanSDK::vkGetPhysicalDeviceProperties(physical_devices[i], &device_properties);
		VulkanSDK::vkGetPhysicalDeviceFeatures(physical_devices[i], &device_features);

		uint major_version = VK_VERSION_MAJOR(device_properties.apiVersion);
		uint minor_version = VK_VERSION_MINOR(device_properties.apiVersion);
		uint patch_version = VK_VERSION_PATCH(device_properties.apiVersion);


		if ((major_version > 1) || (device_properties.limits.maxImageDimension2D >= 4096))
		{
			uint queue_families_count = 0;
			VulkanSDK::vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_families_count, null);
			if (queue_families_count == 0)
				continue;

			queue_family_index = -1;
			array_view<VkQueueFamilyProperties> queue_family_properties = array_view<VkQueueFamilyProperties>::new_array(queue_families_count);

			VulkanSDK::vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_families_count, &queue_family_properties[0]);
			for (uint j = 0; j < queue_families_count; ++j)
			{
				if ((queue_family_properties[j].queueCount > 0) &&
					(queue_family_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT))
				{

					physical_device = physical_devices[i];
					queue_family_index = j;

					array_view<VkQueueFamilyProperties>::delete_array(queue_family_properties);
					array_view<VkPhysicalDevice>::delete_array(physical_devices);
					return true;
				}
			}
			array_view<VkQueueFamilyProperties>::delete_array(queue_family_properties);
		}
	}
	array_view<VkPhysicalDevice>::delete_array(physical_devices);
	return false;
}

inline bool GetPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice& physical_device, uint& graphics_queue_family_index, uint& present_queue_family_index)
{
	physical_device = null;

	uint device_count = 0;
	if (VulkanSDK::vkEnumeratePhysicalDevices(instance, &device_count, null) != VK_SUCCESS) {
		return false;
	}

	array_view<VkPhysicalDevice> physical_devices = array_view<VkPhysicalDevice>::new_array(device_count);

	if (VulkanSDK::vkEnumeratePhysicalDevices(instance, &device_count, &physical_devices[0]) != VK_SUCCESS) {
		array_view<VkPhysicalDevice>::delete_array(physical_devices);
		return false;
	}

	VkPhysicalDeviceProperties device_properties;
	VkPhysicalDeviceFeatures   device_features;


	for (index i = 0; i < device_count; ++i)
	{
		VulkanSDK::vkGetPhysicalDeviceProperties(physical_devices[i], &device_properties);
		VulkanSDK::vkGetPhysicalDeviceFeatures(physical_devices[i], &device_features);

		uint major_version = VK_VERSION_MAJOR(device_properties.apiVersion);
		uint minor_version = VK_VERSION_MINOR(device_properties.apiVersion);
		uint patch_version = VK_VERSION_PATCH(device_properties.apiVersion);


		if ((major_version > 1) || (device_properties.limits.maxImageDimension2D >= 4096))
		{
			uint extensions_count = 0;
			if ((VulkanSDK::vkEnumerateDeviceExtensionProperties(physical_devices[i], null, &extensions_count, null) != VK_SUCCESS) || (extensions_count == 0))
				continue;


			array_view<VkExtensionProperties> available_extensions = array_view<VkExtensionProperties>::new_array(extensions_count);
			if (VulkanSDK::vkEnumerateDeviceExtensionProperties(physical_devices[i], null, &extensions_count, &available_extensions[0]) != VK_SUCCESS)
			{
				array_view<VkExtensionProperties>::delete_array(available_extensions);
				continue;
			}

			auto CheckExtensionAvailability = [&](cstr_t extension_name) ->bool
			{
				for (index i = 0; i < available_extensions.size(); ++i)
					if (available_extensions[i].extensionName == extension_name)
						return true;
				return false;
			};

			if (!CheckExtensionAvailability(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) {
				array_view<VkExtensionProperties>::delete_array(available_extensions);
				continue;
			}

			uint queue_families_count = 0;
			VulkanSDK::vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_families_count, null);
			if (queue_families_count == 0)
				continue;

			graphics_queue_family_index = -1;
			present_queue_family_index = -1;
			array_view<VkQueueFamilyProperties> queue_family_properties = array_view<VkQueueFamilyProperties>::new_array(queue_families_count);
			array_view<uint> queue_present_support = array_view<uint>::new_array(queue_families_count);
			

			VulkanSDK::vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_families_count, &queue_family_properties[0]);
			for (uint j = 0; j < queue_families_count; ++j)
			{
				VulkanSDK::vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices[i], j, surface, &queue_present_support[j]);

				if ((queue_family_properties[j].queueCount > 0) &&
					(queue_family_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT))
				{
					if (graphics_queue_family_index == UINT32_MAX)
						graphics_queue_family_index = j;

					if (queue_present_support[j])
					{
						physical_device = physical_devices[i];
						graphics_queue_family_index = j;
						present_queue_family_index = j;

						array_view<uint>::delete_array(queue_present_support);
						array_view<VkQueueFamilyProperties>::delete_array(queue_family_properties);
						array_view<VkPhysicalDevice>::delete_array(physical_devices);
						return true;
					}
				}
			}


			for (index j = 0; j < queue_families_count; ++j)
			{
				if (queue_present_support[j])
				{
					physical_device = physical_devices[i];
					present_queue_family_index = j;
					array_view<uint>::delete_array(queue_present_support);
					array_view<VkQueueFamilyProperties>::delete_array(queue_family_properties);
					array_view<VkPhysicalDevice>::delete_array(physical_devices);
					return true;
				}
			}

			array_view<uint>::delete_array(queue_present_support);
			array_view<VkQueueFamilyProperties>::delete_array(queue_family_properties);
		}
	}
	array_view<VkPhysicalDevice>::delete_array(physical_devices);
	return false;
}


value<graphics::vulkan::_VKDevice>::value()
{
	_value._graphics_queue_family_index = -1;
	_value._present_queue_family_index = -1;
	_value._device = null;
	auto instance = VulkanSDK::Initialize();

	VkPhysicalDevice physical_device = null;
	uint queue_family_index = 0;
	if (!GetPhysicalDevice(instance, physical_device, queue_family_index)) {
		VulkanSDK::Release();
		throw exception_t(except_code::unsupported);
	}

	collections::vector<float> queue_priorities = { 1.0f };

	VkDeviceQueueCreateInfo queue_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
		nullptr,                                        // const void                  *pNext
		0,                                              // VkDeviceQueueCreateFlags     flags
		queue_family_index,								// uint32_t                     queueFamilyIndex
		queue_priorities->size(),						// uint32_t                     queueCount
		&queue_priorities[0]                            // const float                 *pQueuePriorities
	};

	VkDeviceCreateInfo device_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           // VkStructureType                    sType
		nullptr,                                        // const void                        *pNext
		0,                                              // VkDeviceCreateFlags                flags
		1,                                              // uint32_t                           queueCreateInfoCount
		&queue_create_info,                             // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
		0,                                              // uint32_t                           enabledLayerCount
		nullptr,                                        // const char * const                *ppEnabledLayerNames
		0,                                              // uint32_t                           enabledExtensionCount
		nullptr,                                        // const char * const                *ppEnabledExtensionNames
		nullptr                                         // const VkPhysicalDeviceFeatures    *pEnabledFeatures
	};

	VkAllocationCallbacks allocation_callbacks = {
		static_cast<memory::ialigned_allocator*>(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator)),
		&ang_graphics_vulkan_vkAllocationFunction,
		&ang_graphics_vulkan_vkReallocationFunction,
		&ang_graphics_vulkan_vkFreeFunction,
		null,
		null
	};

	if (VulkanSDK::vkCreateDevice(physical_device, &device_create_info, &allocation_callbacks, &_value._device) != VK_SUCCESS)
	{
		VulkanSDK::Release();
		throw exception_t(except_code::unsupported);
	}
	_value._graphics_queue_family_index = queue_family_index;

	_value.vkGetDeviceProcAddr._callback = new _vk_device_callback_type<value<_VKDevice>, PFN_vkGetDeviceProcAddr>::full_type(
		_value._device, (PFN_vkGetDeviceProcAddr)VulkanSDK::vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr"));

#define VK_EXPORTED_FUNCTION(_FUNC_NAME)
#define VK_GLOBAL_FUNCTION(_FUNC_NAME)
#define VK_INSTANCE_FUNCTION(_FUNC_NAME)
#define VK_DEVICE_FUNCTION(_FUNC_NAME) \
	_value._FUNC_NAME._callback = new _vk_device_callback_type<value<_VKDevice>, PFN_##_FUNC_NAME>::full_type( \
			_value._device, (PFN_##_FUNC_NAME)_value.vkGetDeviceProcAddr(#_FUNC_NAME));

#include "vulkan/VulkanSDK.inl"

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION	
#undef VK_DEVICE_FUNCTION
}


value<graphics::vulkan::_VKDevice>::value(VkSurfaceKHR surface)
{
	_value._device = null;
	_value._graphics_queue_family_index = -1;
	_value._present_queue_family_index = -1;

	auto instance = VulkanSDK::Initialize();

	VkPhysicalDevice physical_device = null;
	uint graphics_queue_family_index = 0;
	uint present_queue_family_index = 0;
	if (!GetPhysicalDevice(instance, surface, physical_device, graphics_queue_family_index, present_queue_family_index)) {
		VulkanSDK::Release();
		throw exception_t(except_code::unsupported);
	}

	collections::vector<float> queue_priorities = { 1.0f };

	collections::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	queue_create_infos += VkDeviceQueueCreateInfo {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
		nullptr,                                        // const void                  *pNext
		0,                                              // VkDeviceQueueCreateFlags     flags
		graphics_queue_family_index,					// uint32_t                     queueFamilyIndex
		queue_priorities->size(),						// uint32_t                     queueCount
		&queue_priorities[0]                            // const float                 *pQueuePriorities
	};

	if (graphics_queue_family_index != present_queue_family_index)
	{
		queue_create_infos += VkDeviceQueueCreateInfo{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
			nullptr,                                        // const void                  *pNext
			0,                                              // VkDeviceQueueCreateFlags     flags
			present_queue_family_index,					// uint32_t                     queueFamilyIndex
			queue_priorities->size(),						// uint32_t                     queueCount
			&queue_priorities[0]                            // const float                 *pQueuePriorities
		};
	}

	const char* extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo device_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,           // VkStructureType                    sType
		nullptr,                                        // const void                        *pNext
		0,                                              // VkDeviceCreateFlags                flags
		queue_create_infos->size(),                     // uint32_t                           queueCreateInfoCount
		&queue_create_infos[0],                         // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
		0,                                              // uint32_t                           enabledLayerCount
		nullptr,                                        // const char * const                *ppEnabledLayerNames
		array_size(extensions),						    // uint32_t                           enabledExtensionCount
		extensions,                                     // const char * const                *ppEnabledExtensionNames
		nullptr                                         // const VkPhysicalDeviceFeatures    *pEnabledFeatures
	};

	VkAllocationCallbacks allocation_callbacks = {
		static_cast<memory::ialigned_allocator*>(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator)),
		&ang_graphics_vulkan_vkAllocationFunction,
		&ang_graphics_vulkan_vkReallocationFunction,
		&ang_graphics_vulkan_vkFreeFunction,
		null,
		null
	};

	if (VulkanSDK::vkCreateDevice(physical_device, &device_create_info, &allocation_callbacks, &_value._device) != VK_SUCCESS)
	{
		VulkanSDK::Release();
		throw exception_t(except_code::unsupported);
	}

	_value._graphics_queue_family_index = graphics_queue_family_index;
	_value._present_queue_family_index = present_queue_family_index;

#define VK_EXPORTED_FUNCTION(_FUNC_NAME)
#define VK_GLOBAL_FUNCTION(_FUNC_NAME)
#define VK_INSTANCE_FUNCTION(_FUNC_NAME)
#define VK_DEVICE_FUNCTION(_FUNC_NAME) \
	_value._FUNC_NAME._callback = new _vk_device_callback_type<value<_VKDevice>, PFN_##_FUNC_NAME>::full_type( \
			_value._device, (PFN_##_FUNC_NAME)_value.vkGetDeviceProcAddr(#_FUNC_NAME));

	_value.vkGetDeviceProcAddr._callback = new _vk_device_callback_type<value<_VKDevice>, PFN_vkGetDeviceProcAddr>::full_type(
		_value._device, (PFN_vkGetDeviceProcAddr)VulkanSDK::vkGetInstanceProcAddr(instance, "vkGetDeviceProcAddr"));

#include "vulkan/VulkanSDK.inl"

#if defined(USE_SWAPCHAIN_EXTENSIONS)
	_value.vkQueuePresentKHR._callback = new _vk_device_callback_type<value<_VKDevice>, PFN_vkQueuePresentKHR>::full_type(
		 (PFN_vkQueuePresentKHR)VulkanSDK::vkGetDeviceProcAddr(_value._device, "vkQueuePresentKHR"));
#endif

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION	
#undef VK_DEVICE_FUNCTION
}


value<graphics::vulkan::_VKDevice>::~value()
{
	_value.vkDeviceWaitIdle();
	_value.vkDestroyDevice(null);

#define VK_DEVICE_FUNCTION(_FUNC_NAME) \
	auto _FUNC_NAME##_ptr = static_cast<_vk_device_callback_type<value<_VKDevice>, PFN_##_FUNC_NAME>::full_type*>(_value._FUNC_NAME._callback.get()); \
	_value._FUNC_NAME._callback = null; \
	delete _FUNC_NAME##_ptr;

	VK_DEVICE_FUNCTION(vkGetDeviceProcAddr);
#undef VK_DEVICE_FUNCTION
	VulkanSDK::Release();
}

value_wrapper<graphics::vulkan::_VKDevice>::value_wrapper() {}

value_wrapper<graphics::vulkan::_VKDevice>::value_wrapper(VkSurfaceKHR surface) : value(surface) {}

//value_wrapper(VkSurfaceKHR);

value_wrapper<graphics::vulkan::_VKDevice>::~value_wrapper() {}

ang::type_name_t ang::value_wrapper<graphics::vulkan::_VKDevice>::class_name() {
	return "ang::graphics::vulkan::vk_device"_s;
}

ang::type_name_t ang::value_wrapper<graphics::vulkan::_VKDevice>::object_name()const {
	return class_name();
}

bool ang::value_wrapper<graphics::vulkan::_VKDevice>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::value_wrapper<graphics::vulkan::_VKDevice>::class_name()
		|| object::is_kind_of(name)
		|| is_type_of<value<graphics::vulkan::_VKDevice>>(name))
		return true;
	return false;
}

bool ang::value_wrapper<graphics::vulkan::_VKDevice>::is_inherited_of(ang::type_name_t name)
{
	if (name == ang::value_wrapper<graphics::vulkan::_VKDevice>::class_name()
		|| object::is_inherited_of(name)
		|| is_type_of<value<graphics::vulkan::_VKDevice>>(name))
		return true;
	return false;
}

bool ang::value_wrapper<graphics::vulkan::_VKDevice>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::value_wrapper<graphics::vulkan::_VKDevice>::class_name())
	{
		*out = static_cast<ang::value_wrapper<graphics::vulkan::_VKDevice>*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	else if (className == type_of<graphics::vulkan::_VKDevice>())
	{
		*out = &value<graphics::vulkan::_VKDevice>::get();
		return true;
	}
	return false;
}

string value_wrapper<graphics::vulkan::_VKDevice>::to_string()const { return class_name(); }


#endif
