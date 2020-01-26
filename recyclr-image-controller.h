#include "recyclr-utils.h"

class ImageController
{
    protected:
    std::thread* _thr;
    bool         _running;

    u32 (ImageController::*_state_fn)();

    public:
    ImageController();
    ~ImageController();

    u32 run();
    u32 start();
    u32 close();
};