/* 
  需求：创建参数服务端并操作参数
  流程：
    1.包含头文件
    2.初始化ROS2客户端
    3.自定义节点类
        3-1.增
        3-2.查
        3-3.改
        3-4.删
    4.调用spin函数并传入节点对象指针
    5.资源释放
*/


#include "rclcpp/rclcpp.hpp"

class ParamServer : public rclcpp::Node {
public:
    //如果允许删除参数，需要通过 NodeOptions 声明
    ParamServer() : Node("param_server_node_cpp", rclcpp::NodeOptions().allow_undeclared_parameters(true)) {
        RCLCPP_INFO(this->get_logger(), "参数服务端创建了!");
        //一个普通的节点就可以作为参数服务端，因此不需要单独显式创建
    }

    //3-1.增
    void declare_param() {
        RCLCPP_INFO(this->get_logger(), "----------增--------");
        this->declare_parameter("car_name", "tiger");
        this->declare_parameter("width", 1.55);
        this->declare_parameter("wheels", 5);

        //也可以用于设置新参数，但必须保证rclcpp::NodeOptions().allow_undeclared_parameters(true)被调用。
        this->set_parameter(rclcpp::Parameter("height", 2.00)); //可以设置新值
    }

    //3-2.查
    void get_param() {
        RCLCPP_INFO(this->get_logger(), "----------查--------");
        // this->get_parameter() 根据参数键来获取参数对象
        // this->get_parameters() 根据参数键来获取一些参数对象
        // this->has_parameter()   判断是否包含某个参数
        //获取指定参数
        auto car = this->get_parameter("car_name");
        RCLCPP_INFO(this->get_logger(), "key = %s, value = %s", car.get_name().c_str(), car.as_string().c_str());
        RCLCPP_INFO(this->get_logger(), "key = %s, value = %s", car.get_name().c_str(), car.as_string().c_str());
        //获取一些参数
        auto params = this->get_parameters({"car_name", "width", "wheels"});
        for (auto &&param : params) {
            RCLCPP_INFO(this->get_logger(), "(%s = %s)", param.get_name().c_str(), param.value_to_string().c_str());
        }
        //判断是否包含
        RCLCPP_INFO(this->get_logger(), "是否包含car_name? %d", this->has_parameter("car_name"));
        RCLCPP_INFO(this->get_logger(), "是否包含height? %d", this->has_parameter("height"));
    }

    //3-3.改
    void update_param() {
        RCLCPP_INFO(this->get_logger(), "----------改--------");
        //改height对象的值
        this->set_parameter(rclcpp::Parameter("height", 1.75)); 
        RCLCPP_INFO(this->get_logger(), "height = %.2f", this->get_parameter("height").as_double());
    }

    //3-4.删
    void del_param() {
        RCLCPP_INFO(this->get_logger(), "----------删--------");
        // this->undeclare_parameter("car_name"); 不能删除声明的参数
        this->undeclare_parameter("height"); //可以删除没有声明然后设置的参数
        RCLCPP_INFO(this->get_logger(), "删除后还包含height吗? %d", this->has_parameter("height"));
    }
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);

    auto node = std::make_shared<ParamServer>();
    node->declare_param();
    node->get_param();
    node->update_param();
    node->del_param();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}