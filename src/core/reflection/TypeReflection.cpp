module;

#include <string>
#include <string_view>

module Core.TypeReflection;

namespace Core {

	TypeReflection::TypeReflection(std::string_view typeName, size_t size)
		: mName(typeName)
		, mSize(size) {}

	const std::string& TypeReflection::getName() const { return mName; }

	size_t TypeReflection::getSize() const { return mSize; }

} // namespace Core
