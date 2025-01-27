/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_manager.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_manager.h"
#include "asset_loader.h"
#include "context.h"
#include "editor_manager.h"
#include "imgui/imgui.h"
#include "string_util.h"
#include "imgui/imgui_internal.h"

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
		add_asset(file);
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

//fe::asset* fields_engine::asset_manager::add_asset(asset&& new_asset) {
//	string name(new_asset.get_name());
//	const auto [it, success] = m_assets.try_emplace(move(name), move(new_asset));
//	if (success) { return &it->second; }
//	else { return nullptr; }
//}

fe::asset* fields_engine::asset_manager::add_asset(std::filesystem::path const& new_asset_path) {
	auto ext = new_asset_path.extension();
	if (ext != ".fea") {
		return nullptr;
	}
	const auto [it, success] = m_assets.try_emplace(
		new_asset_path.stem().stem().string(), 
		new_asset_path
	);
	if (success) { return &it->second; } 
	else { return nullptr; }
}

/*~-------------------------------------------------------------------------~*\
 * Asset Manager Editor Only Definitions                                     *
\*~-------------------------------------------------------------------------~*/

#if EDITOR

namespace fields_engine {
	struct asset_browser_callbacks {
		static constexpr ImGuiInputTextFlags address_bar_input_flags
			= ImGuiInputTextFlags_NoHorizontalScroll
			| ImGuiInputTextFlags_EnterReturnsTrue
			| ImGuiInputTextFlags_AutoSelectAll
			| ImGuiInputTextFlags_CallbackResize;
		static int address_bar_input_callback(ImGuiInputTextCallbackData* data) {
			asset_manager& manager = *reinterpret_cast<asset_manager*>(data->UserData);
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				manager.m_address_bar_buffer.resize(data->BufTextLen, '\0');
				data->Buf = manager.m_address_bar_buffer.data();
			}
			return 0;
		}

		static constexpr ImGuiInputTextFlags search_bar_input_flags
			= ImGuiInputTextFlags_NoHorizontalScroll
			| ImGuiInputTextFlags_CallbackResize;
		static int search_bar_input_callback(ImGuiInputTextCallbackData* data) {
			asset_manager& manager = *reinterpret_cast<asset_manager*>(data->UserData);
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				manager.m_search_bar_buffer.resize(data->BufTextLen, '\0');
				data->Buf = manager.m_search_bar_buffer.data();
			}
			return 0;
		}

		static constexpr ImGuiInputTextFlags rename_input_flags
			= ImGuiInputTextFlags_NoHorizontalScroll
			| ImGuiInputTextFlags_EnterReturnsTrue
			| ImGuiInputTextFlags_CallbackResize;
		static int rename_input_callback(ImGuiInputTextCallbackData* data) {
			asset_manager& manager = *reinterpret_cast<asset_manager*>(data->UserData);
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				manager.m_rename_buffer.resize(data->BufTextLen, '\0');
				data->Buf = manager.m_rename_buffer.data();
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
	constexpr float name_side_padding = 7;
	constexpr ImVec2 name_text_offset{
		name_side_padding,
		5 + thumbnail_margin.y + thumbnail_size.y
	};
	constexpr ImVec2 type_text_offset{
		name_side_padding,
		name_text_offset.y + 17
	};
	constexpr float offscreen_tolerance = 0.1f;
	constexpr int name_compress_max = 14;
	constexpr ImVec4 invisible{ 0,0,0,0 };

	const ImVec2 tall_input_padding{ ImGui::GetStyle().FramePadding.x, 6 };

	const ImVec2 window_pos = ImGui::GetWindowPos();
	const ImVec2 init_cursor_pos = ImGui::GetCursorPos();

	// Address bar

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
				asset_browser_callbacks::address_bar_input_flags,
				asset_browser_callbacks::address_bar_input_callback,
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
		std::filesystem::path const& curr_directory = m_browser_history.top();
		vector<std::filesystem::path> directories(curr_directory.begin(), curr_directory.end());
		// Index into directories
		for (int i = 0; i < directories.size(); ++i) {
			// If the button is pressed and we aren't trying to move to the current directory
			if (ImGui::Button(directories[i].string().c_str(), ImVec2{0, address_bar_height})
				&& directories[i] != curr_directory.filename()
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
							&& entry.path().filename() != curr_directory.filename()
						) {
							browse_to_directory(std::filesystem::path(entry.path()));
							break;
						}
					}
				}
				ImGui::EndPopup();
			}
		}
		const ImVec2 avail = ImGui::GetContentRegionAvail();
		ImGui::SameLine();
		if (ImGui::InvisibleButton("###asset_browser_address_bar_activate", 
			ImVec2{content_max.x, address_bar_height})
		) {
			m_address_bar_buffer = curr_directory.string();
			m_address_bar_state = address_bar_state::activated;
		}
	}
	
	// Left of search buttons

	ImGui::SetCursorPos(init_cursor_pos + ImVec2{ 0, 40 });
	constexpr ImVec2 search_button_size{ address_bar_height, address_bar_height };
	if (ImGui::Button(ICON_ARROW_ROTATE_RIGHT"###asset_browser_refresh", search_button_size)) {
		m_browser_needs_refresh = true;
	}
	ImGui::SetItemTooltip("Refresh browser for new items");

	const bool no_back_history = m_browser_history.at_bottom();
	const bool no_forth_history = m_browser_history.at_top();
	if (no_back_history) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_CIRCLE_ARROW_LEFT"###asset_browser_back", search_button_size)) {
		m_browser_history.scroll_down();
		m_browser_needs_refresh = true;
	}
	if (no_back_history) {
		ImGui::EndDisabled();
	} else if (!m_browser_history.at_bottom() && ImGui::BeginItemTooltip()) {
		ImGui::Text(("Back to " + m_browser_history[m_browser_history.top_index() - 1].filename().string()).c_str());
		ImGui::EndTooltip();
	}
	if (no_forth_history) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_CIRCLE_ARROW_RIGHT"###asset_browser_forth", search_button_size)) {
		m_browser_history.scroll_up();
		m_browser_needs_refresh = true;
	}
	if (no_forth_history) {
		ImGui::EndDisabled();
	} else if (!m_browser_history.at_top() && ImGui::BeginItemTooltip()){
		ImGui::Text(("Forward to " + m_browser_history[m_browser_history.top_index() + 1].filename().string()).c_str());
		ImGui::EndTooltip();
	}
	ImGui::SameLine();

	// Search bar

	const string search_hint = ICON_MAGNIFYING_GLASS" Search " 
		+ m_browser_history.top().filename().string();
	const ImVec2 search_avail = ImGui::GetContentRegionAvail();
	ImGui::PushItemWidth(search_avail.x);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, tall_input_padding);
	if (ImGui::InputTextWithHint(
		"###asset_browser_search_bar_input",
		search_hint.c_str(),
		m_search_bar_buffer.data(),
		m_search_bar_buffer.size() + 1,
		asset_browser_callbacks::search_bar_input_flags,
		asset_browser_callbacks::search_bar_input_callback,
		this
	)) {
		m_browser_needs_refresh = true;
	}
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();
	
	if (m_browser_needs_refresh) {
		refresh_asset_browser();
	}
	
	ImGui::Separator();
	
	// File viewer

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
		const bool left_clicked = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		const bool right_clicked = ImGui::IsMouseReleased(ImGuiMouseButton_Right);

		// We only need to do this at most once per frame
		// Notice how we are checking for click and not release
		if (m_browser_wait_for_mouse_trigger
			&& ImGui::IsMouseClicked(ImGuiMouseButton_Left)
		) {
			m_browser_wait_for_mouse_trigger = false;
		}

		// We will need to know if any entries were clicked on
		// in case the user wants to deselect by clicking on nothing
		bool any_entry_was_clicked = false;

		for (int i = 0; i < m_browser_entries.size(); ++i) {
			file_entry& entry = m_browser_entries[i];
			ImGui::PushID(&entry);
			const ImVec2 cursor_pos = ImGui::GetCursorPos();
			// Selection & button display logic
			bool& selected = entry.selected;

			bool folder_hovered = selected;
			bool folder_clicked = false;
			if (entry.type == file_type::folder && !folder_hovered) {
				folder_clicked = ImGui::InvisibleButton("", entry_size);
				folder_hovered = ImGui::IsItemHovered();
				ImGui::SetCursorPos(cursor_pos);
			}

			constexpr ImGuiButtonFlags entry_button_flags
				= ImGuiButtonFlags_MouseButtonLeft
				| ImGuiButtonFlags_MouseButtonRight;


			void* const thumbnail = get_thumbnail(entry);

			// Show the button border if the entry type is 
			// a selected or hovered folder, or any other type in any state
			if (entry.type != file_type::folder || folder_hovered) {
				// If we change selected this iteration we still want to revert the color change
				const bool was_selected = selected;
				if (was_selected) {
					//ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f,0.5f,1.0f,1.0f }); // Blue
					ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f,0.8f,0.45f,1.0f }); // Green
				}

				const char* asset_right_click_popup_label = "###asset_right_click_popup";

				if (ImGui::ButtonEx("", entry_size, entry_button_flags) || folder_clicked
					&& !m_browser_wait_for_mouse_trigger
				) {
					if (left_clicked) {
						if (shift_held) {
							// There is no way to deselect with shift click
							if (m_prev_entry_clicked == -1 || m_prev_entry_clicked == i) {
								selected = true;
							} else {
								// Select entries between [prev, current]
								const auto [lower, upper] = std::minmax(m_prev_entry_clicked, i);
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
						m_prev_entry_clicked = i;
					} else if (right_clicked) {
						ImGui::OpenPopup(asset_right_click_popup_label);

					}
					// Remember the last click for any type of click
					any_entry_was_clicked = true;

				// Allow double click to enter directory if the button wasn't single clicked
				} else if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					m_browser_wait_for_mouse_trigger = true;
					if (entry.type == file_type::folder) {
						browse_to_directory(std::filesystem::path(entry.path));
					} else if (entry.type == file_type::asset) {
						/// TODO: This branch should open specific editor for this type
					}
				}
				if (was_selected) {
					ImGui::PopStyleColor(); // Border
				}

				if (ImGui::BeginItemTooltip()) {
					ImGui::Text("Name: %s", entry.path.stem().stem().string().c_str());
					ImGui::Text("Path: %s", entry.path.string().c_str());
					ImGui::EndTooltip();
				} // Tooltip
				

				if (ImGui::BeginPopup(asset_right_click_popup_label)) {
					if (ImGui::MenuItem(ICON_PEN_TO_SQUARE" Edit")) {

					}
					if (ImGui::MenuItem(ICON_I_CURSOR" Rename", "F2")) {
						m_rename_state = rename_state::activated;
						m_rename_idx = i;
						m_rename_buffer = entry.path.stem().stem().string();
					}
					if (ImGui::MenuItem(ICON_SQUARE_PLUS" Duplicate", "Ctrl+D")) {
						for (int i = 2; i < 100; ++i) {
							string new_file_name = entry.path.string();
							const size_t pos = new_file_name.find_first_of('.', 0);
							new_file_name.insert(pos, "_" + std::to_string(i));
							std::filesystem::path new_file_path = new_file_name;
							if (!std::filesystem::exists(new_file_path)) {
								std::filesystem::copy(entry.path, new_file_path);
								add_asset(new_file_path);
								break;
							}
						}
						m_browser_needs_refresh = true;
					}
					if (ImGui::MenuItem(ICON_TRASH_CAN" Delete", "Del")) {

					}
					ImGui::EndPopup();
				} // Popup
			} // Any file or hovered folder

			// Allow drag drop

			if (ImGui::BeginDragDropSource()) {
					string path_str = entry.path.string();
				if (entry.type == file_type::asset) {
					ImGui::SetDragDropPayload("ab_asset", path_str.c_str(), path_str.size());
				} else if (entry.type == file_type::folder) {
					ImGui::SetDragDropPayload("ab_folder", path_str.c_str(), path_str.size());
				} else if (entry.type == file_type::other) {
					ImGui::SetDragDropPayload("ab_other", path_str.c_str(), path_str.size());
				}
				ImGui::Image(thumbnail, { 80,80 }, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Text(entry.path.string().c_str());
				ImGui::EndDragDropSource();
			} // Drag drop source

			if (entry.type == file_type::folder) {
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::GetDragDropPayload()) {
						if (string(payload->DataType).substr(0, 3) == "ab_") {
							std::filesystem::path src_path = string((const char*)payload->Data, payload->DataSize);
							if (std::filesystem::exists(src_path)) {
								if (ImGui::AcceptDragDropPayload(payload->DataType)) {
									std::filesystem::rename(src_path, entry.path / src_path.filename());
									m_browser_needs_refresh = true;
									// Not worth figuring out how to change the history if you move a folder, just reset it
									m_browser_history.clear();
								}
							}
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			// Entry type

			ImGui::SetCursorPos(cursor_pos + type_text_offset);
			if (entry.type == file_type::asset) {
				// entry.asset is guaranteed to be non-null
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f), // Transparent white
					entry.asset->get_type().c_str()
				);
			} else if (entry.type == file_type::other) {
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f), // Transparent white
					"unknown"
				);
			}

			// Entry rename/name

			ImGui::SetCursorPos(cursor_pos + name_text_offset);
			if (i == m_rename_idx && 
				(m_rename_state == rename_state::activated || m_rename_state == rename_state::active)
			) {
				ImGui::SetNextItemWidth(entry_size.x - name_side_padding * 2);
				if (ImGui::InputText(
					"###asset_browser_rename_input",
					m_rename_buffer.data(),
					m_rename_buffer.size() + 1,
					asset_browser_callbacks::rename_input_flags,
					asset_browser_callbacks::rename_input_callback,
					this
				)) {
					if (entry.type == file_type::asset) {
						auto it = m_assets.find(m_rename_buffer);
						if (it == m_assets.end()) {
							/// TODO: Implement
						}
					} else {
						//if (!std::filesystem::exists(entry.path.string().replace(entry.path.string().find())))
					}

					m_rename_idx = -1;
					m_rename_state = rename_state::inactive;
				} 

				if (m_rename_state == rename_state::activated) {
					ImGui::SetKeyboardFocusHere(-1);
					m_rename_state = rename_state::active;
				} else if (ImGui::IsItemDeactivated()) {
					m_rename_idx = -1;
					m_rename_state = rename_state::inactive;
				}
			} else {
				if (entry.type == file_type::asset) {
					// Assets have type info in their name and we do not want to display it
					ImGui::Text(ellipsis_compress_middle(
						entry.path.stem().stem().string(), name_compress_max).c_str());
				} else {
					ImGui::Text(ellipsis_compress_middle(
						entry.path.filename().string(), name_compress_max).c_str());
				}
			}

			// Entry thumbnail

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
			ImGui::PopID(); // entry's address
		} // for each entry
		
		// Deselect everything if window background is clicked without any modifier keys
		if (!any_entry_was_clicked && !ctrl_held && !shift_held
			&& ImGui::IsWindowHovered()
		) {
			if (left_clicked) {
				// We arent in the for i loop anymore but j is kept for consistency with above
				for (int j = 0; j < m_browser_entries.size(); ++j) {
					m_browser_entries[j].selected = false;
				}
			}
			if (right_clicked) {
				ImGui::OpenPopup("###browser_right_click_popup");
			}
		}

		if (ImGui::BeginPopup("###browser_right_click_popup")) {
			if (ImGui::MenuItem(ICON_FOLDER_PLUS" Create Folder")) {
				const string new_folder_name = (m_browser_history.top() / "New Folder").string();
				if (!std::filesystem::exists(new_folder_name)) {
					std::filesystem::create_directory(new_folder_name);
				} else {
					for (int i = 2; i < 100; ++i) {
						const string new_folder_name_num = new_folder_name + " (" + std::to_string(i) + ")";
						if (!std::filesystem::exists(new_folder_name_num)) {
							std::filesystem::create_directory(new_folder_name_num);
							break;
						}
					}
				}
				m_browser_needs_refresh = true;
			}
			if (ImGui::MenuItem(ICON_SQUARE_PLUS" Create Asset...")) {

			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(2); // Border & border shadow
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
		std::filesystem::directory_iterator curr_directory(m_browser_history.top());
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
		std::filesystem::recursive_directory_iterator curr_directory(m_browser_history.top());
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

void fields_engine::asset_manager::browse_to_directory(std::filesystem::path&& target) {
	m_browser_history.emplace(move(target));
	m_browser_needs_refresh = true;
}
void* fields_engine::asset_manager::get_thumbnail(file_entry const& entry) {
	if (entry.type == file_type::asset) {
		if (void* thumbnail = entry.asset->get_thumbnail()) {
			return thumbnail;
		} else if (entry.asset->get_type() == "mesh") {
			return m_mesh_thumbnail->get_void_ptr_id();
		} else if (entry.asset->get_type() == "material") {
			return m_material_thumbnail->get_void_ptr_id();
		} else {
			return m_missing_thumbnail->get_void_ptr_id();
		}
	} else if (entry.type == file_type::folder) {
		return m_folder_thumbnail->get_void_ptr_id();
	} else if (entry.type == file_type::other) {
		return m_missing_thumbnail->get_void_ptr_id();
	}
	return nullptr;
}
#endif // EDITOR
