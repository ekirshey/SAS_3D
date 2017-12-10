#include <iostream>
#include <assimp/postprocess.h>     // Post processing flags
#include "render_engine.h"
#include "shaders/input_modules/mvp_module.h"
#include "shaders/input_modules/texture_module.h"

namespace SAS_3D {
	RenderEngine::RenderEngine(const GameConfig& config, LockingQueue<SceneGraph>& queue)
		: _config(config)
		, _shader(config.shaderpath + "texture.vert", config.shaderpath + "texture.frag")
		, _mc(config.modelpath)
		, _scene_queue(queue)
	{
		_shader.AddInputModule<MVPModule>();
		_shader.AddInputModule<TextureModule>();

		_crowidx = _mc.LoadModelFromFile(
			"hagraven/hagraven_idle.fbx"
			, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (_crowidx == -1) {
			std::cerr << "Error Loading Model." << std::endl;
		}

		_mc.LoadModelIntoGPU(_crowidx);
		std::cout << "Done loading textures!" << std::endl;

	}


	RenderEngine::~RenderEngine() {

	}

	void RenderEngine::Run() {
		while (_running) {

		}
	}

}