/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_manager.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "precompiled.h"
#include "asset_manager.h"
#include <filesystem>
#include "asset_loader.h"
#include "context.h"
#include "editor_manager.h"
#include "imgui.h"
#include "string_util.h"


#include "texture.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fields_engine::asset_manager::asset_manager()
	: m_assets()
{
}

fields_engine::asset_manager::~asset_manager() = default;

bool fields_engine::asset_manager::startup() {
	std::filesystem::recursive_directory_iterator content_directory("assets");
	for (auto const& file : content_directory) {
		std::filesystem::path in_path = file;
		auto ext = in_path.extension();
		if (ext != ".fea") {
			continue;
		}
		/// TODO: add debug only check for assets with duplicate names
		m_assets.emplace(
			in_path.stem().stem().string(),
			in_path
		);
	}

#if EDITOR
	context<editor::editor_manager>().add_window(make_box<editor::editor_window>(
		"Asset Browser",
		std::bind(&asset_manager::asset_browser_window, this),
		ICON_FOLDER
	));
	m_missing_thumbnail = make_box<vis::texture>(string_view("engine_assets/missing_asset_thumbnail.png"));
	m_mesh_thumbnail = make_box<vis::texture>(string_view("engine_assets/mesh_asset_thumbnail.png"));
	m_material_thumbnail = make_box<vis::texture>(string_view("engine_assets/material_asset_thumbnail.png"));
	m_folder_thumbnail = make_box<vis::texture>(string_view("engine_assets/folder_thumbnail.png"));
	refresh_asset_browser();
#endif // EDITOR

	return true;
}

bool fields_engine::asset_manager::shutdown() {
#if EDITOR
	m_missing_thumbnail.reset();
	m_mesh_thumbnail.reset();
	m_material_thumbnail.reset();
	m_folder_thumbnail.reset();
#endif // EDITOR
	m_assets.clear();
	return true;
}

fe::asset* fields_engine::asset_manager::get_asset(string const& asset_name) {
	auto it = m_assets.find(asset_name);
	if (it == m_assets.end()) { return nullptr; } // Asset not found
	if (!it->second.load()) { return nullptr; } // Asset couldn't be loaded
	return &it->second;
}

fe::asset const* fields_engine::asset_manager::get_asset(string const& asset_name) const {
	auto it = m_assets.find(asset_name);
	if (it == m_assets.end()) { return nullptr; }
	return &it->second;
}

fe::asset* fields_engine::asset_manager::add_asset(asset&& new_asset) {
	string name(new_asset.get_name());
	const auto [it, success] = m_assets.try_emplace(move(name), move(new_asset));
	if (success) { return &it->second; }
	else { return nullptr; }
}

#if EDITOR

namespace fields_engine {
	struct asset_browser_callback_wrapper {
		static int address_bar_input_callback(ImGuiInputTextCallbackData* data) {
			asset_manager& manager = *reinterpret_cast<asset_manager*>(data->UserData);
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				manager.m_address_bar_buffer.resize(data->BufTextLen, '\0');
				data->Buf = manager.m_address_bar_buffer.data();
			} 
			//else if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
			//
			//} else if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
			//
			//}
			return 0;
		}

		static int search_bar_input_callback(ImGuiInputTextCallbackData* data) {
			asset_manager& manager = *reinterpret_cast<asset_manager*>(data->UserData);
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				manager.m_search_bar_buffer.resize(data->BufTextLen, '\0');
				data->Buf = manager.m_search_bar_buffer.data();
			}
			return 0;
		}
	};
}

bool fields_engine::asset_manager::asset_browser_window() {
	constexpr ImVec2 entry_size{ 120, 156 };
	constexpr ImVec2 thumbnail_size{ 100, 100 };
	constexpr ImVec2 thumbnail_margin{
		(entry_size.x - thumbnail_size.x) / 2,
		(entry_size.x - thumbnail_size.x) / 2
	};
	constexpr float pad_between = 15;
	constexpr ImVec2 name_text_offset{
		7,
		5 + thumbnail_margin.y + thumbnail_size.y
	};
	constexpr ImVec2 type_text_offset{
		name_text_offset.x,
		name_text_offset.y + 17
	};
	constexpr float offscreen_tolerance = 0.1f;
	constexpr int name_compress_max = 14;
	constexpr ImVec4 invisible{ 0,0,0,0 };

	const ImVec2 tall_input_padding{ ImGui::GetStyle().FramePadding.x, 6 };

	const ImVec2 window_pos = ImGui::GetWindowPos();
	const ImVec2 init_cursor_pos = ImGui::GetCursorPos();

	// Address bar

	constexpr ImGuiInputTextFlags address_bar_input_flags
		= ImGuiInputTextFlags_NoHorizontalScroll
		| ImGuiInputTextFlags_EnterReturnsTrue
		| ImGuiInputTextFlags_AutoSelectAll
		| ImGuiInputTextFlags_CallbackResize;
		//| ImGuiInputTextFlags_CallbackEdit
		//| ImGuiInputTextFlags_CallbackCompletion
		//| ImGuiInputTextFlags_CallbackHistory;


	constexpr float address_bar_height = 30;
	constexpr float address_bar_rounding = 3;
	const ImVec2 content_max = ImGui::GetContentRegionMax();
	const ImVec2 address_bar_max = window_pos
		+ ImVec2{content_max.x, init_cursor_pos.y + address_bar_height };
	ImGui::GetWindowDrawList()->AddRectFilled(
		window_pos + init_cursor_pos, 
		address_bar_max, 
		ImGui::ColorConvertFloat4ToU32({0,0,0,1}), 
		address_bar_rounding
	);
	ImGui::SetCursorPos(init_cursor_pos + ImVec2{ 10, 0 });
	if (m_address_bar_state == address_bar_state::activated
		|| m_address_bar_state == address_bar_state::active
	) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, invisible);
		ImGui::PushItemWidth(content_max.x - 20);
		// Increase the height of the text input to roughly match that of the buttons
		// This doesn't affect later items so we can afford to be imprecise
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, tall_input_padding);
		if (ImGui::InputText(
				"###asset_browser_address_bar_input", 
				m_address_bar_buffer.data(), 
				m_address_bar_buffer.size() + 1,
				address_bar_input_flags,
				asset_browser_callback_wrapper::address_bar_input_callback,
				this
		)) {
			// Attempt to go to the directory the user typed
			std::filesystem::path path(m_address_bar_buffer);
			if (std::filesystem::exists(path)) {
				browse_to_directory(move(path));
				m_address_bar_state = address_bar_state::inactive;
			}
		}

		if (m_address_bar_state == address_bar_state::activated) {
			ImGui::SetKeyboardFocusHere(-1);
			m_address_bar_state = address_bar_state::active;
		} else if (ImGui::IsItemDeactivated()) {
			m_address_bar_state = address_bar_state::inactive;
		}
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

	} else {
		vector<std::filesystem::path> directories(
			m_browser_current_directory.begin(), m_browser_current_directory.end());
		// Index into directories
		for (int i = 0; i < directories.size(); ++i) {
			// If the button is pressed and we aren't trying to move to the current directory
			if (ImGui::Button(directories[i].string().c_str(), ImVec2{0, address_bar_height})
				&& directories[i] != m_browser_current_directory.filename()
			) {
				std::filesystem::path new_path{};
				// Reconstruct the new directory path
				for (int j = 0; j <= i; ++j) {
					new_path /= directories[j];
				}
				browse_to_directory(move(new_path));
				break;
			}

			const string dropdown_id("ast_ddc" + std::to_string(i));
			ImGui::SameLine();
			string button_text
				= (ImGui::IsPopupOpen(dropdown_id.c_str()) ? ICON_CHEVRON_DOWN : ICON_CHEVRON_RIGHT)
				+ string("###button")
				+ std::to_string(i);
			if (ImGui::Button(button_text.c_str(), ImVec2{ 0, address_bar_height })) {
				ImGui::OpenPopup(dropdown_id.c_str());
			}
			ImGui::SameLine();
			

			// Directory navigation dropdown
			if (ImGui::BeginPopup(dropdown_id.c_str())) {
				std::filesystem::path selected_path{};
				for (int j = 0; j <= i; ++j) {
					selected_path /= directories[j];
				}
				std::filesystem::directory_iterator dir_it(selected_path);
				for (std::filesystem::directory_entry const& entry : dir_it) {
					if (entry.is_directory()) {
						// If the button is pressed and we aren't trying to move to the current directory
						if (ImGui::Selectable(entry.path().filename().string().c_str())
							&& entry.path().filename() != m_browser_current_directory.filename()
						) {
							browse_to_directory(entry.path());
							break;
						}
					}
				}
				ImGui::EndPopup();
			}
		}
		ImGui::PushStyleColor(ImGuiCol_Button, invisible);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, invisible);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, invisible);
		const ImVec2 avail = ImGui::GetContentRegionAvail();
		ImGui::SameLine();
		if (ImGui::Button("###asset_browser_address_bar_activate", ImVec2{content_max.x, address_bar_height})) {
			m_address_bar_buffer = m_browser_current_directory.string();
			m_address_bar_state = address_bar_state::activated;
		}
		ImGui::PopStyleColor(3);
	}
	
	ImGui::SetCursorPos(init_cursor_pos + ImVec2{ 0, 40 });
	constexpr ImVec2 search_button_size{ address_bar_height, address_bar_height };
	if (ImGui::Button(ICON_ARROW_ROTATE_RIGHT"###asset_browser_refresh", search_button_size)) {
		m_browser_needs_refresh = true;
	}

	const bool no_back_history = m_browser_back_history.empty();
	const bool no_forth_history = m_browser_forth_history.empty();
	if (no_back_history) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_CIRCLE_ARROW_LEFT"###asset_browser_back", search_button_size)) {
		m_browser_forth_history.emplace(move(m_browser_current_directory));
		m_browser_current_directory = move(m_browser_back_history.top());
		m_browser_back_history.pop();
		m_browser_needs_refresh = true;
	}
	if (no_back_history) {
		ImGui::EndDisabled();
	}
	if (no_forth_history) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_CIRCLE_ARROW_RIGHT"###asset_browser_forth", search_button_size)) {
		m_browser_back_history.emplace(move(m_browser_current_directory));
		m_browser_current_directory = move(m_browser_forth_history.top());
		m_browser_forth_history.pop();
		m_browser_needs_refresh = true;
	}
	if (no_forth_history) {
		ImGui::EndDisabled();
	}
	ImGui::SameLine();

	// Search bar

	constexpr ImGuiInputTextFlags search_bar_input_flags
		= ImGuiInputTextFlags_NoHorizontalScroll
		| ImGuiInputTextFlags_CallbackResize;
	const string search_hint = ICON_MAGNIFYING_GLASS" Search " 
		+ (--m_browser_current_directory.end())->string();
	const ImVec2 search_avail = ImGui::GetContentRegionAvail();
	ImGui::PushItemWidth(search_avail.x);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, tall_input_padding);
	if (ImGui::InputTextWithHint(
		"###asset_browser_search_bar_input",
		search_hint.c_str(),
		m_search_bar_buffer.data(),
		m_search_bar_buffer.size() + 1,
		search_bar_input_flags,
		asset_browser_callback_wrapper::search_bar_input_callback,
		this
	)) {
		m_browser_needs_refresh = true;
	}
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();
	
	if (m_browser_needs_refresh) {
		refresh_asset_browser();
	}
	
	// File viewer
	ImGui::Separator();

	if (ImGui::BeginChild("asset_browser_child")) {

		ImGuiStyle const& style = ImGui::GetStyle();
		// Make outlines visible
		ImVec4 border = style.Colors[ImGuiCol_Border];
		ImVec4 border_shadow = style.Colors[ImGuiCol_BorderShadow];
		border.w = border.z;
		border_shadow.w = border.z;
		ImGui::PushStyleColor(ImGuiCol_Border, border);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, border_shadow);

		const bool ctrl_held = ImGui::GetIO().KeyCtrl;
		const bool shift_held = ImGui::GetIO().KeyShift;

		// We will need to know if any entries were clicked on
		// in case the user wants to deselect by clicking on nothing
		bool entry_was_clicked = false;

		for (int i = 0; i < m_browser_entries.size(); ++i) {
			file_entry const& entry = m_browser_entries[i];
			/// TODO: this doesn't really make sense, fix it
			/// Also when fixed, update the comment on the matching PopID() call
			ImGui::PushID(&entry);
			const ImVec2 cursor_pos = ImGui::GetCursorPos();

			// Selection & button display logic

			bool& selected = m_browser_entries[i].selected;

			// Show the button border if the entry type is 
			// a selected or hovered folder, or any other type in any state
			if (entry.type != file_type::folder 
				|| selected
				|| ImGui::IsMouseHoveringRect(
					window_pos + cursor_pos, window_pos + cursor_pos + entry_size)
			) {
				// If we change selected this iteration we still want to revert the color change
				const bool was_selected = selected;
				if (was_selected) {
					//ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f,0.5f,1.0f,1.0f }); // Blue
					ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f,0.8f,0.45f,1.0f }); // Green
				}
				if (ImGui::Button("", entry_size)) {
					if (shift_held) {
						// There is no way to deselect with shift click
						if (m_prev_entry_clicked == -1 || m_prev_entry_clicked == i) {
							selected = true;
						} else {
							// Select entries between [prev, current]
							const auto[lower, upper] = std::minmax(m_prev_entry_clicked, i);
							for (int j = lower; j <= upper; ++j) {
								m_browser_entries[j].selected = true;
							}
						}
					} else if (ctrl_held) {
						selected = !selected;
					} else {
						for (int j = 0; j < m_browser_entries.size(); ++j) {
							m_browser_entries[j].selected = false;
						}
						selected = true;
					}
					// Remember the last click for any type of click
					m_prev_entry_clicked = i;
					entry_was_clicked = true;

				// Allow double click to enter directory if the button wasn't single clicked
				} else if (entry.type == file_type::folder
					&& ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)
				) {
					browse_to_directory(entry.path);
				}

				if (was_selected) {
					ImGui::PopStyleColor(); // Border
				}
			}

			// Entry info display logic

			ImGui::SetCursorPos(cursor_pos + type_text_offset);
			if (entry.type == file_type::asset) {
				// entry.asset is guaranteed to be non-null
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f), // Transparent white
					entry.asset->get_type().c_str()
				);
			} else if (entry.type == file_type::other) {
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f),
					"unknown"
				);
			}

			ImGui::SetCursorPos(cursor_pos + name_text_offset);
			// Assets have type info in their name and we do not want to display it
			if (entry.type == file_type::asset) {
				ImGui::Text(ellipsis_compress_middle(
					entry.path.stem().stem().string(), name_compress_max).c_str());
			} else {
				ImGui::Text(ellipsis_compress_middle(
					entry.path.filename().string(), name_compress_max).c_str());
			}

			void* thumbnail = nullptr;
			
			if (entry.type == file_type::asset) {
				thumbnail = entry.asset->get_thumbnail();
				if (!thumbnail) {
					if (entry.asset->get_type() == "mesh") {
						thumbnail = m_mesh_thumbnail->get_void_ptr_id();
					} else if (entry.asset->get_type() == "material") {
						thumbnail = m_material_thumbnail->get_void_ptr_id();
					} else {
						thumbnail = m_missing_thumbnail->get_void_ptr_id();
					}
				}
			} else if (entry.type == file_type::folder) {
				thumbnail = m_folder_thumbnail->get_void_ptr_id();
			} else if (entry.type == file_type::other) {
				thumbnail = m_missing_thumbnail->get_void_ptr_id();
			}
			ImGui::SetCursorPos(cursor_pos + thumbnail_margin);
			// For some reason ImGui textures are flipped, so we adjust uvs manually here
			ImGui::Image(thumbnail, thumbnail_size, ImVec2(0,1), ImVec2(1,0));
			// We want to check if the next item will be put offscreen
			if (cursor_pos.x + (2.0f - offscreen_tolerance) * entry_size.x + pad_between > content_max.x) {
				// Start a new line
				ImGui::SetCursorPosY(cursor_pos.y + entry_size.y + pad_between);
			} else {
				// Continue with next item horizontally
				ImGui::SetCursorPos(cursor_pos + ImVec2(entry_size.x + pad_between, 0));
			}
			ImGui::PopID(); // entry address
		}
		
		// Deselect everything if window background is clicked without any modifier keys
		if (!entry_was_clicked && !ctrl_held && !shift_held
			&& ImGui::IsMouseReleased(ImGuiMouseButton_Left)
			&& ImGui::IsWindowHovered()
		) {
			// We arent in the for i loop anymore but j is kept for consistency with above
			for (int j = 0; j < m_browser_entries.size(); ++j) {
				m_browser_entries[j].selected = false;
			}
		}

		ImGui::PopStyleColor(2);
	}
	ImGui::EndChild();
	return false;
}

void fields_engine::asset_manager::refresh_asset_browser() {
	m_browser_needs_refresh = false;
	m_browser_entries.clear();
	m_prev_entry_clicked = -1;
	m_address_bar_state = address_bar_state::inactive;

	if (m_search_bar_buffer.empty()) {
		std::filesystem::directory_iterator curr_directory(m_browser_current_directory);
		for (std::filesystem::directory_entry const& entry : curr_directory) {
			std::filesystem::path const& path = entry.path();
			if (entry.is_directory()) {
				m_browser_entries.push_back(file_entry{
					path, nullptr, file_type::folder
					});
				continue;
			}
			if (path.extension() == ".fea") {
				auto it = m_assets.find(path.stem().stem().string());
				if (it != m_assets.end()) {
					m_browser_entries.push_back(file_entry{
						path, &it->second, file_type::asset
						});
					continue;
				}
			}
			m_browser_entries.push_back(file_entry{
				path, nullptr, file_type::other
				});
		}
	} else {
		std::filesystem::recursive_directory_iterator curr_directory(m_browser_current_directory);
		for (std::filesystem::directory_entry const& entry : curr_directory) {
			std::filesystem::path const& path = entry.path();
			if (entry.is_directory()) { continue; }
			if (path.filename().string().find(m_search_bar_buffer) == string::npos) {
				continue;
			}
			if (path.extension() == ".fea") {
				auto it = m_assets.find(path.stem().stem().string());
				if (it != m_assets.end()) {
					m_browser_entries.push_back(file_entry{
						path, &it->second, file_type::asset
					});
					continue;
				}
			}
			m_browser_entries.push_back(file_entry{
				path, nullptr, file_type::other
			});
		}
	}

	std::sort(m_browser_entries.begin(), m_browser_entries.end(), 
		[](file_entry const& l, file_entry const& r) {
			return int(l.type) < int(r.type);
		});
}

void fields_engine::asset_manager::browse_to_directory(std::filesystem::path const& target) {
	m_browser_back_history.emplace(move(m_browser_current_directory));
	m_browser_current_directory = target;
	m_browser_needs_refresh = true;
}

void fields_engine::asset_manager::browse_to_directory(std::filesystem::path&& target) {
	m_browser_back_history.emplace(move(m_browser_current_directory));
	m_browser_current_directory = move(target);
	m_browser_needs_refresh = true;
}
#endif // EDITOR
