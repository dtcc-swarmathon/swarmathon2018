#include "Gripper.h"

std::tuple<double,double> Gripper::getWristFingerValuesForPosition( Position pos )
{
    double wrist = 0.0;
    double fingers = 0.0;

    switch( pos )
    {
        default:
            messaging::errorMsg( __func__, "invalid gripper position passed." );
            break;
        case UP_OPEN:
            wrist = 0.0;
            fingers = M_PI_2;
            break;
        case UP_CLOSED:
            wrist = 0.0;
            fingers = 0.0;
            break;
        case DOWN_OPEN:
            wrist = 1.25;
            fingers = M_PI_2;
            break;
        case DOWN_CLOSED:
            wrist = 1.25;
            fingers = 0.0;
            break;
        case HOVER_OPEN:
            wrist = 0.8;
            fingers = M_PI_2;
            break;
        case HOVER_CLOSED:
            wrist = 0.8;
            fingers = 0.0;
            break;
    }
    return std::make_tuple ( wrist, fingers );
}
