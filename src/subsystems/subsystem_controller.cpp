#include "subsystem_controller.h"
#include "assets/assimp_loader.h"

namespace SAS_3D {

	SubsystemController::SubsystemController(const GameConfig& config) 
		: _config(config)
		, _window(InitializeVideo(_config.windowtitle, _config.screenwidth, _config.screenheight))
		, _renderengine(_config, _window.get())
		, _client(_config.serverip, _config.port)
	{

	}

	SubsystemController::~SubsystemController() {

	}

	bool SubsystemController::Bootstrap() {
		AnimationContainer ac;
		ModelContainer mc;
		AssimpLoader::Load(_config.model_registry, ac, mc, aiProcess_Triangulate | aiProcess_FlipUVs);

		_animengine.Initialize(std::move(ac));
		_renderengine.Start(std::move(mc));

		// Wait for subsystems to start so we can safely free the assimp loader
		while (!_renderengine.isRunning()) {}

		return true;
	}
}