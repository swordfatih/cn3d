#pragma once

#include "view.hpp"

#include <cn/graphics/light.hpp>
#include <memory>
#include <vector>

namespace cn
{

class scene
{
public:
    scene() = default;

    view& add(const model& model);
    void  remove(const view& view_to_remove);
    void  clear();

    void update_all(std::float32_t delta_time);

    [[nodiscard]] const std::vector<std::unique_ptr<view>>& views() const;

    void                                    add_light(const light& l);
    void                                    remove_light(const light& l);
    void                                    clear_lights();
    [[nodiscard]] const std::vector<light>& lights() const;

private:
    std::vector<std::unique_ptr<view>> m_views;
    std::vector<light>                 m_lights;
};

} // namespace cn
