#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "input_module.h"
#include "subsystems/render_engine/shaders/shader_program.h"
#include "subsystems/render_engine/scene.h"


namespace SAS_3D {
	class LightModule : public InputModule {
	public:
		static const InputID ID = LightModuleID;

		LightModule() 
			: _dirlights(nullptr)
			, _pointlights(nullptr)
			, _spotlights(nullptr)
		{
		}

		void ApplyToShader(ShaderProgram* shader) {
			if (_pointlights != nullptr) {
				shader->SetInt("numPointLights", (GLsizei)_pointlights->size());
			}

			shader->SetVec3("viewPos", _viewpos);

			if (_dirlights != nullptr) {
				for (int i = 0; i < _dirlights->size(); i++) {
					shader->SetVec3("dirLight.direction", _dirlights->at(i).m_direction);
					shader->SetVec3("dirLight.ambient", _dirlights->at(i).m_ambient);
					shader->SetVec3("dirLight.diffuse", _dirlights->at(i).m_diffuse);
					shader->SetVec3("dirLight.specular", _dirlights->at(i).m_specular);
				}
			}

			if (_pointlights != nullptr) {
				for (int i = 0; i < _pointlights->size(); i++) {
					shader->SetVec3("pointLights[" + std::to_string(i) + "].position", _pointlights->at(i).m_position);
					shader->SetVec3("pointLights[" + std::to_string(i) + "].ambient", _pointlights->at(i).m_ambient);
					shader->SetVec3("pointLights[" + std::to_string(i) + "].diffuse", _pointlights->at(i).m_diffuse);
					shader->SetVec3("pointLights[" + std::to_string(i) + "].specular", _pointlights->at(i).m_specular);
					shader->SetFloat("pointLights[" + std::to_string(i) + "].constant", _pointlights->at(i).m_constant);
					shader->SetFloat("pointLights[" + std::to_string(i) + "].linear", _pointlights->at(i).m_linear);
					shader->SetFloat("pointLights[" + std::to_string(i) + "].quadratic", _pointlights->at(i).m_quadratic);
				}
			}

			if (_spotlights != nullptr) {
				for (int i = 0; i < _spotlights->size(); i++) {
					shader->SetVec3("spotLight.direction", _spotlights->at(i).m_direction);
					shader->SetVec3("spotLight.position", _spotlights->at(i).m_position);
					shader->SetVec3("spotLight.ambient", _spotlights->at(i).m_ambient);
					shader->SetVec3("spotLight.diffuse", _spotlights->at(i).m_diffuse);
					shader->SetVec3("spotLight.specular", _spotlights->at(i).m_specular);
					shader->SetFloat("spotLight.constant", _spotlights->at(i).m_constant);
					shader->SetFloat("spotLight.linear", _spotlights->at(i).m_linear);
					shader->SetFloat("spotLight.quadratic", _spotlights->at(i).m_quadratic);
					shader->SetFloat("spotLight.cutoff", _spotlights->at(i).m_cutoff);
					shader->SetFloat("spotLight.outercutoff", _spotlights->at(i).m_outercutoff);
					/*
					shader->SetVec3("spotLight[" + std::to_string(i) + "].direction", _spotlights->at(i).m_direction);
					shader->SetVec3("spotLight[" + std::to_string(i) + "].position", _spotlights->at(i).m_position);
					shader->SetVec3("spotLight[" + std::to_string(i) + "].ambient", _spotlights->at(i).m_ambient);
					shader->SetVec3("spotLight[" + std::to_string(i) + "].diffuse", _spotlights->at(i).m_diffuse);
					shader->SetVec3("spotLight[" + std::to_string(i) + "].specular", _spotlights->at(i).m_specular);
					shader->SetFloat("spotLight[" + std::to_string(i) + "].constant", _spotlights->at(i).m_constant);
					shader->SetFloat("spotLight[" + std::to_string(i) + "].linear", _spotlights->at(i).m_linear);
					shader->SetFloat("spotLight[" + std::to_string(i) + "].quadratic", _spotlights->at(i).m_quadratic);
					shader->SetFloat("spotLight[" + std::to_string(i) + "].cutoff", _spotlights->at(i).m_cutoff);
					shader->SetFloat("spotLight[" + std::to_string(i) + "].outerCutOff", _spotlights->at(i).m_outercutoff);
					*/
				}
			}

			_dirlights = nullptr;
			_pointlights = nullptr;
			_spotlights = nullptr;
		}

		InputID UniqueBits() const { return ID; }

		void SetLightSettings( glm::vec3 viewpos,
					   const std::vector<Light>* dirlights,
					   const std::vector<Light>* points,
					   const std::vector<Light>* spotlights) 
		{
			_viewpos = viewpos;
			_dirlights = dirlights;
			_pointlights = points;
			_spotlights = spotlights;
		}

	private:
		glm::vec3 _viewpos;
		const std::vector<Light>* _dirlights;
		const std::vector<Light>* _pointlights;
		const std::vector<Light>* _spotlights;

	};
}
