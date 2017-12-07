include_directories(
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	${FREEIMAGE_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/third_party/asio
)

file(GLOB SRC_IMGUI
	${PROJECT_SOURCE_DIR}/src/third_party/imgui/*.h
	${PROJECT_SOURCE_DIR}/src/third_party/imgui/*.cpp
)

file(GLOB SRC_THIRD_PARTY
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/third_party/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/third_party/*.cpp
	${PROJECT_SOURCE_DIR}/src/third_party/*.c
)

file(GLOB SRC_CORE
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/core/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/core/*.cpp
)

file(GLOB SRC_SHADERS
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/shaders/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/shaders/*.cpp
)

file(GLOB SRC_ASSETS
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/assets/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/assets/*.cpp
)

file(GLOB SRC_ENTITIES
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/entities/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/entities/*.cpp
)

file(GLOB SRC_GSM
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/game_state_machine/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/game_state_machine/*.cpp
)

file(GLOB SRC_NETWORK
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/network/*.h
	
	#SOURCE
	${PROJECT_SOURCE_DIR}/src/network/*.cpp
)

file( GLOB SRC 
	#INCLUDE
	${PROJECT_SOURCE_DIR}/src/*.h

	#SOURCE
	${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(SAS_3D
	${SRC} 
	${SRC_CORE}
	${SRC_ASSETS}
	${SRC_SHADERS}
	${SRC_ENTITIES}
	${SRC_GSM}
	${SRC_NETWORK}
	${SRC_IMGUI}
	${SRC_THIRD_PARTY}
)

source_group("assets"				FILES ${SRC_ASSETS})
source_group("core"					FILES ${SRC_CORE})
source_group("shaders"				FILES ${SRC_SHADERS})
source_group("entities"				FILES ${SRC_ENTITIES})
source_group("game_state_machine"	FILES ${SRC_GSM})
source_group("network"				FILES ${SRC_NETWORK})
source_group("third_party"			FILES ${SRC_THIRD_PARTY})
source_group("third_party\\imgui"	FILES ${SRC_IMGUI})

target_link_libraries (SAS_3D LINK_PUBLIC ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY_DEBUG} ${FREEIMAGE_LIBRARY_DEBUG})

target_compile_definitions(SAS_3D
	PRIVATE ASIO_STANDALONE
	PRIVATE ASIO_HAS_MOVE
	PRIVATE ASIO_HAS_STD_SYSTEM_ERROR
)
