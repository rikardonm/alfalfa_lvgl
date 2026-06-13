#pragma once

#include <alfalfa/lvgl/drivers/display_driver.hpp>
#include <alfalfa/lvgl/drivers/mipi_commands.hpp>

#include <alfalfa/lib/types/units/all.hpp>
#include <alfalfa/oal/timekeeper.hpp>


namespace alfalfa
{
namespace lvgl
{
namespace drivers
{
namespace panels
{
    using namespace Units::Literals;
    using namespace alfalfa::lvgl::drivers;

    inline void _InitializeGc9a01Round1in28(HardwareDisplayDriver& driver)
    {
        static const auto init_cmds = std::to_array<LcdCmd>({
            {0xEF, {}},                 // enable inner register 2
            {0xEB, {0x14}},             // ?

            {0xFE, {}},                 // enable inner register 1
            {0xEF, {}},                 // enable inner register 2
            {0xEB, {0x14}},             // ??

            {0x84, {0x40}},             // ??
            {0x85, {0xFF}},             // ??
            {0x86, {0xFF}},             // ??
            {0x87, {0xFF}},             // ??
            {0x88, {0x0A}},             // ??
            {0x89, {0x21}},             // ??
            {0x8A, {0x00}},             // ??
            {0x8B, {0x80}},             // ??
            {0x8C, {0x01}},             // ??
            {0x8D, {0x01}},             // ??
            {0x8E, {0xFF}},             // ??
            {0x8F, {0xFF}},             // ??

            {0x3A, {0x55}},                 // pixel format: 16 bits per pixel

            {0x34, {}},                     // tearing effect line OFF
            // {0x35, {}},                     // tearing effect line ON
            {0x36, {0x20}},                 // memory access control: display orientation

            {0x62, {0x18, 0x0D, 0x71, 0xED, 0x70, 0x70, 0x18, 0X0F, 0x71, 0xEF, 0x70, 0x70}},       // ??
            {0x63, {0x18, 0x11, 0x71, 0xF1, 0x70, 0x70, 0x18, 0X13, 0x71, 0xF3, 0x70, 0x70}},       // ??
            {0x64, {0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07}},                                     // ??
            {0x66, {0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0X00, 0x00, 0x00}},                   // ??
            {0x67, {0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0X10, 0x32, 0x98}},                   // ??

            {0x70, {0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0X08, 0x03}},                         // ??
            {0x74, {0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00}},                                     // ??

            {0x90, {0x08, 0x08, 0X08, 0X08}},       // ??
            {0x98, {0x3E, 0x07}},                   // ??

            {0xAE, {0x77}},                         // ??
            {0xB6, {0x00, 0x00}},       // Display function control: scan direction
            // {0xB6, {0x00, 0x20}},
            {0xBD, {0x06}},     // ??
            {0xBC, {0x00}},     // ??
            {0xBE, {0x11}},     // ??

            {0xC3, {0x13}},     // Vregla voltage control
            {0xC4, {0x13}},     // Vreglb voltage control
            {0xC9, {0x22}},     // Verg2a voltage control

            {0xCD, {0x63}},     // ??
            {0xDF, {0x21, 0x0C, 0x02}}, // ??
            {0xE1, {0x10, 0x0E}},   // ??
            {0xED, {0x1B, 0x0B}},   // ??
            {0xE8, {0x34}},     // Frame rate: ??

            // gamma control
            {0xF0, {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A}},
            {0xF1, {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F}},
            {0xF2, {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A}},
            {0xF3, {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F}},

            {0xFF, {0x60, 0x01, 0x04}},     //  ??
        });

        for (auto& cmd : init_cmds)
        {
            driver.SendCommand(cmd.cmd, cmd.data);
        }
        driver.SendCommand(MipiCommands::ExitSleepMode, {});
        Timekeeper::DumbDelay(100_ms);
        driver.SendCommand(MipiCommands::SetDisplayOn, {});
        Timekeeper::DumbDelay(100_ms);
    }

    /**
     * @brief 1.28 inch Round Display
     *
     * Reference: https://www.lcdwiki.com/1.28inch_IPS_Module
     *
     *               ---
     *             --   --
     *           --       --
     *           --       --
     *             --   --
     *               ---
     *                |
     *                | <- ribbon
     */
    PanelConfiguration RoundDisplay1in28()
    {
        return PanelConfiguration{
            .panel_size = {.x = 240, .y = 240},
            .address_mode_base_value = 0x48,
            .inverted_colors = true,
            .Initialize = &_InitializeGc9a01Round1in28};
    }

}
}
}
}
