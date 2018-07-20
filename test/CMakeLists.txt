add_subdirectory(Catch2)
include("Catch2/contrib/ParseAndAddCatchTests.cmake")

file(GLOB files "test_*.cpp")
foreach(file ${files})
    get_filename_component(testcase ${file} NAME_WE)

    add_executable(${testcase} ${file})

    #target_include_directories(${testcase} PRIVATE Catch2)
    target_include_directories(${testcase} PRIVATE ${NT_WRAPPER_INCLUDE_BUILD_DIR})
    target_link_libraries(${testcase} ${NT_WRAPPER_TARGET_NAME})
	target_link_libraries(${testcase} Catch2::Catch2)

	set_target_properties(${testcase} PROPERTIES
        COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
        COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
    )

	ParseAndAddCatchTests(${testcase})
endforeach()