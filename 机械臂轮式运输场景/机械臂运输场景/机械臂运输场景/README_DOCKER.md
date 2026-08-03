# Decluttering Domestic Robot - Docker 复现方案 (ROS Melodic)

## 概述
本方案使用 Docker 容器化技术复现 decluttering_domestic_robot 工程，完全隔离于宿主机的 ROS Noetic 环境。

## 环境信息
- **ROS 版本**: Melodic (与项目 README 一致)
- **基础镜像**: `palroboticssl/tiago_tutorials:melodic` (包含完整的 Tiago 机器人环境)
- **项目镜像**: `ros-melodic-tiago:latest`
- **容器名称**: `ros-melodic-tiago`

## 文件说明
- `Dockerfile` - Docker 镜像构建文件
- `docker-compose.yml` - Docker Compose 配置文件
- `build.sh` - 构建 Docker 镜像的脚本
- `start.sh` - 进入 Docker 容器交互式 shell 的脚本
- `run_simulation.sh` - 在 Docker 容器中运行仿真的脚本

## 使用方法

### 1. 构建 Docker 镜像
```bash
./build.sh
```

### 2. 进入 Docker 容器 (交互式 shell)
```bash
./start.sh
```

### 3. 运行仿真
```bash
./run_simulation.sh
```

## 容器内环境
- ROS Melodic (完整桌面版)
- Tiago 机器人相关包 (来自 PAL Robotics 官方镜像 `/tiago_public_ws`)
- MoveIt
- Gazebo
- OpenCV
- 所有必要的依赖包

## 与宿主机 ROS 环境的关系
- **完全隔离**: 容器内使用 ROS Melodic，不影响宿主机的 ROS Noetic 环境
- **独立 ROS Master**: 容器内使用独立的 ROS Master
- **不冲突**: 不会与宿主机的 ROS 包冲突
- **X11 转发**: 支持 Gazebo 和 RViz 的图形界面显示

## 注意事项
- 确保宿主机已安装 Docker 和 Docker Compose
- 需要 X11 显示支持（用于 Gazebo 和 RViz）
- 容器使用 host 网络模式，与宿主机共享网络
- 工程代码通过 volume 挂载到容器中
- 配置文件 (`configuration/smallHouse`) 会自动复制到 Docker volume 中

## 技术细节

### Dockerfile 关键点
- 基于 `palroboticssl/tiago_tutorials:melodic` 镜像，该镜像包含完整的 Tiago 机器人工作空间 (`/tiago_public_ws`)
- 构建时需要同时 source `/opt/ros/melodic/setup.bash` 和 `/tiago_public_ws/devel/setup.bash`
- 覆盖基础镜像的 `ENTRYPOINT` 以避免 `bash /bin/bash` 执行问题

### 仿真启动流程
1. 加载 Tiago 机器人模型和 Gazebo 环境
2. 启动导航相关节点
3. 启动 decluttering_domestic_robot 主节点和物体生成节点
4. 机器人开始执行导航和物体检测任务
