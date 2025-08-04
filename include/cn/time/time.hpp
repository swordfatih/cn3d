#pragma once

#include <chrono>
#include <stdfloat>

namespace cn
{

class time
{
public:
    static void  update();
    static float delta();

private:
    static std::chrono::high_resolution_clock::time_point m_last;
    static float                                          m_delta;
};

} // namespace cn
