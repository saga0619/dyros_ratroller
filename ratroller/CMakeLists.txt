cmake_minimum_required(VERSION 2.8.3)
project(ratroller)

add_compile_options(-std=c++11 -Ofast)

find_package(catkin REQUIRED COMPONENTS
  mujoco_ros_msgs
  eigen_conversions
  geometry_msgs
  realtime_tools
  roscpp
  roslib
  sensor_msgs
  std_msgs
  tf
)

# find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES dyros_red_controller
#  CATKIN_DEPENDS dyros_red_msgs eigen_conversions geometry_msgs realtime_tools roscpp sensor_msgs std_msgs tf
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

file(GLOB AD_SRC
    "src/state_manager.cpp"
    "src/dynamics_manager.cpp"
    "src/mujoco_interface.cpp"
    "src/link.cpp"
    "src/ratroller.cpp" `
    "src/terminal.cpp"
    )

add_library(ratroller_lib
  ${AD_SRC}
)

add_executable(ratroller src/main.cpp)


target_link_libraries(ratroller_lib
   ${catkin_LIBRARIES}
   -lrbdl
   -lrbdl_urdfreader
   -lrt
   -lqpOASES
   -lncurses
 )

 target_link_libraries(ratroller
    ratroller_lib
    ${catkin_LIBRARIES}
    )

add_dependencies(ratroller_lib ${catkin_EXPORTED_TARGETS})
add_dependencies(ratroller ${catkin_EXPORTED_TARGETS})
