#include <ros/ros.h>
#include "ratroller/ratroller.h"
#include "ratroller/terminal.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ratroller");
    DataContainer dc;

    std::string mode;

    dc.nh.param<std::string>("/ratroller/run_mode", mode, "default");
    dc.nh.param("/ratroller/ncurse", dc.ncurse_mode, true);

    Tui tui(dc);
    std::string cs[10][10];
    tui.ReadAndPrint(3, 0, "ascii");

    cs[0][0] = "SIMULATION";
    cs[1][0] = "REALROBOT";
    cs[2][0] = "TEST";
    cs[3][0] = "EXIT";
    std::string menu_slcc;

    if (mode == "simulation")
    {
        //mvprintw(20, 30, " :: SIMULATION MODE :: ");
        //refresh();
        //wait_for_ms(1000);
    }
    else if (mode == "realrobot")
    {
        //mvprintw(20, 30, " :: REAL ROBOT MODE :: ");
        //refresh();
        //wait_for_ms(1000);
    }
    else if (mode == "default")
    {
        wait_for_keypress();
        erase();
        tui.ReadAndPrint(0, 0, "red");
        refresh();

        menu_slcc = tui.menu(3, 35, 2, 0, 4, 1, cs);

        if (menu_slcc == "SIMULATION")
        {
            mvprintw(16, 10, "SIMULATION MODE! ");
            mode = "simulation";
        }
        else if (menu_slcc == "REALROBOT")
        {
            mvprintw(16, 10, "REAL ROBOT IS NOT READY");
            mode = "realrobot";
        }
        else if (menu_slcc == "TEST")
        {
            mvprintw(16, 10, "TEST MODE !");
            mode = "testmode";
        }
        else if (menu_slcc == "EXIT")
        {
            erase();
            endwin();
            return 0;
        }
    }
    else
    {
        rprint_sol(dc.ncurse_mode, 20, 10, " !! SOMETHING WRONG :: Unidentified ROS Param! Press Any Key to End");
        wait_for_keypress();
        endwin();
        return 0;
    }

    erase();
    tui.ReadAndPrint(0, 0, "robot");
    refresh();

    if (!dc.ncurse_mode)
        endwin();

    bool simulation = true;
    dc.dym_hz = 500; //frequency should be divisor of a million (timestep must be integer)
    dc.stm_hz = 4000;
    dc.dym_timestep = std::chrono::microseconds((int)(1000000 / dc.dym_hz));
    dc.stm_timestep = std::chrono::microseconds((int)(1000000 / dc.stm_hz));

    MujocoInterface stm(dc);
    DynamicsManager dym(dc);
    RedController rc(dc, stm, dym);

    std::thread thread[4];
    if (mode == "simulation")
    {
        thread[0] = std::thread(&RedController::stateThread, &rc);
        thread[1] = std::thread(&RedController::dynamicsThreadHigh, &rc);
        thread[2] = std::thread(&RedController::dynamicsThreadLow, &rc);
        thread[3] = std::thread(&RedController::tuiThread, &rc);

        for (int i = 0; i < 3; i++)
        {
            thread[i].join();
            rprint_sol(dc.ncurse_mode, 3 + 2 * i, 35, "Thread %d End", i);
        }
    }
    else if (mode == "realrobot")
    {
    }
    else if (mode == "testmode")
    {
        thread[0] = std::thread(&StateManager::testThread, &stm);
        thread[1] = std::thread(&DynamicsManager::testThread, &dym);
        thread[2] = std::thread(&RedController::tuiThread, &rc);

        for (int i = 0; i < 3; i++)
        {
            thread[i].join();
            rprint_sol(dc.ncurse_mode, 3 + 2 * i, 35, "Thread %d End", i);
        }
    }

    if (dc.ncurse_mode)
        mvprintw(22, 10, "PRESS ANY KEY TO EXIT ...");

    while (ros::ok())
    {
        if (!(getch() == -1))
        {
            endwin();
            std::cout << "PROGRAM ENDED ! \n";
            return 0;
        }
    }
}
