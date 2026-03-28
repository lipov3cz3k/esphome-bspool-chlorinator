cmake_minimum_required(VERSION 3.14)
project(bs_pool_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)
include_directories(${googletest_SOURCE_DIR}/googletest/include)

enable_testing()

# ESPHome stubs first, then repository root for component includes.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stubs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)

add_executable(bs_pool_tests
  main.cpp
  smoke_test.cpp
  data_decoding_test.cpp
  handle_message_test.cpp
  hub_test.cpp
  ../../components/bs_pool/bs_pool.cpp
  ../../components/bs_pool/sensor/bs_pool_sensor.cpp
  ../../components/bs_pool/binary_sensor/bs_pool_binary_sensor.cpp
  ../../components/bs_pool/switch/bs_pool_switch.cpp
  ../../components/bs_pool/switch/user_settings.cpp
  ../../components/bs_pool/select/bs_pool_select.cpp
  ../../components/bs_pool/select/control_mode_select.cpp
  ../../components/bs_pool/text_sensor/bs_pool_text_sensor.cpp
)

target_link_libraries(bs_pool_tests
  GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(bs_pool_tests)

add_executable(pty_harness
  ../integration/pty_harness.cpp
  ../../components/bs_pool/bs_pool.cpp
  ../../components/bs_pool/sensor/bs_pool_sensor.cpp
)
target_include_directories(pty_harness PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/stubs
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
)
