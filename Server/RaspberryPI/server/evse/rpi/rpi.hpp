#ifndef RPI_HPP
#define RPI_HPP

#include <cstdint>

namespace evse{
    namespace rpi{
        void setMaximumWait(int wait);

        float getTemperature();

        float getCorevoltage();
        float getSDRAMCvoltage();
        float getSDRAMIvoltage();
        float getSDRAMPvoltage();

        std::uint64_t getARMfrequency();
        std::uint64_t getCorefrequency();
        std::uint64_t getH264frequency();
        std::uint64_t getISPfrequency();
        std::uint64_t getV3Dfrequency();
        std::uint64_t getUARTfrequency();
        std::uint64_t getPWMfrequency();
        std::uint64_t getEMMCfrequency();
        std::uint64_t getPixelfrequency();
        std::uint64_t getVECfrequency();
        std::uint64_t getHDMIfrequency();
        std::uint64_t getDPIfrequency();
    }
}

#endif
