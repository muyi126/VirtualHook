#include <elf.h>//
// VirtualApp Native Project
//

#include <fb/include/fb/Build.h>
#include <fb/include/fb/fbjni.h>
#include <jni/jni_helpers.h>
#include "VAJni.h"
#include <fb/include/fb/ALog.h>

using namespace facebook::jni;

static void jni_nativeHookExec(alias_ref<jclass> jclazz, jint api_level) {
    IOUniformer::hookExec(api_level);
}

static void jni_nativeLaunchEngine(alias_ref<jclass> clazz, JArrayClass<jobject> javaMethods,
                                   jstring packageName,
                                   jboolean isArt, jint apiLevel, jint cameraMethodType) {
    hookAndroidVM(javaMethods, packageName, isArt, apiLevel, cameraMethodType);
}



static void jni_nativeInvalidateConfig(alias_ref<jclass> clazz, jstring _packageName) {
    JNIEnv *env = Environment::current();

    jclass objectClass = (env)->FindClass("com/lody/virtual/configs/ConfigControlHandler");
    jmethodID configControlHandlerInstanceMid = (env)->GetStaticMethodID(objectClass, "getInstance",
                                                                         "()Lcom/lody/virtual/configs/ConfigControlHandler;");
    jobject configControlHandler = (env)->CallStaticObjectMethod(objectClass,
                                                                 configControlHandlerInstanceMid);

    jmethodID configControlHandlerAppConfigControlMid = (env)->GetMethodID(objectClass,
                                                                           "getAppConfigControl",
                                                                           "(Ljava/lang/String;)Lcom/lody/virtual/configs/ConfigControlHandler$ConfigControl;");

    jobject appConfigControl = (env)->CallObjectMethod(configControlHandler,
                                                       configControlHandlerAppConfigControlMid,
                                                       _packageName);

    jclass aCC_cls = env->GetObjectClass(appConfigControl);
    int enableCamera;
    int enableClipBoard;
    int enableStoreEncrypted;
    jfieldID jId = facebook::getFieldIdOrThrow(env, aCC_cls, "enableCamera", "Z");
    jboolean cc = env->GetBooleanField(appConfigControl, jId);
    enableCamera = (cc == JNI_TRUE);

    jfieldID clipboardId = facebook::getFieldIdOrThrow(env, aCC_cls, "enableClipboard", "Z");
    cc = env->GetBooleanField(appConfigControl, clipboardId);
    enableClipBoard = (cc == JNI_TRUE);

    jfieldID storeEncryptedId = facebook::getFieldIdOrThrow(env, aCC_cls, "enableStoreEncrypted",
                                                            "Z");
    cc = env->GetBooleanField(appConfigControl, storeEncryptedId);
    enableStoreEncrypted = (cc == JNI_TRUE);
    VaConfig vaConfig = { enableCamera,enableClipBoard,enableStoreEncrypted};
    sgm_start_Vhook(&vaConfig);

    ALOGI("enableCamera: %s\n enableClipBoard: %s\n enableStoreEncrypted %s\n",
          (enableCamera ? "true" : "false"),
          ((enableClipBoard ? "true" : "false")),
          ((enableStoreEncrypted ? "true" : "false")));


}


static void jni_nativeEnableIORedirect(alias_ref<jclass>, jstring selfSoPath, jint apiLevel,
                                       jint preview_api_level) {
    ScopeUtfString so_path(selfSoPath);
    IOUniformer::startUniformer(so_path.c_str(), apiLevel, preview_api_level);
}

static void jni_nativeIOWhitelist(alias_ref<jclass> jclazz, jstring _path) {
    ScopeUtfString path(_path);
    IOUniformer::whitelist(path.c_str());
}

static void jni_nativeIOForbid(alias_ref<jclass> jclazz, jstring _path) {
    ScopeUtfString path(_path);
    IOUniformer::forbid(path.c_str());
}


static void jni_nativeIORedirect(alias_ref<jclass> jclazz, jstring origPath, jstring newPath) {
    ScopeUtfString orig_path(origPath);
    ScopeUtfString new_path(newPath);
    IOUniformer::redirect(orig_path.c_str(), new_path.c_str());

}

static jstring jni_nativeGetRedirectedPath(alias_ref<jclass> jclazz, jstring origPath) {
    ScopeUtfString orig_path(origPath);
    const char *redirected_path = IOUniformer::query(orig_path.c_str());
    if (redirected_path != NULL) {
        return Environment::current()->NewStringUTF(redirected_path);
    }
    return NULL;
}

static jstring jni_nativeReverseRedirectedPath(alias_ref<jclass> jclazz, jstring redirectedPath) {
    ScopeUtfString redirected_path(redirectedPath);
    const char *orig_path = IOUniformer::reverse(redirected_path.c_str());
    return Environment::current()->NewStringUTF(orig_path);
}


alias_ref<jclass> nativeEngineClass;


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
    return initialize(vm, [] {
        nativeEngineClass = findClassStatic("com/lody/virtual/client/NativeEngine");
        nativeEngineClass->registerNatives({
                                                   makeNativeMethod("nativeHookExec",
                                                                    jni_nativeHookExec),
                                                   makeNativeMethod("nativeEnableIORedirect",
                                                                    jni_nativeEnableIORedirect),
                                                   makeNativeMethod("nativeIOWhitelist",
                                                                    jni_nativeIOWhitelist),
                                                   makeNativeMethod("nativeIOForbid",
                                                                    jni_nativeIOForbid),
                                                   makeNativeMethod("nativeIORedirect",
                                                                    jni_nativeIORedirect),
                                                   makeNativeMethod("nativeGetRedirectedPath",
                                                                    jni_nativeGetRedirectedPath),
                                                   makeNativeMethod("nativeReverseRedirectedPath",
                                                                    jni_nativeReverseRedirectedPath),
                                                   makeNativeMethod("nativeLaunchEngine",
                                                                    jni_nativeLaunchEngine),
                                                   makeNativeMethod("nativeInvalidateConfig",
                                                                    jni_nativeInvalidateConfig),
                                           }
        );
    });
}

extern "C" __attribute__((constructor)) void _init(void) {
    IOUniformer::init_env_before_all();
}


