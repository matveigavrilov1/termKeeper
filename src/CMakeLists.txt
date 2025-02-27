cmake_minimum_required(VERSION 3.10.0)

set(MVP_TARGET_NAME mvp)
set(TARGET_NAME termKeeper)

add_executable(${MVP_TARGET_NAME} mvp.cpp
					cli/cliCore.cpp)

target_link_libraries(${MVP_TARGET_NAME} 
	PRIVATE
		-lgdi32
		-luser32
		-lkernel32
)

target_include_directories(${MVP_TARGET_NAME}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES 
	main.cpp
	cli/cli.cpp
	cli/cliCore.cpp
	clipboardController/windowsClipboardController.cpp
	model/model.cpp
)
	
add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${MVP_TARGET_NAME} 
	PRIVATE
		-lgdi32
		-luser32
		-lkernel32
)