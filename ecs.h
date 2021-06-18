#pragma once
#include <ctime>

namespace ecs {
    enum CmpId : std::size_t {        
        HandComponent=0,
        HandRender,
        GameManager,
        
        // don't touch the rest of this enum, it is to identify the max
        // number of components
        _TotalComps_
    };
}