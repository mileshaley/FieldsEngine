/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~*
 * File: editor.cpp															 *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "editor.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

	ImGui::NewFrame();

	if (ImGui::Begin("glung")) {
		ImGui::Text("Hello, World!");
	}
	ImGui::End();

	ImGui::EndFrame();
}

fields_engine::editor::~editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
