/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: scroll_stack.h                                                      *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

/*~-------------------------------------------------------------------------~*\
 * Scroll Stack Class Template                                               *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	template<typename T, class Alloc = std::allocator<T>> 
	class scroll_stack {
		using container_type = vector<T, Alloc>;
		//using value_type = container_type::value_type;
		//using iterator = container_type::iterator;
		//using const_iterator = container_type::const_iterator;

	public:
		inline scroll_stack()
			: m_data()
			, m_end_offset(1)
		{
		}

		inline scroll_stack(std::initializer_list<T> init_list) 
			: m_data(init_list)
			, m_end_offset(1)
		{
		}

		inline scroll_stack(scroll_stack const& other)
			: m_data(other.m_data)
			, m_end_offset(other.m_end_offset)
		{
		}

		inline scroll_stack(scroll_stack&& other) noexcept
			: m_data(move(other.m_data))
			, m_end_offset(std::exchange(other.m_end_offset, 0))
		{
		}

		inline scroll_stack& operator=(scroll_stack const& rhs) {
			m_data = rhs.m_data;
			m_end_offset = rhs.m_end_offset;
			return *this;
		}

		inline scroll_stack& operator=(scroll_stack&& rhs) {
			m_data = move(rhs.m_data);
			m_end_offset = std::exchange(rhs.m_end_offset, 0);
			return *this;
		}

	public:
		inline FE_NODISCARD T& operator[](size_t offset) {
			return m_data[offset];
		}
		inline FE_NODISCARD T const& operator[](size_t offset) const {
			return m_data[offset];
		}


		// size() will always be <= true_size()
		inline FE_NODISCARD size_t size() const noexcept {
			return top_index() + 1;
		}

		inline FE_NODISCARD size_t true_size() const noexcept {
			return m_data.size();
		}

		inline FE_NODISCARD bool empty() const noexcept {
			return m_data.empty();
		}

		inline void clear() {
			m_data.clear();
			m_end_offset = 1;
		}

		inline FE_NODISCARD T& top() noexcept {
			return m_data[top_index()];
		}

		inline FE_NODISCARD T const& top() const noexcept {
			return m_data[top_index()];
		}

		// Returns an index such that scrollstack[top_index()] == scrollstack.top()
		// Can be -1 if the stack is scrolled to the very bottom
		inline FE_NODISCARD i64 top_index() const noexcept {
			return static_cast<i64>(m_data.size()) - m_end_offset;
		}

		inline void push(T const& val) {
			truncate();
			m_data.push_back(val);
		}

		inline void push(T&& val) {
			truncate();
			m_data.push_back(move(val));
		}

		template<typename... Args>
		inline T& emplace(Args&&... vals) {
			truncate();
			return m_data.emplace_back(std::forward<Args>(vals)...);
		}

		inline void pop() {
			m_end_offset += 1;
			truncate();
		}

	public:
		inline void scroll_up(size_t offset = 1) noexcept {
			m_end_offset -= offset;
			FE_ASSERT(m_end_offset >= 1, "Scrolled beyond the end of data");
		}

		inline void scroll_down(size_t offset = 1) noexcept {
			m_end_offset += offset;
			FE_ASSERT(m_end_offset <= m_data.size() + 1, "Scrolled before the beginning of data");
		}

		FE_NODISCARD bool at_top() const noexcept {
			return m_end_offset <= 1;
		}

		FE_NODISCARD bool at_bottom() const noexcept {
			return m_end_offset > m_data.size();
		}

		FE_NODISCARD i64 top_distance() const noexcept {
			return static_cast<i64>(m_end_offset) - 1ll;
		}

		FE_NODISCARD i64 bottom_distance() const noexcept {
			return top_index() + 1;
		}

		inline void truncate() {
			if (m_end_offset > 1) {
				m_data.erase(m_data.begin() + bottom_distance(), m_data.end());
				m_end_offset = 1;
			}
		}
	private:

		container_type m_data;
		// Defaults to 1 and cannot go lower than 1
		// Should be subtracted from end() of m_data
		size_t m_end_offset; 
	};

} // namespace fields_engine
