set(libsocket_INCLUDE_DIRS "/home/fona/i2ROS/autonomous_quadruped/build/simulation/libsocket-prefix/src/libsocket/headers")

set(libsocket_BINARY_DIR "/home/fona/i2ROS/autonomous_quadruped/build/simulation/libsocket-prefix/src/libsocket-build")

include(${libsocket_BINARY_DIR}/libsocketTargets.cmake)

set(libsocket_LIBRARIES socket++)
