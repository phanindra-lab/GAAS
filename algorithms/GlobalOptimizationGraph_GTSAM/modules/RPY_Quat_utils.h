#ifndef RPY_QUAT_UTILS_H
#define RPY_QUAT_UTILS_H

#include <glog/logging.h>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
//#include <ar_track_alvar_msgs/AlvarMarkers.h>
//#include "tf/transform_datatypes.h"
//#include "LinearMath/btMatrix3x3.h"

void R2ypr(Eigen::Matrix3d &R,double& y,double& p,double& r)
{
        Eigen::Vector3d n = R.col(0);
        Eigen::Vector3d o = R.col(1);
        Eigen::Vector3d a = R.col(2);

        //Eigen::Vector3d ypr(3);
        y = atan2(n(1), n(0));
        p = atan2(-n(2), n(0) * cos(y) + n(1) * sin(y));
        r = atan2(a(0) * sin(y) - a(1) * cos(y), -o(0) * sin(y) + o(1) * cos(y));
}
Matrix3d ypr2R(double y,double p,double r )//const Vector3d& ypr)
{

  // Eigen::Matrix<double, 3, 3> Rz;
  Matrix3d Rz;
  Rz << cos(y), -sin(y), 0,
      sin(y), cos(y), 0,
      0, 0, 1;

  // Eigen::Matrix<double, 3, 3> Ry;
  Matrix3d Ry;
  Ry << cos(p), 0., sin(p),
      0., 1., 0.,
      -sin(p), 0., cos(p);

  // Eigen::Matrix<double, 3, 3> Rx;
  Matrix3d Rx;
  Rx << 1., 0., 0.,
      0., cos(r), -sin(r),
      0., sin(r), cos(r);
  return Rz * Ry * Rx;
}

void getRPYFromQuat(double x,double y,double z,double w,double& roll,double& pitch,double& yaw)
{
    //tf::Quaternion quat(x,y,z,w);
    //double roll, pitch, yaw;
    //tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
    Quaterniond quat(w,x,y,z);
    auto m = quat.toRotationMatrix();
    R2ypr(m,yaw,pitch,roll);
}


void getNewQuaternionFromOriginalQuaternionAndNewYawAngle(double x,double y,double z,double w,double new_yaw_rad,
                                                          double& newx,double& newy,double& newz,double& neww)
{
    double R,P,Y;//original RPY angles.
    getRPYFromQuat(x,y,z,w,R,P,Y);
    //tf::Matrix3x3 newRmat;
    //newRmat.setRPY(R,P,new_yaw_rad);
    LOG(INFO)<<"In getNewQuaternionFromOriginalQuaternionAndNewYawAngle(): old rad:"<<R<<";new rad:"<<new_yaw_rad<<endl;
    Matrix3d mat_rotate = ypr2R(new_yaw_rad,P,Y);
    Quaterniond q_(mat_rotate);
    /*newRmat.setRPY(new_yaw_rad,P,Y);//maybe we should decompose the rotation matrix directly.
    tf::Quaternion quat_out_;
    newRmat.getRotation(quat_out_);*/
    newx = q_.x();
    newy = q_.y();
    newz = q_.z();
    neww = q_.w();
}

#endif