/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "graphics.h"
#include "glfw/glfw3.h"
#include "application.h"
#include <filesystem>
#include "imgui_stdlib.h"
#include "editor_icons_all.h"
#include "text.h"
#include "imGui/imgui_internal.h"
#include "scene.h"

fields_engine::editor::editor(window& win)
	: m_gui_context(ImGui::CreateContext())
	, m_fonts()
	, m_windows()
{
	ImGui::SetCurrentContext(m_gui_context);
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup fonts with icon font
	std::filesystem::path fonts_path("assets");
	fonts_path /= "fonts";
	const float font_size = 17.0f;
	// Add default font so we can merge in icon font
	m_fonts.emplace_back(io.Fonts->AddFontFromFileTTF(
		(fonts_path / "RobotoMono-Regular.ttf").string().c_str(), font_size
	));
	
	// MUST have persistent lifetime, hence static
	// We want to treat this like a const wchar_t*
	static constexpr ImWchar iconRanges[3] = { 
		FA_ICON_FONT_MIN, FA_ICON_FONT_MAX_16, 0 
	};

	ImFontConfig icon_config;
	icon_config.MergeMode = true;
	icon_config.PixelSnapH = true;
	icon_config.GlyphMinAdvanceX = font_size;
	icon_config.GlyphOffset.y = 1.0f;

	m_fonts.emplace_back(io.Fonts->AddFontFromFileTTF(
		(fonts_path / FA_SOLID_ICON_FONT_FILENAME).string().c_str(),
		font_size, &icon_config, iconRanges
	));

	ImGui_ImplGlfw_InitForOpenGL(win.handle, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	reset_style();

	ImGui::SetNextWindowSize({ 300.0f, 500.0f });

	add_window(make_unique<editor_window>(
		"Root", std::bind(&editor::root_window, this), ICON_FACE_SMILE));

	// Add the window and then set its callback after since it needs to access data inside the window
	
	editor_window* demo_window = &add_window(make_unique<editor_window>(
		"ImGui Demo", editor_window::callback_t{}, ICON_INFO));
	demo_window->callback([demo_window]() {
		ImGui::SetWindowHiddendAndSkipItemsForCurrentFrame(ImGui::GetCurrentWindow());
		ImGui::ShowDemoWindow(&demo_window->open_ref());
		return false; 
	});
	demo_window->close();

	add_window(make_unique<editor_window>(
		"Scene", 
		std::bind(&scene::display_window, &context<scene>()),
		ICON_MOUNTAIN_SUN
	));
}

void fields_engine::editor::tick(float dt) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Begin dockspace
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {

			ImGui::EndMenu();
		}		
		if (ImGui::BeginMenu("View")) {

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			
			ImGui::SeparatorText("Recent");
			const int recent_amt = std::min(5, int(m_recent_windows.size()));
			for (int i = 0; i < recent_amt; ++i) {
				m_windows[m_recent_windows[i]]->menu_item();
			}
			ImGui::SeparatorText("All");
			for (int i = 0; i < m_windows.size(); ++i) {
				m_windows[i]->menu_item();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (int i = 0; i < m_windows.size(); ++i) {
		m_windows[i]->display();
	}

	//if (ImGui::Begin(ICON_FLOPPY_DISK" Glung")) {
	//	ImGui::Text("Hello, World!");
	//	static bool two = false;
	//	if (ImGui::Button("two")) {
	//		two = true;
	//	}
	//	if (two && ImGui::Button("one")) {
	//		two = false;
	//	}
	//}

	//ImGui::ColorPicker4("", &clor_.r);

	//ImGui::End();

	//graphics::clear_background(clor_);

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		context<application>().reinstate();
	}
}

fields_engine::editor::~editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

static ImVec4 rgb(int r, int g, int b) {
	return ImVec4{
		r / 255.0f,
		g / 255.0f,
		b / 255.0f,
		1.0f
	};
}

void fields_engine::editor::reset_style() const {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Style Attributes
	style.WindowPadding		= {8.00f, 8.00f};
	style.FramePadding		= {5.00f, 2.00f};
	style.CellPadding		= {6.00f, 6.00f};
	style.ItemSpacing		= {6.00f, 6.00f};
	style.ItemInnerSpacing	= {6.00f, 6.00f};
	style.TouchExtraPadding = {0.00f, 0.00f};
	style.LogSliderDeadzone = 4;
	style.IndentSpacing		= 25;
	style.ScrollbarSize		= 15;
	style.GrabMinSize		= 10;
	static constexpr float borderSize = 0.0f;
	style.WindowBorderSize	= 0;// = borderSize;
	style.ChildBorderSize	= borderSize;
	style.PopupBorderSize	= borderSize;
	style.FrameBorderSize	= borderSize;
	style.TabBorderSize     = 0; //borderSize;
	static constexpr float rounding = 0.0f;
	style.WindowRounding	= rounding;
	style.ChildRounding		= rounding;
	style.FrameRounding		= rounding;
	style.PopupRounding		= rounding;
	style.GrabRounding		= rounding;
	style.TabRounding		= rounding;
	style.ScrollbarRounding = rounding * 8;

	// 5 or 6 (make a decision on one) colors
	// can be dark (format dark to light)
	// or light (format light to dark)
	// [0] (BG)  = background
	// [1] (BA) = background accent
	// [2] (B)   = bar
	// [3] (IH)  = interactible hovered
	// [4] (I)   = interactible
	// [5] (IA)  = interactible active
	// [6] (T)   = text

	//static const ImVec4 colors[4] = {
	//	rgb(9, 38, 53),
	//	rgb(27, 66, 66),
	//	rgb(92, 131, 116),
	//	rgb(158, 200, 185),
	//};

	//static const ImVec4 abcd[4] = {
	//	rgb(0, 11, 88),
	//	rgb(0, 49, 97),
	//	rgb(0, 106, 103),
	//	rgb(255, 244, 183),
	//};

	//static const ImVec4 abcd[4] = {
	//	rgb(10, 38, 71),
	//	rgb(20, 66, 114),
	//	rgb(32, 82, 149),
	//	rgb(44, 116, 179),
	//};
	//
	//static const ImVec4 & A = abcd[0];
	//static const ImVec4 & B = abcd[1];
	//static const ImVec4 & C = abcd[2];
	//static const ImVec4 & D = abcd[3];
	//static const ImVec4	  E = {1, 1, 1, 1};
	//
	//colors[ImGuiCol_Text]                  = E;
	//colors[ImGuiCol_TextDisabled]          = D;
	//colors[ImGuiCol_TextSelectedBg]        = C;
	//colors[ImGuiCol_WindowBg]              = A;
	//colors[ImGuiCol_ChildBg]               = A;
	//colors[ImGuiCol_PopupBg]               = A;
	//colors[ImGuiCol_Border]                = B;
	//colors[ImGuiCol_BorderShadow]          = A;
	//colors[ImGuiCol_FrameBg]               = A;
	//colors[ImGuiCol_FrameBgHovered]        = B;
	//colors[ImGuiCol_FrameBgActive]         = C;
	//colors[ImGuiCol_TitleBg]               = A;
	//colors[ImGuiCol_TitleBgActive]         = B;
	//colors[ImGuiCol_TitleBgCollapsed]      = A;
	//colors[ImGuiCol_MenuBarBg]             = C;
	//colors[ImGuiCol_ScrollbarBg]           = B;
	//colors[ImGuiCol_ScrollbarGrab]         = C;
	//colors[ImGuiCol_ScrollbarGrabHovered]  = D;
	//colors[ImGuiCol_ScrollbarGrabActive]   = D;
	//colors[ImGuiCol_CheckMark]             = C;
	//colors[ImGuiCol_SliderGrab]            = C;
	//colors[ImGuiCol_SliderGrabActive]      = D;
	//colors[ImGuiCol_Button]                = B;
	//colors[ImGuiCol_ButtonHovered]         = C;
	//colors[ImGuiCol_ButtonActive]          = D;
	//colors[ImGuiCol_Header]                = B;
	//colors[ImGuiCol_HeaderHovered]         = C;
	//colors[ImGuiCol_HeaderActive]          = D;
	//colors[ImGuiCol_Separator]             = B;
	//colors[ImGuiCol_SeparatorHovered]      = C;
	//colors[ImGuiCol_SeparatorActive]       = D;
	//colors[ImGuiCol_ResizeGrip]            = B;
	//colors[ImGuiCol_ResizeGripHovered]     = C;
	//colors[ImGuiCol_ResizeGripActive]      = D;
	//colors[ImGuiCol_Tab]                   = B;
	//colors[ImGuiCol_TabHovered]            = C;
	//colors[ImGuiCol_TabActive]             = D;
	//colors[ImGuiCol_TabUnfocused]          = C;
	//colors[ImGuiCol_TabUnfocusedActive]    = D;
	//colors[ImGuiCol_DockingPreview]        = B;
	//colors[ImGuiCol_DockingEmptyBg]        = A;
	//colors[ImGuiCol_PlotLines]             = B;
	//colors[ImGuiCol_PlotLinesHovered]      = C;
	//colors[ImGuiCol_PlotHistogram]         = B;
	//colors[ImGuiCol_PlotHistogramHovered]  = C;
	//colors[ImGuiCol_TableHeaderBg]         = A;
	//colors[ImGuiCol_TableBorderStrong]     = D;
	//colors[ImGuiCol_TableBorderLight]      = C;
	//colors[ImGuiCol_TableRowBg]            = C;
	//colors[ImGuiCol_TableRowBgAlt]         = D;
	//colors[ImGuiCol_DragDropTarget]        = E;
	//colors[ImGuiCol_NavHighlight]          = D;
	//colors[ImGuiCol_NavWindowingHighlight] = D;
	//colors[ImGuiCol_NavWindowingDimBg]     = B;
	//colors[ImGuiCol_ModalWindowDimBg]      = B;

    colors[ImGuiCol_Text]                  = {1.00f, 1.00f, 1.00f, 1.00f};
    colors[ImGuiCol_TextDisabled]          = {0.50f, 0.50f, 0.50f, 1.00f};
    colors[ImGuiCol_TextSelectedBg]        = {0.20f, 0.22f, 0.23f, 1.00f};
    colors[ImGuiCol_WindowBg]              = {0.10f, 0.10f, 0.10f, 1.00f};
    colors[ImGuiCol_ChildBg]               = {0.00f, 0.00f, 0.00f, 0.00f};
    colors[ImGuiCol_PopupBg]               = {0.14f, 0.14f, 0.14f, 0.95f};
    colors[ImGuiCol_Border]                = {0.39f, 0.39f, 0.39f, 0.39f};
    colors[ImGuiCol_BorderShadow]          = {0.00f, 0.00f, 0.00f, 0.24f};
    colors[ImGuiCol_FrameBg]               = {0.20f, 0.20f, 0.20f, 0.54f};
    colors[ImGuiCol_FrameBgHovered]        = {0.19f, 0.19f, 0.19f, 0.54f};
    colors[ImGuiCol_FrameBgActive]         = {0.20f, 0.22f, 0.23f, 1.00f};
    colors[ImGuiCol_TitleBg]               = {0.00f, 0.00f, 0.00f, 1.00f};
    colors[ImGuiCol_TitleBgActive]         = {0.06f, 0.06f, 0.06f, 1.00f};
    colors[ImGuiCol_TitleBgCollapsed]      = {0.00f, 0.00f, 0.00f, 1.00f};
    colors[ImGuiCol_MenuBarBg]             = {0.14f, 0.14f, 0.14f, 1.00f};
    colors[ImGuiCol_ScrollbarBg]           = {0.05f, 0.05f, 0.05f, 0.54f};
    colors[ImGuiCol_ScrollbarGrab]         = {0.34f, 0.34f, 0.34f, 0.54f};
    colors[ImGuiCol_ScrollbarGrabHovered]  = {0.40f, 0.40f, 0.40f, 0.54f};
    colors[ImGuiCol_ScrollbarGrabActive]   = {0.56f, 0.56f, 0.56f, 0.54f};
    colors[ImGuiCol_CheckMark]             = {0.33f, 0.67f, 0.86f, 1.00f};
    colors[ImGuiCol_SliderGrab]            = {0.34f, 0.34f, 0.34f, 0.54f};
    colors[ImGuiCol_SliderGrabActive]      = {0.56f, 0.56f, 0.56f, 0.54f};
    colors[ImGuiCol_Button]                = {0.05f, 0.05f, 0.05f, 0.54f};
    colors[ImGuiCol_ButtonHovered]         = {0.19f, 0.19f, 0.19f, 0.54f};
    colors[ImGuiCol_ButtonActive]          = {0.20f, 0.22f, 0.23f, 1.00f};
    colors[ImGuiCol_Header]                = {0.00f, 0.00f, 0.00f, 0.52f};
    colors[ImGuiCol_HeaderHovered]         = {0.00f, 0.00f, 0.00f, 0.36f};
    colors[ImGuiCol_HeaderActive]          = {0.20f, 0.22f, 0.23f, 0.33f};
    colors[ImGuiCol_Separator]             = {0.28f, 0.28f, 0.28f, 0.29f};
    colors[ImGuiCol_SeparatorHovered]      = {0.44f, 0.44f, 0.44f, 0.29f};
    colors[ImGuiCol_SeparatorActive]       = {0.40f, 0.44f, 0.47f, 1.00f};
    colors[ImGuiCol_ResizeGrip]            = {0.28f, 0.28f, 0.28f, 0.29f};
    colors[ImGuiCol_ResizeGripHovered]     = {0.44f, 0.44f, 0.44f, 0.29f};
    colors[ImGuiCol_ResizeGripActive]      = {0.40f, 0.44f, 0.47f, 1.00f};
    colors[ImGuiCol_Tab]                   = {0.00f, 0.00f, 0.00f, 0.52f};
    colors[ImGuiCol_TabHovered]            = {0.14f, 0.14f, 0.14f, 1.00f};
    colors[ImGuiCol_TabActive]             = {0.20f, 0.20f, 0.20f, 0.36f};
    colors[ImGuiCol_TabUnfocused]          = {0.00f, 0.00f, 0.00f, 0.52f};
    colors[ImGuiCol_TabUnfocusedActive]    = {0.14f, 0.14f, 0.14f, 1.00f};
    colors[ImGuiCol_DockingPreview]        = {0.33f, 0.67f, 0.86f, 1.00f};
    colors[ImGuiCol_DockingEmptyBg]        = {0.14f, 0.14f, 0.14f, 1.00f};
    colors[ImGuiCol_PlotLines]             = {0.28f, 0.28f, 0.28f, 1.00f};
    colors[ImGuiCol_PlotLinesHovered]      = {0.28f, 0.28f, 0.28f, 1.00f};
    colors[ImGuiCol_PlotHistogram]         = {0.28f, 0.28f, 0.28f, 1.00f};
    colors[ImGuiCol_PlotHistogramHovered]  = {0.28f, 0.28f, 0.28f, 1.00f};
    colors[ImGuiCol_TableHeaderBg]         = {0.00f, 0.00f, 0.00f, 0.52f};
    colors[ImGuiCol_TableBorderStrong]     = {0.00f, 0.00f, 0.00f, 0.52f};
    colors[ImGuiCol_TableBorderLight]      = {0.28f, 0.28f, 0.28f, 0.29f};
    colors[ImGuiCol_TableRowBg]            = {0.00f, 0.00f, 0.00f, 0.00f};
    colors[ImGuiCol_TableRowBgAlt]         = {1.00f, 1.00f, 1.00f, 0.06f};
    colors[ImGuiCol_DragDropTarget]        = {0.33f, 0.67f, 0.86f, 1.00f};
    colors[ImGuiCol_NavHighlight]          = {0.28f, 0.28f, 0.28f, 0.29f};
    colors[ImGuiCol_NavWindowingHighlight] = {0.28f, 0.28f, 0.28f, 0.29f};
    colors[ImGuiCol_NavWindowingDimBg]     = {0.28f, 0.28f, 0.28f, 0.29f};
    colors[ImGuiCol_ModalWindowDimBg]      = {0.28f, 0.28f, 0.28f, 0.29f};
}

static bool do_nothing() {
	return false;
}

void fields_engine::editor::open_icon_selector() {
	ImGui::OpenPopup("icon_selector_popup");
}

// static
bool fields_engine::editor::icon_selector_popup(editor_icon& selected) {

	const float width = ImGui::GetContentRegionAvail().x;
	constexpr float min_width = 20;
	const int num_cols = int(std::ceilf(width / min_width));
	bool changed = false;
	if (ImGui::BeginPopup("icon_selector_popup")) {
		static string search = "";
		ImGui::InputTextWithHint("###Search Icon", "Search for an icon", &search);
		if (ImGui::BeginTable("Icon Selection Table", num_cols, ImGuiTableFlags_SizingStretchSame)) {
			int pos = 0;
			for (int i = 0; i < all_editor_icons.size(); ++i) {
				editor_icon_info const& icon_info = all_editor_icons[i];
				if (!text::is_relevant(icon_info.pretty_name, search)) {
					continue;
				}
				const int mod = pos++ % num_cols;
				if (mod == 0) {
					ImGui::TableNextRow();
				}
				ImGui::TableSetColumnIndex(mod);
				// The const char* == const char* is jank but won't break anything if it doesn't work as intended
				if (ImGui::Selectable(icon_info.icon, selected == icon_info.icon)) {
					selected = icon_info.icon;
					ImGui::CloseCurrentPopup();
					changed = true;
				}
				if (ImGui::BeginItemTooltip()) {
					ImGui::Text("%s %s", icon_info.icon, icon_info.pretty_name);
					ImGui::EndTooltip();
				}
			}
			ImGui::EndTable();
		}
		ImGui::EndPopup();
	}
	return changed;
}

bool fields_engine::editor::root_window() {
	bool res = ImGui::InputTextWithHint(
		"###root_enter_new_window_name", "Enter Window Name", &m_new_window_buf);

	if (ImGui::Button(m_new_window_icon)) {
		open_icon_selector();
	}
	icon_selector_popup(m_new_window_icon);

	if (ImGui::Button(ICON_SQUARE_PLUS" Create window")) {
		add_window(make_unique<editor_window>(
			m_new_window_buf, do_nothing, m_new_window_icon));
		m_new_window_buf.clear();
		res = true;
	}

	return res;
}

fe::editor_window& fields_engine::editor::add_window(unique_ptr<editor_window>&& new_win) {
	m_recent_windows.push_back(int(m_windows.size()));
	return *m_windows.emplace_back(move(new_win));
}
