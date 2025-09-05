module;

#include <string>
#include <vector>

export module Core.EnumReflection;

import Core.Any;
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

	private:

		std::string mName;
		std::vector<Enumerator> mEnumerators;

	};

	class EnumReflectionBuilderBase {
	public:

		EnumReflection build();

	protected:

		explicit EnumReflectionBuilderBase(std::string_view name);

		void enumerator(std::string_view name, Any value);

	private:

		EnumReflection mReflection;

	};

	template <typename T>
	class EnumReflectionBuilder : public EnumReflectionBuilderBase {
	public:

		explicit EnumReflectionBuilder(std::string_view name)
			: EnumReflectionBuilderBase(name) {
		}

		EnumReflectionBuilder& constant(std::string_view name, T value) {
			Any valueAny(value);
			Any valueCopy = valueAny;
			enumerator(name, std::move(valueCopy));
			return *this;
		}

	};

} // namespace Core
