================================================================================
                    机械臂运输场景 —— README
================================================================================

1. 项目基本信息
--------------------------------------------------------------------------------
组号：9
作业名称：           轮式移动机器人自主任务导航系统 —— 机械臂运输场景
项目名称：            decluttering_domestic_robot（Tiago 机器人抓取与放置）


2. 软件包功能说明
--------------------------------------------------------------------------------
本软件包用于完成 Tiago 机器人在 Gazebo 仿真环境中的抓取与放置任务。

主要功能：
• 在 smallHouse 场景中加载 Tiago 机器人模型；
• 生成蓝色方块（blue_box）和目标标记（target_marker）；
• 通过航点导航控制机器人移动到物体附近；
• 控制机械臂展开并抓取蓝色方块；
• 将蓝色方块运输至目标位置并放置。

主要方法：
• 使用 ROS Navigation Stack（move_base + teb_local_planner）进行路径规划；
• 使用状态机管理任务流程（STARTING → IDLE → MOVING_TO_CHECKPOINT → ...）；
• 使用 TF 坐标变换获取物体和机械臂的相对位置；
• 使用 PlayMotion 接口控制机械臂执行抓取和放置动作。

程序输入：
• Gazebo 世界文件（smallHouse.world）；
• 导航地图与参数配置；
• 物体生成位置和目标位置参数。

程序输出：
• Gazebo 仿真可视化结果；
• 终端状态机运行日志；
• 机器人导航轨迹和机械臂运动过程。

适用场景：
• ROS Melodic + Ubuntu 18.04 环境下的 Tiago 机器人仿真；
• 移动机械臂抓取与放置任务验证。


3. 文件目录说明
--------------------------------------------------------------------------------
机械臂运输场景/
├── README.txt                    # 本文件（运行说明）
├── CMakeLists.txt                # ROS 软件包编译配置
├── package.xml                   # ROS 软件包依赖描述
├── Dockerfile                    # Docker 镜像构建文件
├── docker-compose.yml            # Docker Compose 配置文件
├── build.sh                      # 编译脚本
├── start.sh                      # 快速启动脚本
├── run_simulation.sh             # 仿真运行脚本
├── src/                          # 源代码目录
│   ├── dcrobot_node.cpp          # 主节点入口
│   ├── dcrobot.cpp               # 主控逻辑实现
│   ├── navigation.cpp            # 导航模块实现
│   ├── detect_object.cpp         # 物体检测模块实现
│   ├── grasp_object.cpp          # 抓取模块实现
│   └── object_spawner.cpp        # 物体生成模块实现
├── include/                      # 头文件目录
│   ├── dcrobot/dcrobot.hpp
│   ├── navigation/navigation.hpp
│   ├── detect_object/detect_object.hpp
│   ├── grasp_object/grasp_object.hpp
│   └── object_spawner/object_spawner.hpp
├── launch/                       # 启动文件目录
│   └── simulation.launch         # 仿真启动文件
├── world/                        # 世界文件目录
│   └── smallHouse.world          # Gazebo 场景世界文件
├── models/                       # 模型文件目录
│   └── target_marker/            # 目标标记模型
├── configuration/                # 导航参数配置目录
│   └── smallHouse/               # smallHouse 场景地图与参数
├── test/                         # 单元测试目录
│   ├── dcrobot_test.cpp
│   ├── navigation_test.cpp
│   ├── detect_object_test.cpp
│   ├── grasp_object_test.cpp
│   ├── object_spawner_test.cpp
│   ├── main.cpp
│   └── main_test.test
├── docs/                         # 文档目录
│   ├── doxygen_config.conf       # Doxygen 配置
│   ├── html/                     # Doxygen 生成的 HTML 文档
│   ├── latex/                    # Doxygen 生成的 LaTeX 文档
│   ├── home_organizing_robot.gif # 演示动画
│   └── ...                       # 其他文档
├── uml/                          # UML 图目录
│   ├── initial/                  # 初始版本 UML
│   └── revised/                  # 修订版本 UML
└── results/                      # 运行结果目录（程序自动生成）
    ├── cppcheck_result.txt       # Cppcheck 静态分析结果
    └── cpplint_result.txt        # Cpplint 代码风格检查结果

用户可修改文件：
• launch/simulation.launch        — 修改目标坐标、物体位置等参数；
• configuration/smallHouse/       — 修改导航地图和参数；
• world/smallHouse.world          — 修改 Gazebo 场景。

程序自动生成文件：
• results/ 目录下的分析结果文件。


4. 运行环境
--------------------------------------------------------------------------------
操作系统：            Ubuntu 18.04
ROS 版本：            ROS Melodic
编程语言：            C++11 / C++14
开发平台：            Docker（基于 palroboticssl/tiago_tutorials:melodic 镜像）
仿真器：              Gazebo 9
主要依赖：
  • ROS Melodic（roscpp, std_msgs, geometry_msgs, nav_msgs, move_base_msgs,
                 actionlib, tf, play_motion_msgs, gazebo_ros）
  • Gazebo 9
  • Tiago 机器人相关软件包（tiago_description, tiago_moveit_config 等）
  • teb_local_planner（局部路径规划器）

是否需要特定硬件：    否（纯仿真，无需真实机器人）


5. 依赖安装方法
--------------------------------------------------------------------------------
本软件包基于 Docker 镜像运行，依赖已通过镜像预装。
如需手动安装依赖，请在容器内执行：

  cd /catkin_ws
  rosdep install --from-paths src --ignore-src -r -y

如需安装 teb_local_planner：

  sudo apt-get update
  sudo apt-get install ros-melodic-teb-local-planner


6. 运行前配置
--------------------------------------------------------------------------------
1. 确保 Docker 容器已启动：

     docker start ros-melodic-tiago

2. 设置软件渲染环境变量（容器缺少 NVIDIA 驱动时必须）：

     export LIBGL_ALWAYS_SOFTWARE=1

3. （可选）修改 launch/simulation.launch 中的参数：
   • 物体生成位置（默认：(-1.5, 1.5)）
   • 目标位置（默认：(-3.0, -1.0)）
   • 航点坐标


7. 完整运行流程
--------------------------------------------------------------------------------
步骤 1：进入 Docker 容器

    docker exec -it ros-melodic-tiago bash

步骤 2：编译工作空间

    cd /catkin_ws
    catkin build
    source devel/setup.bash

步骤 3：启动 Gazebo 仿真（终端 1）

    docker exec -it ros-melodic-tiago bash
    cd /catkin_ws && catkin build
    roslaunch decluttering_domestic_robot simulation.launch

    预期效果：
    • Gazebo 窗口打开，加载 smallHouse 场景；
    • Tiago 机器人出现在场景中；
    • 蓝色方块（blue_box）出现在 (-1.5, 1.5) 位置；
    • 目标标记（target_marker）出现在 (-3.0, -1.0) 位置。

步骤 4：启动机器人任务节点（终端 2）

    docker exec -it ros-melodic-tiago bash
    rosrun decluttering_domestic_robot dcrobot_node

    预期效果：
    • 节点初始化完成；
    • 状态机开始运行：STARTING → IDLE → MOVING_TO_CHECKPOINT；
    • 机器人依次导航至航点，最终完成抓取与放置。

快速启动（一键执行）：

    # 终端 1
    docker exec -it ros-melodic-tiago bash -c "cd /catkin_ws && catkin build && roslaunch decluttering_domestic_robot simulation.launch"

    # 终端 2
    docker exec -it ros-melodic-tiago bash -c "rosrun decluttering_domestic_robot dcrobot_node"


8. 输入说明
--------------------------------------------------------------------------------
输入项              格式        位置                          说明
--------------------------------------------------------------------------------
世界文件            .world      world/smallHouse.world        Gazebo 场景文件
导航地图            .pgm/.yaml  configuration/smallHouse/     导航地图及参数
启动参数            .launch     launch/simulation.launch      物体位置、目标位置等参数
模型文件            .sdf/.config models/target_marker/         目标标记模型

无需用户交互输入，所有参数通过 launch 文件配置。


9. 输出说明
--------------------------------------------------------------------------------
输出内容            格式        位置                          说明
--------------------------------------------------------------------------------
Gazebo 仿真窗口     可视化      —                             实时显示机器人运动和场景
终端日志            文本        终端输出                       状态机运行日志和调试信息
静态分析结果        .txt        results/cppcheck_result.txt   Cppcheck 代码分析结果
代码风格检查        .txt        results/cpplint_result.txt    Cpplint 代码风格检查结果


10. 运行成功的判断标准
--------------------------------------------------------------------------------
程序运行成功后应出现以下结果：

1. Gazebo 窗口正常打开，显示 smallHouse 场景和 Tiago 机器人；
2. 蓝色方块出现在 (-1.5, 1.5) 位置，目标标记出现在 (-3.0, -1.0) 位置；
3. 终端显示状态机正常流转：STARTING → IDLE → MOVING_TO_CHECKPOINT → ...；
4. 机器人从 (0,0) 依次移动到 (-0.8,1.1)、(-1.0,1.5)；
5. 机器人停在距离物体约 0.5m 处；
6. 机械臂展开，夹爪闭合，成功抓起蓝色方块；
7. 机器人移动至 (-3.0, -1.0)，将方块放入目标区域；
8. 状态机进入 STOP，任务完成；
9. 程序运行过程中没有持续报错。


11. 常见问题
--------------------------------------------------------------------------------
问题 1：Gazebo 无法启动 / 黑屏

    原因：容器缺少 NVIDIA 驱动
    解决方法：
        export LIBGL_ALWAYS_SOFTWARE=1

问题 2：导航失败 / 机器人原地打转

    原因：目标点在障碍物膨胀区域内
    解决方法：
    • 检查 simulation.launch 中的目标坐标；
    • 确保使用航点导航：(0,0) → (-0.8,1.1) → (-1.0,1.5)；
    • 检查 move_base 参数中的 inflation_radius。

问题 3：机器人立即报告到达目标

    原因：TEB 规划器的 isGoalReached 判定过松
    解决方法：
    • 确认代码中已添加 waypoint_start_time_ 和 3 秒延迟；
    • 检查 teb_local_planner 参数配置。

问题 4：抓取失败 / 机械臂够不到物体

    原因：抓取距离阈值过大（0.7m）
    解决方法：
    • 检查代码中距离阈值是否已改为 0.5m；
    • 确认物体生成位置正确：(-1.5, 1.5)。

问题 5：TF 坐标变换错误

    原因：get_object_pose() 返回了错误的坐标系数据
    解决方法：
    • 确认代码中使用 TF Listener 正确进行了坐标变换；
    • 检查 tf 树是否完整（rosrun tf view_frames）。

问题 6：世界文件找不到

    原因：smallHouse.world 路径不正确
    解决方法：
    • 确认文件位于 /tiago_public_ws/src/pal_gazebo_worlds/worlds/；
    • 检查 pal_gazebo.launch 中的 world_name 参数。


================================================================================
提示：如遇到其他问题，请检查 ROS 日志文件（~/.ros/log/）或运行 roswtf 进行诊断。
================================================================================
