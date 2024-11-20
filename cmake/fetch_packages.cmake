include(FetchContent)
include(CMakeFindDependencyMacro)

cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/ec25eea8f8237cf86c30703f59747e42f34b6f75.zip)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        json
        URL
        https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
set(JSON_Install ON)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        nlohmann_json_schema_validator
        URL
        https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.2.0.tar.gz)
FetchContent_MakeAvailable(nlohmann_json_schema_validator)

FetchContent_Declare(
        pybind11
        URL
        https://github.com/pybind/pybind11/archive/refs/tags/v2.10.3.tar.gz)
FetchContent_MakeAvailable(pybind11)

FetchContent_Declare(
    boost_math
    URL https://github.com/boostorg/math/archive/refs/tags/boost-1.86.0.tar.gz
)

FetchContent_GetProperties(boost_math)
if(NOT boost_math_POPULATED)
    FetchContent_Populate(boost_math)
    set(BOOST_MATH_INCLUDE_DIR ${boost_math_SOURCE_DIR}/include)
endif()

FetchContent_Declare(
    boost_utility
    URL https://github.com/boostorg/utility/archive/refs/tags/boost-1.86.0.tar.gz
)

FetchContent_GetProperties(boost_utility)
if(NOT boost_utility_POPULATED)
    FetchContent_Populate(boost_utility)
    set(BOOST_UTILITY_INCLUDE_DIR ${boost_utility_SOURCE_DIR}/include)
endif()



# FetchContent_Declare(
#   boost_multiprecision
#   URL https://github.com/boostorg/multiprecision/archive/refs/tags/Boost_1_86_0.tar.gz
# )

# FetchContent_GetProperties(boost_multiprecision)
# if(NOT boost_multiprecision_POPULATED)
#   FetchContent_Populate(boost_multiprecision)
#   set(BOOST_MULTIPRECISION_INCLUDE_DIR ${boost_multiprecision_SOURCE_DIR}/include)
# endif()

# include_directories(${BOOST_MULTIPRECISION_INCLUDE_DIR})
# include_directories(${BOOST_MATH_INCLUDE_DIR})
