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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
link_directories(/opt/homebrew/Cellar/opencv/4.7.0_2/lib)
set(
    CPP_SOURCE_FILES
    main.cpp
    videowidget.cpp
    videosurface.cpp
    camerawidget.cpp
    facedetector.cpp
)

add_executable(${PROJECT_NAME} ${CPP_SOURCE_FILES})

set(QT5_LINK_LIBS Qt5::Multimedia Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Gui)
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC 
    ${QT5_LINK_LIBS} 
    ${OpenCV_LIBS}
    )