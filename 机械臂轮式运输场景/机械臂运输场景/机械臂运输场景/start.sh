#!/bin/bash

# 设置X11权限
xhost +local:docker 2>/dev/null || true

cd "$(dirname "$0")"

# 启动交互式容器
sg docker -c "docker-compose run --rm ros-melodic-tiago bash"
