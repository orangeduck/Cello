SET(USE_VIM_CLANG_COMPLETE YES CACHE BOOL "If set, will generate a .clang_complete file used by Vim clang_complete plugin")
IF(USE_VIM_CLANG_COMPLETE)
################################################
# GENERATE VIM CLANG_COMPLETE FILE
################################################
GET_PROPERTY(PROJECT_HEADERS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests PROPERTY INCLUDE_DIRECTORIES)
    FILE(WRITE .clang_complete "")
    FOREACH(PATH ${PROJECT_HEADERS})
        FILE(APPEND .clang_complete "-I${PATH}\n")
    ENDFOREACH(PATH)
ENDIF()