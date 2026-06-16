#include <alfalfa/lvgl/display.hpp>

#include <alfalfa/oal/timekeeper.hpp>


namespace alfalfa
{
namespace lvgl
{
    void lv_display_flusher(lv_display_t* display, const lv_area_t* area, uint8_t * px_buff)
    {
        try
        {
            auto user_data = lv_display_get_user_data(display);
            auto _driver = static_cast<alfalfa::lvgl::drivers::HardwareDisplayDriver*>(user_data);
            if (_driver)
            {
                // We are using RGB565, so we need 2 bytes per pixel
                const auto px_count = lv_area_get_size(area);
                // We need to convert the pixel format from LVGL to the display's format (little endian?)
                lv_draw_sw_rgb565_swap(px_buff, px_count);
                const auto size = px_count * 2;
                _driver->FlushArea(*area, {static_cast<const uint8_t*>(px_buff), size});
            }
            lv_display_flush_ready(display);
        }
        catch(const std::exception& e)
        {
            std::string detail("Exception in lv_display_flusher: ");
            Logln(detail + e.what());
        }
    }

    Display::Display(
        std::unique_ptr<alfalfa::lvgl::drivers::HardwareDisplayDriver> hw_display,
        std::span<uint8_t> lv_buff,
        std::span<uint8_t> lv_buff_2)
        : _hw_display(std::move(hw_display))
        , display(nullptr, &lv_display_delete)
    {
        lv_init();
        lv_tick_set_cb(Timekeeper::TickStamp);

        if (not lv_buff.size())
        {
            throw std::invalid_argument("Unexpected empty buffer.");
        }
        if (lv_buff_2.size() and lv_buff.size() != lv_buff_2.size())
        {
            throw std::invalid_argument("Buffer sizes do not match");
        }
        const auto p_size = _hw_display->GetPanelSize();
        auto disp = lv_display_create(p_size.x, p_size.y);
        if (not disp)
        {
            throw std::runtime_error("Failed to create display");
        }
        display = std::unique_ptr<lv_display_t, void(*)(lv_display_t*)>(disp, &lv_display_delete);
        lv_display_set_user_data(display.get(), _hw_display.get());
        uint8_t* buffer_2_pointer = lv_buff_2.size() ? lv_buff_2.data() : nullptr;
        lv_display_set_buffers(display.get(), lv_buff.data(), buffer_2_pointer, lv_buff.size(), LV_DISPLAY_RENDER_MODE_PARTIAL);
        lv_display_set_flush_cb(display.get(), lv_display_flusher);
    }

    uint32_t Display::Tick()
    {
        return lv_timer_handler();
    }
}
}
