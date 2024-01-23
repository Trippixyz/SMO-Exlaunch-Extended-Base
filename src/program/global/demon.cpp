#include "demon.h"

namespace global::demon {

    GVAR_Impl(ca::GreenDemon*, liveActor)
    GCHAR_PTRS_Impl(name, 10)

    GFLOAT_Impl(distanceToPlayer)
    GFLOATV_Impl(clippingDistance, 50000.f)

    GBOOL_Impl(isCheat)

    GINT_Impl(deathCount)
}