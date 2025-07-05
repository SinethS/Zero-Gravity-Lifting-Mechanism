#include "motor.h"
#include "UI_utils.h"
#include "linearControl.h"
#include "ADS1232.h"
#include "IO.h"

class ControllerUtil{

    private:
        IO *io;
        motor *stepper;
        LinearControl *handle_controller;
        ADS1232 *ads;

    public:

        ControllerUtil(IO *io, motor *stepper, LinearControl *handle_controller, ADS1232 *ads);
        
        void handlLinearControl();
        void handleADS1232Control();
        void handleButtonControl();


};
