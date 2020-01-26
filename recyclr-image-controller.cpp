#include "recyclr-image-controller.h"

ImageController::ImageController() :
    _thr(nullptr),
    _running(false),
    _state_fn(nullptr)
{
    _running = true;
    _thr = new std::thread(&ImageController::run, this);

    int r = set_thread_affinity(_thr, IMAGE_THREAD_MASK);
    if (r) {
        ERR("Unable to set thread affinity for image controller thread");
    }
}

ImageController::~ImageController()
{
    if (_thr) {
        _running = false;
        _thr->join();
        delete _thr;
        _thr = nullptr;
    }
}

u32 ImageController::run()
{
    if (!_state_fn) {
        _state_fn = &ImageController::start;
    }

    while (_state_fn && _running) {
        if ((this->*_state_fn)()) {
            break;
        }
    }

    return 0;
}

u32 ImageController::start()
{
    _state_fn = &ImageController::close;
    return 0;
}

u32 ImageController::close()
{
    _state_fn = &ImageController::close;
    return -1;
}
