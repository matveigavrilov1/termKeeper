cmake_minimum_required(VERSION 3.10.0)

set(TARGET_NAME termKeeper)

set(SOURCES 
	main.cpp
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
	
add_executable(${TARGET_NAME} ${SOURCES} tk.rc)

target_include_directories(${TARGET_NAME}
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} 
	pugixml::pugixml
	yaml-cpp)

install(CODE "
    foreach(TARGET ${TARGET_NAME})
        set(TARGET_PATH \"${CMAKE_BINARY_DIR}/src/\${TARGET}.exe\")
        if(EXISTS \"\${TARGET_PATH}\")
            file(INSTALL \${TARGET_PATH} DESTINATION ${CMAKE_INSTALL_DIR})
        else()
            message(WARNING \"File \${TARGET_PATH} not found!\")
        endif()
    endforeach()
")

set(STORAGE_FILE ${CMAKE_SOURCE_DIR}/storage/storage.xml)
set(CACHE_FILE ${CMAKE_SOURCE_DIR}/storage/cache.xml)
set(CONFIG_FILE ${CMAKE_SOURCE_DIR}/config/settings.yaml)

install(CODE "
        file(COPY ${STORAGE_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(COPY ${CACHE_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
		file(COPY ${CONFIG_FILE} DESTINATION ${CMAKE_INSTALL_DIR})
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