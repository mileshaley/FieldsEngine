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

fields_engine::editor::editor(window& wind) 
	: context_(ImGui::CreateContext())
{
	ImGui::SetCurrentContext(context_);
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	const float fontSize = 17.0f;

	//std::filesystem::path fontsPath("assets");
	//fontsPath /= "fonts";
	// 
	// Add default font so we can merge in icon font
	io.Fonts->AddFontFromFileTTF("assets/fonts/RobotoMono-Regular.ttf"
		/*(fontsPath / "RobotoMono-Regular.ttf").string().c_str()*/, fontSize
	);

	static constexpr ImWchar iconRanges[] = { 
		FA_ICON_FONT_MIN, FA_ICON_FONT_MAX_16, 0 
	};

	ImFontConfig iconConfig;
	iconConfig.MergeMode = true;
	iconConfig.PixelSnapH = true;
	iconConfig.GlyphMinAdvanceX = fontSize;
	iconConfig.GlyphOffset.y = 1.0f;

	io.Fonts->AddFontFromFileTTF("assets/fonts/" FA_SOLID_FONT_ICON_FILENAME
		/*(fontsPath / FA_SOLID_FONT_ICON_FILENAME).string().c_str()*/,
		fontSize, &iconConfig, iconRanges
	);

	ImGui_ImplGlfw_InitForOpenGL(wind.handle, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Style Attributes
	style.WindowPadding = ImVec2(8.00f, 8.00f);
	style.FramePadding = ImVec2(5.00f, 2.00f);
	style.CellPadding = ImVec2(6.00f, 6.00f);
	style.ItemSpacing = ImVec2(6.00f, 6.00f);
	style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	style.LogSliderDeadzone = 4;
	style.IndentSpacing = 25;
	style.ScrollbarSize = 15;
	style.GrabMinSize = 10;
	static constexpr float borderSize = 0.0f;
	style.WindowBorderSize = borderSize;
	style.ChildBorderSize = borderSize;
	style.PopupBorderSize = borderSize;
	style.FrameBorderSize = borderSize;
	style.TabBorderSize = borderSize;
	//style.WindowRounding = 7;
	//style.ChildRounding = 4;
	//style.FrameRounding = 3;
	//style.PopupRounding = 4;
	//style.GrabRounding = 3;
	//style.TabRounding = 4;
	static constexpr float rounding = 0.0f;
	style.WindowRounding = rounding;
	style.ChildRounding = rounding;
	style.FrameRounding = rounding;
	style.PopupRounding = rounding;
	style.GrabRounding = rounding;
	style.TabRounding = rounding;
	style.ScrollbarRounding = 9 * rounding;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.95f);
	colors[ImGuiCol_Border] = ImVec4(0.39f, 0.39f, 0.39f, 0.39f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);

	const ImVec2 winSize(300.0f, 500.0f);
	ImGui::SetNextWindowSize(winSize);
}

void fields_engine::editor::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Begin dockspace
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

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
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	if (ImGui::Begin(ICON_FLOPPY_DISK" Glung")) {
		ImGui::Text("Hello, World!");
		static bool two = false;
		if (ImGui::Button("two")) {
			two = true;
		}
		if (two && ImGui::Button("one")) {
			two = false;
		}
	}

	ImGui::ColorPicker4("", &clor_.r);

	ImGui::End();

	//graphics::clear_background(clor_);

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		//GLFWwindow* context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		//glfwMakeContextCurrent(context);
		g_application->reinstate();
	}
}

fields_engine::editor::~editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
