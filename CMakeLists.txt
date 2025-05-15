cmake_minimum_required(VERSION 3.16)
project(MyOrxTest C)

set(ORX_ROOT "D:/Orx/orx/code") # Orx SDKのパスに合わせて変更

include_directories(${ORX_ROOT}/include)
link_directories(${ORX_ROOT}/lib/dynamic) # lib/dynamic に修正

add_executable(MyOrxTest src/main.c)
target_link_libraries(MyOrxTest orx)
