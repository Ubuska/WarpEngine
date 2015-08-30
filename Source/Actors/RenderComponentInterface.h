#pragma once
#include "ActorComponent.h"

class RenderComponentInterface : public ActorComponent
{
public:
//    virtual ComponentId VGetComponentId(void) const override { return COMPONENT_ID; }

    // RenderComponentInterface
    virtual shared_ptr<SceneNode> VGetSceneNode(void) = 0;
};
