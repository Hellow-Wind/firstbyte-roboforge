# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target wpr_simulation2::wpr_simulation2
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${wpr_simulation2_TARGETS}.
if(wpr_simulation2_TARGETS AND NOT TARGET wpr_simulation2::wpr_simulation2)
  add_library(wpr_simulation2::wpr_simulation2 INTERFACE IMPORTED)
  set_target_properties(wpr_simulation2::wpr_simulation2 PROPERTIES
    INTERFACE_LINK_LIBRARIES "${wpr_simulation2_TARGETS}")
endif()
