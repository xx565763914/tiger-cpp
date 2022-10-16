#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hiredis::hiredis" for configuration ""
set_property(TARGET hiredis::hiredis APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(hiredis::hiredis PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libhiredis.so.1.0.0"
  IMPORTED_SONAME_NOCONFIG "libhiredis.so.1.0.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS hiredis::hiredis )
list(APPEND _IMPORT_CHECK_FILES_FOR_hiredis::hiredis "${_IMPORT_PREFIX}/lib/libhiredis.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
