# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/plucca/esp/esp-idf/components/bootloader/subproject"
  "/home/plucca/esp/sample_project/build/bootloader"
  "/home/plucca/esp/sample_project/build/bootloader-prefix"
  "/home/plucca/esp/sample_project/build/bootloader-prefix/tmp"
  "/home/plucca/esp/sample_project/build/bootloader-prefix/src/bootloader-stamp"
  "/home/plucca/esp/sample_project/build/bootloader-prefix/src"
  "/home/plucca/esp/sample_project/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/plucca/esp/sample_project/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/plucca/esp/sample_project/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
