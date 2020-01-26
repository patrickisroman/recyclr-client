#include "recyclr-controller.h"
#include "recyclr-utils.h"

Controller::Controller() :
    _thr(nullptr),
    _running(false),
    _state_fn(nullptr)
{
    _running = true;
    _thr = new std::thread(&Controller::run, this);

    int r = set_thread_affinity(_thr, CONTROLLER_THREAD_MASK);
    if (r) {
        ERR("Unable to set thread affinity for controller thread");
    }
}

Controller::~Controller()
{
    if (_thr) {
        _running = false;
        _thr->join();
        delete _thr;
        _thr = nullptr;
    }
}

u32 Controller::run()
{
    if (!_state_fn) {
        _state_fn = &Controller::start;
    }

    while (_state_fn && _running) {
        if ((this->*_state_fn)()) {
            break;
        }
    }

    return 0;
}

u32 Controller::start()
{
    _state_fn = &Controller::close;
    return 0;
}

u32 Controller::close()
{
    _state_fn = &Controller::close;
    return -1;
}