add_executable(fuzz_llex_output fuzz_llex_output.c)
target_link_libraries(fuzz_llex_output liblua luafuzzhelper)
add_custom_target(fuzz_llex_output_run
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fuzz_llex_output" "${CMAKE_CURRENT_SOURCE_DIR}/../fuzz/corpus/fuzz_llex" "${CMAKE_CURRENT_BINARY_DIR}/output/fuzz_llex"
  DEPENDS fuzz_llex_output
)
