#ifndef RPI_HPP
#define RPI_HPP

namespace evse{
    namespace rpi{
        float getTemperature(int maxwait = 1000);
        float getCoreVoltage(int maxwait = 1000);
    }
}

#endif
