#pragma once
#include <unordered_map>
#include <string>
#include <thread>
#include <memory>
#include "core/sas_io.h"
#include "game_state_machine/game_state.h"
#include "game_state_machine/game_config.h"
#include "subsystems/subsystem_controller.h"

namespace SAS_3D {
	class Game
	{
	public:
		#define MAX_STATES 10
		Game(std::string config);
		virtual ~Game();

		template<typename T, typename... Args>
		void AddState(bool persistence, int index, Args&&... args) {
			if (index < MAX_STATES ) {
				auto stateimpl = std::make_unique<T>(std::forward<Args>(args)...);
				auto state = std::make_unique<GameState>(index, persistence, std::move(stateimpl));
				_gamestates[index] = std::move(state);
			}
			else {
				std::cout << "Error adding state. Index out of bounds" << std::endl;
			}
		}

		void Run();
		GameConfig Config() { return _config; }
	private:
		void Update(int elapsedtime, const InputState& inputstate);

		void RemoveStateAtIndex(int idx);

		GameConfig _config;
		std::vector<std::unique_ptr<GameState>> _gamestates;
		unsigned int _activestate;
		bool _quitgame;

		SubsystemController _subsystems;
	};
}