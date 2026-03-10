vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO https://github.com/cloudofoz/raylib-gizmo.git
    HEAD_REF cmake-support
    SHA512 0
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
)

vcpkg_cmake_install()