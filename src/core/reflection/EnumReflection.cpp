module;

#include <cstdio>
#include <string>
#include <string_view>
#include <vector>

module Core.EnumReflection;

import Core.Any;

namespace Core {

	EnumReflection::EnumReflection(std::string_view name)
		: mName(name) {}

	void EnumReflection::addEnumerator(std::string_view name, Any value) {
		if (hasEnumerator(name)) {
			logEntry(mLog, "Duplicate enumerator '{}' found.", name);
			return;
		}

		mEnumerators.emplace_back(name, std::move(value));
	}

	bool EnumReflection::hasEnumerator(std::string_view name) const {
		return std::ranges::find_if(mEnumerators, [name](const Enumerator& enumerator) {
				   return enumerator.first == name;
			   }) != mEnumerators.end();
	}

	Any EnumReflection::getEnumeratorValue(std::string_view name) const {
		auto it = std::ranges::find_if(
			mEnumerators, [name](const Enumerator& enumerator) { return enumerator.first == name; });
		return it != mEnumerators.end() ? it->second : Any{};
	}

	const std::string& EnumReflection::getName() const { return mName; }

	const std::vector<EnumReflection::Enumerator>& EnumReflection::getEnumerators() const { return mEnumerators; }

	Log EnumReflection::moveLog() {
		return std::move(mLog);
	}

} // namespace Core
