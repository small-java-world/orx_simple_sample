cmake_minimum_required(VERSION 3.16)
project(MyOrxTest C)

set(ORX_ROOT "D:/Orx/orx/code")  # Orx SDK のパス (code フォルダまでか、その親か環境による)

include_directories(${ORX_ROOT}/include)
# link_directories(${ORX_ROOT}/lib/dynamic) # find_library を使うのでこちらはコメントアウトまたは削除しても良い

# Orx ライブラリの検索
find_library(ORX_LIBRARY_DEBUG NAMES orxd PATHS ${ORX_ROOT}/lib/dynamic NO_DEFAULT_PATH)
find_library(ORX_LIBRARY_RELEASE NAMES orx PATHS ${ORX_ROOT}/lib/dynamic NO_DEFAULT_PATH)

if(NOT ORX_LIBRARY_DEBUG)
    message(FATAL_ERROR "Debug Orx library (orxd) not found!")
endif()
message(STATUS "Found Debug Orx Library: ${ORX_LIBRARY_DEBUG}")

if(NOT ORX_LIBRARY_RELEASE)
    message(FATAL_ERROR "Release Orx library (orx) not found!")
endif()
message(STATUS "Found Release Orx Library: ${ORX_LIBRARY_RELEASE}")

add_executable(MyOrxTest src/main.c)

# Debug 用マクロを明示
target_compile_definitions(MyOrxTest PRIVATE
    $<$<CONFIG:Debug>:__orxDEBUG__>
)

# Debug ⇔ Release でリンクライブラリ切替
target_link_libraries(MyOrxTest
    $<$<CONFIG:Debug>:${ORX_LIBRARY_DEBUG}>
    $<$<CONFIG:Release>:${ORX_LIBRARY_RELEASE}>
)
