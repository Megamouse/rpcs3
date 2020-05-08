#pragma once

#include "overlays.h"

namespace rsx
{
	namespace overlays
	{
		struct battery_indicator : public user_interface
		{
		private:
			overlay_element frame;
			label text_view;

			// Fade in/out
			animation_color_interpolate fade_animation;

		public:
			battery_indicator();

			compiled_resource get_compiled() override;

			s32 show(int pad_id, int percentage, bool show);
		};
	}
}
