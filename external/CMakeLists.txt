project(external)

add_subdirectory(boost)
add_subdirectory(bullet)
add_subdirectory(assimp)
add_subdirectory(squirrel)
add_subdirectory(gwen)
add_subdirectory(yaml)
#add_subdirectory(sfml)
#add_subdirectory(freeimage)

if(NOT ANDROID)
add_subdirectory(glew)
add_subdirectory(glfw)
add_subdirectory(freetype)
endif()
