# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   FREEIMAGE_INCLUDE_DIR - include directories for FreeImage
#   FREEIMAGE_LIBRARIES - libraries to link against FreeImage
#   FREEIMAGE_FOUND - true if FreeImage has been found and can be used

find_path(FREEIMAGE_INCLUDE_DIR FreeImage/FreeImage.h PATHS ${PROJECT_SOURCE_DIR}/src/third_party) 

find_library(FREEIMAGE_LIBRARY_RELEASE NAMES FreeImage.lib PATHS ${PROJECT_SOURCE_DIR}/third_party_lib)
find_library(FREEIMAGE_LIBRARY_DEBUG NAMES FreeImage.lib PATHS ${PROJECT_SOURCE_DIR}/third_party_lib)

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(FreeImage
                                  REQUIRED_VARS FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARY_DEBUG)

mark_as_advanced(FREEIMAGE_LIBRARY_RELEASE)
mark_as_advanced(FREEIMAGE_LIBRARY_DEBUG)
mark_as_advanced(FREEIMAGE_INCLUDE_DIR)
