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
	ImGui_ImplGlfw_InitForOpenGL(wind.handle, true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImVec2 wSize(300.0f, 500.0f);
	ImGui::SetNextWindowSize(wSize);
}

void fields_engine::editor::update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


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

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	//PGE::FrameBuffer::UnbindFrameBuffer();

	graphics::clear_background();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//PGE::FrameBuffer::BindFrameBuffer();

	//ImGuiIO& io = ImGui::GetIO();
	//
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	GLFWwindow* context = glfwGetCurrentContext();
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//	glfwMakeContextCurrent(backup_current_context);
	//}
}

fields_engine::editor::~editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
