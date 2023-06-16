#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>
#include <iostream>

ros::Subscriber sub;
std::string info;

//completar
void cbComm(const ::ConstPtr& msg) // função que é executada quando recebe mensagem
{
    //completar
    std::cout << data.c_str() << std::endl; // imprime o conteúdo da mensagem
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "subscriber_node"); //inicia o nó
    ros::NodeHandle nh;

    std::cout << "aqui vai aparecer o que escreveres no outro terminal\n";
    
    // completar
    sub = nh.("nome_do_topico", 1, ); // subscreve ao tópico
    
    // completar
     // faz com que o ROS espere por mensagens novas
   
    return 0;

}