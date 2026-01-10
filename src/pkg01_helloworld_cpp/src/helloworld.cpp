/*
 需求：在终端输出文本hello world
 流程：
 	1.包含头文件
 	2.初始化ROS2客户端
 	3.创建节点指针
 	4.输出日至
 	5.释放资源

*/

#include "rclcpp/rclcpp.hpp"

/* int main(int argc, char** argv) {

	//2.初始化ROS2客户端
	rclcpp::init(argc, argv);
	
 	//3.创建节点指针
 	auto node = rclcpp::Node::make_shared("helloworld_node_cpp");
 	//4.输出日至
 	RCLCPP_INFO(node->get_logger(), "hello world!");
 	//5.释放资源
 	rclcpp::shutdown();
 	
 	return 0;
} */

//方式2(推荐)

//自定义类继承 Node
class MyNode : public rclcpp::Node {
public:
	MyNode():Node("hello_node_cpp") {
		RCLCPP_INFO(this->get_logger(), "hello_world!(继承的方式)");
	}
};

int main(int argc, char* argv[]) {
	//初始化
	rclcpp::init(argc, argv);

	//实例化自定义类
	auto node = std::make_shared<MyNode>();

	//资源释放
	rclcpp::shutdown();

	return 0;
}
