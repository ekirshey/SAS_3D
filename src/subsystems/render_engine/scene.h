#pragma once
#include "subsystems/animation_engine/anim_state.h"
#include "subsystems/render_engine/shaders/shader_program.h"
#include "assets/model_container.h"
#include "assets/cubemap.h"
#include "entities/camera.h"


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
		glm::vec3	m_front;
		glm::mat4	m_viewmatrix;
	};

	using RenderID = unsigned long long;
	struct RenderItem {
		RenderID m_id;
		int m_modelidx;
		glm::mat4 m_model;
		BoneMatrix m_bones;
	};

	class Scene {
	public:
		Scene(float width, float height);
		void AddRenderItem(RenderItem item);
		void AddLight(Light light);
		void SetCamera(const Camera& camera);
		void DrawScene(std::vector<ShaderProgram>& shaders, ModelContainer& mc, const CubeMap& skybox) const;
	private:
		float _width;
		float _height;
		float _width_over_height;	// result of the division

		CameraInfo _camera;

		// Various types of lights
		std::vector<Light> _dirlights;
		std::vector<Light> _pointlights;
		std::vector<Light> _spotlights;

		// All other objects
		std::vector<RenderItem> _objects;


	};
}