#pragma once

#include <alfalfa/lvgl/core/object.hpp>


namespace alfalfa
{
namespace lvgl
{
namespace ui
{
    class Rectangle : public Object
    {
    public:
        Rectangle(Object& parent, const lv_area_t dims, const bool size_not_end = true)
            : Object(lv_obj_create(parent))
        {
            lv_obj_set_pos(obj, dims.x1, dims.y1);
            if (size_not_end)
            {
                lv_obj_set_size(obj, dims.x2, dims.y2);
            }
            else
            {
                lv_obj_set_size(obj, dims.x2 - dims.x1, dims.y2 - dims.y1);
            }
        }
        virtual ~Rectangle() = default;

        Rectangle(const Rectangle&) = delete;
        Rectangle& operator=(const Rectangle&) = delete;
        Rectangle(Rectangle&&) = default;
        Rectangle& operator=(Rectangle&&) = delete;
    };
}
}
}
