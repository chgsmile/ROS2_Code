/* 
    需求：创建客户端，组织数据并提交，然后处理相应结果（需要关注业务流程）。
    流程：
        前提：main函数中需要判断提交的参数个数是否正确
        1.包含头文件
        2.初始化ROS2客户端
        3.自定义节点类
            3-1.创建客户端
            3-2.连接服务器（对于服务通信而言，如果客户端链接不到服务器，不能发送数据）；
            3-3.发送请求
        4.创建对象指针
            需要调用连接服务的函数，根据连接结果做下一步处理。
            连接服务后，调用请求发送函数
            再处理响应结果。
        5.资源释放
*/

// #include "rclcpp/rclcpp.hpp"
// #include "base_interfaces_demo/srv/add_ints.hpp"

// using base_interfaces_demo::srv::AddInts;
// using namespace std::chrono_literals;

// class AddIntsClient : public rclcpp::Node {
// public:
//     AddIntsClient() : Node("add_ints_client_node_cpp") {
//         RCLCPP_INFO(this->get_logger(), "客户端节点创建"); 

//         client_ = this->create_client<AddInts>("add_ints");
//     }

//     //连接服务器
//     bool connect_server() {
//         // client_->wait_for_service(1s);
//         while (!client_->wait_for_service(2s)) {
//             //对ctrl+c做特殊处理
//             //1.怎么判断ctrl+c按下？ 2.如何处理？
//             //按下ctrl+c是结束ROS2程序，意味着要释放资源，比如要关闭context（ROS2里的容器给扔了)
//             //因此如果用this->get_logger会报错; 但如果不检查，循环会一直执行，程序无法正常退出。因为wait_for_service不会自动退出程序，它只关心服务是否可用
//             if (!rclcpp::ok()) {
//                 RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "强行终止客户端");
//                 return false;
//             }
//             RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接中。。。");
//         }

//         return true;
//     }

//     //发送请求
//     //编写发送请求函数：---参数是两个整型数据，返回值是提交服务端的返回结果
//     rclcpp::Client<AddInts>::FutureAndRequestId sent_request(int num1, int num2) {
//         //组织请求数据

//         //发送
//         /* 
//             rclcpp::Client<base_interfaces_demo::srv::AddInts>::FutureAndRequestId 
//             async_send_request(std::shared_ptr<base_interfaces_demo::srv::AddInts_Request> request) //AddInts::Request
//         */
//         auto request = std::make_shared<AddInts::Request>();
//         request->num1 = num1;
//         request->num2 = num2;
//         return client_->async_send_request(request);
//     }

// private:
//   rclcpp::Client<AddInts>::SharedPtr client_;
// };

// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         //除了通过节点获取日志对象，还可以通过rclcpp里的get_logger直接获取，但要传一个logger实例的名称.
//         RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "请输入两个整型数字");
//         return 1;
//     }
//     rclcpp::init(argc, argv);

// //   rclcpp::spin(std::make_shared<AddIntsClient>())
// //  这里客户端不挂起，是基于业务考虑的。客户端发送完数据以后，就可以结束了。没必要一直挂起。

//     //创建客户端对象
//     auto client = std::make_shared<AddIntsClient>();

//     bool flag = client->connect_server();
//     if (!flag) {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务器连接失败，程序退出");
//         // RCLCPP_INFO(client->get_logger(), "强行终止客户端");
//         return 0;
//     }

//     //调用请求提交函数，接受并处理相应结果。
//     auto future = client->sent_request(atoi(argv[1]), atoi(argv[2]));

//     //处理响应
//     if (rclcpp::spin_until_future_complete(client, future) == rclcpp::FutureReturnCode::SUCCESS) {
//         RCLCPP_INFO(client->get_logger(), "响应成功!, sum = %d", future.get()->sum);  //future.get()获取响应对象的指针
        
//     }
//     else {
//         RCLCPP_INFO(client->get_logger(), "响应失败!");
//     }

//     rclcpp::shutdown();

//     return 0;
// }
//------------------------------------------------------------------------------------
// #include "rclcpp/rclcpp.hpp"
// #include "base_interfaces_demo/srv/add_ints.hpp"

// using base_interfaces_demo::srv::AddInts;
// using namespace std::chrono_literals;

// class AddIntsClient : public rclcpp::Node {
// public:
//     AddIntsClient() : Node("add_ints_client_node_cpp") {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "客户端节点创建");
//         client_ = this->create_client<AddInts>("add_ints");
//     }

//     bool connect_server() {
//         while (!client_->wait_for_service(2s)) {
//             if (!rclcpp::ok()) {
//                 RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接失败，程序退出");
//                 return 0;
//             }
//             RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接中。。。");
//         }
//         return true;
//     }

//     rclcpp::Client<AddInts>::FutureAndRequestId sent_request(int num1, int num2) {
//         auto request = std::make_shared<AddInts::Request>();
//         request->num1 = num1;
//         request->num2 = num2;

//         return client_->async_send_request(request);
//     }

// private:
//     rclcpp::Client<AddInts>::SharedPtr client_;
// };

// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "请输入两个整型数字");
//         return 1;
//     }

//     rclcpp::init(argc, argv);

//     //创建客户端对象
//     auto client = std::make_shared<AddIntsClient>();
    
//     bool flag = client->connect_server();
//     if (!flag) {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务器连接失败，程序推出");
//         return 0;
//     }

//     auto future = client->sent_request(atoi(argv[1]), atoi(argv[2]));

//     if (rclcpp::spin_until_future_complete(client, future) == rclcpp::FutureReturnCode::SUCCESS) {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "响应成功, sum = %d", future.get()->sum);
//     }
//     else {
//         RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "响应失败!");
//     }

//     rclcpp::shutdown();

//     return 0;
// }



#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"

using base_interfaces_demo::srv::AddInts;
using namespace::std::chrono_literals;

class AddIntCilent : public rclcpp::Node {
public:
    AddIntCilent() : Node("add_int_client_node_cpp") {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "客户端节点创建。");
        client_ = this->create_client<AddInts>("add_ints");
    }

    bool connect_server() {
        while (!client_->wait_for_service(2s)) {
            if (!rclcpp::ok()) {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接失败，程序退出");
                return 0;
            }
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务连接中。。。");
        }
        return true;
    }

    rclcpp::Client<AddInts>::FutureAndRequestId send_request(int num1, int num2) {
        auto request = std::make_shared<AddInts::Request>();
        request->num1 = num1;
        request->num2 = num2;

        return client_->async_send_request(request);
    }

private: 
    rclcpp::Client<AddInts>::SharedPtr client_;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "请输入两个整型数字");
        return 1;
    }

    rclcpp::init(argc, argv);

    auto client = std::make_shared<AddIntCilent>();

    bool flag = client->connect_server();
    if (!flag) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "服务器连接失败，程序退出");
        return 0;
    }

    auto future = client->send_request(atoi(argv[1]), atoi(argv[2]));

    if (rclcpp::spin_until_future_complete(client, future) == rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "响应成功, sum = %d", future.get()->sum);
    }
    else {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "响应失败!");
    }

    rclcpp::shutdown();

    return 0;
}