vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO assimp/assimp
    REF v6.0.4
    SHA512 f3639e3964ea8ef41ce684eb1b764ece79f64a15ecae068846c5bc0853780e39f600776027d8843e6a3f47988daf067a164161a58f76ec6de13027ae1e473bfb
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DASSIMP_BUILD_DRACO=OFF
        -DASSIMP_BUILD_TESTS=OFF
        -DBUILD_SHARED_LIBS=ON
)

vcpkg_cmake_install()
