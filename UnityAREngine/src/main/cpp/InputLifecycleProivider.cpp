//
// Created by 周旭华 on 2024/4/20.
//

#include "InputLifecycleProivider.h"
#include "IUnityXRInput.h"
#include "InputProvider.h"
#include "IUnityXRTrace.h"
#include "AndroidLog.h"


namespace AREngine {
    char *TAG="InputLifecycleProvider";
    IUnityXRInputInterface *InputProvider::unityXrInputInterface;
    static UnitySubsystemErrorCode UNITY_INTERFACE_API
    Initialize(UnitySubsystemHandle handle, void *data) {
        LOG_INFO(TAG,"Initialize");

        UnityXRInputProvider provider{
            .userData=data,
            .Tick=&InputProvider::Tick,
            .FillDeviceDefinition=&InputProvider::FillDeviceDefinition,
            .UpdateDeviceConfig=&InputProvider::UpdateDeviceConfig,
            .UpdateDeviceState=&InputProvider::UpdateDeviceState,
            .HandleEvent=&InputProvider::HandleEvent,
            .HandleRecenter=&InputProvider::HandleRecenter,
            .HandleHapticImpulse=&InputProvider::HandleHapticImpulse,
            .HandleHapticBuffer=&InputProvider::HandleHapticBuffer,
            .HandleHapticStop=&InputProvider::HandleHapticStop,
            .QueryTrackingOriginMode=&InputProvider::QueryTrackingOriginMode,
            .QuerySupportedTrackingOriginModes=&InputProvider::QuerySupportedTrackingOriginModes,
            .TryGetDeviceStateAtTime=&InputProvider::TryGetDeviceStateAtTime,
        };
        UnitySubsystemErrorCode errorCode = InputProvider::unityXrInputInterface->RegisterInputProvider(handle,&provider);
        if(errorCode!=UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess)
        {
            LOG_ERROR(TAG,"fail to register input provider.Error code:%d",errorCode);
        }
        return errorCode;
    }

    static UnitySubsystemErrorCode UNITY_INTERFACE_API
    Start(UnitySubsystemHandle handle, void *data) {
        LOG_INFO(TAG,"Start");
        //1:phone
        UnitySubsystemErrorCode errorCode = InputProvider::unityXrInputInterface->InputSubsystem_DeviceConnected(handle,1);
        if(errorCode!=UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess){
            LOG_ERROR(TAG," UnityAREngine fail to connect device.ErrorCode:%d",errorCode);
        }
        return errorCode;
    }

    static void UNITY_INTERFACE_API
    Stop(UnitySubsystemHandle handle, void *data) {
        LOG_INFO(TAG,"Stop");
        UnitySubsystemErrorCode  errorCode = InputProvider::unityXrInputInterface->InputSubsystem_DeviceDisconnected(handle,1);
        if(errorCode!=UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess){
            LOG_ERROR(TAG,"UnityAREngine fail to unconnect device.ErrorCode:%d",errorCode);
        }
    }

    static void UNITY_INTERFACE_API
    Shutdown(UnitySubsystemHandle handle, void *data) {
        //why shutdown?
        LOG_INFO(TAG,"Shutdown");
    }

    void InputLifecycleProivider::Register(IUnityInterfaces *unityInterfaces) {
        LOG_INFO(TAG,"Register");
        InputProvider::unityXrInputInterface = unityInterfaces->Get<IUnityXRInputInterface>();
        UnityLifecycleProvider inputLifecycleHandler =
                {
                        nullptr,  // This can be any object you want to be passed as userData to the following functions
                        &Initialize,
                        &Start,
                        &Stop,
                        &Shutdown
                };

        UnitySubsystemErrorCode errorCode = InputProvider::unityXrInputInterface->RegisterLifecycleProvider("UnityAREngine", "AREngine-Input",
                                             &inputLifecycleHandler);
        if(errorCode!=UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess){
            LOG_ERROR(TAG,"fail to register input provider.Error code:%d",errorCode);
        }

    }

}


