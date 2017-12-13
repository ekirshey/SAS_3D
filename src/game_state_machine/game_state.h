#pragma once
#include <memory>
#include <iostream>
#include "core/sas_video.h"
#include "core/sas_io.h"
#include "utility/locking_queue.h"
#include "render_engine/render_engine.h"

namespace SAS_3D {
	enum class FSMStates { INITIALIZE = 0, TRANSITIONIN, UPDATE, TRANSITIONOUT, EXIT };
	class GameStateImpl {
	public:
		virtual ~GameStateImpl() {}

		// Every state must handle init and update
		virtual FSMStates InitializeState( const InputState& input, RenderQueue* event_queue) = 0;
		virtual FSMStates UpdateState(int elapsedtime, RenderQueue* event_queue, const InputState& input) = 0;

		// Not every state has to do these
		virtual FSMStates TransitionIntoState() {
			return FSMStates::UPDATE;
		}

		virtual FSMStates TransitionFromState() {
			return FSMStates::EXIT;
		}

		virtual int NextState() = 0;
	};

	class GameState
	{
	public:
		GameState(int stateid, bool persistent, RenderQueue* event_queue, std::unique_ptr<GameStateImpl> impl)
			: _stateid(stateid)
			, _currentfsmstate(FSMStates::INITIALIZE)
			, _persistent(persistent)
			, _eq(event_queue)
			, _impl(std::move(impl))
		{

		}

		GameState(const GameState& g) = delete;

		virtual ~GameState() {}

		// Default FSM for a state
		int FiniteStateMachine(int elapsedtime, const InputState& input)
		{
			int nextgamestate = _stateid;
			switch (_currentfsmstate)
			{
			case FSMStates::INITIALIZE:
				_currentfsmstate = _impl->InitializeState(input, _eq);
				break;
			case FSMStates::TRANSITIONIN:
				_currentfsmstate = _impl->TransitionIntoState();
				break;
			case FSMStates::UPDATE:
				_currentfsmstate = _impl->UpdateState(elapsedtime, _eq, input);
				break;
			case FSMStates::TRANSITIONOUT:
				_currentfsmstate = _impl->TransitionFromState();
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
		RenderQueue* _eq;
		std::unique_ptr<GameStateImpl> _impl;

	};
}