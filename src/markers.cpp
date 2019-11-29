#include "markers.h"

void Vmarkers::odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
  odom_x = msg->pose.pose.position.x;
  odom_y = msg->pose.pose.position.y;
}


Vmarkers::Vmarkers() {
  float odom_x= 0;
  float odom_y= 0;
  uint8_t count= 0;
  //sub = n.subscribe("/odom", 1000, &Vmarkers::odomCallback);
  vmarker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1  );
  setMarkerProperties();
}


int Vmarkers::setMarkerProperties() {
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.

    //visualization_msgs::Marker marker;
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();
   
    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "vmarkers";
    marker.id = 0;
   
    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER

    marker.type = visualization_msgs::Marker::CUBE;
	  marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = -2.0;
    marker.pose.position.y = 1.0;
    marker.pose.position.z = 0.0;
    marker.pose.orientation.w = 1.0;

	  marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();
   }


 int Vmarkers::visualizeMarker(int argc, char** argv) {
  ros::Rate r(20);
	bool status = false;
	while (ros::ok()) {
	    float actual_dis_x, actual_dis_y;
	    float threshold = 0.2;
	      if (!status)
	      {
			vmarker_pub.publish(marker);
			actual_dis_x = fabs(marker.pose.position.x - odom_x);
			actual_dis_y = fabs(marker.pose.position.y - odom_y);
			if( (actual_dis_x < threshold) && (actual_dis_y < threshold) )	{
			  marker.action = visualization_msgs::Marker::DELETE;			  
			  vmarker_pub.publish(marker);			  
			  status = true;
			}
	      }
	      else
	      {
		  marker.action = visualization_msgs::Marker::ADD;
		  marker.pose.position.x = 0.0;
		  marker.pose.position.y = 0.0;
		  marker.pose.position.z = 0.0;
		  marker.pose.orientation.w = -1.0;
		  ros::Duration(5.0).sleep();
		  vmarker_pub.publish(marker);
	      }
	    ros::spinOnce();
	    r.sleep();
    }
    return 0;
}

Vmarkers::~Vmarkers() {}

 

