/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~*
 * File: asset_browser.cpp                                                   *
\*~-------------------------------------------------------------------------~*/

#include "fields_engine.h"
#include "asset_browser.h"

#if EDITOR

#include "asset_manager.h"
#include "asset_entry.h"
#include "context.h"
#include "editor_manager.h"
#include "imgui/imgui.h"
#include "string_util.h"
#include "imgui/imgui_internal.h"
#include "texture.h"

/*~-------------------------------------------------------------------------~*\
 * Asset Browser Definitions                                                 *
\*~-------------------------------------------------------------------------~*/

fields_engine::editor::asset_browser::asset_browser()
	: m_entries()
	, m_directory_history{ "assets" }
	, m_undo_history()
	, m_address_bar_buffer()
	, m_search_bar_buffer()
	, m_rename_buffer()
	, m_rename_idx(-1)
	, m_prev_entry_clicked(-1)
	, m_need_refresh(true)
	, m_wait_for_mouse_trigger(false)
	, m_address_bar_state(address_bar_state::inactive)
	, m_rename_state(rename_state::inactive)
	, m_missing_thumbnail(std::filesystem::path("engine_assets") / "missing_asset_thumbnail.png")
	, m_folder_thumbnail(std::filesystem::path("engine_assets") / "folder_thumbnail.png")
	, m_material_thumbnail(std::filesystem::path("engine_assets") / "material_asset_thumbnail.png")
	, m_mesh_thumbnail(std::filesystem::path("engine_assets") / "mesh_asset_thumbnail.png")
{
	context<editor::editor_manager>().add_window(
		&asset_browser::display_window,
		"Asset Browser",
		ICON_FOLDER
	);

	refresh();
}

fields_engine::editor::asset_browser::~asset_browser() = default;

/*~-------------------------------------------------------------------------~*\
 * Asset Browser Window Related Definitions                                  *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {
	struct asset_browser_callbacks {
		static constexpr ImGuiInputTextFlags address_bar_input_flags
			= ImGuiInputTextFlags_NoHorizontalScroll
			| ImGuiInputTextFlags_EnterReturnsTrue
			| ImGuiInputTextFlags_AutoSelectAll
			| ImGuiInputTextFlags_CallbackResize;
		static int address_bar_input_callback(ImGuiInputTextCallbackData* data) {
			asset_browser& manager = *reinterpret_cast<asset_browser*>(data->UserData);
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
			asset_browser& manager = *reinterpret_cast<asset_browser*>(data->UserData);
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
			asset_browser& manager = *reinterpret_cast<asset_browser*>(data->UserData);
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				manager.m_rename_buffer.resize(data->BufTextLen, '\0');
				data->Buf = manager.m_rename_buffer.data();
			}
			return 0;
		}
	};
}

bool fields_engine::editor::asset_browser::display_window(editor_window& window) {
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
		+ ImVec2{ content_max.x, init_cursor_pos.y + address_bar_height };
	ImGui::GetWindowDrawList()->AddRectFilled(
		window_pos + init_cursor_pos,
		address_bar_max,
		ImGui::ColorConvertFloat4ToU32({ 0,0,0,1 }),
		address_bar_rounding
	);
	ImGui::SetCursorPos(init_cursor_pos + ImVec2{ 10, 0 });

	bool modif = false;
	const bool address_bar_active = m_address_bar_state == address_bar_state::activated;
	const bool address_bar_activated = m_address_bar_state == address_bar_state::active;

	if (address_bar_active || address_bar_activated) {
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

	} else { // ^^^ address_bar_active || address_bar_activated vvv
		std::filesystem::path const& curr_directory = m_directory_history.top();
		vector<std::filesystem::path> directories(curr_directory.begin(), curr_directory.end());
		// Index into directories
		const int num_directories = int(directories.size());
		for (int i = 0; i < num_directories; ++i) {
			// If the button is pressed and we aren't trying to move to the current directory
			if (ImGui::Button(directories[i].string().c_str(), ImVec2{ 0, address_bar_height })
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
		ImGui::SameLine();
		if (ImGui::InvisibleButton("###asset_browser_address_bar_activate",
			ImVec2{ content_max.x, address_bar_height })
		) {
			m_address_bar_buffer = curr_directory.string();
			m_address_bar_state = address_bar_state::activated;
		}
	}

	// Left of search bar

	ImGui::SetCursorPos(init_cursor_pos + ImVec2{ 0, 40 });
	constexpr ImVec2 search_button_size{ address_bar_height, address_bar_height };
	if (ImGui::Button(ICON_ARROW_ROTATE_RIGHT"###asset_browser_refresh", search_button_size)) {
		m_need_refresh = true;
	}
	ImGui::SetItemTooltip("Refresh browser for new items");

	const bool no_back_history = m_directory_history.bottom_distance() < 2;
	const bool no_forth_history = m_directory_history.at_top();
	if (no_back_history) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_CIRCLE_ARROW_LEFT"###asset_browser_back", search_button_size)) {
		m_directory_history.scroll_down();
		m_need_refresh = true;
	}
	if (no_back_history) {
		ImGui::EndDisabled();
	} else if (m_directory_history.bottom_distance() >= 2 && ImGui::BeginItemTooltip()) {
		ImGui::Text("Back to %s", m_directory_history[m_directory_history.top_index() - 1].filename().string().c_str());
		ImGui::EndTooltip();
	}
	if (no_forth_history) {
		ImGui::BeginDisabled();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_CIRCLE_ARROW_RIGHT"###asset_browser_forth", search_button_size)) {
		m_directory_history.scroll_up();
		m_need_refresh = true;
	}
	if (no_forth_history) {
		ImGui::EndDisabled();
	} else if (!m_directory_history.at_top() && ImGui::BeginItemTooltip()) {
		ImGui::Text("Forward to %s", m_directory_history[m_directory_history.top_index() + 1].filename().string().c_str());
		ImGui::EndTooltip();
	}
	ImGui::SameLine();

	// Search bar

	const string search_hint = ICON_MAGNIFYING_GLASS" Search "
		+ m_directory_history.top().filename().string();
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
		m_need_refresh = true;
	}
	ImGui::PopItemWidth();
	ImGui::PopStyleVar();

	if (m_need_refresh) {
		refresh();
	}

	ImGui::Separator();

	// File viewer

	if (ImGui::BeginChild("asset_browser_child")) {
		// Setup for restoring button outlines
		ImGuiStyle const& style = ImGui::GetStyle();
		ImVec4 border = style.Colors[ImGuiCol_Border];
		ImVec4 border_shadow = style.Colors[ImGuiCol_BorderShadow];
		border.w = border.z;
		border_shadow.w = border.z;
		// Make outlines visible
		ImGui::PushStyleColor(ImGuiCol_Border, border);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, border_shadow);

		const bool ctrl_held = ImGui::GetIO().KeyCtrl;
		const bool shift_held = ImGui::GetIO().KeyShift;
		const bool left_clicked = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
		const bool right_clicked = ImGui::IsMouseReleased(ImGuiMouseButton_Right);

		const auto undo_inputted = [&]() {
			return ctrl_held && ImGui::IsKeyReleased(ImGuiKey_Z);
		};

		if (undo_inputted() && ImGui::IsWindowFocused()) {
			if (shift_held) {
				if (!m_undo_history.at_top()) {
					m_undo_history.scroll_up();
					undo const& redo = m_undo_history.top();
					switch (redo.action) {
					case undo::move_file: FE_FALLTHROUGH
					case undo::rename_file:
						std::filesystem::rename(redo.orig_path, redo.new_path);
						break;
					case undo::delete_file:
						std::filesystem::remove(redo.orig_path);
						break;
					case undo::create_file:
						/// TODO: Implement
						break;
					}
					m_need_refresh = true;
				}
			} else if (!m_undo_history.at_bottom()) {
				undo const& undo = m_undo_history.top();
				switch (undo.action) {
				case undo::move_file: FE_FALLTHROUGH
				case undo::rename_file:
					std::filesystem::rename(undo.new_path, undo.orig_path);
					break;
				case undo::delete_file:
					/// TODO: Implement
					break;
				case undo::create_file:
					std::filesystem::remove(undo.orig_path);
					break;
				}
				m_undo_history.scroll_down();
				m_need_refresh = true;
			}
		}


		// We only need to do this at most once per frame
		// Notice how we are checking for click and not release
		if (m_wait_for_mouse_trigger && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			m_wait_for_mouse_trigger = false;
		}

		// We will need to know if any entries were clicked on
		// in case the user wants to deselect by clicking on nothing
		bool any_entry_was_clicked = false;

		for (int i = 0; i < int(m_entries.size()); ++i) {
			file_entry& entry = m_entries[i];
			ImGui::PushID(&entry);
			const ImVec2 cursor_pos = ImGui::GetCursorPos();
			// Selection & button display logic
			bool& selected = entry.selected;

			bool folder_hovered = selected;
			if (entry.type == file_type::folder && !folder_hovered) {
				const ImVec2 real_pos = cursor_pos + ImGui::GetWindowPos();
				folder_hovered = ImGui::IsMouseHoveringRect(real_pos, real_pos + entry_size);
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
					ImGui::PushStyleColor(ImGuiCol_Border, { 0.3f,0.8f,0.45f,1.0f }); // Green
				}

				const char* asset_popup_label = "###asset_right_click_popup";

				if (ImGui::ButtonEx("", entry_size, entry_button_flags) && !m_wait_for_mouse_trigger) {
					if (left_clicked) {
						if (shift_held) {
							// There is no way to deselect with shift click
							if (m_prev_entry_clicked == -1 || m_prev_entry_clicked == i) {
								selected = true;
							} else {
								// Select entries between [prev, current]
								const auto [lower, upper] = std::minmax(m_prev_entry_clicked, i);
								for (int j = lower; j <= upper; ++j) {
									m_entries[j].selected = true;
								}
							}
						} else if (ctrl_held) {
							selected = !selected;
						} else {
							for (int j = 0; j < int(m_entries.size()); ++j) {
								m_entries[j].selected = false;
							}
							selected = true;
						}
						m_prev_entry_clicked = i;
					} else if (right_clicked) {
						ImGui::OpenPopup(asset_popup_label);
					}
					// Remember the last click for any type of click
					any_entry_was_clicked = true;

					// Allow double click to enter directory if the button wasn't single clicked
				} else if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					m_wait_for_mouse_trigger = true;
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
				} // Item tooltip


				if (ImGui::BeginPopup(asset_popup_label)) {
					if (ImGui::MenuItem(ICON_PEN_TO_SQUARE" Edit")) {
						/// TODO: Implement
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
								asset_manager& manager = context<asset_manager>();
								manager.add_asset(new_file_path);
								break;
							}
						}
						m_need_refresh = true;
					}
					if (ImGui::MenuItem(ICON_TRASH_CAN" Delete", "Del")) {
						/// TODO: Implement
					}
					ImGui::EndPopup();
				} // Popup

				// Allow drag drop

				constexpr ImGuiDragDropFlags_ drag_drop_accept_flags
					= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

				if (ImGui::BeginDragDropSource()) {
					string path_str = entry.path.string();
					if (entry.type == file_type::asset) {
						ImGui::SetDragDropPayload("ab_asset", path_str.c_str(), path_str.size());
					} else if (entry.type == file_type::folder) {
						ImGui::SetDragDropPayload("ab_folder", path_str.c_str(), path_str.size());
					} else if (entry.type == file_type::other) {
						ImGui::SetDragDropPayload("ab_other", path_str.c_str(), path_str.size());
					}
					ImGui::Image(ImTextureID(thumbnail), { 80,80 }, vec2(0, 1), vec2(1, 0));
					ImGui::Text("%s", entry.path.string().c_str());
					ImGui::EndDragDropSource();
				} // Drag drop source

				if (entry.type == file_type::folder && ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::GetDragDropPayload()) {
						if (string(payload->DataType).substr(0, 3) == "ab_") {
							std::filesystem::path src_path = string((const char*)payload->Data, payload->DataSize);
							if (std::filesystem::exists(src_path)) {
								if (ImGui::AcceptDragDropPayload(payload->DataType, drag_drop_accept_flags)) {
									std::filesystem::path new_path = entry.path / src_path.filename();
									std::filesystem::rename(src_path, new_path);
									m_undo_history.push(undo{ undo::move_file, move(src_path), move(new_path) });
									m_need_refresh = true;
									// Not worth figuring out how to change the history if you move a folder, just reset it
									reset_directory_history();
								}
							}
						}
					}
					ImGui::EndDragDropTarget();
				}
			} // Any file or hovered folder

			// Entry type

			ImGui::SetCursorPos(cursor_pos + type_text_offset);
			if (entry.type == file_type::asset) {
				// entry.asset is guaranteed to be non-null
				ImGui::TextColored(
					ImVec4(1, 1, 1, 0.65f), // Transparent white
					"%s",
					entry.asset->get_type().data()
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
					/// TODO: Implement
					if (entry.type == file_type::asset) {
						//auto it = m_assets.find(m_rename_buffer);
						//if (it == m_assets.end()) {
						//	
						//}
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
					ImGui::Text("%s", ellipsis_compress_middle(
						entry.path.stem().stem().string(), name_compress_max).c_str());
				} else {
					ImGui::Text("%s", ellipsis_compress_middle(
						entry.path.filename().string(), name_compress_max).c_str());
				}
			}

			// Entry thumbnail

			ImGui::SetCursorPos(cursor_pos + thumbnail_margin);
			// For some reason ImGui textures are flipped, so we adjust uvs manually here
			ImGui::Image(ImTextureID(thumbnail), thumbnail_size, ImVec2(0, 1), ImVec2(1, 0));
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
				for (size_t j = 0; j < m_entries.size(); ++j) {
					m_entries[j].selected = false;
				}
			}
			if (right_clicked) {
				ImGui::OpenPopup("###browser_right_click_popup");
			}
		}

		if (ImGui::BeginPopup("###browser_right_click_popup")) {
			if (ImGui::MenuItem(ICON_FOLDER_PLUS" Create Folder")) {
				const string new_folder_name = (m_directory_history.top() / "New Folder").string();
				if (!std::filesystem::exists(new_folder_name)) {
					std::filesystem::create_directory(new_folder_name);
				} else {
					for (int i = 2; i < 100; ++i) {
						std::filesystem::path new_folder_path = new_folder_name + " (" + std::to_string(i) + ")";
						if (!std::filesystem::exists(new_folder_path)) {
							std::filesystem::create_directory(new_folder_path);
							m_undo_history.push(undo{ undo::create_file, move(new_folder_path) });
							break;
						}
					}
				}
				m_need_refresh = true;
			}
			if (ImGui::MenuItem(ICON_SQUARE_PLUS" Create Asset...")) {
				/// TODO: Implement
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(2); // Border & border shadow
	}
	ImGui::EndChild();
	return modif;
}

void fields_engine::editor::asset_browser::refresh() {
	m_need_refresh = false;
	m_entries.clear();
	m_prev_entry_clicked = -1;
	m_address_bar_state = address_bar_state::inactive;
	asset_manager& manager = context<asset_manager>();
	if (m_search_bar_buffer.empty()) {
		std::filesystem::directory_iterator curr_directory(m_directory_history.top());
		for (std::filesystem::directory_entry const& entry : curr_directory) {
			std::filesystem::path const& path = entry.path();
			if (entry.is_directory()) {
				m_entries.push_back({ path, nullptr, file_type::folder });
				continue;
			}
			if (path.extension() == ".fea") {
				auto it = manager.m_assets.find(path.stem().string());
				if (it != manager.m_assets.end()) {
					m_entries.push_back({ path, &it->second, file_type::asset });
					continue;
				}
			}
			m_entries.push_back({ path, nullptr, file_type::other });
		}
	} else {
		std::filesystem::recursive_directory_iterator curr_directory(m_directory_history.top());
		for (std::filesystem::directory_entry const& entry : curr_directory) {
			std::filesystem::path const& path = entry.path();
			if (entry.is_directory()) { continue; }
			if (path.filename().string().find(m_search_bar_buffer) == string::npos) {
				continue;
			}
			if (path.extension() == ".fea") {
				auto it = manager.m_assets.find(path.stem().string());
				if (it != manager.m_assets.end()) {
					m_entries.push_back(file_entry{
						path, &it->second, file_type::asset
						});
					continue;
				}
			}
			m_entries.push_back({ path, nullptr, file_type::other });
		}
	}

	std::sort(m_entries.begin(), m_entries.end(),
		[](file_entry const& l, file_entry const& r) {
			if (int(l.type) < int(r.type)) {
				return true;
			} else if (l.type == file_type::asset && r.type == file_type::asset) {
				const string l_type = l.path.stem().extension().string();
				const string r_type = r.path.stem().extension().string();
				return std::lexicographical_compare(l_type.begin(), l_type.end(), r_type.begin(), r_type.end());
			}
			return false;
	});
}

void fields_engine::editor::asset_browser::browse_to_directory(std::filesystem::path&& target) {
	m_directory_history.emplace(move(target));
	m_need_refresh = true;
}
void fields_engine::editor::asset_browser::reset_directory_history() {
	std::filesystem::path temp(move(m_directory_history.top()));
	m_directory_history.clear();
	m_directory_history.push(move(temp));
	m_need_refresh = true;
}
void* fields_engine::editor::asset_browser::get_thumbnail(file_entry const& entry) {
	if (entry.type == file_type::asset) {
		//if (void* thumbnail = entry.asset->get_thumbnail()) {
		//	return thumbnail;
		//} else 
		const string_view type = entry.asset->get_type();
		if (type == "mesh") {
			return m_mesh_thumbnail.get_void_ptr_id();
		} else if (type == "material") {
			return m_material_thumbnail.get_void_ptr_id();
		} else {
			return m_missing_thumbnail.get_void_ptr_id();
		}
		return m_missing_thumbnail.get_void_ptr_id();

	} else if (entry.type == file_type::folder) {
		return m_folder_thumbnail.get_void_ptr_id();
	} else if (entry.type == file_type::other) {
		return m_missing_thumbnail.get_void_ptr_id();
	}
	return nullptr;
}
#endif // EDITOR
