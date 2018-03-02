#include <iostream>
#include "SearchState.h"
#include "../TagUtilities.h"
#include "../waypoints/SimpleWaypoint.h"

void SearchState::action()
{
    forceTransition( internalTransition() );
    internalAction();
}

void SearchState::onEnter( std::string prev_state )
{
    if( waypoints.size() > 0 )
        outputs->current_waypoint = waypoints.front();
}

void SearchState::onExit( std::string next_state )
{

}

std::string SearchState::transition()
{
    std::string transition_to = getIdentifier();

    if( TagUtilities::hasTag( &this->inputs->tags, 0 ) )
        transition_to = "pickup_state";

    return transition_to;
}

SSState SearchState::internalTransition()
{
    SSState transition_to = internal_state;

    switch( internal_state )
    {
        default: break;
        case SEARCHSTATE_INIT:
            if( waypoints.size() != 0 )
                transition_to = SEARCHSTATE_DRIVE;
            break;
        case SEARCHSTATE_DRIVE:
            if( waypoints.size() == 0 )
                transition_to = SEARCHSTATE_INIT;
            break;
    }

    return transition_to;
}

void SearchState::internalAction()
{
    switch( internal_state )
    {
        default: break;
        case SEARCHSTATE_INIT:
        {
            SimpleWaypoint *waypoint = 0;
            SimpleParams params;
            double x = 0.0;
            double y = 0.0;
            /* the constant bits of the params, tuning parts */
            params.skid_steer_threshold = M_PI/12;
            params.arrived_threshold = 0.05;

            params.dist_max_output = 60;
            params.dist_deccel = 0.2;
            params.yaw_max_output = 80;
            params.yaw_deccel = M_PI/12;

            for( double n = 1.0; n < SEARCH_SIZE; n += 1.0 )
            {
                y += n * pow( (-1.0), ( n + 1.0 ) );
                params.goal_x = x;
                params.goal_y = y;
                waypoint = new SimpleWaypoint( this->inputs, params );
                this->waypoints.push_back( dynamic_cast<Waypoint*>( waypoint ) );

                x += ( n + 1.0 ) * pow( (-1.0), ( n + 1.0 ) );
                params.goal_x = x;
                params.goal_y = y;
                waypoint = new SimpleWaypoint( this->inputs, params );
                this->waypoints.push_back( dynamic_cast<Waypoint*>( waypoint ) );
            }
            this->outputs->current_waypoint = waypoints.front();
            break;
        }
        case SEARCHSTATE_DRIVE:
        {
            Waypoint *waypoint = this->waypoints.front();
            if( waypoint )
            {
                if( waypoint->hasArrived() )
                {
                    delete waypoint;
                    waypoints.erase( waypoints.begin() );
                    if( waypoints.size() != 0 )
                        this->outputs->current_waypoint = waypoints.front();
                    else
                        this->outputs->current_waypoint = 0;
                }
            }
            break;
        }
    }
}

void SearchState::forceTransition( SSState transition_to )
{
    SSState prev_state = internal_state;

    internal_state = transition_to;

    if( internal_state != prev_state )
    {
        /* onExit bits */
        switch( prev_state )
        {
            default: break;
        }

        /* onEnter bits */
        switch( internal_state )
        {
            default: break;
        }

    }
}
