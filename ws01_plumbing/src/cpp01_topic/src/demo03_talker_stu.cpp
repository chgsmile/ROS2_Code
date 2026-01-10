#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using base_interfaces_demo::msg::Student; //引入声明
//等价于
// using Student = base_interfaces_demo::msg::Student; //类型别名

using namespace std::chrono_literals;

class TalkerStu : public rclcpp::Node {
public:
    TalkerStu() : Node("talkerstu_node_cpp"), age(8) {
        RCLCPP_INFO(this->get_logger(), "创建发布节点！");
        publisher_ = this->create_publisher<Student>("chatter_stu", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&TalkerStu::on_timer, this));
    }

private:
    void on_timer() {
        auto stu = Student();
        stu.name = "huluwa";
        stu.age = age;
        stu.height = 2.20;
        age++;
        publisher_->publish(stu);
        RCLCPP_INFO(this->get_logger(), "发布的消息: (%s, %d, %.2f)", stu.name.c_str(), stu.age, stu.height);
    }
    rclcpp::Publisher<Student>::SharedPtr publisher_;
    //ROS2推荐写法
    rclcpp::TimerBase::SharedPtr timer_;

    //标准智能指针写法
    // std::shared_ptr<rclcpp::TimerBase> timer_;
    //类型推导：通常可能不幸，因为TimerBase可能是抽象类
    // auto timer_ = std::make_shared<rclcpp::TimerBase>();
    int age;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TalkerStu>());

    rclcpp::shutdown();

    return 0;
}