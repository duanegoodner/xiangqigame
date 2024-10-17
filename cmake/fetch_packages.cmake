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
    Boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.86.0/source/boost_1_86_0.tar.gz
)

FetchContent_GetProperties(Boost)
if(NOT Boost_POPULATED)
    FetchContent_Populate(Boost)
    set(BOOST_INCLUDEDIR ${boost_SOURCE_DIR}/)
endif()
