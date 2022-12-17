#include "jni-common.h"
#include "rime_jni.h"

// customize settings

static RimeLeversApi* get_levers() {
  return (RimeLeversApi*) (RimeFindModule("levers")->get_api());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_osfans_trime_core_Rime_customize_1bool(JNIEnv *env, jclass /* thiz */, jstring name, jstring key, jboolean value) {
  RimeLeversApi* levers = get_levers();
  const char* s = env->GetStringUTFChars(name, nullptr);
  RimeCustomSettings* settings = levers->custom_settings_init(s, TAG);
  Bool b = levers->load_settings(settings);
  env->ReleaseStringUTFChars(name, s);
  if (b) {
    s = env->GetStringUTFChars(key, nullptr);
    if (levers->customize_bool(settings, s, value)) levers->save_settings(settings);
    env->ReleaseStringUTFChars(key, s);
  }
  levers->custom_settings_destroy(settings);
  return b;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_osfans_trime_core_Rime_customize_1int(JNIEnv *env, jclass /* thiz */, jstring name, jstring key, jint value) {
  RimeLeversApi* levers = get_levers();
  const char* s = env->GetStringUTFChars(name, nullptr);
  RimeCustomSettings* settings = levers->custom_settings_init(s, TAG);
  Bool b = levers->load_settings(settings);
  env->ReleaseStringUTFChars(name, s);
  if (b) {
    s = env->GetStringUTFChars(key, nullptr);
    if (levers->customize_int(settings, s, value)) levers->save_settings(settings);
    env->ReleaseStringUTFChars(key, s);
  }
  levers->custom_settings_destroy(settings);
  return b;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_osfans_trime_core_Rime_customize_1double(JNIEnv *env, jclass /* thiz */, jstring name, jstring key, jdouble value) {
  RimeLeversApi* levers = get_levers();
  const char* s = env->GetStringUTFChars(name, nullptr);
  RimeCustomSettings* settings = levers->custom_settings_init(s, TAG);
  Bool b = levers->load_settings(settings);
  env->ReleaseStringUTFChars(name, s);
  if (b) {
    s = env->GetStringUTFChars(key, nullptr);
    if (levers->customize_double(settings, s, value)) levers->save_settings(settings);
    env->ReleaseStringUTFChars(key, s);
  }
  levers->custom_settings_destroy(settings);
  return b;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_osfans_trime_core_Rime_customize_1string(JNIEnv *env, jclass /* thiz */, jstring name, jstring key, jstring value) {
  RimeLeversApi* levers = get_levers();
  const char* s = env->GetStringUTFChars(name, nullptr);
  RimeCustomSettings* settings = levers->custom_settings_init(s, TAG);
  Bool b = levers->load_settings(settings);
  env->ReleaseStringUTFChars(name, s);
  if (b) {
    s = env->GetStringUTFChars(key, nullptr);
    const char* c_value = env->GetStringUTFChars(value, nullptr);
    if (levers->customize_string(settings, s, c_value)) levers->save_settings(settings);
    env->ReleaseStringUTFChars(key, s);
    env->ReleaseStringUTFChars(value, c_value);
  }
  levers->custom_settings_destroy(settings);
  return b;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_osfans_trime_core_Rime_getAvailableRimeSchemaList(JNIEnv *env, jclass /* thiz */) {
  auto levers = get_levers();
  auto switcher = levers->switcher_settings_init();
  RimeSchemaList list = {0};
  levers->load_settings((RimeCustomSettings *) switcher);
  levers->get_available_schema_list(switcher, &list);
  auto array = rimeSchemaListToJObjectArray(env, list);
  levers->schema_list_destroy(&list);
  levers->custom_settings_destroy((RimeCustomSettings *) switcher);
  return array;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_osfans_trime_core_Rime_getSelectedRimeSchemaList(JNIEnv *env, jclass /* thiz */) {
  auto levers = get_levers();
  auto switcher = levers->switcher_settings_init();
  RimeSchemaList list = {0};
  levers->load_settings((RimeCustomSettings *) switcher);
  levers->get_selected_schema_list(switcher, &list);
  auto array = rimeSchemaListToJObjectArray(env, list);
  levers->schema_list_destroy(&list);
  levers->custom_settings_destroy((RimeCustomSettings *) switcher);
  return array;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_osfans_trime_core_Rime_select_1schemas(JNIEnv *env, jclass /* thiz */, jobjectArray stringArray) {
  if (stringArray == nullptr) return false;
  int count = env->GetArrayLength(stringArray);
  if (count == 0) return false;
  const char** schema_id_list = new const char*[count];
  for (int i = 0; i < count; i++) {
    auto string = (jstring) env->GetObjectArrayElement(stringArray, i);
    const char *rawString = env->GetStringUTFChars(string, nullptr);
    schema_id_list[i] = rawString;
  }
  RimeLeversApi* api_ = get_levers();
  RimeSwitcherSettings* settings_ = api_->switcher_settings_init();
  auto *custom_settings_ = (RimeCustomSettings *) settings_;
  Bool b = api_->load_settings(custom_settings_);
  if (b) {
    b = api_->select_schemas(settings_, schema_id_list, count);
    api_->save_settings(custom_settings_);
    api_->custom_settings_destroy(custom_settings_);
  }
  for (int i = 0; i < count; i++) {
    auto string = (jstring) env->GetObjectArrayElement(stringArray, i);
    const char *rawString = schema_id_list[i];
    env->ReleaseStringUTFChars(string, rawString);
  }
  delete[] schema_id_list;
  return b;
}
