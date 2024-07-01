//
// Created by 周旭华 on 2024/4/20.
//

#pragma once

#include "UnitySubsystemTypes.h"
#include "IUnityInterface.h"
#include "IUnityXRInput.h"

namespace AREngine{
    class InputLifecycleProivider {
    public:
        static void Register(IUnityInterfaces *unityInterfaces);
    };

}


