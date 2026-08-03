README.txt
轮式移动机器人自主任务导航系统 — ROS 综合大作业
组号：9
说明
本总仓库包含三套完全独立的 ROS 机器人仿真子项目，三个子项目各自文件夹内均附有独立 README.txt，内部包含对应场景全套详细运行文档。本文件为项目总览说明，统一介绍整体结构、公共依赖、分项目入口；各场景细分参数、运行细节、专属报错处理请查看对应子目录内独立 README。
1. 项目基本信息
组号：9
作业名称：轮式移动机器人自主任务导航系统（多场景 ROS 综合仿真大作业）
项目名称：多场景移动机器人 SLAM、导航、抓取、全覆盖清扫一体化仿真平台
软件包整体功能说明
本项目包含三套互不依赖、可单独编译运行的 ROS 机器人仿真系统，分别覆盖城市配送自主导航、Tiago 移动机械臂抓取转运、TurtleBot3 未知环境自主探索清扫三类典型机器人任务场景。整套代码基于 ROS1 + Gazebo 仿真开发，整合二维激光 SLAM、AMCL 定位、全局 / 局部路径规划、机器视觉目标检测、机械臂运动控制、状态机任务调度、Docker 容器化部署等核心机器人技术。
解决问题：分别实现室内外配送导航、家用机械臂物体搬运、未知环境自动清扫三类移动机器人完整任务流程；
主要方法：gmapping/Karto SLAM、AMCL 自适应蒙特卡洛定位、move_base 导航栈、DWA/TEB 局部规划、YOLOv8 视觉检测、TF 坐标变换、PlayMotion 机械臂控制、自定义全覆盖路径规划；
输入：Gazebo 仿真世界文件、机器人 URDF/Xacro 模型、导航地图、yaml 参数配置、RViz 交互目标点；
输出：Gazebo 仿真可视化画面、RViz 地图与运动轨迹、SLAM 生成地图、终端任务日志、代码静态分析报告、Doxygen 标准化文档；
适用场景：课程 ROS 机器人综合仿真实验，三套子系统运行环境分离，可单独用于导航、机械臂、自主清扫分模块学习。
2. 文件目录说明
plaintext
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
目录关键说明
主程序入口：每个子项目 launch 文件夹下仿真启动文件；C++ 源码存于各子包 src 目录；
参数文件：统一存放于各子项目 config/configuration 目录，用户可按需修改坐标、规划器参数；
输入数据：world 仿真场景、URDF 机器人模型、预存地图为固定输入文件；
输出目录：各子项目 results、maps、docs 为程序运行自动生成，无需手动创建；
可修改文件：各子项目内 *.launch、yaml 配置、world 场景文件；
自动生成文件：地图 pgm/yaml、docs/html、代码检测日志、终端运行轨迹日志。
3. 运行环境（三套子项目区分版本）
子项目 1：城市配送机器人系统
操作系统：Ubuntu 20.04 LTS
ROS 版本：ROS 1 Noetic
仿真平台：Gazebo 11
编程语言：C++、Python3
可视化工具：RViz
硬件要求：普通 PC 可运行；视觉 YOLO 模块推荐带 CUDA GPU
子项目 2：机械臂轮式运输场景
操作系统：Ubuntu 18.04 LTS（推荐 Docker 容器运行，无需本地配置）
ROS 版本：ROS 1 Melodic
仿真平台：Gazebo 9
编程语言：C++11/C++14
部署工具：Docker
硬件要求：普通 PC，容器无 N 卡时使用软渲染
子项目 3：自主建图导航清洁场景
操作系统：Ubuntu 20.04 LTS
ROS 版本：ROS 1 Noetic
仿真平台：Gazebo 11
编程语言：C++、Python3
机器人模型：TurtleBot3 Burger
4. 公共依赖安装方法（Ubuntu20.04 通用，适配 1、3 子项目）
bash
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
子项目 2（Tiago 机械臂）专属依赖
该项目推荐 Docker 镜像部署，容器内已预装全部依赖；如需本地 Ubuntu18.04 手动安装，查看「机械臂轮式运输场景 / README.txt」完整安装命令。
5. 运行前通用配置
切换至目标子项目根目录；
加载对应 ROS 环境变量：
Noetic（配送 / 清扫项目）：source /opt/ros/noetic/setup.bash
Melodic（机械臂项目）：source /opt/ros/melodic/setup.bash
安装子项目专属依赖：
bash
rosdep install --from-paths src --ignore-src -r -y
编译工作空间：
Noetic 项目：catkin_make
Melodic Docker 项目：catkin build
刷新工作空间环境：source devel/setup.bash
特殊配置：
清扫项目：执行 export TURTLEBOT3_MODEL=burger
Gazebo 黑屏：export LIBGL_ALWAYS_SOFTWARE=1
细分参数修改、坐标调整请查看对应子项目 README。
6. 完整运行流程（总览快速启动，详细分步见各子目录 README）
6.1 城市配送机器人系统
步骤 1：进入目录
cd ~/Desktop/ros大作业/城市配送机器人系统
步骤 2：加载 Noetic 环境、安装依赖、编译、刷新工作空间
步骤 3：终端 1 启动仿真
roslaunch bot_description gazebo.launch
步骤 4：终端 2 启动 SLAM 建图
roslaunch bot_slam delybot_slam.launch
步骤 5：终端 3 启动导航
roslaunch bot_navigation delybot_navigation.launch
步骤 6：RViz 内点击 2D Nav Goal 下发目标，完成配送导航。
6.2 机械臂轮式运输场景（Docker 推荐）
步骤 1：进入目录
cd ~/Desktop/ros大作业/机械臂轮式运输场景
步骤 2：启动 Docker 容器 docker start ros-melodic-tiago
步骤 3：进入容器编译环境
docker exec -it ros-melodic-tiago bash
步骤 4：容器内编译、刷新工作空间
步骤 5：容器终端 1 启动仿真场景
roslaunch decluttering_domestic_robot simulation.launch
步骤 6：新开容器终端 2 启动任务主控
rosrun decluttering_domestic_robot dcrobot_node
机器人自动完成抓取、转运、放置全流程。
无 Docker 本地运行完整步骤，查看本文件夹内 README.txt。
6.3 自主建图导航清洁场景
步骤 1：进入目录
cd ~/Desktop/ros大作业/自主建图导航清洁场景
步骤 2：加载 Noetic 环境，设置机器人模型变量
export TURTLEBOT3_MODEL=burger
步骤 3：依赖安装、编译、刷新工作空间
步骤 4：一键启动全套清扫系统
roslaunch clean_robot clean_work.launch
Gazebo、RViz 自动弹出，机器人自主探索并全覆盖清扫。
7. 输入说明（总览，详细表格见各子项目独立 README）
表格
输入项	格式	存放位置	说明
Gazebo 仿真世界	.world	各子项目 world / 目录	仿真室内环境场景
机器人模型	urdf/xacro/sdf	各子项目 models/urdf	配送机器人 / Tiago/TurtleBot3 模型
导航地图	pgm/yaml	maps/configuration	SLAM 生成或预加载导航地图
参数配置文件	yaml	config/configuration	SLAM、导航、规划器、视觉参数
启动文件	.launch	launch/	仿真、建图、导航一键启动脚本
RViz 交互目标	鼠标交互	RViz 可视化窗口	配送场景手动指定导航终点
8. 输出说明（总览，详细表格见各子项目独立 README）
表格
输出内容	格式	保存位置	说明
Gazebo 仿真画面	可视化窗口	程序弹出窗口	机器人实时运动仿真
RViz 可视化轨迹	可视化窗口	程序弹出窗口	地图、激光、规划路径、机器人位姿
SLAM 生成地图	pgm/yaml	各子项目 maps/	探索建图后保存的环境地图
任务运行日志	文本	终端实时输出	导航状态、机械臂状态机、清扫进度
代码分析报告	txt	机械臂场景 results/	Cppcheck、Cpplint 静态检测结果
开发文档	html/latex	机械臂场景 docs/	Doxygen 自动生成接口文档
9. 运行成功的判断标准（通用总标准）
Gazebo 仿真窗口正常打开，无黑屏、模型加载报错；
RViz 窗口正常加载机器人、激光雷达、地图坐标系，无 TF 持续报错；
所有 ROS 核心节点（AMCL、move_base、SLAM、自定义任务节点）稳定运行，无持续 ERROR 日志；
机器人完成对应场景完整任务：
配送场景：可接收 RViz 目标点，自主规划路径抵达目的地；
机械臂场景：自动导航至物体、机械臂抓取、转运至目标区域放下方块；
清扫场景：自主探索全房间，生成 S 型全覆盖清扫轨迹，自动补扫遗漏区域；
程序无崩溃、无循环报错，任务完整闭环结束；
对应输出目录自动生成地图、日志等结果文件。
各场景细分成功细节、截图参考、终端预期输出查看对应子项目 README.txt。
10. 通用常见问题（专属细分问题见各子项目 README）
问题 1：提示无法找到对应 ROS 软件包
解决方法：执行环境加载命令
source /opt/ros/noetic/setup.bash 或 source /opt/ros/melodic/setup.bash，再执行 source devel/setup.bash 刷新工作空间。
问题 2：Gazebo 窗口黑屏、渲染失败
解决方法：运行前添加软渲染环境变量
export LIBGL_ALWAYS_SOFTWARE=1
问题 3：编译提示依赖缺失、头文件找不到
解决方法：进入子项目根目录执行
rosdep install --from-paths src --ignore-src -r -y 自动补全全部 ROS 依赖。
问题 4：机器人导航原地打转、无法到达目标
解决方法：打开对应 launch/config 目录 yaml 参数，调小 move_base 代价地图 inflation_radius 膨胀半径，核对目标点坐标无障碍物遮挡。
问题 5：终端持续输出 TF 坐标变换异常
解决方法：执行 rosrun tf view_frames 查看 TF 树，检查机器人 URDF 模型关节、传感器坐标是否完整关联。
11. 程序停止通用方法
所有场景统一操作：终端按下 Ctrl + C 关闭 ROS 节点，手动关闭 Gazebo、RViz 窗口；Docker 容器场景关闭终端即可停止仿真。
补充说明
本文件仅为项目总览，每个子项目文件夹内独立 README.txt严格按照课程验收标准编写，包含完整分项表格、分步执行命令、细分报错解决方案、预期运行效果，验收、复现运行请优先阅读对应子项目内部文档；
三个子项目运行环境、代码包完全隔离，无交叉依赖，可单独拷贝任意一个文件夹独立运行；
项目绝对路径均使用相对路径，不会绑定作者本地桌面路径，可任意移动文件夹位置使用。

