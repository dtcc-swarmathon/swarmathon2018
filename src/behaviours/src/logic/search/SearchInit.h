#ifndef searchinit_h
#define searchinit_h

#include "SearchMachine.h"
#include "../../state_machine/State.h"
#include "../../waypoints/Waypoint.h"
#include "../../waypoints/SimpleWaypoint.h"
#include "../../waypoints/WaypointUtilities.h"

class SearchInit : public State
{
    public:
 	double k = .4;
        double compX = 0;
        double compY = 0;

        SearchInit() : State( "search_init" ), setup_complete(false) {}
        virtual void action()
        {
            SearchMachine *ssm = dynamic_cast<SearchMachine *> (owner);
            if( ssm )
            {
             	compX = ssm->inputs->odom_accel_gps.x;
                compY = ssm->inputs->odom_accel_gps.y;
                SimpleWaypoint *waypoint = 0;
                SimpleParams params;
                //WaypointUtilities::DrivingParams location;
                
                //double distanceFromHome = hypot(*location.current_x, *location.current_y);
                //double unitX = *location.current_x / distanceFromHome;
                //double unitY = *location.current_y / distanceFromHome;

               double distanceFromHome = hypot(compX, compY);
                compX/=distanceFromHome;
                compY/=distanceFromHome;     
                
                params.arrived_threshold = 0.5;
                params.skid_steer_threshold = M_PI/12;
                double dot = 0;
                double x = 0, y = 0;
                for (double i = 0; i < 30; i+=0.5){
                    x = i * cos(i);
                    y = i * sin(i);

                    dot = -1*(x*compX + y*compY)/(hypot(x-compX,y-compY));
                    dot += 1.3;
                    dot /= 2;

                    params.goal_x = x * dot * k;
                    params.goal_y = y * dot * k;


                    cout << "x: " << params.goal_x << " | y: " << params.goal_y << endl;


                    waypoint = new SimpleWaypoint( ssm->inputs, params );
                    ssm->waypoints.push_back( (Waypoint *)waypoint ); 
                }
                setup_complete = true;
            }
        }
        virtual std::string transition()
        {
            std::string transition_to = getIdentifier();

            if( setup_complete )
                transition_to = "search_drive";

            return transition_to;
        }
    private:
        bool setup_complete;
};

#endif
