//
// Created by 周旭华 on 2024/4/20.
//

#include "include/InputProvider.h"
#include "InputLifecycleProivider.h"
#include "AndroidLog.h"
#include "XRMath.h"

namespace AREngine {
    static UnityXRTimeStamp GetCurrentUnityTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count();
        return static_cast< UnityXRTimeStamp >( ms );
    }

    char *InputProvider::TAG = "InputProvider";
    UnityXRInputFeatureIndex InputProvider::devicePositionIndex;
    UnityXRInputFeatureIndex InputProvider::deviceRotationIndex;

    UnitySubsystemErrorCode InputProvider::Tick(UnitySubsystemHandle handle,
                                                void *userData,
                                                UnityXRInputUpdateType updateType) {
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::FillDeviceDefinition(UnitySubsystemHandle handle,
                                        void *userData,
                                        UnityXRInternalInputDeviceId deviceId,
                                        UnityXRInputDeviceDefinition *definition) {
        LOG_INFO(TAG, "FillDeviceDefinition:%d", deviceId);
        unityXrInputInterface->DeviceDefinition_SetName(definition, "AREngine");
        UnityXRInputDeviceCharacteristics characteristics = (UnityXRInputDeviceCharacteristics) (
                kUnityXRInputDeviceCharacteristicsHeadMounted
                |kUnityXRInputDeviceCharacteristicsTrackedDevice);
        unityXrInputInterface->DeviceDefinition_SetCharacteristics(definition, characteristics);
        InputProvider::devicePositionIndex = unityXrInputInterface->DeviceDefinition_AddFeatureWithUsage(
                definition,
                "DevicePosition",
                UnityXRInputFeatureType::kUnityXRInputFeatureTypeAxis3D,
                kUnityXRInputFeatureUsageDevicePosition);
        InputProvider::deviceRotationIndex = unityXrInputInterface->DeviceDefinition_AddFeatureWithUsage(
                definition,
                "DeviceRotation",
                UnityXRInputFeatureType::kUnityXRInputFeatureTypeRotation,
                kUnityXRInputFeatureUsageDeviceRotation);

        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::UpdateDeviceConfig(UnitySubsystemHandle handle, void *userData,
                                      UnityXRInternalInputDeviceId deviceId,
                                      UnityXRInputDeviceConfig *config) {
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::UpdateDeviceState(UnitySubsystemHandle handle, void *userData,
                                     UnityXRInternalInputDeviceId deviceId,
                                     UnityXRInputUpdateType updateType,
                                     UnityXRInputDeviceState *state) {
        LOG_INFO(TAG, "UpdateDeviceState:%d", deviceId);
        if (InputProvider::arSession == nullptr || InputProvider::arFrame == nullptr) {
            return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeFailure;
        }
        HwArCamera *arCamera;
        HwArFrame_acquireCamera(InputProvider::arSession, InputProvider::arFrame, &arCamera);
        HwArTrackingState trackingState;
        HwArCamera_getTrackingState(InputProvider::arSession, arCamera, &trackingState);
        LOG_INFO(TAG, "tracking state:%d", trackingState);
        if (trackingState == HWAR_TRACKING_STATE_TRACKING) {
            HwArPose *arPose;
            HwArPose_create(InputProvider::arSession, nullptr, &arPose);
            HwArCamera_getDisplayOrientedPose(InputProvider::arSession, arCamera, arPose);
            float rowPose[7];
            HwArPose_getPoseRaw(InputProvider::arSession, arPose, rowPose);
            XRVector3 position{rowPose[4], rowPose[5], -rowPose[6]};
            XRQuaternion rotation{-rowPose[0], -rowPose[1], rowPose[2], rowPose[3]};
//            LOG_INFO(TAG, "position{%f,%f,%f}", position.x, position.y, position.z);
//            LOG_INFO(TAG, "rotation{%f,%f,%f,%f}", rotation.x, rotation.y, rotation.z, rotation.w);
            //in unity,around y -90,left hand, but why?
            XRMatrix4x4 matrix{0,0,1,0,
                               0,1,0,0,
                               -1,0,0,0,
                               0,0,0,1};
            XRQuaternion rot;
            MatrixToQuaternion(matrix,rot);
            position=matrix.TransformPoint(position);
            rotation=rot*rotation;

            unityXrInputInterface->DeviceState_SetAxis3DValue(state,
                                                              InputProvider::devicePositionIndex,
                                                              position);
            unityXrInputInterface->DeviceState_SetRotationValue(state,
                                                                InputProvider::deviceRotationIndex,
                                                                rotation);
//            LOG_INFO(TAG, "position1:{%f,%f,%f}", position.x, position.y, position.z);
//            LOG_INFO(TAG, "rotation1:{%f,%f,%f,%f}", rotation.x, rotation.y, rotation.z, rotation.w);

            HwArPose_destroy(arPose);
        }
        HwArCamera_release(arCamera);
        unityXrInputInterface->DeviceState_SetDeviceTime(state, GetCurrentUnityTimestamp());

        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::HandleEvent(UnitySubsystemHandle handle, void *userData,
                               unsigned int eventType, UnityXRInternalInputDeviceId deviceId,
                               void *buffer, unsigned int size) {
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::HandleRecenter(UnitySubsystemHandle handle, void *userData) {
        LOG_INFO(TAG, "HandleRecenter");
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::HandleHapticImpulse(UnitySubsystemHandle handle, void *userData,
                                       UnityXRInternalInputDeviceId deviceId, int channel,
                                       float amplitude, float duration) {
        LOG_INFO(TAG, "HandleHapticImpulse:%d", deviceId);
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeNotSupported;
    }

    UnitySubsystemErrorCode
    InputProvider::HandleHapticBuffer(UnitySubsystemHandle handle, void *userData,
                                      UnityXRInternalInputDeviceId deviceId, int channel,
                                      unsigned int bufferSize,
                                      const unsigned char *const buffer) {
        LOG_INFO(TAG, "HandleHapticBuffer:%d", deviceId);
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeNotSupported;
    }

    UnitySubsystemErrorCode
    InputProvider::QueryHapticCapabilities(UnitySubsystemHandle handle, void *userData,
                                           UnityXRInternalInputDeviceId deviceId,
                                           UnityXRHapticCapabilities *capabilities) {
        LOG_INFO(TAG, "QueryHapticCapabilities:%d", deviceId);
        capabilities->supportsImpulse = false;
        capabilities->supportsBuffer = false;
        capabilities->numChannels = 0;
        capabilities->bufferFrequencyHz = 0;
        capabilities->bufferMaxSize = 0;
        capabilities->bufferOptimalSize = 0;
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::HandleHapticStop(UnitySubsystemHandle handle, void *userData,
                                    UnityXRInternalInputDeviceId deviceId) {
        LOG_INFO(TAG, "HandleHapticStop:%d", deviceId);
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeNotSupported;
    }

    UnitySubsystemErrorCode
    InputProvider::QueryTrackingOriginMode(UnitySubsystemHandle handle, void *userData,
                                           UnityXRInputTrackingOriginModeFlags *trackingOriginMode) {
        LOG_INFO(TAG, "QueryTrackingOriginMode");
        *trackingOriginMode = UnityXRInputTrackingOriginModeFlags::kUnityXRInputTrackingOriginModeDevice;
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::QuerySupportedTrackingOriginModes(UnitySubsystemHandle handle,
                                                     void *userData,
                                                     UnityXRInputTrackingOriginModeFlags *supportedTrackingOriginModes) {
        LOG_INFO(TAG, "QuerySupportedTrackingOriginModes");
        *supportedTrackingOriginModes = UnityXRInputTrackingOriginModeFlags::kUnityXRInputTrackingOriginModeDevice;
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::HandleSetTrackingOriginMode(UnitySubsystemHandle handle, void *userData,
                                               UnityXRInputTrackingOriginModeFlags trackingOriginMode) {
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }

    UnitySubsystemErrorCode
    InputProvider::TryGetDeviceStateAtTime(UnitySubsystemHandle handle, void *userData,
                                           UnityXRTimeStamp time,
                                           UnityXRInternalInputDeviceId deviceId,
                                           UnityXRInputDeviceState *state) {
        LOG_INFO(TAG, "TryGetDeviceStateAtTime:%d", deviceId);
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
    }
}


