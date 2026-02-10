"""  
    需求：先发布laser到base_link的坐标相对关系，再发布camera到base_link的坐标系相对关系，
         求解laser到cameraa的坐标系相对关系。
    流程：
        1.导包；
        2.初始化ROS2客户端；
        3.自定义节点类；
            3-1.创建一个缓存对象，融合多个坐标系相对关系为一颗坐标树；
            3-2.创建一个监听器，指定缓存对象，会将所有广播器广播的数据写入缓存
            3-3.编写一个定时器，循环实现转换。
        4.调用spain函数，并传入节点对象；
        5.资源释放。 


"""
# 1.导包；
import rclpy
from rclpy.node import Node
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener
from rclpy.time import Time

# 3.自定义节点类；
class TFListenerPy(Node):
    def __init__(self):
        super().__init__("tf_listener_py_node_py")
        # 3-1.创建一个缓存对象，融合多个坐标系相对关系为一颗坐标树；
        self.buffer = Buffer()
        # 3-2.创建一个监听器，指定缓存对象，会将所有广播器广播的数据写入缓存
        self.listener = TransformListener(self.buffer, self)
        # 3-3.编写一个定时器，循环实现转换。
        self.timer = self.create_timer(1.0, self.on_timer)
    
    def on_timer(self):
        # 判断是否可以实现转换
        if self.buffer.can_transform("camera", "laser", Time()):
            ts = self.buffer.lookup_transform("camera", "laser", Time())
            self.get_logger().info("转换后的数据------------")
            self.get_logger().info(
                "转换后的结果, 父坐标系:%s, 子坐标系:%s, 偏移量:(%.2f, %.2f, %.2f)"
                %(ts.header.frame_id, ts.child_frame_id,
                  ts.transform.translation.x,
                  ts.transform.translation.y,
                  ts.transform.translation.z)
            )

        else:
            self.get_logger().info("转换失败....")

def main():
    # 2.初始化ROS2客户端；
    rclpy.init()
    # 4.调用spain函数，并传入节点对象；
    rclpy.spin(TFListenerPy())
    # 5.资源释放。 
    rclpy.shutdown()

if __name__ == '__main__':
    main()