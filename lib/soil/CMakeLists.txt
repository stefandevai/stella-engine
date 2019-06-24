cmake_minimum_required (VERSION 2.6)
project (SOIL)

add_definitions (-D_CRT_SECURE_NO_WARNINGS)
include_directories (code)
add_library (soil STATIC
    code/soil.c
    code/soil.h
    code/etc1_utils.c
    code/etc1_utils.h
    code/image_DXT.c
    code/image_DXT.h
    code/image_helper.c
    code/image_helper.h
    code/pkm_helper.h
    code/pvr_helper.h
    code/stbi_DDS_c.h
    code/stbi_DDS.h
    code/stbi_ext_c.h
    code/stbi_ext.h
    code/stb_image.h
    code/stb_image_write.h
    code/stbi_pkm_c.h
    code/stbi_pkm.h
    code/stbi_pvr_c.h
    code/stbi_pvr.h)


# doc
find_package (Doxygen)

if (DOXYGEN_FOUND)

  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target (soil_doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)

endif (DOXYGEN_FOUND)
