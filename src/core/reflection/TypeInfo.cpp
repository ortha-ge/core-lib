module;

#include <memory>

module Core.TypeInfo;

import Core.TypeInfoImpl;

namespace Core {
	TypeInfo::TypeInfo(size_t size)
		: mSize(size) {}

	TypeInfo::~TypeInfo() {}

	[[nodiscard]] size_t TypeInfo::getSize() const { return mSize; }
} // namespace Core
