#include <algorithm>
#include <cn/scene/scene.hpp>

namespace cn
{

view& scene::add(const model& model)
{
    auto  view_ptr = std::make_unique<view>(model);
    view& view_ref = *view_ptr;
    m_views.push_back(std::move(view_ptr));
    return view_ref;
}

void scene::remove(const view& view_to_remove)
{
    std::erase_if(m_views, [&](const std::unique_ptr<view>& ptr)
    {
        return ptr.get() == &view_to_remove;
    });
}

void scene::clear()
{
    m_views.clear();
    m_lights.clear();
}

void scene::update_all(std::float32_t delta_time)
{
    for(auto& view: m_views)
    {
        view->update(delta_time);
    }
}

const std::vector<std::unique_ptr<view>>& scene::views() const
{
    return m_views;
}

// --- Lights ---

void scene::add_light(const light& l)
{
    m_lights.push_back(l);
}

void scene::remove_light(const light& l)
{
    std::erase_if(m_lights, [&](const light& other)
    {
        return &other == &l;
    });
}

void scene::clear_lights()
{
    m_lights.clear();
}

const std::vector<light>& scene::lights() const
{
    return m_lights;
}

} // namespace cn
