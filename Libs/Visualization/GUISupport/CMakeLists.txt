project(VTKGUISupportQt)

find_package(OpenGL REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL)

find_package(VTK REQUIRED COMPONENTS 
  CommonCore
  CommonDataModel
  CommonSystem
  FiltersExtraction
  InteractionStyle
  RenderingCore
  RenderingOpenGL2
  InteractionWidgets
)

set(CMAKE_AUTOMOC ON)

set(GUISupportQt_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB GUISupportQt_files
    ${GUISupportQt_DIR}/*.h
    ${GUISupportQt_DIR}/*.cxx
)

if(NOT ${CTK_BUILD_SHARED_LIBS})
    set(MY_LIBRARY_TYPE "STATIC")
endif()

if(NOT DEFINED MY_LIBRARY_TYPE)
  set(MY_LIBRARY_TYPE "SHARED")
endif()

add_library(${PROJECT_NAME} ${MY_LIBRARY_TYPE} ${GUISupportQt_files})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT ${CTK_BUILD_SHARED_LIBS})
    add_definitions(-DVTKGUISUPPORTQT_STATIC_DEFINE)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUISupportQt_EXPORTS)
endif()

set(lib_name ${PROJECT_NAME})

# Set labels associated with the target.
set_target_properties(${lib_name} PROPERTIES LABELS ${lib_name})

# Apply user-defined properties to the library target.
if(CTK_LIBRARY_PROPERTIES AND ${CTK_LIBRARY_MODE} STREQUAL "SHARED")
  set_target_properties(${lib_name} PROPERTIES ${CTK_LIBRARY_PROPERTIES})
endif()
set_target_properties(${lib_name} PROPERTIES CTK_LIB_TARGET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Library properties specific to STATIC build
if(MY_LIBRARY_TYPE STREQUAL "STATIC")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
    set_target_properties(${lib_name} PROPERTIES COMPILE_FLAGS "-fPIC")
  endif()
endif()

# Install rules
if(MY_LIBRARY_TYPE STREQUAL "SHARED")
  install(TARGETS ${lib_name} EXPORT CTKExports
    RUNTIME DESTINATION ${CTK_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${CTK_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${CTK_INSTALL_LIB_DIR} COMPONENT Development)
endif()

set(my_libs Qt5::Widgets Qt5::OpenGL ${VTK_LIBRARIES} ${OPENGL_LIBRARIES})

if(MINGW)
  list(APPEND my_libs ssp) # add stack smash protection lib
endif()
target_link_libraries(${lib_name} ${my_libs})

# Update CTK_BASE_LIBRARIES
set(CTK_BASE_LIBRARIES ${my_libs} ${lib_name} CACHE INTERNAL "CTK base libraries" FORCE)
set(CTK_LIBRARIES ${CTK_LIBRARIES} ${lib_name} CACHE INTERNAL "CTK libraries" FORCE)

# Install headers
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.tpp")
install(
  FILES ${headers}
  DESTINATION ${CTK_INSTALL_INCLUDE_DIR} COMPONENT Development
)