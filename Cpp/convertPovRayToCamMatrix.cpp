void convertPovRayToCamMatrix(   //Inputs
                                 Mat_<double>& cam_dir,
						         Mat_<double>& cam_up
						         double cam_angle,
						         double w,
						         double h,
						         //Outputs
						         Mat_<double>& cameraMatrix,
						         Mat_<double>& cameraAffinePose){
    //cameraMatrix is 3x3, takes [x,y,z]' in camera frame to camera plane
    //cameraAffinePose is 4x4 and takes points in world frame to camera frame
    // to reproject from camera image to camera frame at a given depth d do:
    // p_cf=d*cameraMatrix.inv()*p_ci
    // p_cf=p_cf/p_cf(2)


    //povray uses camright for scaling. We want scaling only in the camera matrix, so we recompute cam_right as a cross product
    Mat_<double> cam_right(3,1);
    cam_right(0)=cam_dir(1)*cam_up(2)-cam_dir(2)*cam_up(1);
    cam_right(1)=cam_dir(2)*cam_up(0)-cam_dir(0)*cam_up(2);
    cam_right(2)=cam_dir(0)*cam_up(1)-cam_dir(1)*cam_up(0);

    //camera intrinsics
    double cx,cy,fx,fy;
    cx=w/2-.5;
    cy=h/2-.5;
    fx=fy=tan(cam_angle/2)*w/2;//should be 320 for the example

    //takes camera frame to image frame
    cameraMatrix = (Mat_<double>(3,3) << fx,0,cx,
										    0,fy,cy,
										    0,0,1);

    //takes rotated camera frame to camera frame
    Mat cam_rot=(Mat_<double>(3,3) <<
    cam_right.x, cam_up.x, cam_dir.x, 0,
    cam_right.y, cam_up.y, cam_dir.y, 0,
    cam_right.z, cam_up.z, cam_dir.z, 0,
    0, 0, 0, 1).inv();

    //takes world frame to rotated camera frame
    Mat cam_trans=(Mat_<double>(3,3) <<
    1, 0, 0, -cam_pos.x,
    0, 1, 0, -cam_pos.y,
    0, 0, 1, -cam_pos.z,
    0, 0, 0, 1);


    //so now imgpoint=cam_matrix*cam_rot*cam_trans*worldpoint
    cameraAffinePose=cam_rot*cam_trans;

}

