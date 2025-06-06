function(get_git_commit_hash OUTPUT_VAR)
	set(GIT_COMMIT_HASH "unknown")

	find_package(Git QUIET)
	if(NOT GIT_FOUND)
		message(WARNING "Git not found - cannot determine commit hash")
		set(${OUTPUT_VAR} "${GIT_COMMIT_HASH}" PARENT_SCOPE)
		return()
	endif()

	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_COMMIT_HASH
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
		RESULT_VARIABLE GIT_RESULT
	)

	if(NOT GIT_RESULT EQUAL 0)
		message(WARNING "Failed to get Git commit hash (${GIT_RESULT})")
	endif()

	set(${OUTPUT_VAR} "${GIT_COMMIT_HASH}" PARENT_SCOPE)
endfunction()