module;

#include <functional>

export module Core.Any;

import Core.TypeId;

export namespace Core {

	class Any {
	public:
		Any();
		Any(TypeId typeId, void* instance);
		Any(TypeId typeId, const void* instance);
		Any(TypeId typeId, void* instance, bool ownsInstance);
		Any(TypeId typeId, const void* instance, bool ownsInstance);
		~Any();

		explicit Any(TypeId typeId);

		Any(const Any&);
		Any(Any&&) noexcept;

		Any& operator=(const Any& other);
		Any& operator=(Any&&) noexcept;

		template<typename T>
		Any(T&& instance, std::true_type)
			: mTypeId{ TypeId::get<std::remove_const_t<std::remove_reference_t<T>>>() }
			, mInstance(&const_cast<std::add_lvalue_reference_t<std::remove_const_t<std::remove_reference_t<T>>>>(instance))
			, mOwnsInstance(false) {}

		template <typename T>
		Any(T&& instance, std::false_type)
			: Any(TypeId::get<std::remove_const_t<std::remove_reference_t<T>>>()) {

			*static_cast<T*>(mInstance) = std::forward<T>(instance);
		}

		template<typename T>
		explicit Any(T&& instance)
			: Any(std::forward<T>(instance), std::is_lvalue_reference<T>()) {
		}

		const TypeId& getTypeId() const;
		void* getInstance() const;

	private:
		TypeId mTypeId{};
		void* mInstance{};
		bool mOwnsInstance{};
	};

} // namespace Core

template<>
struct std::less<Core::Any> {
	bool operator()(const Core::Any& lhs, const Core::Any& rhs) const { return &lhs < &rhs; }
};
