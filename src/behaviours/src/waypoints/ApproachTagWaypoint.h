#ifndef approachtagwaypoint_h
#define approachtagwaypoint_h

#include "../Tag.h"
#include "../TagUtilities.h"
#include "../logic/LogicTypes.h"
#include "Waypoint.h"
#include "WaypointUtilities.h"

class ApproachTagWaypoint : public Waypoint
{
    public:
        ApproachTagWaypoint( LogicInputs *i );
    private:
        WaypointUtilities::PidPackage pids;
};

#endif

