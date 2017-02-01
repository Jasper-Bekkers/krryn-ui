include_directories(
    "include"
)


add_definitions(-DGLEW_STATIC)

set(glew_src
	"include/GL/glew.h"
	"include/GL/glxew.h"
	"include/GL/wglew.h"
	"src/glew.c"
)

add_library(GLEW STATIC ${glew_src})
