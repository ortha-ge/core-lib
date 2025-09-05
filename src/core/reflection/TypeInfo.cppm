module;

#include <cstddef>

export module Core.TypeInfo;

export namespace Core {
	class TypeInfo {
	public:
		~TypeInfo();

		TypeInfo(const TypeInfo&) = delete;
		TypeInfo& operator=(const TypeInfo&) = delete;
		TypeInfo(TypeInfo&&) noexcept = delete;
		TypeInfo& operator=(TypeInfo&&) noexcept = delete;

		[[nodiscard]] size_t getSize() const;

	protected:
		TypeInfo(size_t size);

		size_t mSize;
	};
} // namespace Core
