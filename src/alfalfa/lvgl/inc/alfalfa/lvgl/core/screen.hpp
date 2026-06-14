#pragma once

#include <optional>

#include <alfalfa/lvgl/core/object.hpp>


namespace alfalfa
{
namespace lvgl
{
    class Screen : public Object
    {
    public:
        Screen()
        : Object(lv_obj_create(nullptr))
        {
        }
        virtual ~Screen() = default;

        static Screen GetActiveScreen()
        {
            return Screen(lv_screen_active());
        }

        void Load()
        {
            lv_screen_load(obj);
        }

        void Load(lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay)
        {
            // never delete the old screen, as the dtor should do it.
            lv_screen_load_anim(obj, anim_type, time, delay, false);
        }

    private:
        Screen(lv_obj_t* const obj_)
            : Object(obj_)
        {
        }
    };
}
}
