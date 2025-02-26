cmake_minimum_required(VERSION 3.10.0)

set(MVP_TARGET_NAME mvp)
set(TARGET_NAME termKeeper)

add_executable(${MVP_TARGET_NAME} mvp.cpp)

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
	main.
	logger.cpp
	model/model.cpp
	os/clipboardController/windowsClipboardController.cpp)
	
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