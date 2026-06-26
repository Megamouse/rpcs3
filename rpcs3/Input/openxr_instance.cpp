#include "stdafx.h"
#include "openxr_instance.h"

#include <mutex>

LOG_CHANNEL(openxr_log, "openxr");

static const std::string& get_hand_path(u32 controller_index)
{
	static const std::string left_hand_path = "/user/hand/left";
	static const std::string right_hand_path = "/user/hand/right";
	return controller_index == 0 ? left_hand_path : right_hand_path;
}

struct input_option
{
	std::string path_suffix;
	std::string name;
	XrActionType action_type = XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT;

	std::string get_path(u32 controller_index) const
	{
		return get_hand_path(controller_index) + path_suffix;
	}
};

enum class openxr_action_id
{
	grip_pose,
	aim_pose,
	trigger_analog,
	trigger_click,
	squeeze_analog,
	squeeze_click,
	thumbstick,
	thumbstick_click,
	thumbstick_touch,
	trackpad,
	trackpad_click,
	trackpad_touch,
	menu_button,
	a_button,
	b_button,
	x_button,
	y_button,
};

static const std::map<openxr_action_id, input_option> generic_input_options =
{
	{openxr_action_id::grip_pose, { "Grip pose", "/input/grip/pose", XrActionType::XR_ACTION_TYPE_POSE_INPUT }},
	{openxr_action_id::aim_pose, { "Aim pose", "/input/aim/pose", XrActionType::XR_ACTION_TYPE_POSE_INPUT }},
	{openxr_action_id::trigger_analog, { "Trigger analog", "/input/trigger/value", XrActionType::XR_ACTION_TYPE_FLOAT_INPUT }},
	{openxr_action_id::trigger_click, { "Trigger click", "/input/trigger/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::squeeze_analog, { "Squeeze/grip analog", "/input/squeeze/value", XrActionType::XR_ACTION_TYPE_FLOAT_INPUT }},
	{openxr_action_id::squeeze_click, { "Squeeze click", "/input/squeeze/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::thumbstick, { "Thumbstick", "/input/thumbstick", XrActionType::XR_ACTION_TYPE_VECTOR2F_INPUT }},
	{openxr_action_id::thumbstick_click, { "Thumbstick click", "/input/thumbstick/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::thumbstick_touch, { "Thumbstick touch", "/input/thumbstick/touch", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::trackpad, { "Trackpad", "/input/trackpad", XrActionType::XR_ACTION_TYPE_VECTOR2F_INPUT }},
	{openxr_action_id::trackpad_click, { "Trackpad click", "/input/trackpad/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::trackpad_touch, { "Trackpad touch", "/input/trackpad/touch", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::menu_button, { "Menu button", "/input/menu/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::a_button, { "A button", "/input/a/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::b_button, { "B button", "/input/b/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::x_button, { "X button", "/input/x/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
	{openxr_action_id::y_button, { "Y button", "/input/y/click", XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT }},
};

openxr_instance::openxr_instance()
{
}

openxr_instance::~openxr_instance()
{
	// Stop session first
	if (m_session)
	{
		if (m_session_running)
		{
			xrEndSession(m_session);
			m_session_running = false;
		}

		// Destroy spaces before destroying session
		for (auto& ctrl : m_controllers)
		{
			for (auto& [id, pose] : ctrl.poses)
			{
				if (pose.space)
				{
					xrDestroySpace(pose.space);
					pose.space = XR_NULL_HANDLE;
				}
			}
		}

		xrDestroySession(m_session);
		m_session = XR_NULL_HANDLE;
	}

	// Destroy actions
	for (auto& ctrl : m_controllers)
	{
		for (auto& [id, binding] : ctrl.bindings)
		{
			if (binding.action)
			{
				xrDestroyAction(binding.action);
				binding.action = XR_NULL_HANDLE;
			}
		}
	}

	// Destroy action set
	if (m_action_set)
	{
		xrDestroyActionSet(m_action_set);
		m_action_set = XR_NULL_HANDLE;
	}

	// Destroy instance last
	if (m_instance)
	{
		xrDestroyInstance(m_instance);
		m_instance = XR_NULL_HANDLE;
	}
}

bool openxr_instance::is_openxr_available()
{
	static int available {-1};
	static std::mutex mtx;
	std::lock_guard lock(mtx);

	if (available == -1)
	{
		u32 extension_count = 0;
		if (XR_FAILED(xrEnumerateInstanceExtensionProperties(nullptr, 0, &extension_count, nullptr)))
		{
			openxr_log.notice("xrEnumerateInstanceExtensionProperties returned false");
			available = 0;
		}
		else
		{
			openxr_log.notice("xrEnumerateInstanceExtensionProperties returned true");
			available = 1;
		}
	}

	return available == 1;
}

#if defined(HAVE_VULKAN)
bool openxr_instance::init_vulkan(VkInstance vk_instance, VkPhysicalDevice phys_dev, VkDevice vk_device, u32 queue_index, u32 queue_family)
{
	openxr_log.notice("Initializing Vulkan instance");

	m_vk_instance = vk_instance;
	m_vk_physical = phys_dev;
	m_vk_device = vk_device;
	m_vk_queue_index = queue_index;
	m_vk_queue_family = queue_family;

	return init_common();
}
#endif

#ifndef __APPLE__
bool openxr_instance::init_opengl(HDC hdc, HGLRC glrc)
{
	openxr_log.notice("Initializing OpenGl instance");

	m_hdc = hdc;
	m_glrc = glrc;

	return init_common();
}
#endif

bool openxr_instance::init_common()
{
	if (!create_instance())
	{
		openxr_log.error("Failed to create instance");
		return false;
	}
	if (!get_system_info())
	{
		openxr_log.error("Failed to get system info");
		return false;
	}
	if (!create_session())
	{
		openxr_log.error("Failed to create session");
		return false;
	}
	if (!create_actions())
	{
		openxr_log.error("Failed to create actions");
		return false;
	}
	if (!suggest_bindings())
	{
		openxr_log.error("Failed to suggest bindings");
		return false;
	}
	if (!attach_actions())
	{
		openxr_log.error("Failed to attach actions");
		return false;
	}
	if (!begin_session())
	{
		openxr_log.error("Failed to begin session");
		return false;
	}
	if (!create_spaces())
	{
		openxr_log.error("Failed to create spaces");
		return false;
	}
	return true;
}

bool openxr_instance::create_instance()
{
	XrInstanceCreateInfo info{XR_TYPE_INSTANCE_CREATE_INFO};
	strcpy_trunc(info.applicationInfo.applicationName, "RPCS3 OpenXR");
	info.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

	if (XR_FAILED(xrCreateInstance(&info, &m_instance)))
	{
		openxr_log.error("xrCreateInstance failed");
		return false;
	}

	return true;
}

bool openxr_instance::get_system_info()
{
	XrSystemGetInfo info{XR_TYPE_SYSTEM_GET_INFO};
	info.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

	if (XR_FAILED(xrGetSystem(m_instance, &info, &m_system)))
	{
		openxr_log.error("xrGetSystem failed");
		return false;
	}

	return true;
}

bool openxr_instance::create_session()
{
	XrSessionCreateInfo sci{};
	sci.type = XR_TYPE_SESSION_CREATE_INFO;
	sci.systemId = m_system;

	if (true)
	{
#if defined(HAVE_VULKAN)
		XrGraphicsBindingVulkanKHR vk{};
		vk.type = XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR;
		vk.instance = m_vk_instance;
		vk.physicalDevice = m_vk_physical;
		vk.device = m_vk_device;
		vk.queueFamilyIndex = m_vk_queue_family;
		vk.queueIndex = m_vk_queue_index;

		sci.next = &vk;
#endif
	}
	else
	{
#ifndef __APPLE__
		XrGraphicsBindingOpenGLWin32KHR gl{};
		gl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR;
		gl.hDC = m_hdc;
		gl.hGLRC = m_glrc;

		sci.next = &gl;
#endif
	}

	if (XR_FAILED(xrCreateSession(m_instance, &sci, &m_session)))
	{
		openxr_log.error("xrGetSystem failed");
		return false;
	}

	return true;
}

bool openxr_instance::create_actions()
{
	// Action set
	XrActionSetCreateInfo set_info{XR_TYPE_ACTION_SET_CREATE_INFO};
	strcpy_trunc(set_info.actionSetName, "move");
	strcpy_trunc(set_info.localizedActionSetName, "Move");

	if (XR_FAILED(xrCreateActionSet(m_instance, &set_info, &m_action_set)))
	{
		openxr_log.error("xrCreateActionSet failed");
		return false;
	}

	// Subaction paths
	std::vector<XrPath> paths;
	for (u32 i = 0; i < m_controllers.size(); i++)
	{
		controller& ctrl = m_controllers[i];
		if (XR_FAILED(xrStringToPath(m_instance, get_hand_path(i).c_str(), &ctrl.subaction_path)))
		{
			openxr_log.error("xrStringToPath failed for '%s'", get_hand_path(i));
			continue;
		}

		paths.push_back(ctrl.subaction_path);
	}

	if (paths.empty())
	{
		return false;
	}

	// Create actions (each can be used for both hands)
	for (const auto& [id, opt] : generic_input_options)
	{
		XrActionCreateInfo info{};
		info.type = XR_TYPE_ACTION_CREATE_INFO;
		info.actionType = opt.action_type;
		strcpy_trunc(info.actionName, opt.name);
		info.countSubactionPaths = static_cast<u32>(paths.size());
		info.subactionPaths = paths.data();
		//strcpy_trunc(info.localizedActionName, opt.name_localized.c_str()); // TODO

		XrAction action {};
		if (XR_FAILED(xrCreateAction(m_action_set, &info, &action)))
		{
			openxr_log.error("xrCreateAction failed (name='%s')", opt.name);
			continue;
		}

		for (u32 i = 0; i < m_controllers.size(); i++)
		{
			controller& ctrl = m_controllers[i];
			if (ctrl.subaction_path == XR_NULL_PATH) continue;

			ctrl.bindings[id].action = action;
		}
	}

	return true;
}

bool openxr_instance::suggest_bindings()
{
	// TODO
	// /interaction_profiles/khr/simple_controller
	// /interaction_profiles/oculus/touch_controller
	// /interaction_profiles/valve/index_controller
	// /interaction_profiles/htc/vive_controller

	XrPath profile;
	if (XR_FAILED(xrStringToPath(m_instance, "/interaction_profiles/valve/index_controller", &profile)))
	{
		openxr_log.error("xrStringToPath failed for /interaction_profiles/valve/index_controller");
		return false;
	}

	std::vector<XrActionSuggestedBinding> bindings;

	for (u32 i = 0; i < m_controllers.size(); i++)
	{
		controller& ctrl = m_controllers[i];
		if (ctrl.subaction_path == XR_NULL_PATH) continue;

		for (auto& [id, binding] : ctrl.bindings)
		{
			ensure(binding.action != XR_NULL_HANDLE);

			const input_option& opt = ::at32(generic_input_options, id);
			const std::string path = opt.get_path(i);

			if (XR_FAILED(xrStringToPath(m_instance, path.c_str(), &binding.binding)))
			{
				openxr_log.error("xrStringToPath failed for '%s'", path);
				continue;
			}

			bindings.push_back(binding);
		}
	}

	if (bindings.empty())
	{
		return false;
	}

	XrInteractionProfileSuggestedBinding sugg{};
	sugg.type = XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING;
	sugg.interactionProfile = profile;
	sugg.suggestedBindings = bindings.data();
	sugg.countSuggestedBindings = static_cast<u32>(bindings.size());

	if (XR_FAILED(xrSuggestInteractionProfileBindings(m_instance, &sugg)))
	{
		openxr_log.error("xrSuggestInteractionProfileBindings failed");
		return false;
	}

	return true;
}

bool openxr_instance::create_spaces()
{
	bool found_space = false;

	for (controller& ctrl : m_controllers)
	{
		if (ctrl.subaction_path == XR_NULL_PATH) continue;

		for (const auto& [id, binding] : ctrl.bindings)
		{
			const input_option& opt = ::at32(generic_input_options, id);
			if (opt.action_type != XR_ACTION_TYPE_POSE_INPUT) continue;

			XrActionSpaceCreateInfo info{};
			info.type = XR_TYPE_ACTION_SPACE_CREATE_INFO;
			info.action = binding.action;
			info.subactionPath = ctrl.subaction_path;

			XrSpace space {XR_NULL_HANDLE};

			if (XR_FAILED(xrCreateActionSpace(m_session, &info, &space)))
			{
				openxr_log.error("xrCreateActionSpace failed");
				continue;
			}

			ensure(!ctrl.poses.contains(id));

			ctrl.poses[id].space = space;
			found_space = true;
		}
	}

	return found_space;
}

bool openxr_instance::attach_actions()
{
	XrSessionActionSetsAttachInfo info{XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
	info.countActionSets = 1;
	info.actionSets = &m_action_set;

	if (XR_FAILED(xrAttachSessionActionSets(m_session, &info)))
	{
		openxr_log.error("xrAttachSessionActionSets failed");
		return false;
	}

	return true;
}

bool openxr_instance::begin_session()
{
	XrSessionBeginInfo info{XR_TYPE_SESSION_BEGIN_INFO};
	info.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

	if (XR_FAILED(xrBeginSession(m_session, &info)))
	{
		openxr_log.error("xrBeginSession failed");
		return false;
	}

	m_session_running = true;
	return true;
}

void openxr_instance::update_hand(XrSpace ref, XrTime time, controller& ctrl)
{
	if (ctrl.subaction_path == XR_NULL_PATH) return;

	for (const auto& [id, binding] : ctrl.bindings)
	{
		const input_option& opt = ::at32(generic_input_options, id);

		XrActionStateGetInfo info{XR_TYPE_ACTION_STATE_GET_INFO};
		info.action = binding.action;
		info.subactionPath = ctrl.subaction_path;

		switch (opt.action_type)
		{
		case XrActionType::XR_ACTION_TYPE_BOOLEAN_INPUT:
		{
			XrActionStateBoolean state{XR_TYPE_ACTION_STATE_BOOLEAN};
			if (XR_SUCCEEDED(xrGetActionStateBoolean(m_session, &info, &state)))
			{
				ctrl.booleans[id] = state.currentState;
			}
			break;
		}
		case XrActionType::XR_ACTION_TYPE_FLOAT_INPUT:
		{
			XrActionStateFloat state{XR_TYPE_ACTION_STATE_FLOAT};
			if (XR_SUCCEEDED(xrGetActionStateFloat(m_session, &info, &state)))
			{
				ctrl.floats[id] = state.currentState;
			}
			break;
		}
		case XrActionType::XR_ACTION_TYPE_VECTOR2F_INPUT:
		{
			XrActionStateVector2f state{XR_TYPE_ACTION_STATE_VECTOR2F};
			if (XR_SUCCEEDED(xrGetActionStateVector2f(m_session, &info, &state)))
			{
				ctrl.vectors2f[id] = state.currentState;
			}
			break;
		}
		case XrActionType::XR_ACTION_TYPE_POSE_INPUT:
		{
			if (!ctrl.poses.contains(id)) break;

			openxr_pose& pose = ctrl.poses[id];

			XrActionStatePose state{XR_TYPE_ACTION_STATE_POSE};
			if (XR_SUCCEEDED(xrGetActionStatePose(m_session, &info, &state)))
			{
				pose.is_active = state.isActive;

				if (pose.is_active)
				{
					XrSpaceLocation loc{XR_TYPE_SPACE_LOCATION};

					if (XR_FAILED(xrLocateSpace(pose.space, ref, time, &loc)))
					{
						openxr_log.error("xrLocateSpace failed");
						pose.is_valid = false;
						break;
					}

					pose.is_valid = (loc.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) &&
					                (loc.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT);

					if (pose.is_valid)
					{
						pose.pose = loc.pose;
						pose.apply_smoothing();
						pose.apply_calibration();
					}
				}
			}
			else
			{
				pose.is_active = false;
				pose.is_valid = false;
			}
			break;
		}
		default:
			fmt::throw_exception("Unexpected action type 0x%x", static_cast<u32>(opt.action_type));
		}
	}
}

bool openxr_instance::sync_actions() const
{
	XrActiveActionSet active{};
	active.actionSet = m_action_set;
	// TODO subaction_path

	XrActionsSyncInfo sync{};
	sync.type = XR_TYPE_ACTIONS_SYNC_INFO;
	sync.countActiveActionSets = 1;
	sync.activeActionSets = &active;

	if (XR_FAILED(xrSyncActions(m_session, &sync)))
	{
		openxr_log.error("xrSyncActions failed");
		return false;
	}

	return true;
}

void openxr_instance::update(XrSpace reference_space, XrTime predicted_time)
{
	if (!m_session_running)
		return;

	if (!sync_actions())
		return;

	for (controller& ctrl : m_controllers)
	{
		update_hand(reference_space, predicted_time, ctrl);
	}
}

void openxr_instance::openxr_pose::apply_smoothing()
{
	constexpr float alpha = 0.2f;
	constexpr float alpha_inv = 1.0f - alpha;

	filtered_pose.position.x = filtered_pose.position.x * alpha_inv + pose.position.x * alpha;
	filtered_pose.position.y = filtered_pose.position.y * alpha_inv + pose.position.y * alpha;
	filtered_pose.position.z = filtered_pose.position.z * alpha_inv + pose.position.z * alpha;
	filtered_pose.orientation = pose.orientation;
}

void openxr_instance::openxr_pose::apply_calibration()
{
	// simple offset correction
	filtered_pose.position.x -= calibration.position.x;
	filtered_pose.position.y -= calibration.position.y;
	filtered_pose.position.z -= calibration.position.z;
}
