#include "gate.h"
#include "path_marker.h"
#include "marker.h"
#include "torpedo.h"
#include "ros/ros.h"
#include <string>
#include <std_msgs/String.h>

#include <master_layer/CurrentTask.h>

std::string current_task = "marker";
std::string previous_task = "";

bool changeCurrentTask(master_layer::CurrentTask::Request &req,
                       master_layer::CurrentTask::Response &res)
{
    current_task = req.current_task;
    res.status = true;
    return true;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "vision_node");
    ros::NodeHandle nh;
    ros::Time::init();

    ros::Duration(1).sleep();

    Gate gate;
    Torpedo torpedo;
    PathMarker path_marker;
    Marker marker;
    ros::ServiceServer service = nh.advertiseService("anahita/current_task", changeCurrentTask);

    ros::Rate loop_rate(10);

    ROS_INFO("Vision Node started");

    while (ros::ok())
    {
        if (current_task != previous_task)
        {
            if (current_task == "gate")
            {
                ROS_INFO("gate_task");
                gate.frontTaskHandling(true);
            }
            if (previous_task == "gate")
            {
                gate.frontTaskHandling(false);
            }
            if (current_task == "marker") {
                ROS_INFO("marker task");
                marker.frontTaskHandling(true);
            }
            if (previous_task == "marker") {
                marker.frontTaskHandling(false);
            }
            previous_task = current_task;
        }
        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
