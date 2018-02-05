#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include "shaders/input_modules/light_module.h"


namespace SAS_3D {
	Scene::Scene(int width, int height) 
		: m_width(width)
		, m_height(height)
		, m_width_over_height((float)width/height)
	{

	}

	void Scene::AddRenderItem(RenderItem item) {
		// Perhaps add some sort of frustum culling here
		m_objects.emplace_back(item);
	}

	void Scene::AddLight(Light light) {
		switch (light.m_type) {
		case LightType::Directional:
			m_dirlights.emplace_back(light);
			break;
		case LightType::Point:
			m_pointlights.emplace_back(light);
			break;
		case LightType::Spotlight:
			m_spotlights.emplace_back(light);
			break;
		default:
			break;
		}
	}

	void Scene::SetCamera(const Camera& camera) {
		m_camera.m_front = camera.GetFront();
		m_camera.m_position = camera.GetPosition();
		m_camera.m_viewmatrix = camera.GetViewMatrix();
		m_camera.m_zoom = camera.Zoom();
	}

	void ForwardRenderScene(const Scene& scene, std::vector<ShaderProgram>& shaders, ModelContainer& mc, const CubeMap& skybox) {
		auto projectionmatrix = glm::perspective(scene.m_camera.m_zoom, scene.m_width_over_height, 0.1f, 1000.0f);
		glm::mat4 pv = projectionmatrix * scene.m_camera.m_viewmatrix;
		for (const auto &e : scene.m_objects) {
			// Need to group draws by shader type
			if (e.m_bones.size() > 0) {
				shaders[0].UseProgram();
				auto lightmodule = shaders[0].GetInputModule<LightModule*>(LightModuleID);
				lightmodule->SetLightSettings(scene.m_camera.m_position, &scene.m_dirlights, &scene.m_pointlights, &scene.m_spotlights);
				mc.Draw(e.m_modelidx, shaders[0], pv, e.m_model, &e.m_bones);
			}
			else if (mc.HasTextures(e.m_modelidx)) {
				shaders[1].UseProgram();
				auto lightmodule = shaders[1].GetInputModule<LightModule*>(LightModuleID);
				lightmodule->SetLightSettings(scene.m_camera.m_position, &scene.m_dirlights, &scene.m_pointlights, &scene.m_spotlights);
				mc.Draw(e.m_modelidx, shaders[1], pv, e.m_model);
			}
			else {
				shaders[2].UseProgram();
				mc.Draw(e.m_modelidx, shaders[2], pv, e.m_model);
			}
		}

		glDepthFunc(GL_LEQUAL);
		shaders[3].UseProgram();
		glm::mat4 view = glm::mat4(glm::mat3(scene.m_camera.m_viewmatrix));
		skybox.Draw(shaders[3], projectionmatrix, view);
		glDepthFunc(GL_LESS);
	}
}