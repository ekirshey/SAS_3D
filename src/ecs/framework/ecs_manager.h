#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include "ecs/framework/system_manager.h"
#include "ecs/framework/entity_manager.h"

namespace SAS_3D {
	class System;

	class ECSManager
	{
	public:
		ECSManager();
		virtual ~ECSManager();

		int AddSystem(std::unique_ptr<System> system, int priority);

		// Use std::forward to maintain lvalue and rvalue references
		template<typename T, typename... Args>
		int AddSystem(std::string systemname, int priority, Args&&... args) {
			auto system = std::make_unique<T>(systemname, this, std::forward<Args>(args)...);
			return _systemmanager.AddSystem(std::move(system), priority);
		}

		System* GetSystem(int systemid);

		unsigned long long CreateEntity();
		void RemoveEntity(unsigned long long entity);

		template<class T>
		T GetEntityComponent(unsigned long long entity, unsigned long long componentid) {
			return static_cast<T>(entitymanager_.GetEntityComponent(entity, componentid));
		}

		std::vector<Component*> GetAllEntityComponents(unsigned long long entity);

		unsigned long long GetEntityComponentBits(unsigned long long entity) { return _entitymanager.GetEntityComponentBits(entity); }
		unsigned long long EntityCount() { return _entitymanager.EntityCount(); }

		void AddComponentToEntity(unsigned long long entity, std::unique_ptr<Component> componenttoadd);

		template<typename T, typename... Args>
		void AddComponentToEntity(unsigned long long entity, Args&&... args) {
			auto c = std::make_unique<T>(std::forward<Args>(args)...);
			if (entitymanager_.AddComponent(entity, std::move(c))) {
				_systemmanager.AddEntityToSystem(entity, entitymanager_.GetEntityComponentBits(entity));
			}
		}

		void RemoveComponentFromEntity(unsigned long long entity, unsigned long long componentid);

		void Update(int elapsedtime);

		// Tag functions    TODO Should these be in an object? Idk it will just double the function calls, who cares?
		void ClearTagVector(std::string tag);
		void AssignEntityTag(unsigned long long entity, std::string tag);
		void RemoveEntityFromTag(unsigned long long entity, std::string tag);
		void RemoveTag(std::string tag);
		void AddTag(std::string tag);
		std::vector<std::string> GetAssociatedTags(unsigned long long entity);
		std::vector<unsigned long long> GetAssociatedEntities(std::string tag);
		std::vector<unsigned long long>* GetPtrToAssociatedEntities(std::string tag) { return &_tagmanager[tag]; }

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
		std::unordered_map<std::string, std::vector<unsigned long long>> _tagmanager;
	};
}