cmake_minimum_required(VERSION 3.0)
project(Client)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt NAMES Qt5 REQUIRED COMPONENTS Core Widgets Network Sql)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network Sql)
set(client_SRC
  src/main.cpp
  src/client.cpp
)

# Tell CMake to create the helloworld executable
add_executable(${PROJECT_NAME} ${client_SRC})

# Use the Widgets module from Qt 6.
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql)

# Install the executable
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
