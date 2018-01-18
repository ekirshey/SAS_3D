#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include "ecs/framework/system_manager.h"
#include "ecs/framework/entity_manager.h"
#include "core/uuid.h"

namespace SAS_3D {
	class System;
	class SubsystemController;

	class ECSManager
	{
	public:
		ECSManager();
		virtual ~ECSManager();

		SystemID AddSystem(std::unique_ptr<System> system, int priority);

		// Use std::forward to maintain lvalue and rvalue references
		template<typename T, typename... Args>
		SystemID AddSystem(std::string systemname, int priority, Args&&... args) {
			auto system = std::make_unique<T>(systemname, GenerateUUID(), std::forward<Args>(args)...);
			return _systemmanager.AddSystem(std::move(system), priority);
		}

		System* GetSystem(SystemID systemid);

		EntityID CreateEntity();
		void RemoveEntity(EntityID entity);

		template<class T>
		T GetEntityComponent(EntityID entity, EntityID componentid) {
			return static_cast<T>(entitymanager_.GetEntityComponent(entity, componentid));
		}

		std::vector<Component*> GetAllEntityComponents(EntityID entity);

		EntityID GetEntityComponentBits(EntityID entity) { return _entitymanager.GetEntityComponentBits(entity); }
		EntityID EntityCount() { return _entitymanager.EntityCount(); }

		void AddComponentToEntity(EntityID entity, std::unique_ptr<Component> componenttoadd);

		template<typename T, typename... Args>
		void AddComponentToEntity(EntityID entity, Args&&... args) {
			auto c = std::make_unique<T>(std::forward<Args>(args)...);
			if (_entitymanager.AddComponent(entity, std::move(c))) {
				_systemmanager.AddEntityToSystem(entity, _entitymanager.GetEntityComponentBits(entity));
			}
		}

		void RemoveComponentFromEntity(EntityID entity, EntityID componentid);

		void Update(int elapsedtime, SubsystemController* subsystems);

		// Tag functions    TODO Should these be in an object? Idk it will just double the function calls, who cares?
		void ClearTagVector(std::string tag);
		void AssignEntityTag(EntityID entity, std::string tag);
		void RemoveEntityFromTag(EntityID entity, std::string tag);
		void RemoveTag(std::string tag);
		void AddTag(std::string tag);
		std::vector<std::string> GetAssociatedTags(EntityID entity);
		std::vector<EntityID> GetAssociatedEntities(std::string tag);
		std::vector<EntityID>* GetPtrToAssociatedEntities(std::string tag) { return &_tagmanager[tag]; }

		template<typename T, typename... Args>
		void SendMessage(std::string queuename, Args&&... args) {
			messagequeuecontainer_.SendMessage<T>(queuename, std::forward<Args>(args)...);
		}

		void Exit(int code) {
			_errorcode = code;
			_status = false;
		}

		bool GetStatus(int& errorcode) {
			errorcode = _errorcode;
			return _status;
		}

	private:
		bool _status;
		int _errorcode;
		SystemManager _systemmanager;
		EntityManager _entitymanager;
		std::unordered_map<std::string, std::vector<EntityID>> _tagmanager;
	};
}