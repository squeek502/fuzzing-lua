add_executable(fuzz_llex_output fuzz_llex_output.c tools_utils.c)
target_link_libraries(fuzz_llex_output liblua luafuzzhelper)
add_custom_target(fuzz_llex_output_run
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fuzz_llex_output" "${CMAKE_CURRENT_SOURCE_DIR}/../fuzz/corpus/fuzz_llex" "${CMAKE_CURRENT_BINARY_DIR}/output/fuzz_llex"
  DEPENDS fuzz_llex_output
)

add_executable(fuzz_strings_output fuzz_strings_output.c tools_utils.c)
target_link_libraries(fuzz_strings_output liblua luafuzzhelper)
add_custom_target(fuzz_strings_output_run
  COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fuzz_strings_output" "${CMAKE_CURRENT_SOURCE_DIR}/../fuzz/corpus/fuzz_strings" "${CMAKE_CURRENT_BINARY_DIR}/output/fuzz_strings"
  DEPENDS fuzz_strings_output
)

add_custom_target(package_data
  COMMAND tar -czf "${CMAKE_CURRENT_BINARY_DIR}/fuzzing-lua-data.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}" output -C "${CMAKE_CURRENT_SOURCE_DIR}/../fuzz" corpus
  DEPENDS fuzz_llex_output_run fuzz_strings_output_run
  COMMENT "Packaging data to: ${CMAKE_CURRENT_BINARY_DIR}/fuzzing-lua-data.tar.gz"
)
