/* 
  需求：修改turtlesim_node 的背景色
  流程：
    1.包含头文件
    2.初始化ROS2客户端
    3.自定义节点类
        3-1.创建参数客户端
        3-2.连接参数服务端
        3-3.更新参数
    4.创建节点对象指针，并调用其函数
    5.资源释放
*/


#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;

class Exer06Param : public rclcpp::Node {
public:
    Exer06Param() : Node{"exer06_param_node_cpp"} {
        RCLCPP_INFO(this->get_logger(), "参数客户端");
        // 3-1.创建参数客户端
        client_ = std::make_shared<rclcpp::SyncParametersClient>(this, "/turtlesim");
    }
    // 3-2.连接参数服务端
    bool connect_server() {
        while (!client_->wait_for_service(1s)) {
            if (!rclcpp::ok()) {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "客户端强制退出!");
                return false;
            }
            RCLCPP_INFO(this->get_logger(), "服务连接中...");
        }
        return true;
    }
    // 3-3.更新参数
    void update_param() {
        //背景色需要递进修改
        //background_r [0,255]
        // 1.获取参数
        int red = client_->get_parameter<int>("background_r");

        // 2.编写循环,修改参数(通过休眠控制修改频率)
        rclcpp::Rate rate(30.0);
        /* 
            需求：要求背景色渐变, 有浅变深，再反之
            实现：
                1.一个完整周期计数是511个，中间值是255
                2.创建一个计数器，初始值和red相同，递增的步进制也和red相同，取值[0, 511]
                3.当计数器在[0,255]时，red递增；如果是[256, 511]，red递减
                4.当计数器大于等于511时，归0
        */
       int count = red;
        while (rclcpp::ok()) {
            // red += 5;
            count <= 255 ? red += 5 : red -= 5;

            count += 5;
            if (count > 511) {
                count = 0;
            }
            //修改服务端参数
            client_->set_parameters({rclcpp::Parameter("background_r", red)});

            rate.sleep();
        }
    }

private:
    rclcpp::SyncParametersClient::SharedPtr client_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    // rclcpp::spin(std::make_shared<Exer06Param>());
    auto client = std::make_shared<Exer06Param>();
    if (!client->connect_server()) {
        return 1;
    }
    client->update_param();

    rclcpp::shutdown();
    return 0;
}