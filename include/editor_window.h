/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE															 *
 *~-------------------------------------------------------------------------~* 
 * File: editor_window.h													 *
\*~-------------------------------------------------------------------------~*/

#pragma once

#ifdef EDITOR

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "editor_icons.h" // editor_icon

namespace fields_engine::editor {
	class editor_window;
} // namespace fields_engine::editor

/*~-------------------------------------------------------------------------~*\
 * Functors for Editor Window Invokers                                       *
\*~-------------------------------------------------------------------------~*/

/// TODO: Relocate
namespace fields_engine {
	template<class T>
	class context_object_getter {
	public:
		T* operator()() const {
			return context_unchecked<T>();
		}
	}; // class context_object_getter

	template<class T>
	class constant_object_getter {
	public:

		T* operator()() const {
			return m_object;
		}
	private:
		T* m_object;
	}; // class constant_obejct_getter
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Editor Window Class                                                       *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine::editor {

	class window_invoker {
	public:
		virtual ~window_invoker() = default;
		virtual bool invoke(editor_window& window) const = 0;
		virtual bool equals(window_invoker const& other) const = 0;
	};

	template<class T, class ObjectGetter = context_object_getter<T>>
	class object_window_invoker : public window_invoker {
	public:
		using display_function_t = bool (T::*)(editor_window&);

		object_window_invoker(
			display_function_t display_function,
			ObjectGetter object_getter = ObjectGetter{}
		)
			: m_display_function(display_function)
			, m_object_getter(object_getter) {
		}

		virtual bool invoke(editor_window& window) const override {
			T* object = m_object_getter();
			return object ? (object->*m_display_function)(window) : false;
		}

		virtual bool equals(window_invoker const& other) const override {
			if (object_window_invoker const* p_other = dynamic_cast<object_window_invoker const*>(&other)) {
				return m_display_function == p_other->m_display_function;
			}
			return false;
		}

	private:
		display_function_t m_display_function;
		ObjectGetter m_object_getter;
	}; // class object_window_invoker

	//class static_window_invoker : public window_invoker {
	//public:
	//	using display_function_t = bool (*)(editor_window&);
	//
	//	static_invoker(display_function_t display_function)
	//		: m_display_function(display_function) {
	//	}
	//	virtual bool invoke(editor_window& window) const override {
	//		return m_display_function(window);
	//	}
	//
	//	virtual void* get_function() const override {
	//		return static_cast<void*>(m_display_function);
	//	}
	//
	//private:
	//	display_function_t m_display_function;
	//}; // class static_invoker

	/// TODO: Add support for grouping windows by category
	class editor_window {
	public:
		

	public:
		editor_window(string_view name, own<window_invoker>&& function, editor_icon icon = "");
		editor_window(editor_window&& other) noexcept;

		bool display();
		bool force_display();

		bool begin_window();
		void end_window() const;

		/// TODO: Add shortcut support integrated with input

		bool menu_item();

		void open();
		void close();
		FE_NODISCARD bool is_open() const;
		FE_NODISCARD bool& ref_open();


		FE_NODISCARD window_invoker const& get_invoker() const;
		void set_invoker(own<window_invoker>&& new_invoker);

		FE_NODISCARD string const& get_str_id() const;

	private:
		string m_name;
		string m_str_id; // Trade some memory for time by precomputing the string ID
		own<window_invoker> m_callback;
		editor_icon m_icon;
		bool m_open;
	}; // class editor_window

} // namespace fields_engine::editor

#endif // EDITOR
