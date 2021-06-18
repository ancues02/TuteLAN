#pragma once
#include <ctime>

namespace ecs {
    enum CmpId : std::size_t {
        Transform = 0,
        CardData,
        CardRender,
        HandData,
        HandRender,
        GameManager,
        
        // don't touch the rest of this enum, it is to identify the max
        // number of components
        _TotalComps_
    };
}