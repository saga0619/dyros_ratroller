#ifndef REALROBOT_INTERFACE_H
#define REALROBOT_INTERFACE_H

#include "state_manager.h"

class RealRobotInterface : public StateManager
{
public:
    RealRobotInterface(DataContainer &dc_global);
    virtual ~RealRobotInterface() {}

    //update state of Robot from mujoco
    virtual void updateState() override;

    //Send command to Mujoco
    //virtual void sendCommand(Eigen::VectorQd command) override;
    virtual void sendCommand(Eigen::VectorQd command, double sim_time) override;

    //connect to RealRobot
    virtual bool connect() override;

private:
    DataContainer &dc;
};

#endif
