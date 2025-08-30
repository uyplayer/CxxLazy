






file(GLOB_RECURSE source_files ${CMAKE_SOURCE_DIR}/src/*.cpp)
message(STATUS "source_files:")
foreach(src ${source_files})
    message(STATUS "  ${src}")
endforeach()

add_library(cxxlazy STATIC ${source_files})
target_include_directories(cxxlazy  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)




install(TARGETS cxxlazy
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)


install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
        DESTINATION include/cxxlazy
        FILES_MATCHING PATTERN "*.h")


