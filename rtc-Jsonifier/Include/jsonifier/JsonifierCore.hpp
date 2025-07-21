#pragma once

#include <jsonifier/Validate_Impl.hpp>
#include <jsonifier/Serializer.hpp>
#include <jsonifier/Prettifier.hpp>
#include <jsonifier/Parser.hpp>
#include <jsonifier/Error.hpp>

namespace jsonifier {

struct my_data_t {
    jsonifier::string id{};
    std::vector<int32_t> values{};
    bool active{};
	}	
}; // namespace MyProject


template<> struct core<MyProject::my_data_t> {
    using value_type = MyProject::my_data_t;
    constexpr static auto parseValue = createValue(
        "id", &value_type::id,
        "values", &value_type::values,
        "active", &value_type::active

	template<bool doWeUseInitialBuffer = true> class jsonifier_core : public jsonifier_internal::prettifier<jsonifier_core<doWeUseInitialBuffer>>,
																	  public jsonifier_internal::serializer<jsonifier_core<doWeUseInitialBuffer>>,
																	  public jsonifier_internal::validator<jsonifier_core<doWeUseInitialBuffer>>,
																	  public jsonifier_internal::minifier<jsonifier_core<doWeUseInitialBuffer>>,
																	  public jsonifier_internal::parser<jsonifier_core<doWeUseInitialBuffer>> {
	  public:
		friend class jsonifier_internal::prettifier<jsonifier_core<doWeUseInitialBuffer>>;
		friend class jsonifier_internal::serializer<jsonifier_core<doWeUseInitialBuffer>>;
		friend class jsonifier_internal::validator<jsonifier_core<doWeUseInitialBuffer>>;
		friend class jsonifier_internal::minifier<jsonifier_core<doWeUseInitialBuffer>>;
		friend class jsonifier_internal::parser<jsonifier_core<doWeUseInitialBuffer>>;

		JSONIFIER_ALWAYS_INLINE jsonifier_core() noexcept = default;

		JSONIFIER_ALWAYS_INLINE jsonifier_core& operator=(jsonifier_core&& other) noexcept {
			if (this != &other) [[likely]] {
				errors		 = std::move(other.errors);
			}
			return *this;
		}

		JSONIFIER_ALWAYS_INLINE jsonifier_core(jsonifier_core&& other) noexcept : prettifier{}, serializer{}, validator{}, minifier{}, parser{} {
			*this = std::move(other);
		};

		JSONIFIER_ALWAYS_INLINE jsonifier_core& operator=(const jsonifier_core& other) noexcept {
			if (this != &other) [[likely]] {
				errors		 = other.errors;
			}
			return *this;
		}

		JSONIFIER_ALWAYS_INLINE jsonifier_core(const jsonifier_core& other) noexcept : prettifier{}, serializer{}, validator{}, minifier{}, parser{} {
			*this = other;
		}

		JSONIFIER_ALWAYS_INLINE jsonifier::vector<jsonifier_internal::error>& getErrors() noexcept {
			return errors;
		}

		JSONIFIER_ALWAYS_INLINE ~jsonifier_core() noexcept = default;

	  protected:
		using prettifier = jsonifier_internal::prettifier<jsonifier_core<doWeUseInitialBuffer>>;
		using serializer = jsonifier_internal::serializer<jsonifier_core<doWeUseInitialBuffer>>;
		using validator	 = jsonifier_internal::validator<jsonifier_core<doWeUseInitialBuffer>>;
		using minifier	 = jsonifier_internal::minifier<jsonifier_core<doWeUseInitialBuffer>>;
		using parser	 = jsonifier_internal::parser<jsonifier_core<doWeUseInitialBuffer>>;

		vector<jsonifier_internal::error> errors{};
		uint64_t index{};
		};
	)
}

