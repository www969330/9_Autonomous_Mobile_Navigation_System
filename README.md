# README.txt
# 轮式移动机器人自主任务导航系统 — ROS综合大作业
组号：9
## 说明
本总仓库包含三套**完全独立**的ROS机器人仿真子项目，三个子项目各自文件夹内均附有独立README.txt，内部包含对应场景全套详细运行文档。本文件为项目总览说明，统一介绍整体结构、公共依赖、分项目入口；各场景细分参数、运行细节、专属报错处理请查看对应子目录内独立README。

## 1. 项目基本信息
组号：9
作业名称：轮式移动机器人自主任务导航系统（多场景ROS综合仿真大作业）
项目名称：多场景移动机器人SLAM、导航、抓取、全覆盖清扫一体化仿真平台
### 软件包整体功能说明
本项目包含三套互不依赖、可单独编译运行的ROS机器人仿真系统，分别覆盖城市配送自主导航、Tiago移动机械臂抓取转运、TurtleBot3未知环境自主探索清扫三类典型机器人任务场景。整套代码基于ROS1 + Gazebo仿真开发，整合二维激光SLAM、AMCL定位、全局/局部路径规划、机器视觉目标检测、机械臂运动控制、状态机任务调度、Docker容器化部署等核心机器人技术。
- 解决问题：分别实现室内外配送导航、家用机械臂物体搬运、未知环境自动清扫三类移动机器人完整任务流程；
- 主要方法：gmapping/Karto SLAM、AMCL自适应蒙特卡洛定位、move_base导航栈、DWA/TEB局部规划、YOLOv8视觉检测、TF坐标变换、PlayMotion机械臂控制、自定义全覆盖路径规划；
- 输入：Gazebo仿真世界文件、机器人URDF/Xacro模型、导航地图、yaml参数配置、RViz交互目标点；
- 输出：Gazebo仿真可视化画面、RViz地图与运动轨迹、SLAM生成地图、终端任务日志、代码静态分析报告、Doxygen标准化文档；
- 适用场景：课程ROS机器人综合仿真实验，三套子系统运行环境分离，可单独用于导航、机械臂、自主清扫分模块学习。

## 2. 文件目录说明
```
ros大作业/
├── README.txt                # 本总说明文档（公共依赖、整体结构、分项目快速启动）
├── 城市配送机器人系统/        # 子项目1：轮式机器人城市配送导航
│   ├── README.txt            # 子项目独立完整文档（环境、依赖、分步运行、输入输出、排错）
│   ├── bot/                  # ROS功能包集合（bot_description、bot_slam、bot_navigation、bot_perception）
│   ├── models/               # 机器人、场景模型文件
│   ├── maps/                 # SLAM保存地图
│   ├── config/               # SLAM、导航、视觉参数配置
│   ├── launch/               # 仿真、建图、导航启动文件
│   └── results/              # 运行日志、地图输出（程序自动生成）
├── 机械臂轮式运输场景/        # 子项目2：Tiago移动机械臂抓取搬运
│   ├── README.txt            # 子项目独立完整文档（含Docker部署流程）
│   ├── CMakeLists.txt / package.xml  # ROS包编译与依赖描述
│   ├── src/ include/         # 主控、导航、检测、抓取C++源码与头文件
│   ├── launch/ world/ models/ # 仿真启动、场景世界、物体模型
│   ├── configuration/        # 导航地图与参数
│   ├── test/                 # 单元测试代码
│   ├── docs/                 # Doxygen自动生成html/latex文档（程序自动生成）
│   ├── docker-compose.yml / Dockerfile / *.sh  # Docker一键部署脚本
│   └── results/              # 代码静态检测报告（程序自动生成）
└── 自主建图导航清洁场景/      # 子项目3：TurtleBot3全覆盖清扫机器人
    ├── README.txt            # 子项目独立完整文档
    ├── src/
    │   ├── clean_robot/      # 全覆盖清扫核心功能包
    │   ├── turtlebot3 / turtlebot3_msgs / turtlebot3_simulations # 机器人官方依赖包
    │   └── explore/          # 自主探索建图功能包
    ├── config/ launch/       # 导航、SLAM、清扫启动与参数文件
    └── maps/                 # 自主探索生成地图（程序自动生成）
```
### 目录关键说明
1. 主程序入口：每个子项目launch文件夹下仿真启动文件；C++源码存于各子包src目录；
2. 参数文件：统一存放于各子项目config/configuration目录，用户可按需修改坐标、规划器参数；
3. 输入数据：world仿真场景、URDF机器人模型、预存地图为固定输入文件；
4. 输出目录：各子项目results、maps、docs为程序运行自动生成，无需手动创建；
5. 可修改文件：各子项目内*.launch、yaml配置、world场景文件；
6. 自动生成文件：地图pgm/yaml、docs/html、代码检测日志、终端运行轨迹日志。

## 3. 运行环境（三套子项目区分版本）
### 子项目1：城市配送机器人系统
操作系统：Ubuntu 20.04 LTS
ROS版本：ROS 1 Noetic
仿真平台：Gazebo 11
编程语言：C++、Python3
可视化工具：RViz
硬件要求：普通PC可运行；视觉YOLO模块推荐带CUDA GPU

<div align="center">
  <img width="2376" height="1347" alt="image" src="https://github.com/user-attachments/assets/c67f4c77-0032-43cc-a4fc-e0858f21d2f4" />
</div>

<div align="center">
  <img width="662" height="498" style="max-width: 45%; margin-right: 10px;" alt="image" src="https://github.com/user-attachments/assets/0d631e9e-8d9b-404e-aac4-7be0c20c2c1e" />
  <img width="656" height="485" style="max-width: 45%;" alt="image" src="https://github.com/user-attachments/assets/fb53b4af-1ce4-4aac-b2ef-c236472a2cda" />
</div>

### 子项目2：机械臂轮式运输场景
操作系统：Ubuntu 18.04 LTS（推荐Docker容器运行，无需本地配置）
ROS版本：ROS 1 Melodic
仿真平台：Gazebo 9
编程语言：C++11/C++14
部署工具：Docker
硬件要求：普通PC，容器无N卡时使用软渲染
<img width="952" height="563" alt="image" src="https://github.com/user-attachments/assets/bb8d5541-b52b-4cc1-a054-3819e75df750" />
<img width="1040" height="742" alt="image" src="https://github.com/user-attachments/assets/8730f5a5-4ee1-4cbe-ace6-6f85d71279e0" />


### 子项目3：自主建图导航清洁场景
操作系统：Ubuntu 20.04 LTS
ROS版本：ROS 1 Noetic
仿真平台：Gazebo 11
编程语言：C++、Python3
机器人模型：TurtleBot3 Burger
<img width="518" height="317" alt="image" src="https://github.com/user-attachments/assets/c4b4c413-998a-4262-8bd9-7550faad618e" />


## 4. 公共依赖安装方法（Ubuntu20.04通用，适配1、3子项目）
```bash
# 更新软件源
sudo apt update
# ROS Noetic完整桌面环境
sudo apt install ros-noetic-desktop-full
# SLAM与导航通用依赖
sudo apt install ros-noetic-navigation ros-noetic-slam-gmapping ros-noetic-slam-karto
sudo apt install ros-noetic-amcl ros-noetic-move-base ros-noetic-teb-local-planner
sudo apt install ros-noetic-dwa-local-planner ros-noetic-gazebo-ros
# TurtleBot3专用依赖
sudo apt install ros-noetic-turtlebot3 ros-noetic-xacro ros-noetic-rviz
# 编译与工具依赖
sudo apt install python3-rosdep git libopencv-dev xterm
# 初始化rosdep
sudo rosdep init
rosdep update
```
### 子项目2（Tiago机械臂）专属依赖
该项目推荐Docker镜像部署，容器内已预装全部依赖；如需本地Ubuntu18.04手动安装，查看「机械臂轮式运输场景/README.txt」完整安装命令。

## 5. 运行前通用配置
1. 切换至目标子项目根目录；
2. 加载对应ROS环境变量：
   - Noetic（配送/清扫项目）：`source /opt/ros/noetic/setup.bash`
   - Melodic（机械臂项目）：`source /opt/ros/melodic/setup.bash`
3. 安装子项目专属依赖：
```bash
rosdep install --from-paths src --ignore-src -r -y
```
4. 编译工作空间：
   - Noetic项目：`catkin_make`
   - Melodic Docker项目：`catkin build`
5. 刷新工作空间环境：`source devel/setup.bash`
6. 特殊配置：
   - 清扫项目：执行 `export TURTLEBOT3_MODEL=burger`
   - Gazebo黑屏：`export LIBGL_ALWAYS_SOFTWARE=1`
细分参数修改、坐标调整请查看对应子项目README。

## 6. 完整运行流程（总览快速启动，详细分步见各子目录README）
### 6.1 城市配送机器人系统
步骤1：进入目录
`cd ~/Desktop/ros大作业/城市配送机器人系统`
步骤2：加载Noetic环境、安装依赖、编译、刷新工作空间
步骤3：终端1启动仿真
`roslaunch bot_description gazebo.launch`
步骤4：终端2启动SLAM建图
`roslaunch bot_slam delybot_slam.launch`
步骤5：终端3启动导航
`roslaunch bot_navigation delybot_navigation.launch`
步骤6：RViz内点击2D Nav Goal下发目标，完成配送导航。

### 6.2 机械臂轮式运输场景（Docker推荐）
步骤1：进入目录
`cd ~/Desktop/ros大作业/机械臂轮式运输场景`
步骤2：启动Docker容器 `docker start ros-melodic-tiago`
步骤3：进入容器编译环境
`docker exec -it ros-melodic-tiago bash`
步骤4：容器内编译、刷新工作空间
步骤5：容器终端1启动仿真场景
`roslaunch decluttering_domestic_robot simulation.launch`
步骤6：新开容器终端2启动任务主控
`rosrun decluttering_domestic_robot dcrobot_node`
机器人自动完成抓取、转运、放置全流程。
> 无Docker本地运行完整步骤，查看本文件夹内README.txt。

### 6.3 自主建图导航清洁场景
步骤1：进入目录
`cd ~/Desktop/ros大作业/自主建图导航清洁场景`
步骤2：加载Noetic环境，设置机器人模型变量
`export TURTLEBOT3_MODEL=burger`
步骤3：依赖安装、编译、刷新工作空间
步骤4：一键启动全套清扫系统
`roslaunch clean_robot clean_work.launch`
Gazebo、RViz自动弹出，机器人自主探索并全覆盖清扫。

## 7. 输入说明（总览，详细表格见各子项目独立README）
| 输入项 | 格式 | 存放位置 | 说明 |
| ---- | ---- | ---- | ---- |
| Gazebo仿真世界 | .world | 各子项目world/目录 | 仿真室内环境场景 |
| 机器人模型 | urdf/xacro/sdf | 各子项目models/urdf | 配送机器人/Tiago/TurtleBot3模型 |
| 导航地图 | pgm/yaml | maps/configuration | SLAM生成或预加载导航地图 |
| 参数配置文件 | yaml | config/configuration | SLAM、导航、规划器、视觉参数 |
| 启动文件 | .launch | launch/ | 仿真、建图、导航一键启动脚本 |
| RViz交互目标 | 鼠标交互 | RViz可视化窗口 | 配送场景手动指定导航终点 |

## 8. 输出说明（总览，详细表格见各子项目独立README）
| 输出内容 | 格式 | 保存位置 | 说明 |
| ---- | ---- | ---- | ---- |
| Gazebo仿真画面 | 可视化窗口 | 程序弹出窗口 | 机器人实时运动仿真 |
| RViz可视化轨迹 | 可视化窗口 | 程序弹出窗口 | 地图、激光、规划路径、机器人位姿 |
| SLAM生成地图 | pgm/yaml | 各子项目maps/ | 探索建图后保存的环境地图 |
| 任务运行日志 | 文本 | 终端实时输出 | 导航状态、机械臂状态机、清扫进度 |
| 代码分析报告 | txt | 机械臂场景results/ | Cppcheck、Cpplint静态检测结果 |
| 开发文档 | html/latex | 机械臂场景docs/ | Doxygen自动生成接口文档 |

## 9. 运行成功的判断标准（通用总标准）
1. Gazebo仿真窗口正常打开，无黑屏、模型加载报错；
2. RViz窗口正常加载机器人、激光雷达、地图坐标系，无TF持续报错；
3. 所有ROS核心节点（AMCL、move_base、SLAM、自定义任务节点）稳定运行，无持续ERROR日志；
4. 机器人完成对应场景完整任务：
   - 配送场景：可接收RViz目标点，自主规划路径抵达目的地；
   - 机械臂场景：自动导航至物体、机械臂抓取、转运至目标区域放下方块；
   - 清扫场景：自主探索全房间，生成S型全覆盖清扫轨迹，自动补扫遗漏区域；
5. 程序无崩溃、无循环报错，任务完整闭环结束；
6. 对应输出目录自动生成地图、日志等结果文件。
> 各场景细分成功细节、截图参考、终端预期输出查看对应子项目README.txt。
<img width="212" height="160" alt="屏幕截图 2026-07-16 004308" src="https://github.com/user-attachments/assets/9608aed7-ebfc-42b5-bfc1-551302136da8" />

## 10. 通用常见问题（专属细分问题见各子项目README）
问题1：提示无法找到对应ROS软件包
解决方法：执行环境加载命令
`source /opt/ros/noetic/setup.bash` 或 `source /opt/ros/melodic/setup.bash`，再执行 `source devel/setup.bash` 刷新工作空间。

问题2：Gazebo窗口黑屏、渲染失败
解决方法：运行前添加软渲染环境变量
`export LIBGL_ALWAYS_SOFTWARE=1`

问题3：编译提示依赖缺失、头文件找不到
解决方法：进入子项目根目录执行
`rosdep install --from-paths src --ignore-src -r -y` 自动补全全部ROS依赖。

问题4：机器人导航原地打转、无法到达目标
解决方法：打开对应launch/config目录yaml参数，调小move_base代价地图inflation_radius膨胀半径，核对目标点坐标无障碍物遮挡。

问题5：终端持续输出TF坐标变换异常
解决方法：执行 `rosrun tf view_frames` 查看TF树，检查机器人URDF模型关节、传感器坐标是否完整关联。

## 11. 程序停止通用方法
所有场景统一操作：终端按下 `Ctrl + C` 关闭ROS节点，手动关闭Gazebo、RViz窗口；Docker容器场景关闭终端即可停止仿真。

## 补充说明
1. 本文件仅为项目总览，每个子项目文件夹内**独立README.txt**严格按照课程验收标准编写，包含完整分项表格、分步执行命令、细分报错解决方案、预期运行效果，验收、复现运行请优先阅读对应子项目内部文档；
2. 三个子项目运行环境、代码包完全隔离，无交叉依赖，可单独拷贝任意一个文件夹独立运行；
3. 项目绝对路径均使用相对路径，不会绑定作者本地桌面路径，可任意移动文件夹位置使用。
