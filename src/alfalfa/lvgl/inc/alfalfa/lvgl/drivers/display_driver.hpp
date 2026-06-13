#pragma once

#include <memory>
#include <optional>
#include <span>

#include <alfalfa/hal/spi.hpp>
#include <alfalfa/hal/dio.hpp>

#include <lvgl.h>


namespace alfalfa
{
namespace lvgl
{
namespace drivers
{
    class HardwareDisplayDriver;

    struct PanelConfiguration
    {
        // @brief Physical size of the panel, in pixels, width x height
        lv_point_t panel_size;
        // @brief Base value for the address mode command, specific to the display controller and TFT panel
        uint8_t address_mode_base_value;
        // @brief Whether the display colors are inverted (white is black, black is white, etc)
        bool inverted_colors;
        void (*Initialize)(HardwareDisplayDriver& driver);
    };

    class HardwareDisplayDriver
    {
    public:
        HardwareDisplayDriver(
            std::shared_ptr<alfalfa::hal::spi::iModule> spi,
            std::unique_ptr<::alfalfa::hal::dio::iOutput> dc_pin,
            std::unique_ptr<::alfalfa::hal::dio::iOutput> chip_select_n,
            std::unique_ptr<::alfalfa::hal::dio::iOutput> rst_pin,
            const PanelConfiguration& panel_config,
            const lv_display_rotation_t rotation = LV_DISPLAY_ROTATION_0);
        virtual ~HardwareDisplayDriver() = default;

        lv_point_t GetPanelSize() const;
        void FlushArea(const lv_area_t& area, const std::span<const uint8_t> data);

        void SleepIn(void);
        void SleepOut(void);
        void SetOrientation(bool flip_on_x, bool flip_on_y);
        void SetColorInversion(const bool enabled);
        void ExecuteHardReset();

        void SendCommand(const uint8_t cmd, const uint8_t data);
        void SendCommand(const uint8_t cmd, const std::span<const uint8_t> data);

    private:
        std::shared_ptr<alfalfa::hal::spi::iModule> _spi;
        std::unique_ptr<::alfalfa::hal::dio::iOutput> _dc_pin;
        std::unique_ptr<::alfalfa::hal::dio::iOutput> _chip_select_n;
        std::unique_ptr<::alfalfa::hal::dio::iOutput> _rst_pin;
        const PanelConfiguration _panel_config;

        static PanelConfiguration TransformRotation(const lv_display_rotation_t rotation, const PanelConfiguration config);
    };
}
}
}
