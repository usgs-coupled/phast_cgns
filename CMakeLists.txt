cmake_minimum_required(VERSION 3.12)

project(cgns C CXX)

include(GNUInstallDirs)

# # set CMAKE_PREFIX_PATH to the directory containing iriclibConfig.cmake
# # ie -DCMAKE_PREFIX_PATH=d:/Downloads/iriclib-artifacts/iricsdk/iriclib-4.0.0/lib/cmake/iriclib
# find_package(iriclib 4 REQUIRED)

# # Change this to ON if solver requires OpenMP
# option(IRIC_SOLVER_USE_OPENMP "Solver requires OpenMP" OFF)
# if(IRIC_SOLVER_USE_OPENMP)
#   find_package(OpenMP REQUIRED)
# endif()

#set(CMAKE_PREFIX_PATH "C:/Users/charlton/source/repos/coupled/iricdev-2019/lib/install/cgnslib-4.1.1-patch1/release;C:/Users/charlton/source/repos/coupled/iricdev-2019/lib/install/hdf5-1.8.21/cmake/hdf5")
set(CMAKE_PREFIX_PATH "C:/Users/charlton/source/repos/coupled/iricdev-2019/lib/install/hdf5-1.8.21/cmake/hdf5;C:/Users/charlton/source/repos/coupled/iricdev-2019/lib/install/cgnslib-4.1.1-patch1/release")

find_package(HDF5 1.8 REQUIRED)

find_path(CGNS_INC cgnslib.h)
include_directories("${CGNS_INC}")

find_library(CGNS_LIBRARY cgnsdll)

add_executable(write_c
  write_grid_str.c
)

add_executable(ex4_cgns
  write_ex4.cpp
)

add_executable(ex4_sol_cgns
  write_ex4_solution.cpp
)

configure_file(ex4.h5 ex4.h5 COPYONLY)

add_executable(ex4_hdf5
  PhastH5File.cpp
  PhastH5File.h
  read_ex4_hdf5.cpp
)


# # default real kind 8
# if(CMAKE_Fortran_COMPILER_ID STREQUAL Intel)
#   if(WIN32)
#     set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} /real-size:64")
#   else()
#     set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -real-size 64")
#   endif()
# elseif(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
#   set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fdefault-real-8")
# endif()


target_compile_definitions(write_c PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(write_c PRIVATE ${CGNS_INC})
target_link_libraries(write_c PRIVATE ${CGNS_LIBRARY})

target_compile_definitions(ex4_cgns PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(ex4_cgns PRIVATE ${CGNS_INC})
target_link_libraries(ex4_cgns PRIVATE ${CGNS_LIBRARY})

target_compile_definitions(ex4_sol_cgns PRIVATE _CRT_SECURE_NO_WARNINGS)
target_include_directories(ex4_sol_cgns PRIVATE ${CGNS_INC})
target_link_libraries(ex4_sol_cgns PRIVATE ${CGNS_LIBRARY})

target_compile_definitions(ex4_hdf5 PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(ex4_hdf5 PRIVATE hdf5::hdf5_cpp-shared)

# if(IRIC_SOLVER_USE_OPENMP AND OpenMP_Fortran_FOUND)
#   target_link_libraries(Fastmech PRIVATE OpenMP::OpenMP_Fortran)
# endif()

# set stack on windows
# if(WIN32)
#   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /HEAP:100000000,100000000 /STACK:1000000000,1000000000")
# #else()
# #  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fmax-stack-var-size=100000000")
# endif()

install(TARGETS write_c DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(TARGETS ex4_cgns DESTINATION "${CMAKE_INSTALL_BINDIR}")

# include(CTest)

# add_subdirectory(tests)

# POST_BUILD
if (WIN32)
  # ie "-DCMAKE_PREFIX_PATH=d:/Downloads/iriclib-artifacts/iricsdk-4.0.9/iriclib-4.0.9/lib/cmake/iriclib;d:/Downloads/iriclib-artifacts/iricsdk-4.0.9/hdf5-1.8.21/cmake/hdf5;d:/Downloads/iriclib-artifacts/iricsdk-4.0.9/poco-1.9.4/lib/cmake/Poco"
  # copy reqd dlls to test1 directory
  add_custom_command(TARGET write_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CGNS_INC}/../bin/cgnsdll.dll"   "$<TARGET_FILE_DIR:write_c>"
    )

  add_custom_command(TARGET ex4_hdf5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:hdf5::hdf5-shared>"       "$<TARGET_FILE_DIR:ex4_hdf5>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:hdf5::hdf5_cpp-shared>"   "$<TARGET_FILE_DIR:ex4_hdf5>"
    )

endif()
