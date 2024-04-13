cmake_minimum_required(VERSION 3.0)
project(server LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find the QtCore, QtNetwork and QtSql library
find_package(Qt NAMES Qt6 REQUIRED COMPONENTS Core Network Sql)
find_package(Qt6 REQUIRED COMPONENTS Core Network Sql)

set(server_SRC
    src/main.cpp
    src/server.cpp
    src/logger.cpp
    src/logoutputconsole.cpp
    src/logoutputfile.cpp
    src/databasemanager.cpp
)

# Tell CMake to create the laboratory server executable
add_executable(${PROJECT_NAME} ${server_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

