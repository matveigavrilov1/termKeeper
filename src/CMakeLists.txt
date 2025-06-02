cmake_minimum_required(VERSION 3.10.0)

set(EXEC_TARGET_NAME ${PROJECT_NAME})

set(EXEC_SOURCES 
	main.cpp
	ui/ui.cpp
	core/controller.cpp
	core/controllerm.cpp
	core/event.cpp
	core/eventm.cpp
	core/form.cpp
	core/interface.cpp
	core/screen.cpp
	core/utils.cpp
	core/window.cpp
	forms/hintsForm.cpp
	forms/inputForm.cpp
	forms/selectionListForm.cpp
	windows/utils/borderedWindow.cpp
	windows/utils/controllerWindow.cpp
	windows/cacheWindow.cpp
	windows/menuWindow.cpp
	windows/storageWindow.cpp
	config/config.cpp
	config/configData.cpp
	os/windows/console.cpp
	os/windows/interface.cpp
	data/cache.cpp
	data/storage.cpp
	data/xmlCacheManager.cpp
	data/xmlStorageManager.cpp
	utils/generate_uuid.cpp
)
	
add_executable(${EXEC_TARGET_NAME} ${EXEC_SOURCES} tk.rc)

target_include_directories(${EXEC_TARGET_NAME}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXEC_TARGET_NAME} 
	pugixml::pugixml
	yaml-cpp
	stduuid::stduuid
)


set(STORAGE_FILE ${CMAKE_SOURCE_DIR}/storage/storage.xml)
set(CACHE_FILE ${CMAKE_SOURCE_DIR}/storage/cache.xml)
set(CONFIG_FILE ${CMAKE_SOURCE_DIR}/config/settings.yaml)

install(CODE "
        file(COPY ${STORAGE_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(COPY ${CACHE_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(COPY ${CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(INSTALL \"${CMAKE_BINARY_DIR}/src/${EXEC_TARGET_NAME}.exe\" DESTINATION ${CMAKE_INSTALL_DIR})
")

set(MINGW_PATH "/usr/lib/gcc/x86_64-w64-mingw32/13-win32")

set(MINGW_LIBS
    libstdc++-6.dll
    libgcc_s_seh-1.dll
)

install(CODE "
    foreach(LIB ${MINGW_LIBS})
        file(COPY ${MINGW_PATH}/\${LIB} DESTINATION ${CMAKE_INSTALL_DIR})
    endforeach()
")