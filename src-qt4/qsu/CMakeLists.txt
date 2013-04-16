project(qsu)

cmake_minimum_required(VERSION 2.4.3)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# warnings
add_definitions( -W -Wall )

# dependencies
find_library(PAM_LIBRARY pam)
find_package(X11)
find_package(Qt4 COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})

# configuration
set(QSU_AUTH_TIMEOUT "180" CACHE STRING "Session timeout in seconds (default: 3M)")
set(QSU_DATABASE_PATH "/var/db/qsu" CACHE STRING "Session timestamp location (prefix prepended)")
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

# sources
include_directories(${PROJECT_BINARY_DIR})
include_directories(${X11_X11_INCLUDE_PATH})
file(GLOB QSU_HDRS "src/*.h")
file(GLOB QSU_SRCS "src/*.c" "src/*.cpp")

# qt4
QT4_ADD_RESOURCES(QSU_RC_SRCS "resources/qsu.qrc")
QT4_WRAP_CPP(QSU_MOC_SRCS "src/passwordpromptdialog.h")
QT4_WRAP_UI(QSU_UI_HDRS "src/passwordpromptdialog.ui")

# target
add_executable(qsu ${QSU_SRCS} ${QSU_HDRS} ${QSU_RC_SRCS} ${QSU_MOC_SRCS} ${QSU_UI_HDRS})
target_link_libraries(qsu ${PAM_LIBRARY} ${X11_X11_LIB} ${QT_LIBRARIES})

# install
install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/qsu"
        DESTINATION bin
        PERMISSIONS SETUID OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}${QSU_DATABASE_PATH})")
