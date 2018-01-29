#include "scene.h"

namespace SAS_3D {
	void Scene::AddLight(const Light& light) {
		switch (light.m_type) {
		case LightType::Directional:
			m_dirlights.push_back(light);
			break;
		case LightType::Point:
			m_pointlights.push_back(light);
			break;
		case LightType::Spotlight:
			m_spotlights.push_back(light);
			break;
		default:
			break;
		}
	}
}