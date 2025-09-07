module;

#include <string>
#include <vector>

export module Core.EnumReflection;

import Core.Any;
import Core.Log;
import Core.TypeId;

export namespace Core {

	class EnumReflection {
	public:
		using Enumerator = std::pair<std::string, Any>;

		explicit EnumReflection(std::string_view name);
		~EnumReflection() = default;

		EnumReflection(const EnumReflection&) = default;
		EnumReflection(EnumReflection&&) noexcept = default;

		EnumReflection& operator=(const EnumReflection&) = default;
		EnumReflection& operator=(EnumReflection&&) noexcept = default;

		void addEnumerator(std::string_view name, Any value);
		bool hasEnumerator(std::string_view name) const;
		Any getEnumeratorValue(std::string_view name) const;

		const std::string& getName() const;
		const std::vector<Enumerator>& getEnumerators() const;

		Log moveLog();

	private:
		std::string mName;
		std::vector<Enumerator> mEnumerators;
		Log mLog;
	};

} // namespace Core
