from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类--------------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取-----------------
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# 文件包含相关-------------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关----------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关----------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
# 获取功能包下share目录路径-------
from ament_index_python.packages import get_package_share_directory
import os

""" 
    需求: 演示Node的使用

    构造函数参数说明：
        :param: package 被执行程序所属的功能包
        :param: executable 可执行程序
        :param: name 节点名称
        :param: namespace 设置名命名空间
        :param: exec_name 设置程序标签
        :param: parameters 设置参数
        :param: remappings 实现话题重映射（重要）
        :param: ros_arguments 为节点传参  --ros-args xx yy （顺序）
        :param: arguments 为节点传参  xx yy zz --ros-args
"""

def generate_launch_description():
    # turtle1 = Node(
    #     package="turtlesim",
    #     executable="turtlesim_node",
    #     exec_name="my_lable",
    #     ros_arguments=["--remap", "__ns:=/t2"]
    #         #ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t1
    # )
    turtle2 = Node(
        package="turtlesim",
        executable="turtlesim_node",
        respawn=True,
        name = "haha",
        # 方式1,动态往节点里传入数据（直接设置）
        # parameters=[{"background_r": 255, "background_g": 0, "background_b": 0}]
        # 方式2(更常用), 读取yaml文件(通过yaml文件的绝对路径读取)
        #parameters=["/home/chg/ws02_tools/install/cpp01_launch/share/cpp01_launch/config/haha.yaml"]
        # 优化：动态获取路径
        parameters=[os.path.join(get_package_share_directory("cpp01_launch"),"config", "haha.yaml")]

    )

    return LaunchDescription([turtle2])