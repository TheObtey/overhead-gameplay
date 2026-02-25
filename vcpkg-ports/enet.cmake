vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO https://github.com/zpl-c/enet
    REF v1.0.0
    SHA512 0
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
)

vcpkg_cmake_install()
