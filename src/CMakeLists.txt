cmake_minimum_required(VERSION 3.10.0)

set(EXEC_TARGET_NAME Executable)
set(HK_BINDER_TARGET_NAME TermKeeper)

set(EXEC_SOURCES 
	Executable.cpp
	cli/cli.cpp
	cli/core/consolem.cpp
	cli/core/event.cpp
	cli/core/eventm.cpp
	cli/core/form.cpp
	cli/core/inputm.cpp
	cli/core/interface.cpp
	cli/core/screen.cpp
	cli/core/utils.cpp
	cli/core/window.cpp
	cli/forms/hintsForm.cpp
	cli/forms/inputForm.cpp
	cli/forms/selectionListForm.cpp
	cli/windows/borderedWindow.cpp
	cli/windows/cacheWindow.cpp
	cli/windows/menuWindow.cpp
	cli/windows/storageWindow.cpp
	clipboardController/windowsClipboardController.cpp
	config/config.cpp
	config/configData.cpp
	storage/cache.cpp
	storage/storage.cpp
	storage/xmlCacheManager.cpp
	storage/xmlStorageManager.cpp
)
	
add_executable(${EXEC_TARGET_NAME} ${EXEC_SOURCES} tk.rc)

target_include_directories(${EXEC_TARGET_NAME}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EXEC_TARGET_NAME} 
	pugixml::pugixml
	yaml-cpp)

set(HK_BINDER_SOURCES 
	HotKeyBinder.cpp
	config/config.cpp
	config/configData.cpp
	utils/winHotKeyParser.cpp
)
	
add_executable(${HK_BINDER_TARGET_NAME} ${HK_BINDER_SOURCES} tk.rc)

target_include_directories(${HK_BINDER_TARGET_NAME}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${HK_BINDER_TARGET_NAME} 
	yaml-cpp)
	
set(STORAGE_FILE ${CMAKE_SOURCE_DIR}/storage/storage.xml)
set(CACHE_FILE ${CMAKE_SOURCE_DIR}/storage/cache.xml)
set(CONFIG_FILE ${CMAKE_SOURCE_DIR}/config/settings.yaml)

install(CODE "
        file(COPY ${STORAGE_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(COPY ${CACHE_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(COPY ${CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(INSTALL \"${CMAKE_BINARY_DIR}/src/${EXEC_TARGET_NAME}.exe\" DESTINATION ${CMAKE_INSTALL_DIR})
		file(INSTALL \"${CMAKE_BINARY_DIR}/src/${HK_BINDER_TARGET_NAME}.exe\" DESTINATION ${CMAKE_INSTALL_DIR})
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