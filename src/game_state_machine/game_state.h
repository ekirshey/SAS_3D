#pragma once
#include <memory>
#include <iostream>
#include "core/sas_video.h"
#include "core/sas_input.h"

namespace SAS_3D {
	namespace GSM {
		enum class FSMStates { INITIALIZE = 0, TRANSITIONIN, UPDATE, TRANSITIONOUT, EXIT };
		class GameStateImpl {
			public:
				virtual ~GameStateImpl() {}

				// Every state must handle init and update
				virtual FSMStates InitializeState(Core::SASWindow* window, const Core::InputState& input) = 0;
				virtual FSMStates UpdateState(int elapsedtime, Core::SASWindow* window, const Core::InputState& input) = 0;

				// Not every state has to do these
				virtual FSMStates TransitionIntoState(Core::SASWindow* window) {
					return FSMStates::UPDATE;
				}

				virtual FSMStates TransitionFromState(Core::SASWindow* window) {
					return FSMStates::EXIT;
				}

				virtual int NextState() = 0;
		};

		class GameState
		{
			public:
				GameState(int stateid, bool persistent, std::unique_ptr<GameStateImpl> impl)
					: _stateid(stateid)
					, _currentfsmstate(FSMStates::INITIALIZE)
					, _persistent(persistent)
					, _impl(std::move(impl))
				{

				}

				GameState(const GameState& g) = delete;

				virtual ~GameState() {}

				// Default FSM for a state
				int FiniteStateMachine(int elapsedtime, Core::SASWindow* window, const Core::InputState& input)
				{
					int nextgamestate = _stateid;
					switch (_currentfsmstate)
					{
					case FSMStates::INITIALIZE:
						_currentfsmstate = _impl->InitializeState(window, input);
						break;
					case FSMStates::TRANSITIONIN:
						_currentfsmstate = _impl->TransitionIntoState(window);
						break;
					case FSMStates::UPDATE:
						_currentfsmstate = _impl->UpdateState(elapsedtime, window, input);
						break;
					case FSMStates::TRANSITIONOUT:
						_currentfsmstate = _impl->TransitionFromState(window);
						break;
					case FSMStates::EXIT:
						nextgamestate = _impl->NextState();
						if (_persistent)
							_currentfsmstate = FSMStates::TRANSITIONIN;
						else
							_currentfsmstate = FSMStates::INITIALIZE;
					default:
						break;
					}
					return nextgamestate;
				}

				bool IsPersistent() { return _persistent; }

			private:
				int _stateid;
				FSMStates _currentfsmstate;
				bool _persistent;
				std::unique_ptr<GameStateImpl> _impl;

		};
	}
}