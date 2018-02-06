/*
* System.h 
* Defines the wrapper System class for all other Systems.
* This System can contain any object as long as it defines the appropriate Update call
* and it also MUST have a copy constructor
* This allows new systems to be created without having to inherit from some base class.
*
*/
#include <cstdint>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include "message.h"

namespace SAS_3D {
	// Forward declerations
	class SubsystemController;
	class EntityManager;

	// Useful typedefs
	using EntityID = unsigned long long;
	using ComponentID = unsigned long long;
	using SystemID = unsigned long long;

	class System final
	{
	public:
		template<typename T>
		System(T x, std::string systemname, long long componentids)
			: _self(std::make_unique<model<T>>(std::move(x)))
			, _id(GenerateUUID())
			, _systemname(systemname)
			, _componentids(componentids)
			, _elapsedtime(0)
			, _frametime(0)
			, _entitycount(0)
		{
		}

		System(const System& x) : _self(x._self->_copy()) {
		}
		System(System&&) noexcept = default;

		System& operator=(const System& x) {
			return *this = System(x);
		}

		System& operator=(System&&) noexcept = default;

		~System() {}

		void Update(long long elapsedtime, EntityManager* em, SubsystemController* subsystems);
		void HandleCallback(Message& m);

		ComponentID ComponentBits() const { return _componentids; }

		void AddEntity(EntityID entityid);
		void RemoveEntity(EntityID entityid);

		bool ContainsEntity(EntityID entityid);    // TODO Not sure if needed
		bool ValidEntity(EntityID entitycomponents);

		EntityID GetEntityCount()  const { return _entitycount; }
		void EntityCount(EntityID entitycount) { _entitycount = entitycount; }

		long long FrameTime() const { return _frametime; }
		void SetFrameTime(long long frametime) { _frametime = frametime; }

		long long TimeRunning() const { return _elapsedtime; }
		void UpdateTimeRunning(long long runtime) { _elapsedtime += runtime; }

		std::string SystemName() const { return _systemname; }
		void SetSystemName(std::string name) { _systemname = name; }

		SystemID GetID() const { return _id; }

	private:
		struct concept_t {
			virtual ~concept_t() = default;
			virtual std::unique_ptr<concept_t> _copy() const = 0;
			virtual void Update(long long elapsedtime, const std::vector<EntityID>& entities, EntityManager* em, SubsystemController* subsystems) = 0;
			virtual void HandleCallback(Message& m) = 0;
		};

		template<typename T>
		struct model final : concept_t {
			model(T x) : _data(std::move(x)) {}
			std::unique_ptr<concept_t> _copy() const { return std::make_unique<model>(*this); }
			void Update(long long elapsedtime, const std::vector<EntityID>& entities, EntityManager* em, SubsystemController* subsystems) override {
				_data.Update(elapsedtime, entities, em, subsystems);
			}

			void HandleCallback(Message& m) override {
				_data.HandleCallback(m);
			}

			T _data;
		};

		std::unique_ptr<concept_t> _self;
		SystemID _id;
		std::string _systemname;    // For debugging purposes
		long long _componentids;          // relevant components
		long long _elapsedtime;
		long long _frametime;
		EntityID _entitycount;
		std::vector<EntityID> _relevantentities;
	};
}