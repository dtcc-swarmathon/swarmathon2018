#ifndef searchdrive_h
#define searchdrive_h

#include "../../state_machine/State.h"
#include "SearchMachine.h"

class SearchDrive : public State
{
    public:
        SearchDrive() : State( "search_drive" ) {}
        virtual void action()
        {
            SearchMachine *ssm = dynamic_cast<SearchMachine *> (owner);
            if( ssm && ssm->waypoints.size() > 0 )
            {
                Waypoint *waypoint = ssm->waypoints.front();
                if( waypoint )
                {
                    if( waypoint->hasArrived() )
                        ssm->nextWaypoint();
                }
                else
                   std::cout << "waypoint is null... shouldn't be" << std::endl;
            }
        }
};

#endif
