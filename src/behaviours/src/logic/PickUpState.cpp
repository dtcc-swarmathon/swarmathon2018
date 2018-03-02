#include "PickUpState.h"
#include "../Gripper.h"

/********************
 * Basic Operations *
 ********************/

void PickUpState::action()
{
    forceTransition( internalTransition() );
    internalAction();
}

void PickUpState::onEnter( std::string prev_state )
{
    forceTransition( PICKUP_INIT );
    num_tries = 0;
    attempts = 0;
    cube_secured = false;
}

void PickUpState::onExit( std::string next_state )
{
    if( this->approach )
    {
        delete this->approach;
        this->approach = 0;
    }

    if( this->linear )
    {
        delete this->linear;
        this->linear = 0;
    }
}

std::string PickUpState::transition()
{
    std::string transition_to = getIdentifier();

    switch( internal_state )
    {
        case PICKUP_COMPLETE:
            if( ( this->inputs->time.toSec() - this->timer ) >= 1.5 )
                transition_to = "findhome_state";
            break;
        case PICKUP_COMPLETE_FAILURE:
            transition_to = "search_state";
            break;
    }

    return transition_to;
}

/***********************
 * Internal Operations *
 ***********************/
PUState PickUpState::internalTransition()
{
    PUState transition_to = internal_state;
    std::cout << "Current State: " << transition_to << std::endl;
    switch( internal_state )
    {
        case PICKUP_INIT:
            if( approach )
            {
                transition_to = PICKUP_APPROACH;
            }
            else
            {
                if( this->attempts > MAX_ATTEMPTS )
                    transition_to = PICKUP_COMPLETE_FAILURE;
            }
            break;
        case PICKUP_COMPLETE_FAILURE:
            //complete do nothing state
            break;
        case PICKUP_APPROACH:
            if( approach && approach->hasArrived() )
            {
                outputs->current_waypoint = 0;
                delete this->approach;
                this->approach = 0;

                transition_to = PICKUP_FINAL_APPROACH;
            }
            else if( attempts > MAX_ATTEMPTS )
            {
                transition_to = PICKUP_COMPLETE_FAILURE;
            }
            break;
        case PICKUP_FINAL_APPROACH:
            if( linear && linear->hasArrived() )
            {
                outputs->current_waypoint = 0;
                delete this->linear;
                this->linear = 0;

                transition_to = PICKUP_CLAW_CLOSE;
                this->timer = this->inputs->time.toSec();
            }
            break;
        case PICKUP_CLAW_CLOSE:
            if( ( this->inputs->time.toSec() - this->timer ) >= CLOSE_TIME )
            {
                transition_to = PICKUP_CLAW_UP;
                this->timer = this->inputs->time.toSec();
            }
            break;
        case PICKUP_CLAW_UP:
            if( ( this->inputs->time.toSec() - this->timer ) >= UP_TIME )
            {
                transition_to = PICKUP_CONFIRM;
                this->timer = this->inputs->time.toSec();
            }
            break;
        case PICKUP_CONFIRM:
            if( cube_secured )
            {
                transition_to = PICKUP_COMPLETE;
                this->timer = this->inputs->time.toSec();
            }
            else if( ( this->inputs->time.toSec() - this->timer ) >= CONFIRM_TIME )
            {
                transition_to = PICKUP_FAIL;
            }
        case PICKUP_COMPLETE:
            break;
        case PICKUP_FAIL:
            if( linear && linear->hasArrived() )
            {
                transition_to = PICKUP_INIT;
            }
            break;
    }
    return transition_to;
}

void PickUpState::internalAction()
{
    switch( internal_state )
    {
        case PICKUP_INIT:
            if( TagUtilities::hasTag( &inputs->tags, 0 ) )
            {
                TagParams t_params;

                t_params.desired_tag = 0;

                t_params.dist_deccel = 0.05;
                t_params.dist_goal = 0.24;
                t_params.dist_max_output = 10;

                t_params.yaw_deccel = 0.10;
                t_params.yaw_goal = 0.0;
                t_params.yaw_max_output = (80/3);

                t_params.type = CLOSEST;

                approach = new ApproachTagWaypoint( inputs, t_params );
                outputs->current_waypoint = approach;
            }
            else
            {
                this->attempts++;
            }
            break;
        case PICKUP_COMPLETE_FAILURE:
            //complete do nothing state
            break;
        case PICKUP_APPROACH:
            outputs->gripper_position = Gripper::DOWN_OPEN;
            if( TagUtilities::hasTag( &inputs->tags, 0 ) )
                this->attempts = 0;
            else
                this->attempts++;
            break;
        case PICKUP_FINAL_APPROACH:
            outputs->gripper_position = Gripper::DOWN_OPEN;
            break;
        case PICKUP_CLAW_CLOSE:
            outputs->gripper_position = Gripper::DOWN_CLOSED;
            break;
        case PICKUP_CLAW_UP:
            outputs->gripper_position = Gripper::UP_CLOSED;
            break;
        case PICKUP_CONFIRM:
            if( this->inputs->us_center < 0.13 )
                cube_secured = true;
            else if( TagUtilities::hasTag( &this->inputs->tags, 0 ) && TagUtilities::getDistance( this->inputs->tags.back() ) < 0.15 )
                cube_secured = true;
            break;
        case PICKUP_COMPLETE:
            outputs->gripper_position = Gripper::HOVER_CLOSED;
            break;
        case PICKUP_FAIL:
            outputs->gripper_position = Gripper::DOWN_OPEN;
            break;

    }
}

void PickUpState::forceTransition( PUState transition_to )
{
    PUState prev_state = internal_state;

    /* transition */
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
            case PICKUP_FINAL_APPROACH:
            {
                /* on Enter */
                LinearParams l_params;

                l_params.distance = 0.11;
                l_params.deccel_point = 0;
                l_params.max_vel = 5;

                this->linear = new LinearWaypoint( this->inputs, l_params );
                this->outputs->current_waypoint = this->linear;
                break;
            }
            case PICKUP_FAIL:
            {
                /* on Enter */
                this->num_tries++;

                LinearParams l_params;

                l_params.distance = .2;
                l_params.deccel_point = 0.05;
                l_params.max_vel = 10;
                l_params.reverse = true;

                this->linear = new LinearWaypoint( this->inputs, l_params );
                this->outputs->current_waypoint = this->linear;
                break;
            }
        }
    }
}
