/**
* This file is part of Fast-Planner.
*
* Copyright 2019 Boyu Zhou, Aerial Robotics Group, Hong Kong University of Science and Technology, <uav.ust.hk>
* Developed by Boyu Zhou <bzhouai at connect dot ust dot hk>, <uv dot boyuzhou at gmail dot com>
* for more information see <https://github.com/HKUST-Aerial-Robotics/Fast-Planner>.
* If you use this code, please cite the respective publications as
* listed on the above website.
*
* Fast-Planner is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Fast-Planner is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with Fast-Planner. If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef _EDT_ENVIRONMENT_H_
#define _EDT_ENVIRONMENT_H_

#include <Eigen/Eigen>
#include <geometry_msgs/PoseStamped.h>
#include <iostream>
#include <ros/ros.h>
#include <utility>

#include <plan_env/obj_predictor.h>
#include <plan_env/sdf_map.h>
#include <plan_env/raycast.h>

using std::cout;
using std::endl;
using std::list;
using std::pair;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

namespace fast_planner {
class EDTEnvironment {
private:
  /* data */
  // DYNAMIC
  // 目前这里面存放的不是predictor，而是预测出来的各个物体的轨迹方程ObjPrediction
  // 为什么不直接把predictor放进来呢？
  ObjPrediction obj_prediction_;
  ObjScale obj_scale_;
  double resolution_inv_;
  double distToBox(int idx, const Eigen::Vector3d& pos, const double& time);//和动态物体相关的？
  double minDistToAllBox(const Eigen::Vector3d& pos, const double& time);

public:
  EDTEnvironment(/* args */) {
  }
  ~EDTEnvironment() {
  }

  SDFMap::Ptr sdf_map_;
  RayCaster raycasterLineCheck_;
  void init();
  void setMap(SDFMap::Ptr map);
  void setObjPrediction(ObjPrediction prediction);
  void setObjScale(ObjScale scale);
  //获取8邻域的点的esdf信息
  void getSurroundDistance(Eigen::Vector3d pts[2][2][2], double dists[2][2][2]);
  void getSurroundDistance2D(Eigen::Vector3d pts[2][2], double dists[2][2]);
  void interpolateTrilinear(double values[2][2][2], const Eigen::Vector3d& diff,
                                                     double& value, Eigen::Vector3d& grad);
  void interpolateTrilinear2D(double values[2][2],
                              const Eigen::Vector3d& diff,
                              double& value,
                              Eigen::Vector3d& grad);
  void evaluateEDTWithGrad(const Eigen::Vector3d& pos, double time,
                                                    double& dist, Eigen::Vector3d& grad);
  void evaluateEDTWithGrad2D(const Eigen::Vector3d& pos, double time, double& dist, Eigen::Vector3d& grad);                                           
  double evaluateCoarseEDT(Eigen::Vector3d& pos, double time, bool only2D = false);
  void getMapRegion(Eigen::Vector3d& ori, Eigen::Vector3d& size) //原点和size
  {
    sdf_map_->getRegion(ori, size);
  }
  double getMinDisViaLineSeg2D(const Eigen::Vector3d &pt1, const Eigen::Vector3d &pt2);
  typedef shared_ptr<EDTEnvironment> Ptr;
};

}  // namespace fast_planner

#endif