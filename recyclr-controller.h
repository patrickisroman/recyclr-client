#include "recyclr-utils.h"

class Controller
{
    protected:
    std::thread* _thr;
    bool         _running;

    u32 (Controller::*_state_fn)();

    public:
    Controller();
    ~Controller();

    u32 run();
    u32 start();
    u32 close();
};

