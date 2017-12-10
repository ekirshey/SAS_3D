#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "core/sas_video.h"
#include "core/sas_input.h"
#include "network/client.h"
#include "game_state_machine/game_state.h"
#include "game_state_machine/game_config.h"

namespace SAS_3D {
	class Game
	{
	public:
		Game(std::string config);
		virtual ~Game();

		template<typename T, typename... Args>
		void AddState(bool persistence, int index, Args&&... args) {
			auto stateimpl = std::make_unique<T>(std::forward<Args>(args)...);
			auto state = std::make_unique<GameState>(index, persistence, std::move(stateimpl));
			_gamestates.insert({ index,std::move(state) });
		}

		void Run();
		int ViewportWidth() { return _window->GetScreenWidth(); }
		int ViewportHeight() { return _window->GetScreenHeight(); }
		GameConfig Config() { return _config; }
	private:
		void Update(int elapsedtime, const InputState& inputstate);

		void RemoveStateAtIndex(int idx);

		GameConfig _config;
		std::unordered_map<int, std::unique_ptr<GameState>> _gamestates;
		unsigned int _activestate;
		bool _gamerunning;

		uptrSASWindow _window;
		Client _client;
	};
}