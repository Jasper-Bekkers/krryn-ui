add_subdirectory("GLEW")

include_directories(
    "GLEW/include"
)

add_executable(krryn-ui-test 
	main.cpp
	)

target_link_libraries(krryn-ui-test 
	krryn-ui
	GLEW)