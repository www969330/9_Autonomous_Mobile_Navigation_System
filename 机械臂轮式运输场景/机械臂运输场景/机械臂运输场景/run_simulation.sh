#!/bin/bash

# 设置X11权限
xhost +local:docker 2>/dev/null || true

cd "$(dirname "$0")"

# 使用 docker-compose 启动容器并运行仿真
sg docker -c "docker-compose run --rm ros-melodic-tiago bash -c 'source /opt/ros/melodic/setup.bash && source /tiago_public_ws/devel/setup.bash && source /catkin_ws/devel/setup.bash && roslaunch decluttering_domestic_robot simulation.launch'"
