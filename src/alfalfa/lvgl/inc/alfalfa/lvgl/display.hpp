#pragma once

#include <cstdint>
#include <memory>

#include <lvgl.h>
#include <alfalfa/lvgl/drivers/display_driver.hpp>


namespace alfalfa
{
namespace lvgl
{
    class Display
    {
    public:
        Display(
            std::unique_ptr<drivers::HardwareDisplayDriver> hw_display,
            std::span<uint8_t> lv_buff,
            std::span<uint8_t> lv_buff_2);

        uint32_t Tick();
    private:
        std::unique_ptr<drivers::HardwareDisplayDriver> _hw_display;
        std::unique_ptr<lv_display_t, void(*)(lv_display_t*)> display;
    };
}
}
