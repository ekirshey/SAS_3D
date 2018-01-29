#pragma once
#include "subsystems/animation_engine/anim_state.h"

namespace SAS_3D {
	enum class LightType { Directional, Point, Spotlight };
	// Light definitions
	struct Light {
		LightType m_type;
		glm::vec3 m_direction;
		glm::vec3 m_position;

		// Used in all lights
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;

		// Used in spotlight
		float m_cutoff;
		float m_outercutoff;

		// Used in point and spotlight
		float m_constant;
		float m_linear;
		float m_quadratic;
	};

	struct CameraInfo {
		float		m_zoom;
		glm::vec3	m_position;
		glm::mat4	m_viewmatrix;
	};

	using RenderID = unsigned long long;
	struct RenderItem {
		RenderID m_id;
		int m_modelidx;
		glm::mat4 m_model;
		BoneMatrix m_bones;
	};

	struct Scene {
		CameraInfo m_camera;

		// Various types of lights
		std::vector<Light> m_dirlights;
		std::vector<Light> m_pointlights;
		std::vector<Light> m_spotlights;

		// All other objects
		std::vector<RenderItem> m_objects;

		void AddLight(const Light& light);
	};
}