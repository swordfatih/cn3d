#include <cn/time/time.hpp>

namespace cn
{

std::chrono::high_resolution_clock::time_point time::m_last{std::chrono::high_resolution_clock::now()};
float                                          time::m_delta{0.0f};

void time::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    m_delta = std::chrono::duration<float>(now - m_last).count();
    m_last = now;
}

float time::delta()
{
    return m_delta;
}

} // namespace cn
