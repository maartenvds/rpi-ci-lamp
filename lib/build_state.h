/*
 *  Request parser
 *  Author: Maarten Vandersteegen
 */

#ifndef BUILD_STATE_H
#define BUILD_STATE_H

enum BuildState
{
    BUILD_STATE_FAILED = 0,
    BUILD_STATE_RUNNING,
    BUILD_STATE_PASSED,
};

#endif /* BUILD_STATE_H */
