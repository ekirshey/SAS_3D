#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "render_engine.h"
#include "shaders/input_modules/pvm_module.h"
#include "shaders/input_modules/texture_module.h"
#include "shaders/input_modules/animation_module.h"
#include "shaders/input_modules/light_module.h"
#include "core/sas_video.h"

namespace SAS_3D {
	void renderQuad();

	//////// IMPL DETAILS
	RenderImpl::RenderImpl(const GameConfig& config,
		SASWindow* window,
		RenderQueue* queue)
		: _config(config)
		, _window(window)
		, _event_queue(queue)
		, _running(false)
		, _shaders(4)
		, _deferredshaders(4)
		, _scene(_config.screenwidth, _config.screenheight)
	{

	}

	RenderImpl::~RenderImpl() {

	}

	void RenderImpl::Initialize(ModelContainer&& mc, TextureContainer&& tc) {
		_window->SwitchContext();

		_mc = std::move(mc);
		_tc = std::move(tc);

		_shaders[0].Load(_config.shaderpath + "animation.vert", _config.shaderpath + "lighting.frag");
		_shaders[0].AddInputModule<PVMModule>();
		_shaders[0].AddInputModule<MaterialModule>();
		_shaders[0].AddInputModule<AnimationModule>();
		_shaders[0].AddInputModule<LightModule>();

		_shaders[1].Load(_config.shaderpath + "model.vert", _config.shaderpath + "lighting.frag");
		_shaders[1].AddInputModule<PVMModule>();
		_shaders[1].AddInputModule<MaterialModule>();
		_shaders[1].AddInputModule<LightModule>();

		_shaders[2].Load(_config.shaderpath + "simple.vert", _config.shaderpath + "simple.frag");
		_shaders[2].AddInputModule<PVMModule>();

		_shaders[3].Load("F:/github/opengl_experiments/shaders/skybox.vs", "F:/github/opengl_experiments/shaders/skybox.fs");

		// Set up deferred shaders
		_deferredshaders[0].Load(_config.shaderpath + "gbuffer_animated.vs", _config.shaderpath + "gbuffer.fs");
		_deferredshaders[1].Load(_config.shaderpath + "gbuffer.vs", _config.shaderpath + "gbuffer.fs");
		_deferredshaders[2].Load(_config.shaderpath + "deferred_shading.vs", _config.shaderpath + "deferred_shading.fs");


		_skybox.Load("F:/github/SAS_3D/media/skybox/", tc);
	}

	void RenderImpl::Run() {
		// Render loop 
		if (!_config.vsync) {
			_window->DisableVSYNC();
		}

		std::cout << "Entering main render loop" << std::endl;
		_running = true;
		_window->SwapWindow();

		unsigned int gBuffer;
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		unsigned int gPosition, gNormal, gAlbedoSpec;

		// position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->GetScreenWidth(), _window->GetScreenHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

		// normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _window->GetScreenWidth(), _window->GetScreenHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		// color + specular color buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _window->GetScreenWidth(), _window->GetScreenHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

		// Tell OpenGL which color attachments are being used (of this framebuffer) for rendering
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _window->GetScreenWidth(), _window->GetScreenHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		_deferredshaders[2].UseProgram();
		_deferredshaders[2].SetInt("gPosition", 0);
		_deferredshaders[2].SetInt("gNormal", 1);
		_deferredshaders[2].SetInt("gAlbedoSpec", 2);

		while (_running)
		{
			// Do we add a light component to the render event or do we handle the light sources in a separate queue?
			_window->Clear(0.1f, 0.1f, 0.1f);
			size_t size = 1;


			Scene newscene(_config.screenwidth, _config.screenheight);
			size = _event_queue->dequeue(newscene);
			if (size != 0) {
				// Manage the scene index
				_scene = std::move(newscene);
			}

			///////////////////// GEOMETRY PASS
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			auto projectionmatrix = glm::perspective(_scene.m_camera.m_zoom, _scene.m_width_over_height, 0.1f, 1000.0f);
			for (const auto &e : _scene.m_objects) {
				if (e.m_bones.size() > 0) {
					_deferredshaders[0].UseProgram();
					_deferredshaders[0].SetMat4("projection", projectionmatrix);
					_deferredshaders[0].SetMat4("view", _scene.m_camera.m_viewmatrix);
					_mc.Draw(e.m_modelidx, _deferredshaders[0], projectionmatrix * _scene.m_camera.m_viewmatrix, e.m_model, &e.m_bones);
				}
				else if (_mc.HasTextures(e.m_modelidx)) {
					_deferredshaders[1].UseProgram();
					_deferredshaders[1].SetMat4("projection", projectionmatrix);
					_deferredshaders[1].SetMat4("view", _scene.m_camera.m_viewmatrix);
					_mc.Draw(e.m_modelidx, _deferredshaders[1], projectionmatrix * _scene.m_camera.m_viewmatrix, e.m_model);
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			///////////////////// LIGHTING PASS
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			_deferredshaders[2].UseProgram();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

			int i = 0;
			for (const auto& pl : _scene.m_pointlights) {
				_deferredshaders[2].SetVec3("lights[" + std::to_string(i) + "].Position", pl.m_position);
				_deferredshaders[2].SetVec3("lights[" + std::to_string(i) + "].Color", pl.m_diffuse);
				_deferredshaders[2].SetFloat("lights[" + std::to_string(i) + "].Linear", pl.m_linear);
				_deferredshaders[2].SetFloat("lights[" + std::to_string(i) + "].Quadratic", pl.m_quadratic);
				// then calculate radius of light volume/sphere
				const float maxBrightness = std::fmaxf(std::fmaxf(pl.m_diffuse.r, pl.m_diffuse.g), pl.m_diffuse.b);
				float radius = (-pl.m_linear + std::sqrt(pl.m_linear * pl.m_linear - 4 * pl.m_quadratic * (pl.m_constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * pl.m_quadratic);
				_deferredshaders[2].SetFloat("lights[" + std::to_string(i) + "].Radius", radius);

				i++;
			}
			_deferredshaders[2].SetVec3("viewPos", _scene.m_camera.m_position);

			renderQuad();

			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
													   // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
													   // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
													   // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
			glBlitFramebuffer(0, 0, _window->GetScreenWidth(), _window->GetScreenHeight(), 0, 0, _window->GetScreenWidth(), _window->GetScreenHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDepthFunc(GL_LEQUAL);
			_shaders[3].UseProgram();
			glm::mat4 view = glm::mat4(glm::mat3(_scene.m_camera.m_viewmatrix));
			_skybox.Draw(_shaders[3], projectionmatrix, view);
			glDepthFunc(GL_LESS);
			//ForwardRenderScene(_scene, _shaders, _mc, _skybox);
			

			_window->SwapWindow();

		}   // End Main Loop
	}

	// Engine wrapper 
	RenderEngine::RenderEngine(const GameConfig& config, SASWindow* window)
		: _impl(config, window, &_eventqueue)
	{

	}

	RenderEngine::~RenderEngine() {
		_impl.Stop();
		_renderthread.join();
	}

	void RenderEngine::Start(ModelContainer&& mc, TextureContainer&& tc) {
		SDL_GL_MakeCurrent(NULL, NULL);
		_renderthread = std::thread([&]() {
			// Separate initialize due to needing to switch context first
			_impl.Initialize(std::move(mc), std::move(tc));
			_impl.Run();
		});
	}

	bool RenderEngine::isRunning() {
		return _impl.isRunning();
	}

	void RenderEngine::RegisterScene(Scene& scene) {
		_eventqueue.enqueue(scene);
	}


	// renderQuad() renders a 1x1 XY quad in NDC
	// -----------------------------------------
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}