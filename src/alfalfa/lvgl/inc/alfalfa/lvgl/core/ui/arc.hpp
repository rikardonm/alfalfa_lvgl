#pragma once

#include <alfalfa/lvgl/core/object.hpp>


namespace alfalfa
{
namespace lvgl
{
namespace ui
{
    class Arc : public Object
    {
    public:
        Arc(Object& parent)
            : Object(lv_arc_create(parent))
        {
        }
        virtual ~Arc() = default;

        void SetRotation(int32_t rotation)
        {
            lv_arc_set_rotation(obj, rotation);
        }

        void SetBackgroundAngles(int32_t start, int32_t end)
        {
            lv_arc_set_bg_angles(obj, start, end);
        }

        void SetValue(int32_t value)
        {
            lv_arc_set_value(obj, value);
        }
    };
}
}
}
