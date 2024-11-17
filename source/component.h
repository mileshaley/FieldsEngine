/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: component.h                                                         *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {
	class entity;
} // namespace fields_engine

/*~-------------------------------------------------------------------------~*\
 * Component Class                                                           *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class component {
	public:
		component();
		virtual ~component();
		virtual unique_ptr<component> clone() const = 0;
		
		virtual void tick(float dt) = 0;
		virtual void render() const {}
	private:
		entity* m_owner;
		component* m_parent;
	};

} // namespace fields_engine
