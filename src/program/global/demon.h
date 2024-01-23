#pragma once

#include "globalUtil.h"

#include "ca/GreenDemon.h"

namespace global::demon {

    GVAR_Decl(ca::GreenDemon*, liveActor);
    GCHAR_PTR_Decl(name);

    GFLOAT_Decl(distanceToPlayer);
    GFLOAT_Decl(clippingDistance);

    GBOOL_Decl(isCheat);

    GINT_Decl(deathCount);
}