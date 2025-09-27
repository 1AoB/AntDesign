cmake_minimum_required(VERSION 3.16)
project(QtAntDesign VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁止 Windows 控制台弹出
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

# -------------------------------
# 启用解决方案文件夹 (VS/IDE 分组)
# -------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Qt 自动处理 MOC/RCC，但 UI 手动处理
set(CMAKE_AUTOUIC OFF)   # 关闭自动 UI
set(CMAKE_AUTOMOC ON)    # MOC 仍自动
set(CMAKE_AUTORCC ON)    # RCC 仍自动

# 源代码目录
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign")

# Qt 模块
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
    OpenGL
    Svg
)



# -------------------------------
# 文件分组
# -------------------------------
file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")
file(GLOB HEADER_FILES "${SRC_DIR}/*.h" "${SRC_DIR}/*.hpp")
file(GLOB FORM_FILES "${SRC_DIR}/*.ui")
file(GLOB RESOURCE_FILES "${SRC_DIR}/*.qrc")
file(GLOB_RECURSE THIRD_PARTY_SRC "${SRC_DIR}/ThirdParty/*.cpp")
file(GLOB_RECURSE THIRD_PARTY_HEADERS "${SRC_DIR}/ThirdParty/*.h" "${SRC_DIR}/ThirdParty/*.hpp")

# -------------------------------
# 手动生成 UI 文件对应的 ui_*.h
# -------------------------------
Qt5_wrap_ui(UI_HEADERS ${FORM_FILES})

# -------------------------------
# 添加可执行文件
# -------------------------------
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_HEADERS}        # 手动生成的 ui_*.h
    ${RESOURCE_FILES}
    ${THIRD_PARTY_SRC}
    ${THIRD_PARTY_HEADERS}
)

# 将主项目放到单独的解决方案文件夹
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "QtAntDesign Project")

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::OpenGL
    Qt5::Svg
)

# 根据平台判断是否需要链接 Dwmapi
if(WIN32)  # Windows 平台
    target_link_libraries(${PROJECT_NAME} PRIVATE Dwmapi)
elseif(UNIX)  # Linux 或其他 UNIX-like 系统
    # 如果需要在 Linux 上链接其他库，可以在这里添加
endif()

# 设置 include 目录，让 VS IntelliSense 能找到 ui_*.h
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"                               # ui_*.h
    "${SRC_DIR}/ThirdParty/QR-Code-generator-master"            # qrcodegen.hpp
    "${SRC_DIR}/ThirdParty/FastGaussianBlur-main"               # 其他第三方库
)


# -------------------------------
# IDE 分组 (Visual Studio / Qt Creator)
# -------------------------------
source_group("Form Files" FILES ${FORM_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SRC_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})
source_group("ThirdParty" FILES ${THIRD_PARTY_SRC} ${THIRD_PARTY_HEADERS})
source_group("UI Generated Files" FILES ${UI_HEADERS})
