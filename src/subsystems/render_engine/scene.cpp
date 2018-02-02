#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include "shaders/input_modules/light_module.h"


namespace SAS_3D {
	Scene::Scene(float width, float height) 
		: _width(width)
		, _height(height)
		, _width_over_height(width/height)
	{

	}

	void Scene::AddRenderItem(RenderItem item) {
		_objects.emplace_back(item);
	}

	void Scene::AddLight(Light light) {
		switch (light.m_type) {
		case LightType::Directional:
			_dirlights.emplace_back(light);
			break;
		case LightType::Point:
			_pointlights.emplace_back(light);
			break;
		case LightType::Spotlight:
			_spotlights.emplace_back(light);
			break;
		default:
			break;
		}
	}

	void Scene::SetCamera(const Camera& camera) {
		_camera.m_front = camera.GetFront();
		_camera.m_position = camera.GetPosition();
		_camera.m_viewmatrix = camera.GetViewMatrix();
		_camera.m_zoom = camera.Zoom();
	}

	void Scene::DrawScene(std::vector<ShaderProgram>& shaders, ModelContainer& mc, const CubeMap& skybox) const{
		auto projectionmatrix = glm::perspective(_camera.m_zoom, _width_over_height, 0.1f, 1000.0f);
		glm::mat4 pv = projectionmatrix * _camera.m_viewmatrix;
		for (const auto &e : _objects) {
			// Need to group draws by shader type
			if (e.m_bones.size() > 0) {
				shaders[0].UseProgram();
				auto lightmodule = shaders[0].GetInputModule<LightModule*>(LightModuleID);
				lightmodule->SetLightSettings(_camera.m_position, &_dirlights, &_pointlights, &_spotlights);
				mc.Draw(e.m_modelidx, shaders[0], pv, e.m_model, &e.m_bones);
			}
			else if (mc.HasTextures(e.m_modelidx)) {
				shaders[1].UseProgram();
				auto lightmodule = shaders[1].GetInputModule<LightModule*>(LightModuleID);
				lightmodule->SetLightSettings(_camera.m_position, &_dirlights, &_pointlights, &_spotlights);
				mc.Draw(e.m_modelidx, shaders[1], pv, e.m_model);
			}
			else {
				shaders[2].UseProgram();
				mc.Draw(e.m_modelidx, shaders[2], pv, e.m_model);
			}
		}

		glDepthFunc(GL_LEQUAL);
		shaders[3].UseProgram();
		glm::mat4 view = glm::mat4(glm::mat3(_camera.m_viewmatrix));
		skybox.Draw(shaders[3], projectionmatrix, view);
		glDepthFunc(GL_LESS);
	}
}