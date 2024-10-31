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

fields_engine::editor::editor(window& wind) 
	: context_(ImGui::CreateContext())
{
	ImGui::SetCurrentContext(context_);
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(wind.handle, true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImVec2 wSize(300.0f, 500.0f);
	ImGui::SetNextWindowSize(wSize);
}

void fields_engine::editor::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

	if (ImGui::Begin("glung")) {
		ImGui::Text("Hello, World!");
		static bool two = false;
		if (ImGui::Button("two")) {
			two = true;
		}
		if (two && ImGui::Button("one")) {
			two = false;
		}
	}
	ImGui::End();

	graphics::clear_background();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(context);
	}
}

fields_engine::editor::~editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
