#include "stdafx.h"
#include "overlay_battery_indicator.h"
#include "Emu/RSX/RSXThread.h"

namespace rsx
{
	namespace overlays
	{
		battery_indicator::battery_indicator()
		{
			frame.set_pos(0, 0);
			frame.set_size(300, 80);
			frame.back_color.a = 0.85f;

			text_view.set_pos(85, 0);
			text_view.set_padding(0, 0, 24, 0);
			text_view.set_font("Arial", 9);
			text_view.align_text(overlay_element::text_align::center);
			text_view.back_color.a = 0.f;
		}

		compiled_resource battery_indicator::get_compiled()
		{
			if (!visible)
			{
				return {};
			}

			auto result = frame.get_compiled();
			result.add(text_view.get_compiled());

			return result;
		}

		s32 battery_indicator::show(int pad_id, int percentage, bool show)
		{
			if (!show)
			{
				close(false, false);
				return CELL_OK;
			}

			const std::string message = "Pad " + std::to_string(pad_id) + " at " + std::to_string(percentage) + "%";
			text_view.set_text(message);
			text_view.auto_resize();

			// Resize background to cover the text
			frame.w = text_view.x + text_view.w;

			visible = true;
			return CELL_OK;
		}
	} // namespace overlays
} // namespace rsx
