#pragma once

#include <cstdint>
#include <vector>
#include <initializer_list>


namespace alfalfa
{
namespace lvgl
{
namespace drivers
{
    // From: https://zhuoxunweihong.com/wp-content/uploads/2023/08/MIPI-Display-Command-Set-DCS.pdf
    // page 30, table 1
    // Other refs:
    //  - https://file.hstatic.net/1000180878/file/mipi_protocol.pdf
    //  - https://foofoodamon.github.io/references/spec/MIPI_CSI-2_specification_v3-0.pdf
    enum MipiCommands
    {
        EnterIdleMode = 0x39,
        EnterInvertMode = 0x21,
        EnterNormalMode = 0x13,
        EnterPartialMode = 0x12,
        EnterSleepMode = 0x10,
        ExitIdleMode = 0x38,
        ExitInvertMode = 0x20,
        ExitSleepMode = 0x11,
        Get3dControl = 0x3f,
        GetAddressMode = 0x0b,
        GetBlueChannel = 0x08,
        GetCompressionMode = 0x3c,
        GetDiagnosticResult = 0x0f,
        GetDisplayMode = 0x0d,
        GetGreenChannel = 0x07,
        GetPixelFormat = 0x0c,
        GetPowerMode = 0x0a,
        GetRedChannel = 0x06,
        GetScanline = 0x45,
        GetSignalMode = 0x0e,
        Nop = 0x00,
        ReadDdbContinue = 0xa8,
        ReadDdbStart = 0xa1,
        ReadMemoryContinue = 0x3e,
        ReadMemoryStart = 0x2e,
        Set3dControl = 0x3d,
        SetAddressMode = 0x36,
        SetColumnAddress = 0x2a,
        SetDisplayOff = 0x28,
        SetDisplayOn = 0x29,
        SetGammaCurve = 0x26,
        SetPageAddress = 0x2b,
        SetPartialColumns = 0x31,
        SetPartialRows = 0x30,
        SetPixelFormat = 0x3a,
        SetScrollArea = 0x33,
        SetScrollStart = 0x37,
        SetTearOff = 0x34,
        SetTearOn = 0x35,
        SetTearScanline = 0x44,
        SetVsyncTiming = 0x40,
        SoftReset = 0x01,
        WriteLut = 0x2d,
        WriteMemoryContinue = 0x3c,
        WriteMemoryStart = 0x2c,
    };

    struct LcdCmd
    {
        LcdCmd(const uint8_t cmd, std::initializer_list<uint8_t> data)
            : cmd(cmd), data{data}
        {
        }
        const uint8_t cmd;
        const std::vector<uint8_t> data;
    };
}
}
}
