#pragma once
#include <unordered_map>
#include <string>
#include <thread>
#include <memory>
#include "core/sas_io.h"
#include "network/client.h"
#include "game_state_machine/game_state.h"
#include "game_state_machine/game_config.h"
#include "utility/locking_queue.h"
#include "render_engine/render_engine.h"

namespace SAS_3D {
	class Game
	{
	public:
		Game(std::string config);
		virtual ~Game();

		template<typename T, typename... Args>
		void AddState(bool persistence, int index, Args&&... args) {
			auto stateimpl = std::make_unique<T>(std::forward<Args>(args)...);
			auto state = std::make_unique<GameState>(index, persistence, &_event_queue, std::move(stateimpl));
			_gamestates.insert({ index,std::move(state) });
		}

		void Run();
		GameConfig Config() { return _config; }
	private:
		void Update(int elapsedtime, const InputState& inputstate);

		void RemoveStateAtIndex(int idx);

		GameConfig _config;
		uptrSASWindow _window;
		std::unordered_map<int, std::unique_ptr<GameState>> _gamestates;
		unsigned int _activestate;
		bool _gamerunning;

		std::unique_ptr<RenderEngine> _renderengine;
		RenderQueue _event_queue;
		std::thread _renderthread;

		Client _client;
	};
}