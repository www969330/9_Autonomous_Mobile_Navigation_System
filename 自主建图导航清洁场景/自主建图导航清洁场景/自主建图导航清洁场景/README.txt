 基于TurtleBot3的自主探索建图与全覆盖清扫机器人系统

 1. 项目基本信息

组号：9
作业名称：
轮式移动机器人自主任务导航系统
项目名称：
轮式移动机器人自主任务导航系统——自主建图导航清洁场景

2. 软件包功能说明

本软件包实现基于ROS1的移动机器人自主探索、二维地图构建、自主定位导航以及全覆盖清扫功能。

主要功能：

1. TurtleBot3 Burger机器人Gazebo仿真；
2. Karto SLAM二维地图构建；
3. explore_lite自主探索建图；
4. AMCL机器人定位；
5. move_base自主导航；
6. DWA局部路径规划；
7. 全覆盖路径规划；
8. S型清扫路径生成；
9. A*算法寻找遗漏区域并继续清扫。


系统流程：

Gazebo启动室内环境
↓
TurtleBot3 Burger生成
↓
Karto SLAM建图
↓
explore_lite自主探索
↓
保存地图
↓
map_server加载地图
↓
AMCL定位
↓
全覆盖路径规划
↓
S型覆盖清扫
↓
遗漏区域检测
↓
A*重新规划


3. 文件目录说明

clean_robot_ws/

├── src/

│   └── Clean-robot-turtlebot3/

│       ├── clean_robot/

│       │   ├── src/

│       │   │   ├── path_planning_node.cpp

│       │   │   └── next_goal.cpp

│       │   ├── launch/

│       │   └── config/

│       ├── turtlebot3/

│       ├── turtlebot3_msgs/

│       ├── turtlebot3_simulations/

│       └── explore/


目录说明：

clean_robot：
全覆盖清扫核心程序。

path_planning_node：
生成机器人清扫路径。

next_goal：
向move_base发送导航目标。

turtlebot3：
机器人模型与控制程序。

launch：
系统启动文件。

config：
参数配置文件。


4. 运行环境

操作系统：
Ubuntu 20.04 LTS

ROS版本：
ROS Noetic

Gazebo版本：
Gazebo 11

编程语言：
C++、Python3

仿真平台：
Gazebo + RViz

机器人模型：
TurtleBot3 Burger


 5. 依赖安装方法

安装ROS依赖：

sudo apt update


sudo apt install -y \
ros-noetic-turtlebot3 \
ros-noetic-navigation \
ros-noetic-dwa-local-planner \
ros-noetic-slam-karto \
ros-noetic-gazebo-ros-pkgs \
ros-noetic-map-server \
ros-noetic-amcl \
ros-noetic-xacro \
ros-noetic-rviz \
python3-rosdep \
libopencv-dev \
git \
xterm


初始化rosdep：

sudo rosdep init

rosdep update


 6. 工作空间创建与源码下载

创建工作空间：

cd ~

mkdir -p ~/clean_robot_ws/src


进入源码目录：

cd ~/clean_robot_ws/src


下载项目：

git clone -b melodic-devel https://github.com/li-haojia/Clean-robot-turtlebot3.git


 7. 依赖安装与编译

进入工作空间：

cd ~/clean_robot_ws


加载ROS环境：

source /opt/ros/noetic/setup.bash


安装依赖：

rosdep install --from-paths src --ignore-src -r -y


编译：

catkin_make -DCMAKE_BUILD_TYPE=Release


成功后应出现：

Built target path_planning_node

Built target next_goal


 8. 运行前配置

加载工作空间：

source ~/clean_robot_ws/devel/setup.bash


设置机器人型号：

export TURTLEBOT3_MODEL=burger


永久配置：

echo "source ~/clean_robot_ws/devel/setup.bash" >> ~/.bashrc

echo "export TURTLEBOT3_MODEL=burger" >> ~/.bashrc


 9. 完整运行流程

步骤1：

进入工作空间：

cd ~/clean_robot_ws


步骤2：

加载环境：

source devel/setup.bash


步骤3：

启动自主清扫：

roslaunch clean_robot clean_work.launch


启动后会打开：

Gazebo仿真环境

RViz可视化界面


10. 输入说明

输入内容：

1. Gazebo世界环境文件；
2. TurtleBot3机器人模型；
3. 地图文件yaml和pgm；
4. ROS导航参数文件。


 11. 输出说明

输出内容：

1. 全覆盖规划路径：

/path_planning_node/cleaning_plan_nodehandle/cleaning_path


2. 已完成清扫轨迹：

/clean_robot/passed_path


3. 导航目标：

/move_base_simple/goal


RViz中显示：

- 地图
- 规划路径
- 机器人位置
- 实际运动轨迹


 12. 运行成功判断标准

运行成功时：

1. Gazebo正常启动；
2. RViz显示机器人和地图；
3. ROS节点正常运行：

/amcl

/move_base

/path_planning_node

/next_goal


4. 机器人能够自动运动；
5. 完成室内区域覆盖清扫。


 13. 常见问题


问题1：
找不到ROS包。

解决：

source /opt/ros/noetic/setup.bash

source ~/clean_robot_ws/devel/setup.bash


问题2：
TURTLEBOT3_MODEL未设置。

解决：

export TURTLEBOT3_MODEL=burger


问题3：
catkin_make编译失败。

解决：

重新安装依赖：

rosdep install --from-paths src --ignore-src -r -y


问题4：
Gazebo启动失败。

解决：

确认：

Ubuntu版本为20.04；

ROS版本为Noetic；

Gazebo11安装正常。


 14. 停止程序

关闭Gazebo和RViz窗口。

终端按：

Ctrl+C


停止ROS节点。


15. 项目总结

本项目基于ROS1 Noetic实现了一个完整的移动机器人自主清扫系统。

系统融合：

SLAM建图；
自主探索；
AMCL定位；
Navigation导航；
DWA路径规划；
全覆盖路径规划。

最终实现TurtleBot3机器人在仿真室内环境中的自主导航和覆盖清扫。
