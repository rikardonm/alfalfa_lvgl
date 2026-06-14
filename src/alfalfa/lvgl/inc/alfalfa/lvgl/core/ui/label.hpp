#pragma once

#include <alfalfa/lvgl/core/object.hpp>


namespace alfalfa
{
namespace lvgl
{
namespace ui
{
    class Label : public Object
    {
    public:
        Label(Object& parent, lv_color_t text_color)
            : Object(lv_label_create(parent))
        {
            SetTextColor(text_color);
        }
        virtual ~Label() = default;

        void SetText(const char* const text)
        {
            lv_label_set_text(obj, text);
        }

        void SetText(const std::string& text)
        {
            lv_label_set_text(obj, text.c_str());
        }

        void SetTextColor(const lv_color_t color)
        {
            lv_obj_set_style_text_color(obj, color, 0);
        }

        void SetFont(const lv_font_t& font)
        {
            lv_obj_set_style_text_font(obj, &font, 0);
        }
    };
}
}
}
