//
// Created by 周旭华 on 2024/4/20.
//

#pragma once

#include "IUnityXRInput.h"
#include "huawei_arengine_interface.h"

namespace AREngine {

    class InputProvider {
    public:
        static char *TAG;
        static  HwArSession *arSession;
        static  HwArFrame  *arFrame;
        static IUnityXRInputInterface *unityXrInputInterface;
        static UnityXRInputFeatureIndex isTrackedIndex;
        static UnityXRInputFeatureIndex trackingStateIndex;
        static UnityXRInputFeatureIndex devicePositionIndex;
        static UnityXRInputFeatureIndex deviceRotationIndex;
        static UnityXRInputFeatureIndex cameraPositionIndex;
        static UnityXRInputFeatureIndex cameraRotationIndex;
        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        Tick(UnitySubsystemHandle handle, void *userData, UnityXRInputUpdateType updateType);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        FillDeviceDefinition(UnitySubsystemHandle handle, void *userData,
                             UnityXRInternalInputDeviceId deviceId,
                             UnityXRInputDeviceDefinition *definition);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        UpdateDeviceConfig(UnitySubsystemHandle handle, void *userData,
                           UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceConfig *config);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        UpdateDeviceState(UnitySubsystemHandle handle, void *userData,
                          UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType,
                          UnityXRInputDeviceState *state);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        HandleEvent(UnitySubsystemHandle handle, void *userData, unsigned int eventType,
                    UnityXRInternalInputDeviceId deviceId, void *buffer, unsigned int size);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        HandleRecenter(UnitySubsystemHandle handle, void *userData);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        HandleHapticImpulse(UnitySubsystemHandle handle, void *userData,
                            UnityXRInternalInputDeviceId deviceId, int channel, float amplitude,
                            float duration);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        HandleHapticBuffer(UnitySubsystemHandle handle, void *userData,
                           UnityXRInternalInputDeviceId deviceId, int channel,
                           unsigned int bufferSize,
                           const unsigned char *const buffer);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        QueryHapticCapabilities(UnitySubsystemHandle handle, void *userData,
                                UnityXRInternalInputDeviceId deviceId,
                                UnityXRHapticCapabilities *capabilities);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        HandleHapticStop(UnitySubsystemHandle handle, void *userData,
                         UnityXRInternalInputDeviceId deviceId);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        QueryTrackingOriginMode(UnitySubsystemHandle handle, void *userData,
                                UnityXRInputTrackingOriginModeFlags *trackingOriginMode);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        QuerySupportedTrackingOriginModes(UnitySubsystemHandle handle, void *userData,
                                          UnityXRInputTrackingOriginModeFlags *supportedTrackingOriginModes);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        HandleSetTrackingOriginMode(UnitySubsystemHandle handle, void *userData,
                                    UnityXRInputTrackingOriginModeFlags trackingOriginMode);

        static UnitySubsystemErrorCode UNITY_INTERFACE_API
        TryGetDeviceStateAtTime(UnitySubsystemHandle handle, void *userData, UnityXRTimeStamp time,
                                UnityXRInternalInputDeviceId deviceId,
                                UnityXRInputDeviceState *state);

    };

}


