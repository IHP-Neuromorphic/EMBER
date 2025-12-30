
#include <type_traits>
#include <cstddef>

#include "em_math.hpp"
#include "em_random.hpp"
#include "emf_random.hpp"

ember::ISaboteur* ember::fault::uniform(std::vector<ember::ISaboteur*> vec, const ember::fault::model& fModel)
{
    size_t weightSum = 0;
    for(int i=0; i<vec.size(); i++) {
        weightSum += vec[i]->locations(fModel);
    }

    size_t sel = ember::math::random::uniform((size_t)0, weightSum);

    for(int i=0; i<vec.size(); i++) {
        if(sel < vec[i]->locations(fModel)) {
            return vec[i];
        }
        sel -= vec[i]->locations(fModel);
    }

    assert(!"-E(Out of Scope): during random uniform component selection");
    return nullptr;
}