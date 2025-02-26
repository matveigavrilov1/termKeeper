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

add_executable(${TARGET_NAME} main.cpp)
