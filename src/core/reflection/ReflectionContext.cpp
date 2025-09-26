module;

#include <format>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

module Core.ReflectionContext;

import Core.TypeId;

namespace Core {

	ClassReflectionBuilderBase::ClassReflectionBuilderBase(TypeId typeId, std::string_view className)
		: mTypeId(std::move(typeId))
		, mClassReflection(className) {}

	ClassReflection ClassReflectionBuilderBase::getReflection() { return std::move(mClassReflection); }

	void ClassReflectionBuilderBase::property(ClassProperty property) {
		mClassReflection.addProperty(std::move(property));
	}

	EnumReflectionBuilderBase::EnumReflectionBuilderBase(std::string_view name)
		: mReflection(name) {}

	EnumReflection EnumReflectionBuilderBase::getReflection() { return std::move(mReflection); }

	void EnumReflectionBuilderBase::enumerator(std::string_view name, Any value) {
		mReflection.addEnumerator(name, std::move(value));
	}

	ReflectionContext::ReflectionContext() { initializeBasicTypes(); }

	void ReflectionContext::addReflection(TypeId typeId, ReflectionTypes typeReflection) {
		appendLogEntries(mLog, std::visit([](auto&& reflection) { return reflection.moveLog(); }, typeReflection));

		const std::string& typeName{ std::visit([](auto&& reflection) {
			return reflection.getName();
		}, typeReflection) };

		if (hasReflection(typeId)) {
			const std::string& existingTypeName{ std::visit([](auto&& reflection) {
					return reflection.getName();
				}, getReflection(typeId))
			};

			logEntry(mLog,
				"Failed to add reflection for '{}' with TypeId as type '{}' already exists.",
				typeName,
				existingTypeName
			);
			return;
		}

		if (auto it = mTypeNameLookup.find(typeName); it != mTypeNameLookup.end()) {
			std::string type{ "Unknown" };
			std::string name{ "Unknown" };
			if (hasClass(it->second)) {
				type = "Class";
				name = getClass(it->second).getName();
			} else if (hasEnum(it->second)) {
				type = "Enum";
				name = getEnum(it->second).getName();
			} else if (hasBasicType(it->second)) {
				type = "BasicType";
				name = getBasicType(it->second).getName();
			}

			logEntry(mLog,
				"Failed to register type '{}' as {} '{}' is already registered to it.",
				typeName,
				type,
				name
			);
			return;
		}

		mTypeNameLookup.emplace(typeName, typeId);
		mTypeReflections.emplace(typeId, std::move(typeReflection));
	}

	bool ReflectionContext::hasReflection(const TypeId& typeId) const {
		return mTypeReflections.contains(typeId);
	}

	const ReflectionContext::ReflectionTypes& ReflectionContext::getReflection(const TypeId& typeId) const {
		return mTypeReflections.at(typeId);
	}

	bool ReflectionContext::hasBasicType(const TypeId& typeId) const {
		if (auto it = mTypeReflections.find(typeId); it != mTypeReflections.end()) {
			return std::holds_alternative<TypeReflection>(it->second);
		}

		return false;
	}

	const TypeReflection& ReflectionContext::getBasicType(TypeId typeId) const {
		return std::get<TypeReflection>(mTypeReflections.at(typeId));
	}

	bool ReflectionContext::hasClass(const TypeId& typeId) const {
		if (auto it = mTypeReflections.find(typeId); it != mTypeReflections.end()) {
			return std::holds_alternative<ClassReflection>(it->second);
		}

		return false;
	}

	const ClassReflection& ReflectionContext::getClass(const TypeId& typeId) const {
		return std::get<ClassReflection>(mTypeReflections.at(typeId));
	}

	bool ReflectionContext::hasEnum(const TypeId& typeId) const {
		if (auto it = mTypeReflections.find(typeId); it != mTypeReflections.end()) {
			return std::holds_alternative<EnumReflection>(it->second);
		}

		return false;
	}

	const EnumReflection& ReflectionContext::getEnum(const TypeId& typeId) const {
		return std::get<EnumReflection>(mTypeReflections.at(typeId));
	}

	std::optional<TypeId> ReflectionContext::getTypeIdByName(const std::string& name) const {
		if (auto it = mTypeNameLookup.find(name); it != mTypeNameLookup.end()) {
			return it->second;
		}

		return std::nullopt;
	}

	std::optional<std::string> ReflectionContext::getNameFromTypeId(const TypeId& typeId) const {
		if (auto it = mTypeReflections.find(typeId); it != mTypeReflections.end()) {
			return std::visit([](auto&& reflection) {
				return reflection.getName();
			}, it->second);
		}

		return std::nullopt;
	}

	Log ReflectionContext::moveLog() {
		return std::move(mLog);
	}

	void ReflectionContext::forEachClass(const std::function<void(const ClassReflection&)>& visitor) const {
		for (auto&& [typeId, typeReflection] : mTypeReflections) {
			if (std::holds_alternative<ClassReflection>(typeReflection)) {
				visitor(std::get<ClassReflection>(typeReflection));
			}
		}
	}

	void ReflectionContext::initializeBasicTypes() {
		addReflection(TypeId::get<std::string>(), TypeReflection("std::string", sizeof(std::string)));
		addReflection(TypeId::get<int32_t>(), TypeReflection("int32", sizeof(int32_t)));
		addReflection(TypeId::get<uint8_t>(), TypeReflection("uint8", sizeof(uint8_t)));
		addReflection(TypeId::get<double>(), TypeReflection("double", sizeof(double)));
		addReflection(TypeId::get<float>(), TypeReflection("float", sizeof(float)));
		addReflection(TypeId::get<bool>(), TypeReflection("bool", sizeof(bool)));
	}

} // namespace Core
