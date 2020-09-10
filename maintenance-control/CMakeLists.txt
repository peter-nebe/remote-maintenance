cmake_minimum_required (VERSION 3.17)
project (maintenance-control)

set (sharedSrc_DIR
  ../sharedSrc
)
set (project_SRC
  src/JobMaster.cpp
  src/MaintenanceControl.cpp
  src/sampleControlApp.cpp
  ${sharedSrc_DIR}/NetworkClient.cpp
  ${sharedSrc_DIR}/ostream_operator.cpp
)
include_directories (
  ${sharedSrc_DIR}
)
link_libraries (
  paho-mqttpp3
  paho-mqtt3as
  boost_serialization
  boost_log
  boost_filesystem
  boost_chrono
  boost_regex
  boost_atomic
  boost_thread
  pthread
)
set (CMAKE_CXX_STANDARD
  17
)
set (CMAKE_CXX_STANDARD_REQUIRED
  ON
)
add_executable (
  ${PROJECT_NAME}
  ${project_SRC}
)
