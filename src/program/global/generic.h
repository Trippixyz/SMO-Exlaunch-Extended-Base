#pragma once

#include "globalUtil.h"

#include "al/Library/Player/PlayerUtil.h"
#include "al/Library/Scene/SceneUtil.h"

#include "RedStar/Player/PlayerActorHakoniwa.h"
#include "RedStar/Scene/StageScene.h"

namespace global {

    GINT_Decl(stageSceneTimer);

    GVAR_Decl(StageScene*, stageScene);

    GVAR_Create(PlayerActorHakoniwa*, playerActorHakoniwa);
}