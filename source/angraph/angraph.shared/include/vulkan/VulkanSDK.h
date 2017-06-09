#pragma once

#include <angsys.h>
#include <ang/platform/platform.h>

#if VULKAN_SUPPORT

#define VK_NO_PROTOTYPES
#define USE_SWAPCHAIN_EXTENSIONS

#ifdef WINDOWS_PLATFORM
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined LINUX_PLATFORM
//...
#endif
#include <vulkan.h>


namespace ang
{
	namespace graphics
	{
		namespace vulkan
		{
			template<typename owner_t, typename T>
			struct vk_callback;

			template<typename owner_t, typename T, typename... Ts>
			struct vk_callback<owner_t, T(Ts...)>
			{
				T operator() (Ts... arg)const { return _callback(arg...); }
			private:
				core::delegates::method<T(Ts...)> _callback;
				friend owner_t;
			};

			template<typename calleable_t, typename T, typename... Ts>
			class vk_static_method final
				: public core::delegates::ifunction<T(Ts...)>
			{
			public:
				typedef calleable_t function_type;

			private:
				function_type _function;

			public:
				inline vk_static_method(function_type f) : _function(f) { }
				inline virtual~vk_static_method() {}
				inline void set(function_type f) { _function = f; }
				inline T invoke(Ts... args)const override { return _function(ang::move(args)...); }
				inline core::delegates::ifunction<T(Ts...)>* clone()const override { return const_cast<vk_static_method*>(this); }
			};

			template<typename O, typename T, typename... Ts>
			class vk_object_method final
				: public core::delegates::ifunction<T(Ts...)>
			{
			public:
				typedef T(VKAPI_PTR*function_type)(O, Ts...);

			private:
				function_type _function;
				O _object;

			public:
				inline vk_object_method() : _function(null), _object(null) { }
				inline vk_object_method(O obj, function_type f) : _function(f), _object(obj) { }
				inline virtual~vk_object_method() {}
				inline void set(O obj) { _object = obj; }
				inline void set(function_type f) { _function = f; }
				inline T invoke(Ts... args)const override { return _function(_object, ang::move(args)...); }
				inline core::delegates::ifunction<T(Ts...)>* clone()const override { return const_cast<vk_object_method*>(this); }
			};

			template<class owner_t, class T>
			struct _vk_callback_type;
			template<class owner_t, class T>
			struct _vk_device_callback_type;

			template<class owner_t, class T, class... Ts>
			struct _vk_callback_type<owner_t, T(VKAPI_PTR*)(Ts...)>
			{
				typedef vk_callback<owner_t, T(Ts...)> type;
				typedef core::delegates::method<T(Ts...)> intf_type;
				typedef vk_static_method<T(VKAPI_PTR*)(Ts...), T, Ts...> full_type;
			};

			template<class owner_t, class T, class... Ts>
			struct _vk_device_callback_type<owner_t, T(VKAPI_PTR*)(Ts...)>
			{
				typedef vk_callback<owner_t, T(Ts...)> type;
				typedef core::delegates::method<T(Ts...)> intf_type;
				typedef vk_static_method<T(VKAPI_PTR*)(Ts...), T, Ts...> full_type;
			};

			template<class owner_t, class T, class... Ts>
			struct _vk_device_callback_type<owner_t, T(VKAPI_PTR*)(VkDevice, Ts...)>
			{
				typedef vk_callback<owner_t, T(Ts...)> type;
				typedef core::delegates::method<T(Ts...)> intf_type;
				typedef vk_object_method<VkDevice, T, Ts...> full_type;
			};


			struct VulkanSDK
			{
				static VkInstance Instance();
				static VkInstance Initialize();
				static void Release();

#define VK_EXPORTED_FUNCTION(_FUNC_NAME) static typename _vk_callback_type<VulkanSDK, PFN_##_FUNC_NAME>::type _FUNC_NAME
#define VK_GLOBAL_FUNCTION(_FUNC_NAME) static typename _vk_callback_type<VulkanSDK, PFN_##_FUNC_NAME>::type _FUNC_NAME
#define VK_INSTANCE_FUNCTION(_FUNC_NAME) static typename _vk_callback_type<VulkanSDK, PFN_##_FUNC_NAME>::type _FUNC_NAME
#define VK_DEVICE_FUNCTION(_FUNC_NAME)

#include "vulkan\VulkanSDK.inl"

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION	
#undef VK_DEVICE_FUNCTION

			};

			struct _VKDevice
			{
#define VK_EXPORTED_FUNCTION(_FUNC_NAME)
#define VK_GLOBAL_FUNCTION(_FUNC_NAME)
#define VK_INSTANCE_FUNCTION(_FUNC_NAME)
#define VK_DEVICE_FUNCTION(_FUNC_NAME) typename _vk_device_callback_type<value<_VKDevice>, PFN_##_FUNC_NAME>::type _FUNC_NAME

				VK_DEVICE_FUNCTION(vkGetDeviceProcAddr);

				#include "vulkan\VulkanSDK.inl"

#if defined(USE_SWAPCHAIN_EXTENSIONS)
				VK_DEVICE_FUNCTION(vkQueuePresentKHR);
#endif

#undef VK_EXPORTED_FUNCTION
#undef VK_GLOBAL_FUNCTION
#undef VK_INSTANCE_FUNCTION	
#undef VK_DEVICE_FUNCTION

				inline bool IsPresentSupported()const { return _present_queue_family_index != uint(-1); }

			private:
				VkDevice _device;
				uint _graphics_queue_family_index;
				uint _present_queue_family_index;
				friend value<_VKDevice>;
			};

			typedef value_wrapper<_VKDevice> VKInternalDevice;
			typedef shared_ptr<_VKDevice> VKInternalDevice_t;

		}
	}

	template<> struct value<graphics::vulkan::_VKDevice>
	{
	public: /*type definitions*/
		typedef graphics::vulkan::_VKDevice type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value();
		value(VkSurfaceKHR);
		~value();
	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }

	public: /*operators*/
		operator type& () { return get(); }
		operator type ()const { return get(); }

		type* operator -> () { return &get(); }
		type const* operator -> ()const { return &get(); }
	};

	template<> class value_wrapper<graphics::vulkan::_VKDevice>
		: public object
		, public value<graphics::vulkan::_VKDevice>
	{
	public:
		value_wrapper();
		value_wrapper(VkSurfaceKHR);

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;

	private:
		virtual~value_wrapper();
	};

}


ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::vulkan::_VKDevice)

ANG_REGISTER_RUNTIME_TYPENAME(VkInstance)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDevice)
ANG_REGISTER_RUNTIME_TYPENAME(VkDevice)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueue)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBuffer)

ANG_REGISTER_RUNTIME_TYPENAME(VkApplicationInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkInstanceCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkAllocationCallbacks)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDeviceFeatures)
ANG_REGISTER_RUNTIME_TYPENAME(VkFormatProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkExtent3D)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageFormatProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDeviceLimits)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDeviceSparseProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDeviceProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueueFamilyProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryType)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryHeap)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDeviceMemoryProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkDeviceQueueCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDeviceCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkExtensionProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkLayerProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkSubmitInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryAllocateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkMappedMemoryRange)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryRequirements)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseImageFormatProperties)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseImageMemoryRequirements)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseMemoryBind)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseBufferMemoryBindInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseImageOpaqueMemoryBindInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageSubresource)
ANG_REGISTER_RUNTIME_TYPENAME(VkOffset3D)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseImageMemoryBind)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseImageMemoryBindInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkBindSparseInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkFenceCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkSemaphoreCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkEventCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueryPoolCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferViewCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkSubresourceLayout)
ANG_REGISTER_RUNTIME_TYPENAME(VkComponentMapping)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageSubresourceRange)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageViewCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkShaderModuleCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineCacheCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkSpecializationMapEntry)
ANG_REGISTER_RUNTIME_TYPENAME(VkSpecializationInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineShaderStageCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkVertexInputBindingDescription)
ANG_REGISTER_RUNTIME_TYPENAME(VkVertexInputAttributeDescription)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineVertexInputStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineInputAssemblyStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineTessellationStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkViewport)
ANG_REGISTER_RUNTIME_TYPENAME(VkOffset2D)
ANG_REGISTER_RUNTIME_TYPENAME(VkExtent2D)
ANG_REGISTER_RUNTIME_TYPENAME(VkRect2D)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineViewportStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineRasterizationStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineMultisampleStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkStencilOpState)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineDepthStencilStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineColorBlendAttachmentState)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineColorBlendStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineDynamicStateCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkGraphicsPipelineCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkComputePipelineCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkPushConstantRange)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineLayoutCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkSamplerCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorSetLayoutBinding)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorSetLayoutCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorPoolSize)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorPoolCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorSetAllocateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorImageInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorBufferInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkWriteDescriptorSet)
ANG_REGISTER_RUNTIME_TYPENAME(VkCopyDescriptorSet)
ANG_REGISTER_RUNTIME_TYPENAME(VkFramebufferCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkAttachmentDescription)
ANG_REGISTER_RUNTIME_TYPENAME(VkAttachmentReference)
ANG_REGISTER_RUNTIME_TYPENAME(VkSubpassDescription)
ANG_REGISTER_RUNTIME_TYPENAME(VkSubpassDependency)
ANG_REGISTER_RUNTIME_TYPENAME(VkRenderPassCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandPoolCreateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBufferAllocateInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBufferInheritanceInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBufferBeginInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferCopy)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageSubresourceLayers)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageCopy)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageBlit)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferImageCopy)
ANG_REGISTER_RUNTIME_TYPENAME(VkClearColorValue)
ANG_REGISTER_RUNTIME_TYPENAME(VkClearDepthStencilValue)
ANG_REGISTER_RUNTIME_TYPENAME(VkClearValue)
ANG_REGISTER_RUNTIME_TYPENAME(VkClearAttachment)
ANG_REGISTER_RUNTIME_TYPENAME(VkClearRect)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageResolve)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryBarrier)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferMemoryBarrier)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageMemoryBarrier)
ANG_REGISTER_RUNTIME_TYPENAME(VkRenderPassBeginInfo)
ANG_REGISTER_RUNTIME_TYPENAME(VkDispatchIndirectCommand)
ANG_REGISTER_RUNTIME_TYPENAME(VkDrawIndexedIndirectCommand)
ANG_REGISTER_RUNTIME_TYPENAME(VkDrawIndirectCommand)


ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineCacheHeaderVersion)
ANG_REGISTER_RUNTIME_TYPENAME(VkResult)
ANG_REGISTER_RUNTIME_TYPENAME(VkStructureType)
ANG_REGISTER_RUNTIME_TYPENAME(VkSystemAllocationScope)
ANG_REGISTER_RUNTIME_TYPENAME(VkInternalAllocationType)
ANG_REGISTER_RUNTIME_TYPENAME(VkFormat)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageType)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageTiling)
ANG_REGISTER_RUNTIME_TYPENAME(VkPhysicalDeviceType)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueryType)
ANG_REGISTER_RUNTIME_TYPENAME(VkSharingMode)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageLayout)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageViewType)
ANG_REGISTER_RUNTIME_TYPENAME(VkComponentSwizzle)
ANG_REGISTER_RUNTIME_TYPENAME(VkVertexInputRate)
ANG_REGISTER_RUNTIME_TYPENAME(VkPrimitiveTopology)
ANG_REGISTER_RUNTIME_TYPENAME(VkPolygonMode)
ANG_REGISTER_RUNTIME_TYPENAME(VkFrontFace)
ANG_REGISTER_RUNTIME_TYPENAME(VkCompareOp)
ANG_REGISTER_RUNTIME_TYPENAME(VkStencilOp)
ANG_REGISTER_RUNTIME_TYPENAME(VkLogicOp)
ANG_REGISTER_RUNTIME_TYPENAME(VkBlendFactor)
ANG_REGISTER_RUNTIME_TYPENAME(VkBlendOp)
ANG_REGISTER_RUNTIME_TYPENAME(VkDynamicState)
ANG_REGISTER_RUNTIME_TYPENAME(VkFilter)
ANG_REGISTER_RUNTIME_TYPENAME(VkSamplerMipmapMode)
ANG_REGISTER_RUNTIME_TYPENAME(VkSamplerAddressMode)
ANG_REGISTER_RUNTIME_TYPENAME(VkBorderColor)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorType)
ANG_REGISTER_RUNTIME_TYPENAME(VkAttachmentLoadOp)
ANG_REGISTER_RUNTIME_TYPENAME(VkAttachmentStoreOp)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineBindPoint)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBufferLevel)
ANG_REGISTER_RUNTIME_TYPENAME(VkIndexType)
ANG_REGISTER_RUNTIME_TYPENAME(VkSubpassContents)
ANG_REGISTER_RUNTIME_TYPENAME(VkFormatFeatureFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageUsageFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageCreateFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkSampleCountFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueueFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryPropertyFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkMemoryHeapFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineStageFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkImageAspectFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseImageFormatFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkSparseMemoryBindFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkFenceCreateFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueryPipelineStatisticFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueryResultFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferCreateFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkBufferUsageFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkPipelineCreateFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkShaderStageFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkCullModeFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkColorComponentFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkDescriptorPoolCreateFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkAttachmentDescriptionFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkAccessFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkDependencyFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandPoolCreateFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandPoolResetFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBufferUsageFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkQueryControlFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkCommandBufferResetFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkStencilFaceFlagBits)
ANG_REGISTER_RUNTIME_TYPENAME(VkColorSpaceKHR)
ANG_REGISTER_RUNTIME_TYPENAME(VkPresentModeKHR)
ANG_REGISTER_RUNTIME_TYPENAME(VkSurfaceTransformFlagBitsKHR)
ANG_REGISTER_RUNTIME_TYPENAME(VkCompositeAlphaFlagBitsKHR)
ANG_REGISTER_RUNTIME_TYPENAME(VkDisplayPlaneAlphaFlagBitsKHR)

ANG_REGISTER_RUNTIME_TYPENAME(VkSwapchainCreateInfoKHR)
ANG_REGISTER_RUNTIME_TYPENAME(VkSurfaceFormatKHR)
ANG_REGISTER_RUNTIME_TYPENAME(VkSurfaceCapabilitiesKHR)



#endif//VULKAN_SUPPORT