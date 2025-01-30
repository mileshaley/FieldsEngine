/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: asset_browser.h                                                     *
\*~-------------------------------------------------------------------------~*/

#pragma once

#if EDITOR

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include <filesystem>
#include "scroll_stack.h"

namespace fields_engine {
	namespace vis {
		class texture;
	} // namespace vis
	class asset;
}

/*~-------------------------------------------------------------------------~*\
 * Asset Browser Class                                                       *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {

	class asset_browser {
	public: // Lifecycle
		asset_browser();
		~asset_browser();

	public: // Interface
		bool display_window();

	private: // Implementation
		enum class file_type : i8 { folder, asset, other };
		struct file_entry {
			std::filesystem::path path;
			asset* asset; // So we save some string manipulation and retrieval for assets
			file_type type;
			bool selected;
		};

		void refresh();
		void browse_to_directory(std::filesystem::path&& target);
		void reset_directory_history();
		void* get_thumbnail(file_entry const& entry);

		enum class undo_action {
			move_file,
			rename_file,
			delete_file,
			create_file,
		};

		struct undo {
			enum undo_action {
				move_file,
				rename_file,
				delete_file,
				create_file,
			} action;

			std::filesystem::path orig_path;
			std::filesystem::path new_path;
		};

		vector<file_entry> m_entries;
		scroll_stack<std::filesystem::path> m_directory_history{ "assets" };
		scroll_stack<undo> m_undo_history;
		string m_address_bar_buffer;
		string m_search_bar_buffer;
		string m_rename_buffer;
		int m_rename_idx = -1;
		int m_prev_entry_clicked = -1; // For shift multi-select
		bool m_need_refresh = true;
		// We make sure that the user doesn't acidentally make an
		// extra on-release input after double clicking a folder
		bool m_wait_for_mouse_trigger = false;
		enum class address_bar_state : i8 {
			inactive = 0,
			activated,
			active,
		} m_address_bar_state = address_bar_state::inactive;

		enum class rename_state : i8 {
			inactive = 0,
			activated,
			active,
		} m_rename_state = rename_state::inactive;
		friend struct asset_browser_callbacks;

		/// TODO: Relocate
		box<vis::texture> m_mesh_thumbnail = nullptr;
		box<vis::texture> m_material_thumbnail = nullptr;
		box<vis::texture> m_missing_thumbnail = nullptr;
		box<vis::texture> m_folder_thumbnail = nullptr;
	};

} // namespace fields_engine::editor

#endif // EDITOR
