#include <jni.h>
#include <string>
#include "huawei_arengine_interface.h"
#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include "IUnityInterface.h"
#include "IUnityXRInput.h"
#include "InputLifecycleProivider.h"
#include "IUnityGraphics.h"
#include "InputProvider.h"
#include "AndroidLog.h"

JavaVM *javaVM;
JNIEnv *jniEnv;

HwArSession *AREngine::InputProvider::arSession;
HwArFrame *AREngine::InputProvider::arFrame;

const char *TAG="UnityAREngine";



GLchar* GlenumToGlChar(GLenum source){
    switch(source){
        //source enum
        case GL_DEBUG_SOURCE_API:
            return  "GL_DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "GL_DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "GL_DEBUG_SOURCE_THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "GL_DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "GL_DEBUG_SOURCE_OTHER";

            //Debug Type

        case   GL_DEBUG_TYPE_ERROR:
            return "GL_DEBUG_TYPE_ERROR";
        case    GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        case    GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        case    GL_DEBUG_TYPE_PORTABILITY:
            return "GL_DEBUG_TYPE_PORTABILITY";
        case    GL_DEBUG_TYPE_PERFORMANCE:
            return "GL_DEBUG_TYPE_PERFORMANCE";
        case    GL_DEBUG_TYPE_MARKER:
            return "GL_DEBUG_TYPE_MARKER";
        case   GL_DEBUG_TYPE_PUSH_GROUP:
            return "GL_DEBUG_TYPE_PUSH_GROUP";
        case    GL_DEBUG_TYPE_POP_GROUP:
            return  "GL_DEBUG_TYPE_POP_GROUP";
        case   GL_DEBUG_TYPE_OTHER:
            return "GL_DEBUG_TYPE_OTHER";

            //SEVERITY Type
        case   GL_DEBUG_SEVERITY_HIGH:
            return "GL_DEBUG_SEVERITY_HIGH";
        case    GL_DEBUG_SEVERITY_MEDIUM:
            return "GL_DEBUG_SEVERITY_MEDIUM";
        case    GL_DEBUG_SEVERITY_LOW:
            return "GL_DEBUG_SEVERITY_LOW";
        case    GL_DEBUG_SEVERITY_NOTIFICATION:
            return "GL_DEBUG_SEVERITY_NOTIFICATION";
        default:
            return "not implenented";
    }
}

void MessageCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam){
    if(severity==GL_DEBUG_SEVERITY_HIGH){
        __android_log_print(android_LogPriority::ANDROID_LOG_ERROR, "UnityAREngine", "GL callback:soure:%s \n type:%s \n id:%d \n severity:%s \n message:%s",
                            GlenumToGlChar(source),GlenumToGlChar(type),id,GlenumToGlChar(severity),message);
    }else if(severity==GL_DEBUG_SEVERITY_MEDIUM){
        __android_log_print(android_LogPriority::ANDROID_LOG_WARN, "UnityAREngine", "GL callback:soure:%s \n type:%s \n id:%d \n severity:%s \n message:%s",
                            GlenumToGlChar(source),GlenumToGlChar(type),id,GlenumToGlChar(severity),message);
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    if (javaVM->GetEnv((void **) &jniEnv, JNI_VERSION_1_6) == JNI_OK) {
        LOG_INFO(TAG,"JNI_OnLoad");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(MessageCallback,0);
        return JNI_VERSION_1_6;
    } else {
        return -1;
    }
}

extern "C" JNIEXPORT void *JNICALL
UnityAREngine_GetJavaVM() {
    return javaVM;
}

extern "C" JNIEXPORT void *JNICALL
UnityAREngine_GetJNIEnv() {
    return jniEnv;
}
extern "C" JNIEXPORT void *JNICALL
UnityAREngine_GetContext() {
    jclass unityPlayerClass = jniEnv->FindClass("com/unity3d/player/UnityPlayer");
    jfieldID jfieldId = jniEnv->GetStaticFieldID(unityPlayerClass, "currentActivity",
                                                 "Landroid/app/Activity;");
    jobject currentActivity = jniEnv->GetStaticObjectField(unityPlayerClass, jfieldId);
    jclass activityClass = jniEnv->FindClass("android/app/Activity");
    jmethodID jmethodId = jniEnv->GetMethodID(activityClass, "getApplicationContext",
                                              "()Landroid/content/Context;");
    jobject context = jniEnv->CallObjectMethod(currentActivity, jmethodId);
    jniEnv->DeleteLocalRef(activityClass);
    jniEnv->DeleteLocalRef(currentActivity);
    jniEnv->DeleteLocalRef(unityPlayerClass);
    return context;

}





extern "C" JNIEXPORT GLuint JNICALL
UnityAREngine_GenerateGLTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
    return texture;
}
extern "C" JNIEXPORT void JNICALL
UnityAREngine_DeleteGlTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}

extern "C" JNIEXPORT void JNICALL
UnityAREngine_SetReferences(const HwArSession *arSession, const HwArFrame *arFrame) {
    AREngine::InputProvider::arSession =const_cast<HwArSession *>(arSession);
    AREngine::InputProvider::arFrame = const_cast<HwArFrame *>(arFrame);
    __android_log_print(android_LogPriority::ANDROID_LOG_INFO, "UnityAREngine", "UnityAREngine_SetReferences");

}





extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces *unityInterfaces) {
    LOG_INFO(TAG,"UnityPluginLoad");
    AREngine::InputLifecycleProivider::Register(unityInterfaces);

}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginUnload() {
    LOG_INFO(TAG,"UnityPluginUnload");
}
    // Unknown / unsupported graphics device type? Do nothing









