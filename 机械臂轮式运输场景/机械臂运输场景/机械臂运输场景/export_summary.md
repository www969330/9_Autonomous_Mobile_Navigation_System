# Docker ROS Melodic 环境导出总结

## 项目概述
为 `decluttering_domestic_robot` 项目构建了基于 Docker 的 ROS Melodic 运行环境，
解决了主机 ROS Noetic 与项目要求的 ROS Melodic 之间的版本冲突问题。

## 使用的镜像
- **基础镜像**: `palroboticssl/tiago_tutorials:melodic` (包含 Tiago 机器人完整环境)
- **项目镜像**: `ros-melodic-tiago:latest` (基于基础镜像添加项目代码构建)

## 构建的文件

### 1. Dockerfile
```dockerfile
FROM palroboticssl/tiago_tutorials:melodic

ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=melodic

WORKDIR /catkin_ws
RUN mkdir -p src

COPY . /catkin_ws/src/decluttering_domestic_robot/

RUN /bin/bash -c "source /opt/ros/melodic/setup.bash && cd /catkin_ws && catkin build"

RUN echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc && \
    echo "source /catkin_ws/devel/setup.bash" >> ~/.bashrc

WORKDIR /catkin_ws

CMD ["/bin/bash"]
```

### 2. docker-compose.yml
```yaml
version: "2.2"
services:
  ros-melodic-tiago:
    build: .
    image: ros-melodic-tiago:latest
    container_name: ros-melodic-tiago
    environment:
      - DISPLAY=${DISPLAY}
      - QT_X11_NO_MITSHM=1
      - ROS_MASTER_URI=http://localhost:11311
    volumes:
      - /tmp/.X11-unix:/tmp/.X11-unix:rw
      - ./:/catkin_ws/src/decluttering_domestic_robot:rw
      - tiago_maps:/root/.pal/tiago_maps/configurations
    network_mode: host
    privileged: true
    stdin_open: true
    tty: true
    command: /bin/bash
```

### 3. 辅助脚本

#### build.sh
```bash
#!/bin/bash
set -e
echo "Building ROS Melodic Tiago Docker image..."
sg docker -c "docker-compose build"
echo "Build completed!"
```

#### start.sh
```bash
#!/bin/bash
sg docker -c "docker-compose run --rm ros-melodic-tiago bash"
```

#### run_simulation.sh
```bash
#!/bin/bash
sg docker -c "docker-compose run --rm ros-melodic-tiago bash -c 'source /catkin_ws/devel/setup.bash && roslaunch decluttering_domestic_robot simulation.launch'"
```

## Docker 代理配置
由于网络问题，配置了 Docker 使用 clash-meta 代理：n
文件: `/etc/systemd/system/docker.service.d/http-proxy.conf`
```
[Service]
Environment="HTTP_PROXY=http://127.0.0.1:7897"
Environment="HTTPS_PROXY=http://127.0.0.1:7897"
Environment="NO_PROXY=localhost,127.0.0.1"
```

## 使用方法

### 构建镜像
```bash
cd ~/decluttering_domestic_robot_docker
./build.sh
```

### 启动交互式容器
```bash
./start.sh
```

### 运行仿真
```bash
./run_simulation.sh
```

### 手动启动容器
```bash
docker-compose up -d
docker exec -it ros-melodic-tiago bash
```

## 构建结果
- 镜像构建成功，包含所有 Tiago 机器人依赖
- 项目包 `decluttering_domestic_robot` 编译成功
- 容器可以正常运行并访问 ROS Melodic 环境

## 注意事项
1. 需要配置 X11 转发才能显示 Gazebo 等 GUI 程序
2. 使用 `sg docker -c "..."` 是因为当前用户可能需要切换 docker 组权限
3. 首次运行前确保执行 `xhost +local:` 允许本地 X11 连接
