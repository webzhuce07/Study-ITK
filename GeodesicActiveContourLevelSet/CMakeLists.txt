#**********************************************************************
#
#  Copyright (c) Mr.Bin. All rights reserved.
#  For more information visit: http://blog.csdn.net/webzhuce 
#
#**********************************************************************
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(GeodesicActiveContourLevelSet)
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
ADD_EXECUTABLE(GeodesicActiveContourLevelSet main.cpp)
TARGET_LINK_LIBRARIES(GeodesicActiveContourLevelSet ${ITK_LIBRARIES} ${VTK_LIBRARIES})


