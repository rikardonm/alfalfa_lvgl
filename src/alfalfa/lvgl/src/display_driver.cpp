#include <alfalfa/lvgl/drivers/display_driver.hpp>
#include <alfalfa/oal/timekeeper.hpp>

#include <alfalfa/lvgl/drivers/mipi_commands.hpp>

using namespace Units::Literals;


namespace alfalfa
{
namespace lvgl
{
namespace drivers
{
    HardwareDisplayDriver::HardwareDisplayDriver(
        std::shared_ptr<alfalfa::hal::spi::iModule> spi,
        std::unique_ptr<::alfalfa::hal::dio::iOutput> dc_pin,
        std::unique_ptr<::alfalfa::hal::dio::iOutput> chip_select_n,
        std::unique_ptr<::alfalfa::hal::dio::iOutput> rst_pin,
        const PanelConfiguration& panel_config,
        const lv_display_rotation_t rotation)
        : _spi(spi)
        , _dc_pin(std::move(dc_pin))
        , _chip_select_n(std::move(chip_select_n))
        , _rst_pin(std::move(rst_pin))
        , _panel_config(TransformRotation(rotation, panel_config))
    {
        _dc_pin->Write(false);
        _chip_select_n->Write(true);
        if (_rst_pin)
        {
            ExecuteHardReset();
        }
        else
        {
            SendCommand(MipiCommands::SoftReset, {});
            Timekeeper::DumbDelay(150_ms);
            SendCommand(MipiCommands::ExitSleepMode, {});
            Timekeeper::DumbDelay(150_ms);
        }
        if (_panel_config.Initialize)
        {
            _panel_config.Initialize(*this);
        }
        else
        {
            throw std::runtime_error("No initialization function provided for display");
        }
        SetOrientation(false, false);
        SetColorInversion(false);
    }

    lv_point_t HardwareDisplayDriver::GetPanelSize() const
    {
        return _panel_config.panel_size;
    }

    void HardwareDisplayDriver::FlushArea(const lv_area_t& area, const std::span<const uint8_t> data)
    {
        const std::array<uint8_t, 4> data_x{
            static_cast<uint8_t>(area.x1 >> 8),
            static_cast<uint8_t>(area.x1 & 0xFF),
            static_cast<uint8_t>(area.x2 >> 8),
            static_cast<uint8_t>(area.x2 & 0xFF)
        };
        SendCommand(MipiCommands::SetColumnAddress, data_x);

        const std::array<uint8_t, 4> data_y {
            static_cast<uint8_t>(area.y1 >> 8),
            static_cast<uint8_t>(area.y1 & 0xFF),
            static_cast<uint8_t>(area.y2 >> 8),
            static_cast<uint8_t>(area.y2 & 0xFF)
        };
        SendCommand(MipiCommands::SetPageAddress, data_y);

        SendCommand(MipiCommands::WriteMemoryStart, data);
    }

    void HardwareDisplayDriver::SleepIn(void)
    {
        SendCommand(MipiCommands::EnterSleepMode, 0x08);
    }

    void HardwareDisplayDriver::SleepOut(void)
    {
        SendCommand(MipiCommands::ExitSleepMode, 0x08);
    }

    void HardwareDisplayDriver::SetOrientation(
        bool flip_on_x,
        bool flip_on_y)
    {
        uint8_t data = _panel_config.address_mode_base_value;
        if (flip_on_x)
        {
            data ^= 0x01;
        }
        if (flip_on_y)
        {
            data ^= 0x02;
        }
        SendCommand(MipiCommands::SetAddressMode, data);
    }

    void HardwareDisplayDriver::SetColorInversion(const bool enabled)
    {
        const auto cmd = (enabled ^ _panel_config.inverted_colors) ? MipiCommands::EnterInvertMode : MipiCommands::ExitInvertMode;
        SendCommand(cmd, {});
    }

    void HardwareDisplayDriver::ExecuteHardReset()
    {
        if (not _rst_pin)
        {
            return;
        }
        _rst_pin->Write(false);
        Timekeeper::DumbDelay(10_ms);
        _rst_pin->Write(true);
        Timekeeper::DumbDelay(150_ms);
    }

    void HardwareDisplayDriver::SendCommand(const uint8_t cmd, const uint8_t data)
    {
        _dc_pin->Write(false);
        _chip_select_n->Write(false);
        _spi->Transceive(cmd);
        _dc_pin->Write(true);
        _spi->Transceive(data);
        _chip_select_n->Write(true);
    }

    void HardwareDisplayDriver::SendCommand(const uint8_t cmd, const std::span<const uint8_t> data)
    {
        _dc_pin->Write(false);
        _chip_select_n->Write(false);
        _spi->Transceive(cmd);
        if (data.size())
        {
            _dc_pin->Write(true);
            _spi->Transceive(data);
        }
        _chip_select_n->Write(true);
    }

    PanelConfiguration HardwareDisplayDriver::TransformRotation(const lv_display_rotation_t rotation, const PanelConfiguration config)
    {
        auto base_value = config.address_mode_base_value;
        auto size = config.panel_size;
        switch (rotation)
        {
            case LV_DISPLAY_ROTATION_0:
                base_value ^= 0x00;
                break;
            case LV_DISPLAY_ROTATION_90:
                base_value ^= 0x60;
                std::swap(size.x, size.y);
                break;
            case LV_DISPLAY_ROTATION_180:
                base_value ^= 0x03;
                break;
            case LV_DISPLAY_ROTATION_270:
                base_value ^= 0xA0;
                std::swap(size.x, size.y);
                break;
            default:
                throw std::invalid_argument("Invalid rotation value");
        }
        return {
            .panel_size = size,
            .address_mode_base_value = base_value,
            .inverted_colors = config.inverted_colors,
            .Initialize = config.Initialize
        };
    }
}
}
}
