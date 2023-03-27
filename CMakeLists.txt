cmake_minimum_required(VERSION 3.10)
project(QFaceDetect LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# set(QT6_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.4.3")
set(QT5_PREFIX_PATH "/opt/homebrew/Cellar/qt@5/5.15.8_2")
set(OPENCV4_PREFIX_PATH "/opt/homebrew/Cellar/opencv/4.7.0_2")
set(
    CMAKE_PREFIX_PATH 
    ${QT5_PREFIX_PATH} 
    ${OPENCV4_PREFIX_PATH} 
    )

get_filename_component(PROJECT_BASE_DIR ./ ABSOLUTE)
add_definitions(-DSHADER_DIR="${PROJECT_BASE_DIR}/shaders/")
add_definitions(-DMODEL_DIR="${PROJECT_BASE_DIR}/model/")

set(QT5_REQUIRED_PACKAGES OpenGL Gui Multimedia Core Widgets)
# set(QT6_REQUIRED_PACKAGES OpenGLWidgets OpenGL Gui )
find_package(Qt5 REQUIRED COMPONENTS ${QT5_REQUIRED_PACKAGES})
find_package(OpenCV REQUIRED ${OpenCV_Libs})

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

find_path(AVFILTER_INCLUDE_DIR libavfilter/avfilter.h)
find_library(AVFILTER_LIBRARY avfilter)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

message(${AVCODEC_INCLUDE_DIR})
message(${AVFORMAT_INCLUDE_DIR})
message(${AVUTIL_INCLUDE_DIR})
message(${AVDEVICE_INCLUDE_DIR})
message(${AVFILTER_INCLUDE_DIR})
message(${SWSCALE_INCLUDE_DIR})

set(FFMPEG_INC_DIRS ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR} ${AVFILTER_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR})
set(FFMPEG_LINK_LIBS ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${AVDEVICE_LIBRARY} ${AVFILTER_LIBRARY} ${SWSCALE_LIBRARY})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(
    CPP_SOURCE_FILES
    main.cpp
    videowidget.cpp
    videosurface.cpp
    camerawidget.cpp
    facedetector.cpp
    streamer.cpp
)

add_executable(${PROJECT_NAME} ${CPP_SOURCE_FILES})

set(QT5_LINK_LIBS Qt5::Multimedia Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Gui)
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${OpenCV_INCLUDE_DIRS}
    ${FFMPEG_INC_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC 
    ${QT5_LINK_LIBS} 
    ${OpenCV_LIBS}
    ${FFMPEG_LINK_LIBS}
    )