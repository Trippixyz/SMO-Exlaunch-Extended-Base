#include "generic.h"

#include "al/Library/Player/PlayerUtil.h"
#include "al/Library/Scene/SceneUtil.h"

#include "RedStar/Player/PlayerActorHakoniwa.h"
#include "RedStar/Scene/StageScene.h"

namespace global {

    GINT_Impl(stageSceneTimer)

    GVAR_Impl(StageScene*, stageScene)

    GVAR_Create(PlayerActorHakoniwa*, playerActorHakoniwa) {

        return (PlayerActorHakoniwa*)al::tryGetPlayerActor(al::getScenePlayerHolder(stageScene()), 0);
    }
}