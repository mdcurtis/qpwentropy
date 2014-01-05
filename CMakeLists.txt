
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo Debug Debugfull Profile MinSizeRel."
      FORCE)
endif()

project( qpwentropy )

cmake_minimum_required(VERSION 2.6.4)

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)


set(QT_REQUIRED_MODULES QtCore QtGui QtTest  )

find_package(Qt4 4.6.0 REQUIRED ${QT_REQUIRED_MODULES})
include(${QT_USE_FILE})

set( qpwentropy_SOURCES 
  SubstitutionCipher.cpp
  RankedDictionary.cpp
  DictionaryMatcher.cpp
  SubstitutionMatcher.cpp
  Range.cpp
  Match.cpp
  main.cpp
)

add_executable( qpwentropy ${qpwentropy_SOURCES} )
target_link_libraries( qpwentropy ${QT_QTCORE_LIBRARIES} )