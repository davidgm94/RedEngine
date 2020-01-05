#pragma once
#include "red_os.h"
#include "graphics/d3d/d3d11.h"
#include "graphics/d3d/d3d12.h"
#include "graphics/opengl.h"
#include "graphics/vulkan.h"

void vk_loadTriangle(vulkan_renderer* vk, os_window_handler* window, os_window_dimension* windowDimension);
void vk_renderTriangle(vulkan_renderer* vk);

static inline void vk_load(vulkan_renderer* vk, os_window_handler* window, os_window_dimension* windowDimension)
{
	vk_loadTriangle(vk, window, windowDimension);
}

static inline void vk_render(vulkan_renderer* vk)
{
	vk_renderTriangle(vk);
}

static inline void vk_destroy(vulkan_renderer* vk)
{
	vkDestroySemaphore(vk->device, vk->imageAcquireSemaphore, vk->allocator);
	vkDestroySemaphore(vk->device, vk->imageReleaseSemaphore, vk->allocator);
	vkFreeCommandBuffers(vk->device, vk->commandPools[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS], IMAGE_COUNT, vk->commandBuffers);
	vkDestroyPipeline(vk->device, vk->graphicsPipeline, vk->allocator);
	vkDestroyPipelineLayout(vk->device, vk->pipelineLayout, vk->allocator);
	vkDestroyRenderPass(vk->device, vk->renderPass, vk->allocator);
	for (int i = 0; i < IMAGE_COUNT; i++)
	{
		vkDestroyCommandPool(vk->device, vk->commandPools[i], vk->allocator);
		vkDestroyFramebuffer(vk->device, vk->framebuffers[i], vk->allocator);
		vkDestroyImageView(vk->device, vk->swapchainImageViews[i], vk->allocator);
	}
	vkDestroyShaderModule(vk->device, vk->shaders.vs, vk->allocator);
	vkDestroyShaderModule(vk->device, vk->shaders.fs, vk->allocator);
	vkDestroySwapchainKHR(vk->device, vk->swapchain, vk->allocator);
    vkDestroyDevice(vk->device, vk->allocator);
    vkDestroySurfaceKHR(vk->instance, vk->surface, vk->allocator);
#if NDEBUG == 0
    vkDestroyDebugReportCallbackEXT(vk->instance, vk->debugCallback, vk->allocator);
#endif
    vkDestroyInstance(vk->instance, vk->allocator);
}

static inline VkRenderPass vk_setupRenderPass(VkAllocationCallbacks* allocator, VkDevice device,
	VkFormat format)
{
	VkAttachmentDescription attachments[1] = {};
	attachments[0].format = format;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachments = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

	VkSubpassDescription subpass;
	subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = null;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachments;
    subpass.pResolveAttachments = 0;
	subpass.pDepthStencilAttachment = null;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = null;

	VkRenderPass renderPass = vk_createRenderPass(allocator, device, attachments, ARRAYCOUNT(attachments), null, 0, &subpass, 1);

	return renderPass;
}

static inline VkPipeline vk_createGraphicsPipeline(VkAllocationCallbacks* allocator, VkDevice device, VkPipelineCache pipelineCache, VkRenderPass renderPass, VkShaderModule vs, VkShaderModule fs, VkPipelineLayout layout)
{
	VkGraphicsPipelineCreateInfo createInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

	VkPipelineShaderStageCreateInfo stages[2] = {0};
	stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	stages[0].module = vs;
	stages[0].pName = "main";
	stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	stages[1].module = fs;
	stages[1].pName = "main";

	createInfo.stageCount = sizeof(stages) / sizeof(stages[0]);
	createInfo.pStages = stages;

	VkPipelineVertexInputStateCreateInfo vertexInput = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
	createInfo.pVertexInputState = &vertexInput;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	createInfo.pInputAssemblyState = &inputAssembly;

	VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;
	createInfo.pViewportState = &viewportState;

	VkPipelineRasterizationStateCreateInfo rasterizationState = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
	rasterizationState.lineWidth = 1.f;
	createInfo.pRasterizationState = &rasterizationState;

	VkPipelineMultisampleStateCreateInfo multisampleState = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
	multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.pMultisampleState = &multisampleState;

	VkPipelineDepthStencilStateCreateInfo depthStencilState = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
	createInfo.pDepthStencilState = &depthStencilState;

	VkPipelineColorBlendAttachmentState colorAttachmentState = {0};
	colorAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendState = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = &colorAttachmentState;
	createInfo.pColorBlendState = &colorBlendState;

	VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
	dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
	dynamicState.pDynamicStates = dynamicStates;
	createInfo.pDynamicState = &dynamicState;

	createInfo.layout = layout;
	createInfo.renderPass = renderPass;

	VkPipeline pipeline = 0;
	VKCHECK(vkCreateGraphicsPipelines(device, pipelineCache, 1, &createInfo, allocator, &pipeline));

	return pipeline;
}

void vk_loadTriangle(vulkan_renderer* vk, os_window_handler* window, os_window_dimension* windowDimension)
{
	//vk->allocator = &vk->allocator_;
	vk->allocator = null;
    // Using volk code to load Vulkan function pointers
    vk->instance = vk_createInstance(vk->allocator);
    // Here ends all Volk code usage
#if _DEBUG
	VkDebugReportFlagsEXT debugCallbackFlags =
		// VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		
    vk->debugCallback = vk_createDebugCallback(vk->allocator, vk->instance, debugCallbackFlags, vk_debugCallback);
#endif
	vk->physicalDevice = vk_pickPhysicalDevice(vk->instance);
	vk->surface = vk_createSurface(nullptr, vk->instance, window);
    vk->extent = (VkExtent2D){ windowDimension->width, windowDimension->height };
    vk_fillSwapchainProperties(&vk->swapchainProperties, vk->physicalDevice, vk->surface);
    vk_fillSwapchainRequirements(&vk->swapchainRequirements, &vk->swapchainProperties, &vk->extent);
    VkQueueFlags availableQueues[] = { VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT };
    VkQueueFlags queuesToCreate = 0;
	const float priorities[] = { 0.0f };
    u32 availableQueueCount = ARRAYCOUNT(availableQueues);
    for (u32 i = 0; i < availableQueueCount; i++)
    {
	   queuesToCreate |= availableQueues[i];
    }

	VkDeviceQueueCreateInfo queueCreateInfoArray[ARRAYCOUNT(availableQueues)];
	vk_setupQueueCreation(queueCreateInfoArray, &vk->swapchainProperties.queueFamily, queuesToCreate, priorities);
	vk->device = vk_createDevice(vk->allocator, vk->physicalDevice, vk->instance, queueCreateInfoArray, ARRAYCOUNT(queueCreateInfoArray));
	vk->renderPass = vk_setupRenderPass(vk->allocator, vk->device, vk->swapchainRequirements.surfaceFormat.format);
	vk_getDeviceQueues(vk->device, vk->swapchainProperties.queueFamily.indices, vk->queues);
    vk->swapchain = vk_createSwapchain(vk->allocator, vk->device, vk->surface, &vk->swapchainRequirements, nullptr);
	vk_getSwapchainImages(vk->swapchainImages, vk->device, vk->swapchain);
	vk_createImageViews(vk->allocator, vk->device, vk->swapchainImages, vk->swapchainImageViews, VK_IMAGE_VIEW_TYPE_2D, vk->swapchainRequirements.surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
	vk_createFramebuffers(vk->allocator, vk->device, vk->framebuffers, vk->renderPass, vk->swapchainImageViews, IMAGE_COUNT, &vk->extent);
	vk_createImageMemoryBarriers(vk->beginRenderBarriers, vk->swapchainImages, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	vk_createImageMemoryBarriers(vk->endRenderBarriers, vk->swapchainImages, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

	raw_str vsFile = platform_readFile("src/graphics/shaders/triangle.vert.spv");
	raw_str fsFile = platform_readFile("src/graphics/shaders/triangle.frag.spv");
	vk->shaders.vs = vk_createShaderModule(vk->allocator, vk->device, vsFile.c_str, vsFile.size);
	vk->shaders.fs = vk_createShaderModule(vk->allocator, vk->device, fsFile.c_str, fsFile.size);
	free(vsFile.c_str);
	free(fsFile.c_str);

	VkPipelineCache pipelineCache = null;
	vk->pipelineLayout = vk_createPipelineLayout(vk->allocator, vk->device, null, 0, null, 0);
	vk->graphicsPipeline = vk_createGraphicsPipeline(vk->allocator, vk->device, pipelineCache, vk->renderPass, vk->shaders.vs, vk->shaders.fs, vk->pipelineLayout);

	// COMMANDS AND QUEUES
	vk->imageAcquireSemaphore = vk_createSemaphore(vk->allocator, vk->device);
	vk->imageReleaseSemaphore = vk_createSemaphore(vk->allocator, vk->device);
	vk_createCommandPools(vk->allocator, vk->device, vk->commandPools, vk->swapchainProperties.queueFamily.indices);
	vk_createCommandBuffers(vk->device, vk->commandPools[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS], IMAGE_COUNT, vk->commandBuffers);
}

void vk_renderTriangle(vulkan_renderer* vk)
{
	u32 currentImageIndex = vk_acquireNextImage(vk->device, vk->swapchain, vk->imageAcquireSemaphore);
	VKCHECK(vkResetCommandPool(vk->device, vk->commandPools[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS], 0));

	VkCommandBuffer commandBuffer = vk->commandBuffers[currentImageIndex];
	vk_beginOneTimeSubmitCommandBuffer(commandBuffer);
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, null, 0, null, 1, &vk->beginRenderBarriers[currentImageIndex]);

	VkClearColorValue color;
	color.float32[0] = 0.5f;
	color.float32[1] = 0.5f;
	color.float32[2] = 0.5f;
	color.float32[3] = 0.5f;
	VkClearValue clearColor = { color };
	
	VkRect2D renderArea;
	renderArea.extent = vk->extent;
	renderArea.offset.x = 0;
	renderArea.offset.y = 0;

	vk_beginRenderPass(commandBuffer, vk->renderPass, vk->framebuffers[currentImageIndex], renderArea, &clearColor, 1, VK_SUBPASS_CONTENTS_INLINE);
	
	VkViewport viewport;
    viewport.x = 0;
    viewport.y = (float)vk->extent.height;
    viewport.width = (float)vk->extent.width;
    viewport.height = - (float)vk->extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

	VkRect2D scissor = renderArea;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk->graphicsPipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, 0, 0, 0, 1, &vk->endRenderBarriers[currentImageIndex]);

	vk_endCommandBuffer(commandBuffer);

	VkPipelineStageFlags submitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	vk_submitCommandsToQueue(&vk->imageAcquireSemaphore, 1, &submitStageMask, vk->queues[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS], &commandBuffer, 1, &vk->imageReleaseSemaphore, 1, null);

	vk_presentImage(vk->queues[VULKAN_QUEUE_FAMILY_INDEX_GRAPHICS], &vk->imageReleaseSemaphore, 1, &vk->swapchain, 1, &currentImageIndex);

	VKCHECK(vkDeviceWaitIdle(vk->device));
}
