#pragma once

#include <optional>

#include <lvgl.h>


namespace alfalfa
{
namespace lvgl
{
    class Object
    {
    public:
        Object(lv_obj_t* const obj_)
            : obj(obj_)
        {
        }
        virtual ~Object() = default;
        operator lv_obj_t*() { return obj; }
        operator const lv_obj_t*() const { return obj; }

        static Object GetActiveScreen()
        {
            return Object(lv_screen_active());
        }

        void SetBackgroundColor(const lv_color_t color)
        {
            lv_obj_set_style_bg_color(obj, color, 0);
        }

        void SetBorderColor(const lv_color_t color)
        {
            lv_obj_set_style_border_color(obj, color, 0);
        }

        void SetBorderWidth(const int32_t width)
        {
            lv_obj_set_style_border_width(obj, width, 0);
        }

        void SetRadius(const int32_t radius)
        {
            lv_obj_set_style_radius(obj, radius, 0);
        }

        void CenterOnParent()
        {
            lv_obj_center(obj);
        }

        void SetPosition(const int32_t x, const int32_t y)
        {
            lv_obj_set_pos(obj, x, y);
        }

    protected:
        lv_obj_t* obj = nullptr;
    };
}
}
