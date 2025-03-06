/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor_manager.cpp													 *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "editor_manager.h"

#if EDITOR
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "graphics.h"
#include "glfw/include/GLFW/glfw3.h"
#include "application.h"
#include <filesystem>
#include "imgui/imgui_stdlib.h"
#include "editor_icons_all.h"
#include "text.h"
#include "imGui/imgui_internal.h"
#include "scene.h"
#include "context.h"
#include "entity.h"
#include <fstream>

#include <iostream>

/*~-------------------------------------------------------------------------~*\
 * Editor Manager Definitions                                                *
\*~-------------------------------------------------------------------------~*/

fields_engine::editor::editor_manager::editor_manager()
	: m_gui_context(ImGui::CreateContext())
	, m_frame_buffer(ivec2{ 1920, 1080 })
	, m_fonts()
	, m_windows()
	, m_recent_windows()
	, m_game_window_size(1000, 800)
	, m_game_window_focused(true)
	, m_game_window_hovered(true)
	, m_new_window_buf()
	, m_new_window_icon(ICON_ELLIPSIS_VERTICAL)
	, m_ini_name((std::filesystem::path("user_data") / "editor" / "imgui.ini").string())
{}

fields_engine::editor::editor_manager::~editor_manager() = default;

void fields_engine::editor::editor_manager::startup(window_handle& win) {
	//std::ifstream config_file(std::filesystem::path("user_data") / "config.json"));
	//if (config_file) {
	//	m_config = json::parse(config_file, nullptr, false);
	//	if (!m_config.is_discarded()) {
	//		
	//	}
	//}

	ImGui::SetCurrentContext(m_gui_context);
	ImGuiIO& io = ImGui::GetIO();

	io.IniFilename = m_ini_name.c_str();

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup fonts with icon font
	const std::filesystem::path fonts_path = std::filesystem::path("engine_assets") / "fonts";
	const float font_size = 17.0f;

	// Add default font so we can merge in icon font
	m_fonts[size_t(font_type::regular)] = io.Fonts->AddFontFromFileTTF(
		(fonts_path / "Montserrat" / "Montserrat-Medium.ttf").string().c_str(),
		font_size,
		nullptr,
		io.Fonts->GetGlyphRangesDefault()
	);

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

	m_fonts[size_t(font_type::internal_icon)] = (io.Fonts->AddFontFromFileTTF(
		(fonts_path / FA_SOLID_ICON_FONT_FILENAME).string().c_str(),
		16.0f, 
		&icon_config, 
		iconRanges
	));

	// Add other fonts now that icon font has been merged
	m_fonts[size_t(font_type::monospace)] = io.Fonts->AddFontFromFileTTF(
		(fonts_path / "RobotoMono" / "RobotoMono-Regular.ttf").string().c_str(),
		font_size,
		nullptr,
		io.Fonts->GetGlyphRangesDefault()
	);


	ImGui_ImplGlfw_InitForOpenGL(win.handle, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	reset_style();

	editor_window* root = add_window(
		&editor_manager::root_window,
		"Root", 
		ICON_FACE_SMILE
	);

	// Add the window and then set its callback after since it needs to access data inside the window
	editor_window* demo = add_window(
		&editor_manager::demo_window,
		"ImGui Demo", 
		ICON_INFO
	);

	add_window(
		&editor_manager::game_window,
		"Game View",
		ICON_GAMEPAD
	);

	add_window(
		&editor_manager::style_window,
		"Editor Styler",
		ICON_PALETTE
	);
}
void fields_engine::editor::editor_manager::tick(float dt) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Begin dockspace
	
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::Text("FPS: %2f", 1.0f / dt);
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
			for (own<editor_window>& window : m_windows) {
				window->menu_item();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	m_game_window_focused = false;
	m_game_window_hovered = false;

	for (own<editor_window>& window : m_windows) {
		window->display();
	}
	m_frame_buffer.unuse();
	//if (context<input_manager>().was_button_triggered(GLFW_KEY_R)) {
	//	m_frame_buffer.resize(context<application>().get_window_size());
	//}
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_frame_buffer.use();
	m_frame_buffer.viewport();

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
#if FE_USING_GLFW
		glfwMakeContextCurrent(context<application>().get_window_handle().handle);
#elif FE_USING_SDL3
		///
#endif // FE_USING_SDL3
	}
}

void fields_engine::editor::editor_manager::shutdown() {
	for (own<editor_window> const& window : m_windows) {
		window->write();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

//static ImVec4 rgb(int r, int g, int b) {
//	return ImVec4{
//		r / 255.0f,
//		g / 255.0f,
//		b / 255.0f,
//		1.0f
//	};
//}

static bool do_nothing() {
	return false;
}

void fields_engine::editor::editor_manager::open_icon_selector() {
	ImGui::OpenPopup("icon_selector_popup");
}

// static
bool fields_engine::editor::editor_manager::icon_selector_popup(editor_icon& selected) {

	const float width = ImGui::GetContentRegionAvail().x;
	constexpr float min_width = 20;
	
	const int num_cols = int(glm::ceil(width / min_width));
	bool changed = false;
	if (ImGui::BeginPopup("icon_selector_popup")) {
		static string search = "";
		ImGui::InputTextWithHint("###Search Icon", "Search for an icon", &search);
		if (ImGui::BeginTable("Icon Selection Table", num_cols, ImGuiTableFlags_SizingStretchSame)) {
			int pos = 0;
			const int all_icons_count = int(all_editor_icons.size());
			for (int i = 0; i < all_icons_count; ++i) {
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

bool fields_engine::editor::editor_manager::is_capturing_mouse() const {
	return ImGui::GetIO().WantCaptureMouse && !m_game_window_hovered;
}

bool fields_engine::editor::editor_manager::is_capturing_keyboard() const {
	return ImGui::GetIO().WantCaptureKeyboard && !m_game_window_hovered;
}

fe::vis::frame_buffer& fields_engine::editor::editor_manager::ref_frame_buffer() {
	return m_frame_buffer;
}

fe::ivec2 fields_engine::editor::editor_manager::get_game_window_size() const {
	return m_game_window_size;
}

ImFont* fields_engine::editor::editor_manager::get_font_handle(font_type font) {
	return m_fonts[static_cast<size_t>(font)];
}

/*~-------------------------------------------------------------------------~*\
 * Editor Hosted Windows                                                     *
\*~-------------------------------------------------------------------------~*/

bool fields_engine::editor::editor_manager::game_window(editor_window& window) {
	/// TODO: This is janky, fix it
	const ImVec2 size = ImGui::GetContentRegionMax() - ImVec2{ 30, 30 };
	m_game_window_size = { size.x, size.y };
	ImGui::SetCursorPos(ImGui::GetCursorPos());
	ImGui::Image(
		(ImTextureID)(i64)m_frame_buffer.get_texture_id(),
		{ size },
		// Flip the uvs for ImGui
		{ 0, 1 },
		{ 1, 0 }
	);
	m_game_window_hovered = ImGui::IsWindowHovered();
	m_game_window_focused = ImGui::IsWindowFocused();

	return false;
}



static const char* color_names[ImGuiCol_::ImGuiCol_COUNT] = {
	"ImGuiCol_Text",
	"ImGuiCol_TextDisabled",
	"ImGuiCol_WindowBg",					// Background of normal windows
	"ImGuiCol_ChildBg",						// Background of child windows
	"ImGuiCol_PopupBg",						// Background of popups, menus, tooltips windows
	"ImGuiCol_Border",
	"ImGuiCol_BorderShadow",
	"ImGuiCol_FrameBg",						// Background of checkbox, radio button, plot, slider, text input
	"ImGuiCol_FrameBgHovered",
	"ImGuiCol_FrameBgActive",
	"ImGuiCol_TitleBg",						// Title bar
	"ImGuiCol_TitleBgActive",				// Title bar when focused
	"ImGuiCol_TitleBgCollapsed",			// Title bar when collapsed
	"ImGuiCol_MenuBarBg",
	"ImGuiCol_ScrollbarBg",
	"ImGuiCol_ScrollbarGrab",
	"ImGuiCol_ScrollbarGrabHovered",
	"ImGuiCol_ScrollbarGrabActive",
	"ImGuiCol_CheckMark",					// Checkbox tick and RadioButton circle
	"ImGuiCol_SliderGrab",
	"ImGuiCol_SliderGrabActive",
	"ImGuiCol_Button",
	"ImGuiCol_ButtonHovered",
	"ImGuiCol_ButtonActive",
	"ImGuiCol_Header",						// Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
	"ImGuiCol_HeaderHovered",
	"ImGuiCol_HeaderActive",
	"ImGuiCol_Separator",
	"ImGuiCol_SeparatorHovered",
	"ImGuiCol_SeparatorActive",
	"ImGuiCol_ResizeGrip",					// Resize grip in lower-right and lower-left corners of windows.
	"ImGuiCol_ResizeGripHovered",
	"ImGuiCol_ResizeGripActive",
	"ImGuiCol_TabHovered",					// Tab background, when hovered
	"ImGuiCol_Tab",							// Tab background, when tab-bar is focused & tab is unselected
	"ImGuiCol_TabSelected",					// Tab background, when tab-bar is focused & tab is selected
	"ImGuiCol_TabSelectedOverline",			// Tab horizontal overline, when tab-bar is focused & tab is selected
	"ImGuiCol_TabDimmed",					// Tab background, when tab-bar is unfocused & tab is unselected
	"ImGuiCol_TabDimmedSelected",			// Tab background, when tab-bar is unfocused & tab is selected
	"ImGuiCol_TabDimmedSelectedOverline",   //..horizontal overline, when tab-bar is unfocused & tab is selected
	"ImGuiCol_DockingPreview",				// Preview overlay color when about to docking something
	"ImGuiCol_DockingEmptyBg",				// Background color for empty node (e.g. CentralNode with no window docked into it)
	"ImGuiCol_PlotLines",
	"ImGuiCol_PlotLinesHovered",
	"ImGuiCol_PlotHistogram",
	"ImGuiCol_PlotHistogramHovered",
	"ImGuiCol_TableHeaderBg",				// Table header background
	"ImGuiCol_TableBorderStrong",			// Table outer and header borders (prefer using Alpha=1.0 here)
	"ImGuiCol_TableBorderLight",			// Table inner borders (prefer using Alpha=1.0 here)
	"ImGuiCol_TableRowBg",					// Table row background (even rows)
	"ImGuiCol_TableRowBgAlt",				// Table row background (odd rows)
	"ImGuiCol_TextLink",					// Hyperlink color
	"ImGuiCol_TextSelectedBg",
	"ImGuiCol_DragDropTarget",				// Rectangle highlighting a drop target
	"ImGuiCol_NavCursor",					// Color of keyboard/gamepad navigation cursor/rectangle, when visible
	"ImGuiCol_NavWindowingHighlight",		// Highlight window when using CTRL+TAB
	"ImGuiCol_NavWindowingDimBg",			// Darken/colorize entire screen behind the CTRL+TAB window list, when active
	"ImGuiCol_ModalWindowDimBg",			// Darken/colorize entire screen behind a modal window, when one is active
};

bool fields_engine::editor::editor_manager::style_window(editor_window& window) {
	bool modif = false;
	/// TODO: this is a trainwreck, fix eventually (low priority)
	static int slot = 0;
	ImGui::DragInt("Slot", &slot, 1, 0, 9999);
	if (ImGui::Button("  Save  ")) {
		write_style(slot);
	}
	ImGui::SameLine();
	if (ImGui::Button("  Load  ")) {
		read_style(slot);
	}

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	for (int i = 0; i < ImGuiCol_COUNT; ++i) {
		modif |= ImGui::ColorEdit4(color_names[i], &colors[i].x);
	}

	return modif;
}

bool fields_engine::editor::editor_manager::root_window(editor_window& window) {
	bool modif = ImGui::InputTextWithHint(
		"###root_enter_new_window_name", "Enter Window Name", &m_new_window_buf);

	if (ImGui::Button(m_new_window_icon)) {
		open_icon_selector();
	}
	modif |= icon_selector_popup(m_new_window_icon);

	//if (ImGui::Button(ICON_SQUARE_PLUS" Create window")) {
	//	add_window(make_own<editor_window>(
	//		m_new_window_buf, do_nothing, m_new_window_icon));
	//	m_new_window_buf.clear();
	//	modif = true;
	//}
	return modif;
}

bool fields_engine::editor::editor_manager::demo_window(editor_window& window) {
	ImGui::ShowDemoWindow(&window.get_open_flag());
	return false;
}

/*~-------------------------------------------------------------------------~*\
 * Editor Helper Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fe::editor::editor_window* fields_engine::editor::editor_manager::emplace_window(own<editor_window>&& new_window) {
	m_recent_windows.push_back(int(m_windows.size()));
	editor_window* emplaced = m_windows.emplace_back(move(new_window)).get();
	emplaced->read();
	return emplaced;
}

void fields_engine::editor::editor_manager::reset_style() const {
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
	static constexpr float borderSize = 2.0f;
	style.WindowBorderSize	= borderSize;
	style.ChildBorderSize	= borderSize;
	style.PopupBorderSize	= borderSize;
	style.FrameBorderSize	= borderSize;
	style.TabBorderSize     = borderSize;
	static constexpr float rounding = 4.0f;
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
    //colors[ImGuiCol_Border]                = {0.39f, 0.39f, 0.39f, 0.39f};
    //colors[ImGuiCol_BorderShadow]          = {0.00f, 0.00f, 0.00f, 0.24f};
    colors[ImGuiCol_Border]                = {0.39f, 0.39f, 0.39f, 0}; // Invisible
    colors[ImGuiCol_BorderShadow]          = {0.00f, 0.00f, 0.00f, 0}; // Invisible
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

void fields_engine::editor::editor_manager::read_style(int slot) const {
	std::ifstream in_file(std::filesystem::path("test_assets") / ("editor_style" + std::to_string(slot) + ".json"));
	if (!in_file) { return; }
	const json in(json::parse(in_file));
	json const& in_colors = in.at("colors");

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	for (int i = 0; i < ImGuiCol_COUNT; ++i) {
		colors[i] = in_colors[color_names[i]].get<vec4>();
	}
}

void fields_engine::editor::editor_manager::write_style(int slot) const {
	ImGuiStyle const& style = ImGui::GetStyle();
	ImVec4 const* colors = style.Colors;
	std::ofstream out_file(std::filesystem::path("test_assets") / ("editor_style" + std::to_string(slot) + ".json"));
	if (!out_file) { return; }
	json out;
	json& out_colors = out["colors"];
	constexpr int num_colors = sizeof(color_names) / sizeof(color_names[0]);
	for (int i = 0; i < num_colors; ++i) {
		out_colors[color_names[i]] = vec4(colors[i]);
	}

	out_file << std::setw(4) << out << std::endl;
	out_file.close();
}

#endif // EDITOR
