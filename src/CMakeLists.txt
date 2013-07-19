FILE(GLOB
    CELLO_SRC
        ${PROJECT_SOURCE_DIR}/src/Array.c
        ${PROJECT_SOURCE_DIR}/src/Bool.c
        ${PROJECT_SOURCE_DIR}/src/Char.c
        ${PROJECT_SOURCE_DIR}/src/Dictionary.c
        ${PROJECT_SOURCE_DIR}/src/Exception.c
        ${PROJECT_SOURCE_DIR}/src/File.c
        ${PROJECT_SOURCE_DIR}/src/Format.c
        ${PROJECT_SOURCE_DIR}/src/Function.c
        ${PROJECT_SOURCE_DIR}/src/Lambda.c
        ${PROJECT_SOURCE_DIR}/src/List.c
        ${PROJECT_SOURCE_DIR}/src/Map.c
        ${PROJECT_SOURCE_DIR}/src/Number.c
        ${PROJECT_SOURCE_DIR}/src/Pool.c
        ${PROJECT_SOURCE_DIR}/src/Prelude.c
        ${PROJECT_SOURCE_DIR}/src/Reference.c
        ${PROJECT_SOURCE_DIR}/src/String.c
        ${PROJECT_SOURCE_DIR}/src/Table.c
        ${PROJECT_SOURCE_DIR}/src/Tree.c
        ${PROJECT_SOURCE_DIR}/src/Type.c
    )

FILE(GLOB
    CELLO_HEADERS
    ${PROJECT_SOURCE_DIR}/include/Cello/Array.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Bool.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Char.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Dictionary.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Exception.h
    ${PROJECT_SOURCE_DIR}/include/Cello/File.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Format.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Function.h
    ${PROJECT_SOURCE_DIR}/include/FunctionAutoCello/.h 
    ${PROJECT_SOURCE_DIR}/include/Cello/Lambda.h
    ${PROJECT_SOURCE_DIR}/include/Cello/LambdaCurry.h
    ${PROJECT_SOURCE_DIR}/include/Cello/List.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Map.h
    ${PROJECT_SOURCE_DIR}/include/Cello/None.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Number.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Pool.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Prelude.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Reference.h
    ${PROJECT_SOURCE_DIR}/include/Cello/String.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Table.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Tree.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Type.h
    ${PROJECT_SOURCE_DIR}/include/Cello/Value.h
    )

IF(BUILD_SHARED_LIBS)
    ADD_LIBRARY(cello SHARED ${CELLO_SRC})
    SET_TARGET_PROPERTIES(cello PROPERTIES VERSION ${CELLO_VERSION_STRING})
    TARGET_LINK_LIBRARIES(cello m)
    INSTALL(TARGETS cello DESTINATION lib)
    
    SET(CELLO_LIBRARY_TARGET cello)
ENDIF(BUILD_SHARED_LIBS)
IF(BUILD_STATIC_LIBS)
    ADD_LIBRARY(cello-static STATIC ${CELLO_SRC})
    SET_TARGET_PROPERTIES(cello-static PROPERTIES VERSION ${CELLO_VERSION_STRING})
    TARGET_LINK_LIBRARIES(cello-static m)
    INSTALL(TARGETS cello-static DESTINATION lib)
    
    SET(CELLO_LIBRARY_TARGET cello-static)
ENDIF(BUILD_STATIC_LIBS)

SET(CELLO_LIBRARY_TARGET ${CELLO_LIBRARY_TARGET} PARENT_SCOPE)

INSTALL(FILES ${PROJECT_SOURCE_DIR}/include/Cello.h DESTINATION include)
INSTALL(FILES ${CELLO_HEADERS} DESTINATION include/Cello)

    
