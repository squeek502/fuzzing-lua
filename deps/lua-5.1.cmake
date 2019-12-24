SET(LUA_DIR ${CMAKE_CURRENT_LIST_DIR}/lua-5.1/src CACHE PATH "Lua source file dir")

IF(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

INCLUDE(CheckLibraryExists)
check_library_exists(m sin "" LUA_USE_LIBM)
if ( LUA_USE_LIBM )
  list ( APPEND LIBS m )
endif ()

set ( SRC_CORE ${LUA_DIR}/lapi.c ${LUA_DIR}/lcode.c ${LUA_DIR}/ldebug.c ${LUA_DIR}/ldo.c ${LUA_DIR}/ldump.c ${LUA_DIR}/lfunc.c ${LUA_DIR}/lgc.c ${LUA_DIR}/llex.c
  ${LUA_DIR}/lmem.c ${LUA_DIR}/lobject.c ${LUA_DIR}/lopcodes.c ${LUA_DIR}/lparser.c ${LUA_DIR}/lstate.c ${LUA_DIR}/lstring.c ${LUA_DIR}/ltable.c
  ${LUA_DIR}/ltm.c ${LUA_DIR}/lundump.c ${LUA_DIR}/lvm.c ${LUA_DIR}/lzio.c )
set ( SRC_LIB ${LUA_DIR}/lauxlib.c ${LUA_DIR}/lbaselib.c ${LUA_DIR}/ldblib.c ${LUA_DIR}/liolib.c
  ${LUA_DIR}/lmathlib.c ${LUA_DIR}/loslib.c ${LUA_DIR}/lstrlib.c ${LUA_DIR}/ltablib.c ${LUA_DIR}/loadlib.c ${LUA_DIR}/linit.c )

add_library ( liblua STATIC ${SRC_CORE} ${SRC_LIB} )
set_target_properties ( liblua PROPERTIES OUTPUT_NAME lua51 )
target_link_libraries ( liblua INTERFACE ${LIBS} )
target_include_directories ( liblua PUBLIC ${LUA_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
