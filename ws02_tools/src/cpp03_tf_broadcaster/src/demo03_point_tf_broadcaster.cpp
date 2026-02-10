/* 
    需求：发布相对于laser坐标系的座标点数据.
    流程：
        1.包含头文件
        2.初始化ROS2客户端
        3.自定义节点类
            3-1.创建发布方
            3-2.创建定时器
            3-3.回调函数中组织并发布消息
        4.调用spin函数，并传入节点对象指针
        5.资源释放
*/


#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

using namespace::std::chrono_literals;

class PointBroadcaster : public rclcpp::Node {
public:
    PointBroadcaster() : Node{"point_broadcaster_cpp"}, x{0.0} {
        // 3-1.创建发布方
        point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point", 10);
        // 3-2.创建定时器
        timer_ = this->create_wall_timer(1s, std::bind(&PointBroadcaster::on_timer, this));
        
    }

private:
    // 3-3.回调函数中组织并发布消息
    void on_timer() {
        //组织消息
        geometry_msgs::msg::PointStamped ps;
        ps.header.stamp = this->now();
        ps.header.frame_id = "laser"; //所属坐标系
        x += 0.05;
        ps.point.x = x;
        ps.point.y = 0.0;
        ps.point.z = -0.1;

        //发布消息
        point_pub_->publish(ps);
    }
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    double x;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<PointBroadcaster>());

    rclcpp::shutdown();
    return 0;
}