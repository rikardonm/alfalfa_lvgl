#pragma once

#include <optional>

#include <alfalfa/lvgl/ui/core/object.hpp>
#include <alfalfa/lvgl/ui/core/rectangle.hpp>
#include <alfalfa/lvgl/ui/core/label.hpp>


namespace alfalfa
{
namespace lvgl
{
    inline std::vector<std::tuple<Rectangle, Label>> BuildRgbPattern(
        Object& root,
        const lv_point_t d_size,
        const lv_font_t& font = lv_font_montserrat_12)
    {
        std::vector<std::tuple<Rectangle, Label>> output;

        const auto y_split = d_size.y / 5;
        for (auto& [idx, name, color, text_color] : {
            std::tuple(0, "white", lv_color_white(), lv_color_black()),
            std::tuple(1, "black", lv_color_black(), lv_color_white()),
            std::tuple(2, "red", lv_color_make(255, 0, 0), lv_color_make(0, 255, 255)),
            std::tuple(3, "green", lv_color_make(0, 255, 0), lv_color_make(255, 0, 255)),
            std::tuple(4, "blue", lv_color_make(0, 0, 255), lv_color_make(255, 255, 0))
        })
        {
            auto rect = alfalfa::lvgl::Rectangle(root, {.x1 = 0, .y1 = y_split * idx, .x2 = d_size.x, .y2 = y_split});
            rect.SetBackgroundColor(color);
            rect.SetBorderWidth(0);
            rect.SetRadius(0);

            auto label = alfalfa::lvgl::Label(rect, text_color);
            label.SetText(name);
            label.CenterOnParent();
            label.SetFont(font);

            output.emplace_back(std::make_tuple(std::move(rect), std::move(label)));
        }
        return output;
    }
}
}
