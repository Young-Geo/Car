# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "node_ultrasonic: 1 messages, 0 services")

set(MSG_I_FLAGS "-Inode_ultrasonic:/home/pi/Car/src/node_ultrasonic/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(node_ultrasonic_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg" NAME_WE)
add_custom_target(_node_ultrasonic_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "node_ultrasonic" "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(node_ultrasonic
  "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_ultrasonic
)

### Generating Services

### Generating Module File
_generate_module_cpp(node_ultrasonic
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_ultrasonic
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(node_ultrasonic_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(node_ultrasonic_generate_messages node_ultrasonic_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg" NAME_WE)
add_dependencies(node_ultrasonic_generate_messages_cpp _node_ultrasonic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_ultrasonic_gencpp)
add_dependencies(node_ultrasonic_gencpp node_ultrasonic_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_ultrasonic_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(node_ultrasonic
  "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_ultrasonic
)

### Generating Services

### Generating Module File
_generate_module_lisp(node_ultrasonic
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_ultrasonic
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(node_ultrasonic_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(node_ultrasonic_generate_messages node_ultrasonic_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg" NAME_WE)
add_dependencies(node_ultrasonic_generate_messages_lisp _node_ultrasonic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_ultrasonic_genlisp)
add_dependencies(node_ultrasonic_genlisp node_ultrasonic_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_ultrasonic_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(node_ultrasonic
  "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_ultrasonic
)

### Generating Services

### Generating Module File
_generate_module_py(node_ultrasonic
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_ultrasonic
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(node_ultrasonic_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(node_ultrasonic_generate_messages node_ultrasonic_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/Car/src/node_ultrasonic/msg/ultr.msg" NAME_WE)
add_dependencies(node_ultrasonic_generate_messages_py _node_ultrasonic_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_ultrasonic_genpy)
add_dependencies(node_ultrasonic_genpy node_ultrasonic_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_ultrasonic_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_ultrasonic)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_ultrasonic
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(node_ultrasonic_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_ultrasonic)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_ultrasonic
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(node_ultrasonic_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_ultrasonic)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_ultrasonic\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_ultrasonic
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(node_ultrasonic_generate_messages_py std_msgs_generate_messages_py)
endif()
