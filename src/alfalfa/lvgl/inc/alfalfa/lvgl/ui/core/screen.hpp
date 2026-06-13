#pragma once

#include <optional>

#include <lvgl.h>
#include <lvgl_cpp/ui/core/object.hpp>


namespace lvgl_cpp
{
    class Screen : public Object
    {
    public:
        Screen()
            : Object(nullptr)
        {
        }

        static Object GetActiveScreen()
        {
            return Object(lv_screen_active());
        }
    };
}
