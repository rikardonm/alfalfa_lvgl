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
        virtual ~Object()
        {
            lv_obj_delete(obj);
        }

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

        void SetSize(const int32_t width, const int32_t height)
        {
            lv_obj_set_size(obj, width, height);
        }

    protected:
        Object(lv_obj_t* const obj_) : obj(obj_) {}

        // Avoid mis-use of the interface, thus deleting the underlying object
        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;

        Object(Object&&) = default;
        Object& operator=(Object&&) = default;

        lv_obj_t* obj = nullptr;
    };
}
}
