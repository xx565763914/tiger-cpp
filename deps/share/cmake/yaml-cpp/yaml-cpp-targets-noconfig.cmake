#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "yaml-cpp" for configuration ""
set_property(TARGET yaml-cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(yaml-cpp PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libyaml-cpp.so.0.7.0"
  IMPORTED_SONAME_NOCONFIG "libyaml-cpp.so.0.7"
  )

list(APPEND _IMPORT_CHECK_TARGETS yaml-cpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_yaml-cpp "${_IMPORT_PREFIX}/lib/libyaml-cpp.so.0.7.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
