include(FetchContent)
FetchContent_Declare(cjson
                     DOWNLOAD_EXTRACT_TIMESTAMP ON
                     URL https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.16.tar.gz
                     URL_HASH SHA512=3a894de03c33d89f1e7ee572418d5483c844d38e1e64aa4f6297ddaa01f4111f07601f8d26617b424b5af15d469e3955dae075d9f30b5c25e16ec348fdb06e6f)
FetchContent_Populate(cjson)

# set(ENABLE_CJSON_TEST OFF CACHE BOOL "")
# set(BUILD_SHARED_AND_STATIC_LIBS OFF CACHE BOOL "")
# set(CJSON_BUILD_SHARED_LIBS OFF CACHE BOOL "")
# set(CJSON_OVERRIDE_BUILD_SHARED_LIBS ON CACHE BOOL "")

# add_subdirectory(${cjson_SOURCE_DIR} ${cjson_BINARY_DIR})

# add_library(cjson::cjson STATIC IMPORTED)
# set_target_properties(cjson::cjson PROPERTIES
#                       INTERFACE_INCLUDE_DIRECTORIES ${cjson_SOURCE_DIR}
#                       IMPORTED_LOCATION lib/libcjson.a)
