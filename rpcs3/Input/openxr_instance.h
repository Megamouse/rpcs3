#pragma once

#if defined(HAVE_VULKAN)
#include "Emu/RSX/VK/VulkanAPI.h"
#endif

#ifdef _WIN32
#include <windows.h>
#include <Unknwn.h>
#endif

#include "openxr/openxr.h"
#include "openxr/openxr_platform.h"

enum class openxr_action_id;

class openxr_instance
{
public:
	openxr_instance();
	~openxr_instance();
	static bool is_openxr_available();

#if defined(HAVE_VULKAN)
	bool init_vulkan(VkInstance vk_instance, VkPhysicalDevice phys_dev, VkDevice vk_device, u32 queue_index, u32 queue_family);
#endif
#ifndef __APPLE__
	bool init_opengl(HDC hdc, HGLRC glrc);
#endif

	void update(XrSpace reference_space, XrTime predicted_time);

private:
	struct openxr_pose
	{
		void apply_smoothing();
		void apply_calibration();

		XrSpace space {XR_NULL_HANDLE};
		XrPosef pose{};
		XrPosef filtered_pose{};
		XrPosef calibration {};
		bool is_active = false;
		bool is_valid = false;
	};

	struct controller
	{
		XrPath subaction_path {XR_NULL_PATH};

		std::map<openxr_action_id, XrActionSuggestedBinding> bindings;
		std::map<openxr_action_id, openxr_pose> poses;
		std::map<openxr_action_id, XrBool32> booleans;
		std::map<openxr_action_id, float> floats;
		std::map<openxr_action_id, XrVector2f> vectors2f;
	};

	bool init_common();
	bool create_instance();
	bool get_system_info();
	bool create_session();
	bool create_actions();
	bool suggest_bindings();
	bool create_spaces();
	bool attach_actions();
	bool begin_session();
	void update_hand(XrSpace ref, XrTime time, controller& ctrl);
	bool sync_actions() const;

	XrInstance m_instance{XR_NULL_HANDLE};
	XrSystemId m_system{};
	XrSession m_session{XR_NULL_HANDLE};
	bool m_session_running = false;

	XrActionSet m_action_set{XR_NULL_HANDLE};

	std::array<controller, 2> m_controllers;

#if defined(HAVE_VULKAN)
	VkInstance m_vk_instance{};
	VkPhysicalDevice m_vk_physical{};
	VkDevice m_vk_device{};
	u32 m_vk_queue_index{};
	u32 m_vk_queue_family{};
#endif

#ifndef __APPLE__
	HDC m_hdc{};
	HGLRC m_glrc{};
#endif
};
