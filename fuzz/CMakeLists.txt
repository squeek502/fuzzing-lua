if (ENABLE_FUZZING)
  enable_language(CXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

  add_custom_target(fuzz_check_all)

  if (FUZZ_ENTRY)
    add_definitions(-DFUZZ_ENTRY=${FUZZ_ENTRY})
  endif()

  function(add_fuzz name dict corpus)
    if (COVERAGE)
      add_executable(${name} "${name}.cc" StandaloneFuzzTargetMain.c)
      target_link_libraries(${name} liblua ${COVERAGE_LINKER_FLAGS})
      add_custom_target(${name}_cov
        COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${name}" "${corpus}/*"
        DEPENDS ${name}
      )
    else()
      add_executable(${name} "${name}.cc")
      target_link_libraries(${name} liblua luafuzzhelper pthread ${FUZZ_ENGINE})
      add_custom_target(${name}_run COMMAND ${CMAKE_COMMAND} -E env "ASAN_OPTIONS=allocator_may_return_null=1" "${CMAKE_CURRENT_BINARY_DIR}/${name}" -timeout=2 -dict="${dict}" "${corpus}" DEPENDS ${name})
      set(${name}_new_corpus "${CMAKE_CURRENT_BINARY_DIR}/corpus/${name}")
      add_custom_target(${name}_minimize
        COMMAND mkdir -p "${${name}_new_corpus}"
        COMMAND ${name} -timeout=2 "${${name}_new_corpus}" "${corpus}" -merge=1
        COMMAND rm -rf "${corpus}"
        COMMAND mv "${${name}_new_corpus}" "${corpus}"
      )
      # for checking that the corpus still runs without exploding
      add_custom_target(${name}_check COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${name}" -timeout=2 "${corpus}/*" DEPENDS ${name})
      add_dependencies(fuzz_check_all ${name}_check)
    endif()
  endfunction()

  function(add_fuzz_simple name dict)
    add_fuzz(${name} "${CMAKE_CURRENT_SOURCE_DIR}/dict/${dict}" "${CMAKE_CURRENT_SOURCE_DIR}/corpus/${name}")
  endfunction()

  add_fuzz_simple(fuzz_llex lua.dict)
endif()
