/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: editor.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#if EDITOR

#include "editor_manager.h" // Fonts

#include "imgui/imgui.h"

/*~-------------------------------------------------------------------------~*\
 * Editor Functions                                                          *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {
	
	//bool big_search_bar(string& str, string_view hint);

	void push_font(font_type font, editor_manager& = context<editor_manager>());
	void pop_font();

	namespace impl {
		template<ImGuiDataType_ Value>
		using imgui_type_constant = std::integral_constant<ImGuiDataType_, Value>;
		template<typename T = void> struct imgui_datatype {};
		template<> struct imgui_datatype<float > : imgui_type_constant<ImGuiDataType_Float > {};
		template<> struct imgui_datatype<double> : imgui_type_constant<ImGuiDataType_Double> {};
		template<> struct imgui_datatype<i8 > : imgui_type_constant<ImGuiDataType_S8 > {};
		template<> struct imgui_datatype<i16> : imgui_type_constant<ImGuiDataType_S16> {};
		template<> struct imgui_datatype<i32> : imgui_type_constant<ImGuiDataType_S32> {};
		template<> struct imgui_datatype<i64> : imgui_type_constant<ImGuiDataType_S64> {};
		template<> struct imgui_datatype<u8 > : imgui_type_constant<ImGuiDataType_U8 > {};
		template<> struct imgui_datatype<u16> : imgui_type_constant<ImGuiDataType_U16> {};
		template<> struct imgui_datatype<u32> : imgui_type_constant<ImGuiDataType_U32> {};
		template<> struct imgui_datatype<u64> : imgui_type_constant<ImGuiDataType_U64> {};

		template<typename T>
		inline ImGuiDataType_ imgui_datatype_v = imgui_datatype<T>::value;
	} // namespace impl

	template<typename T>
	struct drag_range {
		T begin = 0;
		T end = 0;
		T step = 1;
	};

	//template<int Count, typename T>
	//inline bool drag(T* data, string_view label) {
	//	ImGui::DragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, format, flags);
	//}
	//
	//template<glm::length_t L, typename T, glm::qualifier Q>
	//inline bool drag(
	//	vec<L, T, Q>& data,
	//	string_view label,
	//	drag_range<T> range,
	//	editor_manager& manager = context<editor_manager>()
	//) {
	//	push_font(font_type::monospace, manager);
	//	ImGui::DragFloat3
	//	return DragScalarN(label.data(), impl::imgui_datatype_v<T>, &data.x, L, range.step, &v_min, &v_max, "%.3f", flags);
	//	pop_font();
	//}

} // namespace fields_engine::editor
#endif // EDITOR
