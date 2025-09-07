module;

#include <format>
#include <functional>
#include <optional>
#include <string>
#include <utility>

module Core.ReflectionContext;

import Core.TypeId;

namespace Core {

	ClassReflectionBuilderBase::ClassReflectionBuilderBase(TypeId typeId, std::string_view className)
		: mTypeId{ std::move(typeId) }
		, mClassReflection{ className } {}

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

	void ReflectionContext::addBasicType(TypeId typeId, TypeReflection typeReflection) {
		if (mBasicTypeReflections.contains(typeId)) {
			return;
		}

		const std::string& typeName{ typeReflection.getName() };
		if (mTypeNameLookup.contains(typeName)) {
			logEntry(mLog, "Type with name '{}' already exists.", typeName);
			return;
		}

		mTypeNameLookup.emplace(typeName, typeId);
		mBasicTypeReflections.emplace(typeId, std::move(typeReflection));
	}

	bool ReflectionContext::hasBasicType(const TypeId& typeId) const {
		return mBasicTypeReflections.contains(typeId);
	}

	const TypeReflection& ReflectionContext::getBasicType(TypeId typeId) const {
		return mBasicTypeReflections.at(typeId);
	}

	void ReflectionContext::addClass(TypeId typeId, ClassReflection classReflection) {
		appendLogEntries(mLog, classReflection.moveLog());

		const std::string& className{ classReflection.getName() };
		if (hasClass(typeId)) {
			const auto& existingClassName{ getClass(typeId).getName() };
			logEntry(mLog,
				"Failed to register Class '{}' with TypeId as Class '{}' already exists.",
				className,
				existingClassName
			);
			return;
		}

		if (auto it = mTypeNameLookup.find(className); it != mTypeNameLookup.end()) {
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
				"Failed to register Class '{}' as {} '{}' is already registered to it.",
				className,
				type,
				name
			);
			return;
		}

		mTypeNameLookup.emplace(className, typeId);
		mClassReflections.emplace(typeId, std::move(classReflection));
	}

	bool ReflectionContext::hasClass(const TypeId& typeId) const { return mClassReflections.contains(typeId); }

	const ClassReflection& ReflectionContext::getClass(const TypeId& typeId) const {
		return mClassReflections.at(typeId);
	}

	void ReflectionContext::addEnum(TypeId typeId, EnumReflection enumReflection) {
		appendLogEntries(mLog, enumReflection.moveLog());

		if (hasEnum(typeId)) {
			logEntry(mLog, "Enum already exists.\n");
			return;
		}

		const std::string& enumName{ enumReflection.getName() };
		if (mTypeNameLookup.contains(enumName)) {
			logEntry(mLog, "Type with name '%s' already exists.\n", enumName.c_str());
			return;
		}

		mTypeNameLookup.emplace(enumName, typeId);
		mEnumReflections.emplace(std::move(typeId), std::move(enumReflection));
	}

	bool ReflectionContext::hasEnum(const TypeId& typeId) const { return mEnumReflections.contains(typeId); }

	const EnumReflection& ReflectionContext::getEnum(const TypeId& typeId) const { return mEnumReflections.at(typeId); }

	std::optional<TypeId> ReflectionContext::getTypeIdByName(const std::string& name) const {
		if (auto it = mTypeNameLookup.find(name); it != mTypeNameLookup.end()) {
			return it->second;
		}

		return std::nullopt;
	}

	Log ReflectionContext::moveLog() {
		return std::move(mLog);
	}

	void ReflectionContext::initializeBasicTypes() {
		addBasicType(TypeId::get<std::string>(), TypeReflection("std::string", sizeof(std::string)));
		addBasicType(TypeId::get<int32_t>(), TypeReflection("int32_t", sizeof(int32_t)));
		addBasicType(TypeId::get<double>(), TypeReflection("double", sizeof(double)));
		addBasicType(TypeId::get<float>(), TypeReflection("float", sizeof(float)));
		addBasicType(TypeId::get<bool>(), TypeReflection("bool", sizeof(bool)));
	}

} // namespace Core
