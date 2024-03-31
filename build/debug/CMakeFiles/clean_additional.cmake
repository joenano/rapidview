# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/dayjur_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/dayjur_autogen.dir/ParseCache.txt"
  "dayjur_autogen"
  )
endif()
