#include "WaypointUtilities.h"


/******************
 * Trig Shortcuts *
 ******************/
float WaypointUtilities::getDistance( DrivingParams params )
{
    //simple hypotenuse of the difference between the two points
    //order shouldnt matter as long as its consistant
    return hypot( fabs( params.goal_x - *params.current_x ),
                  fabs( params.goal_y - *params.current_y ) );
}

float WaypointUtilities::getGoalTheta( DrivingParams params )
{
    //goal_theta -> tan = opposite/adjacent, ie this gives us an angular heading we need to be on to reach out destination
    return atan2( ( params.goal_y - *params.current_y ), ( params.goal_x - *params.current_x ) );
}

float WaypointUtilities::getAngularCorrectionNeeded( DrivingParams params )
{
    //shortest_angular_distance finds the smaller of the two amounts we could rotate in either direction to arrive there
    return angles::shortest_angular_distance( *params.current_theta, getGoalTheta( params ) );
}

/*************
 * PID Stuff *
 *************/

/*
std::tuple<int,int> WaypointUtilities::executePid( PidParams &params, PidPackage &pids )
{
    float vel = 0.0;
    float yaw = 0.0;
    int left = 0;
    int right = 0;

    vel = pids.vel_pid.PIDOut( params.velocity_error, params.velocity_goal );
    yaw = pids.yaw_pid.PIDOut( params.angular_error, params.angular_goal );
    left = (int)(vel - yaw);
    right = (int)(vel + yaw);

    if (left  >  params.saturation_point) {left  =  params.saturation_point;}
    if (left  < -params.saturation_point) {left  = -params.saturation_point;}
    if (right >  params.saturation_point) {right =  params.saturation_point;}
    if (right < -params.saturation_point) {right = -params.saturation_point;}

    return std::make_tuple( left, right );
}
*/

/* linear */

/* generate a linear output based on a linaer distance in meters from a target */
PidParams WaypointUtilities::getDistPIDParams()
{
    PidParams params;

    params.Kp = 60.;
    params.Ki = 5.;
    params.Kd = 0.;
    params.integration_point = 0.2;

    return params;
}

/* rotation */

/* generate a rotational output based on radian measurements from a target */
PidParams WaypointUtilities::getRadianRotPIDParams()
{
    PidParams params;

    params.Kp = 80.;
    params.Ki = 5.;
    params.Kd = 0.;
    params.integration_point = M_PI/12;
    return params;
}

/* generate a rotational output based on meter measurements from a target */
/* TODO: may need to break this up into camera and sonar distances but will try as one */
PidParams WaypointUtilities::getLinearRotPIDParams()
{
    PidParams params;

    params.Kp = 100.;
    params.Ki = 5.;
    params.Kd = 0.;
    params.integration_point = 0.1;

    return params;
}

/* skid linear */

PidParams WaypointUtilities::getDistSkidBasedPIDParams()
{
    PidParams params;

    params.Kp = 60.;
    params.Ki = 5.;
    params.Kd = 0.;
    params.integration_point = 0.2;

    return params;

}

/* skid rotation */

PidParams WaypointUtilities::getRadianRotSkidBasedPIDParams()
{
    PidParams params;

    params.Kp = 80.;
    params.Ki = 5.;
    params.Kd = 0.;
    params.integration_point = M_PI/12;
    return params;
}

PidParams WaypointUtilities::getLinearRotSkidBasedPIDParams()
{
    PidParams params;

    params.Kp = 100.;
    params.Ki = 5.;
    params.Kd = 0.;
    params.integration_point = 0.1;

    return params;
}

PidParams WaypointUtilities::getLinearVelBasedPIDParams()
{
    PidParams params;

    params.Kp = 60.;
    params.Ki = 1.;
    params.Kd = 0.;
    params.integration_point = 0;

    return params;
}

