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
	${PROJECT_SOURCE_DIR}/src/third_party/*.h
	${PROJECT_SOURCE_DIR}/src/third_party/*.hpp
	${PROJECT_SOURCE_DIR}/src/third_party/*.cpp
	${PROJECT_SOURCE_DIR}/src/third_party/*.c
)

file(GLOB SRC_CORE
	${PROJECT_SOURCE_DIR}/src/core/*.h
	${PROJECT_SOURCE_DIR}/src/core/*.cpp
)

file(GLOB SRC_RENDER_ENGINE
	${PROJECT_SOURCE_DIR}/src/render_engine/*.h
	${PROJECT_SOURCE_DIR}/src/render_engine/*.cpp
)

file(GLOB SRC_SHADERS
	${PROJECT_SOURCE_DIR}/src/render_engine/shaders/*.h
	${PROJECT_SOURCE_DIR}/src/render_engine/shaders/*.cpp
)

file(GLOB SRC_INPUT_MODULES
	${PROJECT_SOURCE_DIR}/src/render_engine/shaders/input_modules/*.h
	${PROJECT_SOURCE_DIR}/src/render_engine/shaders/input_modules/*.cpp
)

file(GLOB SRC_ASSETS
	${PROJECT_SOURCE_DIR}/src/assets/*.h
	${PROJECT_SOURCE_DIR}/src/assets/*.cpp
)

file(GLOB SRC_ENTITIES
	${PROJECT_SOURCE_DIR}/src/entities/*.h
	${PROJECT_SOURCE_DIR}/src/entities/*.cpp
)

file(GLOB SRC_GSM
	${PROJECT_SOURCE_DIR}/src/game_state_machine/*.h
	${PROJECT_SOURCE_DIR}/src/game_state_machine/*.cpp
)

file(GLOB SRC_NETWORK
	${PROJECT_SOURCE_DIR}/src/network/*.h
	${PROJECT_SOURCE_DIR}/src/network/*.cpp
)

file(GLOB SRC_PHYSICS
	${PROJECT_SOURCE_DIR}/src/physics/*.h
	${PROJECT_SOURCE_DIR}/src/physics/*.cpp
)

file(GLOB SRC_ECS_FRAMEWORK
	${PROJECT_SOURCE_DIR}/src/ecs/framework/*.h
	${PROJECT_SOURCE_DIR}/src/ecs/framework/*.cpp
)

file(GLOB SRC_ECS_COMPONENTS
	${PROJECT_SOURCE_DIR}/src/ecs/components/*.h
	${PROJECT_SOURCE_DIR}/src/ecs/components/*.cpp
)

file(GLOB SRC_ECS_SYSTEMS
	${PROJECT_SOURCE_DIR}/src/ecs/systems/*.h
	${PROJECT_SOURCE_DIR}/src/ecs/systems/*.cpp
)

file(GLOB SRC_UTILITY
	${PROJECT_SOURCE_DIR}/src/utility/*.h
	${PROJECT_SOURCE_DIR}/src/utility/*.cpp
)

file( GLOB SRC 
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(SAS_3D
	${SRC} 
	${SRC_CORE}
	${SRC_ASSETS}
	${SRC_RENDER_ENGINE}
	${SRC_SHADERS}
	${SRC_INPUT_MODULES}
	${SRC_ENTITIES}
	${SRC_GSM}
	${SRC_NETWORK}
	${SRC_PHYSICS}
	${SRC_ECS_FRAMEWORK}
	${SRC_ECS_SYSTEMS}
	${SRC_ECS_COMPONENTS}
	${SRC_UTILITY}
	${SRC_IMGUI}
	${SRC_THIRD_PARTY}
)

source_group("assets"									FILES ${SRC_ASSETS})
source_group("core"										FILES ${SRC_CORE})
source_group("render_engine"							FILES ${SRC_RENDER_ENGINE})
source_group("render_engine\\shaders"					FILES ${SRC_SHADERS})
source_group("render_engine\\shaders\\input_modules"	FILES ${SRC_INPUT_MODULES})
source_group("entities"									FILES ${SRC_ENTITIES})
source_group("game_state_machine"						FILES ${SRC_GSM})
source_group("network"									FILES ${SRC_NETWORK})
source_group("physics"									FILES ${SRC_PHYSICS})
source_group("ecs\\framework"							FILES ${SRC_ECS_FRAMEWORK})
source_group("ecs\\systems"								FILES ${SRC_ECS_SYSTEMS})
source_group("ecs\\components"							FILES ${SRC_ECS_COMPONENTS})
source_group("utility"									FILES ${SRC_UTILITY})
source_group("third_party"								FILES ${SRC_THIRD_PARTY})
source_group("third_party\\imgui"						FILES ${SRC_IMGUI})

target_link_libraries (SAS_3D LINK_PUBLIC ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY_DEBUG} ${FREEIMAGE_LIBRARY_DEBUG})

target_compile_definitions(SAS_3D
	PRIVATE ASIO_STANDALONE
	PRIVATE ASIO_HAS_MOVE
	PRIVATE ASIO_HAS_STD_SYSTEM_ERROR
)
