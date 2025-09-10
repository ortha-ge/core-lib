module;

#include <string>
#include <string_view>

export module Core.TypeReflection;

import Core.Log;
import Core.TypeId;

export namespace Core {

	class TypeReflection {
	public:
		TypeReflection(std::string_view name, size_t size);
		~TypeReflection() = default;
		TypeReflection(const TypeReflection&) = default;
		TypeReflection(TypeReflection&&) = default;

		TypeReflection& operator=(const TypeReflection&) = default;
		TypeReflection& operator=(TypeReflection&&) = default;

		[[nodiscard]] const std::string& getName() const;
		[[nodiscard]] size_t getSize() const;

		Log moveLog();

	private:
		std::string mName;
		size_t mSize{};
	};

} // namespace Core
