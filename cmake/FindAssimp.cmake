find_path(ASSIMP_INCLUDE_DIR assimp/importer.hpp ${CMAKE_SOURCE_DIR}/dependence/include)
find_library(ASSIMP_LIBRARY assimp-vc130-mtd ${CMAKE_SOURCE_DIR}/dependence/lib)

IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
	SET(ASSIMP_FOUND TRUE)
ENDIF()