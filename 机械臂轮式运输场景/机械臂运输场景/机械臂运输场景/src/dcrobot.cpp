/**
* MIT License
*
* Copyright(c) 2021
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* 
* @file dcrobot.cpp
* @author Driver: Ameya Konkar (ameyakonk), Navigator: Hrushikesh Budhale
* @brief LIbrary file for DCRobot class
* @version 0.1
* @date 2021-11-29
* 
* @copyright Copyright (c) 2021
* 
*/

#include <dcrobot/dcrobot.hpp>

DCRobot::DCRobot(ros::NodeHandle* node_handle):
                    detectObj(node_handle),
                    graspObj(node_handle),
                    navigator(node_handle),
                    tf_listener_(this->tfBuffer_) {
    nh_ = node_handle;
    state_ = robotState::STARTING;
    ROS_INFO_STREAM("[DCRobot] DCRobot object initialized");
}

// Known object position in map frame (hardcoded)
static const double KNOWN_OBJECT_X = -1.5;
static const double KNOWN_OBJECT_Y = 1.5;
static const double KNOWN_OBJECT_Z = 0.025;

// Navigation offset: stop this far from the object so robot doesn't stand on it
// Robot stops south of the object (negative y direction), facing the object
static const double NAV_OFFSET_Y = 0.7;  // stop 0.7m south of object

// Fixed waypoints for navigation (map frame coordinates)
// Waypoint 1: intermediate point away from starting position
static const double WAYPOINT1_X = -0.8;
static const double WAYPOINT1_Y = 1.1;
// Waypoint 2: stop south of object for grasping (not on top of it)
static const double WAYPOINT2_X = -1.0;
static const double WAYPOINT2_Y = 1.5;  // 0.4m south of object

void DCRobot::handle_states() {
    //  Checks if the state of the robot has been changed
    if (pre_state_ != state_) {
        ROS_INFO_STREAM("[DCRobot] in state: " << state_ << "________________");
        pre_state_ = state_;
    }

    //  State Switches to the respective state
    switch (state_) {
        case STARTING:
            // Create a point head action client to move the TIAGo's head
            create_point_head_client(point_head_client_);
            set_head_down();
            ROS_INFO_STREAM("[DCRobot] Setting head position");
            state_ = IDLE;
            break;

        case IDLE:
            // Navigate to first waypoint
            {
                ROS_INFO_STREAM("[DCRobot] Navigating to waypoint 1: (" 
                                << WAYPOINT1_X << ", " << WAYPOINT1_Y << ")");
                geometry_msgs::Pose navPose;
                navPose.position.x = WAYPOINT1_X;
                navPose.position.y = WAYPOINT1_Y;
                navPose.position.z = KNOWN_OBJECT_Z;
                navigator.set_object_pose_as_goal(navPose);
                waypoint_start_time_ = ros::Time::now();
            }
            state_ = MOVING_TO_CHECKPOINT;
            break;

        case MOVING_TO_CHECKPOINT:
            // Check if waypoint 1 reached, then go to waypoint 2
            // Wait at least 3 seconds before checking to avoid immediate true
            if ((ros::Time::now() - waypoint_start_time_).toSec() > 3.0 && navigator.is_goal_reached()) {
                ROS_INFO_STREAM("[DCRobot] Waypoint 1 reached, going to waypoint 2");
                geometry_msgs::Pose navPose;
                navPose.position.x = WAYPOINT2_X;
                navPose.position.y = WAYPOINT2_Y;
                navPose.position.z = KNOWN_OBJECT_Z;
                navigator.set_object_pose_as_goal(navPose);
                state_ = MOVING_TOWARDS_OBJECT;
            }
            break;

        case TURNING_AROUND:
            // Not used in this simplified flow
            state_ = MOVING_TOWARDS_OBJECT;
            break;

        case MOVING_TOWARDS_OBJECT:
            // Check if near object
            if (is_obj_within_reach()) {
               navigator.stop_moving();
               state_ = PICKING_OBJECT;
            }
            break;

        case PICKING_OBJECT:
            // Acion of object picking
            pick_up_object();
            navigator.set_bin_location_as_goal();
            state_ = MOVING_TO_BIN_LOCATION;
            break;

        case MOVING_TO_BIN_LOCATION:
            // Moving the object to the bin location
            if (navigator.is_goal_reached())
                state_ = PLACING_OBJECT;
            break;

        case PLACING_OBJECT:
            // Acion of object placing at the bin location
            place_object();
            ROS_INFO_STREAM("[DCRobot] Task accomplished!");
            state_ = STOP;
            break;
    }
}

bool DCRobot::is_obj_within_reach() {
    //  Checks if object is within reach of the robot arm.
    auto objectPose = get_object_pose("base_link");
    double dx = objectPose.position.x;
    double dy = objectPose.position.y;
    double distance = std::sqrt(dx*dx + dy*dy);
    ROS_INFO_STREAM("[DCRobot] object is at distance: " << distance);
    if (distance < 0.5) return true;
    return false;
}

geometry_msgs::Pose DCRobot::get_object_pose(std::string wrt) {
    // Return the known object position, transformed to the requested frame
    geometry_msgs::Pose objectPoseMap;
    objectPoseMap.position.x = KNOWN_OBJECT_X;
    objectPoseMap.position.y = KNOWN_OBJECT_Y;
    objectPoseMap.position.z = KNOWN_OBJECT_Z;
    objectPoseMap.orientation.x = 0.0;
    objectPoseMap.orientation.y = 0.0;
    objectPoseMap.orientation.z = 0.0;
    objectPoseMap.orientation.w = 1.0;
    
    // If requesting map frame, return directly
    if (wrt == "map" || wrt == "") {
        return objectPoseMap;
    }
    
    // Transform from map frame to requested frame
    geometry_msgs::PoseStamped poseStampedMap;
    poseStampedMap.pose = objectPoseMap;
    poseStampedMap.header.frame_id = "map";
    poseStampedMap.header.stamp = ros::Time::now();
    
    geometry_msgs::PoseStamped poseStampedWrt;
    try {
        tfBuffer_.transform(poseStampedMap, poseStampedWrt, wrt, ros::Duration(1.0));
        return poseStampedWrt.pose;
    } catch (tf2::TransformException& ex) {
        ROS_WARN_STREAM("[DCRobot] TF transform failed: " << ex.what());
        // Fallback: return map pose if transform fails
        return objectPoseMap;
    }
}

void DCRobot::pick_up_object() {
    //  Picking up the object
    graspObj.move_to_object_pose(get_object_pose("base_link"));
    graspObj.move_to_pick_pose();
    ROS_INFO_STREAM("[DCRobot] Object Picked up");
}

void DCRobot::place_object() {
    //  Placing the object
    graspObj.move_to_place_pose();
    graspObj.move_to_pick_pose();
    ROS_INFO_STREAM("[DCRobot] Object Placed");
}

void DCRobot::set_head_down() {
    //  Setting the head orientation of the robot.
    std::string camera_frame = "/xtion_rgb_optical_frame";
    geometry_msgs::PointStamped pointStamped;
    pointStamped.header.frame_id = camera_frame;
    pointStamped.header.stamp    = ros::Time::now();
    pointStamped.point.x = 0.0;
    pointStamped.point.y = 0.8;    // value for looking down
    pointStamped.point.z = 1.0;

    // build the action goal
    control_msgs::PointHeadGoal goal;

    goal.pointing_frame = camera_frame;
    goal.pointing_axis.x = 0.0;
    goal.pointing_axis.y = 0.0;
    goal.pointing_axis.z = 1.0;
    goal.min_duration = ros::Duration(1.0);
    goal.max_velocity = 0.25;
    goal.target = pointStamped;

    point_head_client_->sendGoal(goal);
    ros::Duration(0.5).sleep();
}

void DCRobot::create_point_head_client(PointHeadClientPtr& actionClient) {
    ROS_INFO("[DCRobot] Creating action client to head controller ...");
    actionClient.reset(
                    new PointHeadClient("/head_controller/point_head_action"));

    int iterations = 0, max_iterations = 3;
    // Wait for head controller action server to come up
    while (!actionClient->waitForServer(ros::Duration(2.0)) && ros::ok()
                                            && iterations < max_iterations) {
        ROS_DEBUG(R"([DCRobot] Waiting for the point_head_action server to come
                                                                         up)");
        ++iterations;
  }

  if ( iterations == max_iterations )
    throw std::runtime_error(R"(Error in create_point_head_client: 
                                head controller action server not available)");
}
