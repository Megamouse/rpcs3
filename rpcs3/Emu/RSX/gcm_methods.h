#pragma once

#include "Emu/Memory/vm_ptr.h"

namespace gcm
{
	u32 gcm_method(u32 method, u32 param_count);
	u32 gcm_method(u32 method, u32 index, u32 param_count);
	void gcm_method_set(vm::bptr<u32>& dst, u32 method, u32 data);
	void gcm_method_set(vm::bptr<u32>& dst, u32 method, const std::vector<u32>& data);
}
