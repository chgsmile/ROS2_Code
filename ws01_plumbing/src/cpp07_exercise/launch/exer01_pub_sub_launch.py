from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess,RegisterEventHandler
from launch.event_handlers import OnProcessExit

def generate_launch_description():
    # 1.启动两个 turtlesim_node，其中一个要设置命名空间
    t1 = Node(package="turtlesim", executable="turtlesim_node")
    t2 = Node(package="turtlesim", executable="turtlesim_node", namespace="t2")

    # 2.控制第二个乌龟调头
    rotate = ExecuteProcess(
        # 3.14个弧度，即180度
        cmd=["ros2 action send_goal /t2/turtle1/rotate_absolute turtlesim/action/RotateAbsolute \"{'theta': 3.14}\""],
        output="both", #既输出到文件，又输出到终端
        shell=True  #把列表里的数据当作终端指令来执行
    )

    # 3.调用自定义的节点，该节点的调用顺序有要求（要在调头完毕才能执行）
    exer01 = Node(package="cpp07_exercise", executable="exer01_pub_sub")
    # 怎么控制节点的执行顺序？ -- 注册事件
    # 创建事件注册对象，在对象当中声明针对哪个目标节点，在哪个事件触发时，执行哪种操作？
    register_rotate_exit_event = RegisterEventHandler(
        # 创建一个对象
        event_handler = OnProcessExit(  # 触发动作
            target_action=rotate, # 目标节点
            on_exit=exer01  # 触发执行的事件
        )
    )
    

    return LaunchDescription([t1, t2, rotate, register_rotate_exit_event])