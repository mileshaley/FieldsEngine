/*~-------------------------------------------------------------------------~*\
 * FIELDS ENGINE                                                             *
 *~-------------------------------------------------------------------------~* 
 * File: camera.h                                                            *
\*~-------------------------------------------------------------------------~*/

#pragma once

/*~-------------------------------------------------------------------------~*\
 * Includes & Forward Declarations                                           *
\*~-------------------------------------------------------------------------~*/

#include "component.h"

/*~-------------------------------------------------------------------------~*\
 * Camera Class                                                              *
\*~-------------------------------------------------------------------------~*/

namespace fields_engine {

	class camera : public component {
	public:
		camera();
		camera(camera const& other);

		COMPONENT_BODY(camera);


		virtual void init() override;
		virtual void tick(float dt) override;
		virtual void exit() override;

#ifdef EDITOR
		virtual bool display() override;
#endif // EDITOR

		void recalculate_view_matrix();
		void recalculate_proj_matrix();

		mat4 const& world_view_matrix() const;
		mat4 const& world_proj_matrix() const;

		float get_zoom() const;
		float get_fov() const;
		float get_near() const;
		float get_far() const;

		void set_zoom(float new_zoom);
		void set_fov(float new_fov);
		void set_near(float new_near);
		void set_far(float new_far);




	private:
		float m_zoom = 1;
		float m_fov = 90;
		float m_near = 0.25f;
		float m_far = 5000.0f;

		mat4 m_world_proj_matrix{1};
		mat4 m_world_view_matrix{1};
	};

} // namespace fields_engine
