#pragma once
#include <angsys.h>
#define VK_NO_PROTOTYPES 
#define USE_SWAPCHAIN_EXTENSIONS
#ifdef WINDOWS_PLATFORM
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined LINUX_PLATFORM
//...
#endif

#include <vulkan\vulkan.h>
//#include <vulkan\vulkan_win32.h>

namespace ang
{
	namespace graphics
	{
		namespace vulkan
		{
			template<typename owner, typename T> struct VkFunction;
			template<typename owner, typename T> struct VkFunctionType;
			template<typename owner, typename T> struct VkObjectFunctionType;

			template<typename owner, typename T, typename... Ts>
			struct VkFunction<owner, T(Ts...)> {
				T operator ()(Ts... args)const {
					return function->invoke(ang::forward<Ts>(args)...);
				}
			private:
				core::delegates::method<T(Ts...)> function;
				friend owner;
			};

			template<typename owner, typename T, typename... Ts> 
			struct VkFunctionType<owner, T(VKAPI_PTR*)(Ts...)>
			{
				typedef VkFunction<owner, T(Ts...)> type;
				typedef core::delegates::method<T(Ts...)> intf_type;
				typedef core::delegates::static_function<T(VKAPI_PTR*)(Ts...), T, Ts...> full_type;
			};

			template<typename owner, typename T0, typename T1, typename... Ts>
			struct VkObjectFunctionType<owner, T0(VKAPI_PTR*)(T1, Ts...)>
			{
				typedef VkFunction<owner, T0(Ts...)> type;
				typedef core::delegates::method<T0(Ts...)> intf_type;
				typedef core::delegates::pseudo_member_function<T1, false, T0, Ts...> full_type;
			};


			typedef class VulkanDriver vk_driver;
			typedef object_wrapper<vk_driver> vk_driver_t;
			typedef object_wrapper_ptr<vk_driver> vk_driver_ptr_t;

			class VulkanDriver
				: public object
			{
			private:
				pointer m_hLibrary;
				VkInstance m_vkInstance;
				VkDevice m_vkDevice;	
				PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
				//PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

			public:
				VulkanDriver();
				VulkanDriver(pointer native_window);


			private:
				virtual~VulkanDriver();

		
#define VK_GLOBAL_FUNCTION(_FUNC_NAME) typename VkFunctionType<VulkanDriver, PFN_##_FUNC_NAME>::type _FUNC_NAME
#define VK_INSTANCE_FUNCTION(_FUNC_NAME) typename VkFunctionType<VulkanDriver, PFN_##_FUNC_NAME>::type _FUNC_NAME
#define VK_DEVICE_FUNCTION(_FUNC_NAME) typename VkObjectFunctionType<VulkanDriver, PFN_##_FUNC_NAME>::type _FUNC_NAME

			private:
				VK_GLOBAL_FUNCTION(vkCreateInstance);
				VK_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties);
				VK_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties);

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

				VK_DEVICE_FUNCTION(vkDestroyDevice);

			public:
			
				VK_DEVICE_FUNCTION(vkGetDeviceQueue);
				VK_DEVICE_FUNCTION(vkDeviceWaitIdle);
				VK_DEVICE_FUNCTION(vkAllocateMemory);
				VK_DEVICE_FUNCTION(vkFreeMemory);
				VK_DEVICE_FUNCTION(vkMapMemory);
				VK_DEVICE_FUNCTION(vkUnmapMemory);
				VK_DEVICE_FUNCTION(vkFlushMappedMemoryRanges);
				VK_DEVICE_FUNCTION(vkInvalidateMappedMemoryRanges);
				VK_DEVICE_FUNCTION(vkGetDeviceMemoryCommitment);
				VK_DEVICE_FUNCTION(vkBindBufferMemory);
				VK_DEVICE_FUNCTION(vkBindImageMemory);
				VK_DEVICE_FUNCTION(vkGetBufferMemoryRequirements);
				VK_DEVICE_FUNCTION(vkGetImageMemoryRequirements);
				VK_DEVICE_FUNCTION(vkGetImageSparseMemoryRequirements);
				VK_DEVICE_FUNCTION(vkCreateFence);
				VK_DEVICE_FUNCTION(vkDestroyFence);
				VK_DEVICE_FUNCTION(vkResetFences);
				VK_DEVICE_FUNCTION(vkGetFenceStatus);
				VK_DEVICE_FUNCTION(vkWaitForFences);
				VK_DEVICE_FUNCTION(vkCreateSemaphore);
				VK_DEVICE_FUNCTION(vkDestroySemaphore);
				VK_DEVICE_FUNCTION(vkCreateEvent);
				VK_DEVICE_FUNCTION(vkDestroyEvent);
				VK_DEVICE_FUNCTION(vkGetEventStatus);
				VK_DEVICE_FUNCTION(vkSetEvent);
				VK_DEVICE_FUNCTION(vkResetEvent);
				VK_DEVICE_FUNCTION(vkCreateQueryPool);
				VK_DEVICE_FUNCTION(vkDestroyQueryPool);
				VK_DEVICE_FUNCTION(vkGetQueryPoolResults);
				VK_DEVICE_FUNCTION(vkCreateBuffer);
				VK_DEVICE_FUNCTION(vkDestroyBuffer);
				VK_DEVICE_FUNCTION(vkCreateBufferView);
				VK_DEVICE_FUNCTION(vkDestroyBufferView);
				VK_DEVICE_FUNCTION(vkCreateImage);
				VK_DEVICE_FUNCTION(vkDestroyImage);
				VK_DEVICE_FUNCTION(vkGetImageSubresourceLayout);
				VK_DEVICE_FUNCTION(vkCreateImageView);
				VK_DEVICE_FUNCTION(vkDestroyImageView);
				VK_DEVICE_FUNCTION(vkCreateShaderModule);
				VK_DEVICE_FUNCTION(vkDestroyShaderModule);
				VK_DEVICE_FUNCTION(vkCreatePipelineCache);
				VK_DEVICE_FUNCTION(vkDestroyPipelineCache);
				VK_DEVICE_FUNCTION(vkGetPipelineCacheData);
				VK_DEVICE_FUNCTION(vkMergePipelineCaches);
				VK_DEVICE_FUNCTION(vkCreateGraphicsPipelines);
				VK_DEVICE_FUNCTION(vkCreateComputePipelines);
				VK_DEVICE_FUNCTION(vkDestroyPipeline);
				VK_DEVICE_FUNCTION(vkCreatePipelineLayout);
				VK_DEVICE_FUNCTION(vkDestroyPipelineLayout);
				VK_DEVICE_FUNCTION(vkCreateSampler);
				VK_DEVICE_FUNCTION(vkDestroySampler);
				VK_DEVICE_FUNCTION(vkCreateDescriptorSetLayout);
				VK_DEVICE_FUNCTION(vkDestroyDescriptorSetLayout);
				VK_DEVICE_FUNCTION(vkCreateDescriptorPool);
				VK_DEVICE_FUNCTION(vkDestroyDescriptorPool);
				VK_DEVICE_FUNCTION(vkResetDescriptorPool);
				VK_DEVICE_FUNCTION(vkAllocateDescriptorSets);
				VK_DEVICE_FUNCTION(vkFreeDescriptorSets);
				VK_DEVICE_FUNCTION(vkUpdateDescriptorSets);
				VK_DEVICE_FUNCTION(vkCreateFramebuffer);
				VK_DEVICE_FUNCTION(vkDestroyFramebuffer);
				VK_DEVICE_FUNCTION(vkCreateRenderPass);
				VK_DEVICE_FUNCTION(vkDestroyRenderPass);
				VK_DEVICE_FUNCTION(vkGetRenderAreaGranularity);
				VK_DEVICE_FUNCTION(vkCreateCommandPool);
				VK_DEVICE_FUNCTION(vkDestroyCommandPool);
				VK_DEVICE_FUNCTION(vkResetCommandPool);
				VK_DEVICE_FUNCTION(vkAllocateCommandBuffers);
				VK_DEVICE_FUNCTION(vkFreeCommandBuffers);

				// From extensions
#if defined(USE_SWAPCHAIN_EXTENSIONS)
				VK_DEVICE_FUNCTION(vkCreateSwapchainKHR);
				VK_DEVICE_FUNCTION(vkDestroySwapchainKHR);
				VK_DEVICE_FUNCTION(vkGetSwapchainImagesKHR);
				VK_DEVICE_FUNCTION(vkAcquireNextImageKHR);
#endif

#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION	
#undef VK_DEVICE_FUNCTION


			private:
				template<typename T> typename VkFunctionType<VulkanDriver, T>::intf_type GetGlobalProcAddr(cstr_t name)const {
					return new typename VkFunctionType<VulkanDriver, T>::full_type((T)vkGetInstanceProcAddr(null, name));
				}
				template<typename T> typename VkFunctionType<VulkanDriver, T>::intf_type GetInstanceProcAddr(cstr_t name)const {
					return new typename VkFunctionType<VulkanDriver, T>::full_type((T)vkGetInstanceProcAddr(m_vkInstance, name));
				}
				template<typename T> typename VkObjectFunctionType<VulkanDriver, T>::intf_type GetDeviceProcAddr(cstr_t name)const {
					return new typename VkObjectFunctionType<VulkanDriver, T>::full_type(reinterpret_cast<T>(vkGetDeviceProcAddr(m_vkDevice, name)));
				}

				void Initialize(pointer native_window);
				void Terminate();
				
			};
		}
	}
}