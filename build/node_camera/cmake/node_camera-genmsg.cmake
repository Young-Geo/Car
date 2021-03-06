# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "node_camera: 1 messages, 0 services")

set(MSG_I_FLAGS "-Inode_camera:/home/pi/Car/src/node_camera/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(node_camera_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/pi/Car/src/node_camera/msg/camera.msg" NAME_WE)
add_custom_target(_node_camera_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "node_camera" "/home/pi/Car/src/node_camera/msg/camera.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(node_camera
  "/home/pi/Car/src/node_camera/msg/camera.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_camera
)

### Generating Services

### Generating Module File
_generate_module_cpp(node_camera
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_camera
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(node_camera_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(node_camera_generate_messages node_camera_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/Car/src/node_camera/msg/camera.msg" NAME_WE)
add_dependencies(node_camera_generate_messages_cpp _node_camera_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_camera_gencpp)
add_dependencies(node_camera_gencpp node_camera_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_camera_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(node_camera
  "/home/pi/Car/src/node_camera/msg/camera.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_camera
)

### Generating Services

### Generating Module File
_generate_module_lisp(node_camera
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_camera
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(node_camera_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(node_camera_generate_messages node_camera_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/Car/src/node_camera/msg/camera.msg" NAME_WE)
add_dependencies(node_camera_generate_messages_lisp _node_camera_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_camera_genlisp)
add_dependencies(node_camera_genlisp node_camera_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_camera_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(node_camera
  "/home/pi/Car/src/node_camera/msg/camera.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_camera
)

### Generating Services

### Generating Module File
_generate_module_py(node_camera
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_camera
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(node_camera_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(node_camera_generate_messages node_camera_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/Car/src/node_camera/msg/camera.msg" NAME_WE)
add_dependencies(node_camera_generate_messages_py _node_camera_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_camera_genpy)
add_dependencies(node_camera_genpy node_camera_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_camera_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_camera)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_camera
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(node_camera_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_camera)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_camera
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(node_camera_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_camera)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_camera\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_camera
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(node_camera_generate_messages_py std_msgs_generate_messages_py)
endif()
