include(FetchContent)
include(CMakeFindDependencyMacro)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/ec25eea8f8237cf86c30703f59747e42f34b6f75.zip)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        nlohmann_json
        URL
        https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
find_dependency(nlohmann_json) # need this so json_schema_validator can find nlohmann

FetchContent_Declare(
        json_schema_validator
        URL
        https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.2.0.tar.gz)
FetchContent_MakeAvailable(json_schema_validator)

FetchContent_Declare(
        pybind11
        URL
        https://github.com/pybind/pybind11/archive/refs/tags/v2.10.3.tar.gz)
FetchContent_MakeAvailable(pybind11)

FetchContent_Declare(
        rapidjson
        URL
        https://github.com/Tencent/rapidjson/archive/012be8528783cdbf4b7a9e64f78bd8f056b97e24.zip 
        )
set(RAPIDJSON_BUILD_TESTS OFF)
set(RAPIDJSON_BUILD_DOC OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF)
message(STATUS ${rapidjson_SOURCE_DIR})
FetchContent_Populate(rapidjson)
message(STATUS ${rapidjson_SOURCE_DIR})
FetchContent_MakeAvailable(rapidjson)
# find_dependency(rapidjson)
# set(RAPIDJSON_INCLUDE_DIR ${rapidjson_SOURCE_DIR}/include)