/*
 * JCuda - Java bindings for NVIDIA CUDA driver and runtime API
 *
 * Copyright (c) 2009-2015 Marco Hutter - http://www.jcuda.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef JNIUTILS
#define JNIUTILS

#include <jni.h>
#include <string>

#ifdef _WIN32
    // Disable "unreferenced formal parameter" warnings
    #pragma warning (disable : 4100)
#endif


bool init(JNIEnv *env, jclass& cls, const char *name);
bool initGlobal(JNIEnv *env, jclass &globalCls, const char *className);
bool init(JNIEnv *env, jclass cls, jfieldID& field, const char *name, const char *signature);
bool init(JNIEnv *env, jclass cls, jmethodID& method, const char *name, const char *signature);
bool init(JNIEnv *env, jclass& globalCls, jmethodID& constructor, const char *className);
bool initNativePointer(JNIEnv *env, jfieldID& field, const char *className);

bool set(JNIEnv *env, jintArray ja, int index, jint value);
bool set(JNIEnv *env, jlongArray ja, int index, jlong value);
bool set(JNIEnv *env, jfloatArray ja, int index, jfloat value);
bool set(JNIEnv *env, jdoubleArray ja, int index, jdouble value);

int* getArrayContents(JNIEnv *env, jintArray ja, int* length = NULL);
char* getArrayContents(JNIEnv *env, jbyteArray ja, int* length = NULL);
long long* getArrayContents(JNIEnv *env, jlongArray ja, int* length = NULL);

template <typename NativeElement>
NativeElement* getLongArrayContentsGeneric(JNIEnv *env, jlongArray ja, int* length = NULL)
{
    if (ja == NULL)
    {
        return NULL;
    }
    jsize len = env->GetArrayLength(ja);
    if (length != NULL)
    {
        *length = (int)len;
    }
    jlong* a = env->GetLongArrayElements(ja, NULL);
    if (a == NULL)
    {
        // OutOfMemoryError is pending
        return NULL;
    }
    NativeElement *result = new NativeElement[len];
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory during array creation");
        return NULL;
    }
    for (int i = 0; i<len; i++)
    {
        result[i] = (NativeElement)a[i];
    }
    env->ReleaseLongArrayElements(ja, a, JNI_ABORT);
    return result;
}

template <typename NativeElement>
NativeElement* getIntArrayContentsGeneric(JNIEnv *env, jintArray ja, int* length = NULL)
{
    if (ja == NULL)
    {
        return NULL;
    }
    jsize len = env->GetArrayLength(ja);
    if (length != NULL)
    {
        *length = (int)len;
    }
    jint* a = env->GetIntArrayElements(ja, NULL);
    if (a == NULL)
    {
        // OutOfMemoryError is pending
        return NULL;
    }
    NativeElement *result = new NativeElement[len];
    if (result == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory during array creation");
        return NULL;
    }
    for (int i = 0; i<len; i++)
    {
        result[i] = (NativeElement)a[i];
    }
    env->ReleaseIntArrayElements(ja, a, JNI_ABORT);
    return result;
}

template <typename NativeElement>
void readIntArrayContentsGeneric(JNIEnv *env, jintArray ja, NativeElement* target, int* length = NULL)
{
    if (ja == NULL)
    {
        return;
    }
    if (target == NULL)
    {
        return;
    }
    jsize len = env->GetArrayLength(ja);
    if (length != NULL)
    {
        *length = (int)len;
    }
    jint* a = env->GetIntArrayElements(ja, NULL);
    if (a == NULL)
    {
        // OutOfMemoryError is pending
        return;
    }
    for (int i = 0; i<len; i++)
    {
        target[i] = (NativeElement)a[i];
    }
    env->ReleaseIntArrayElements(ja, a, JNI_ABORT);
}

template <typename NativeElement>
void writeIntArrayContentsGeneric(JNIEnv *env, NativeElement* source, jintArray ja, int* length = NULL)
{
    if (ja == NULL)
    {
        return;
    }
    if (source == NULL)
    {
        return;
    }
    jsize len = env->GetArrayLength(ja);
    if (length != NULL)
    {
        *length = (int)len;
    }
    jint* a = env->GetIntArrayElements(ja, NULL);
    if (a == NULL)
    {
        // OutOfMemoryError is pending
        return;
    }
    for (int i = 0; i<len; i++)
    {
        a[i] = (jint)source[i];
    }
    env->ReleaseIntArrayElements(ja, a, 0);
}

template <typename NativeElement>
void writeLongArrayContentsGeneric(JNIEnv *env, NativeElement* source, jlongArray ja, int* length = NULL)
{
    if (ja == NULL)
    {
        return;
    }
    if (source == NULL)
    {
        return;
    }
    jsize len = env->GetArrayLength(ja);
    if (length != NULL)
    {
        *length = (int)len;
    }
    jlong* a = env->GetLongArrayElements(ja, NULL);
    if (a == NULL)
    {
        // OutOfMemoryError is pending
        return;
    }
    for (int i = 0; i<len; i++)
    {
        a[i] = (jlong)source[i];
    }
    env->ReleaseLongArrayElements(ja, a, 0);
}


void readFloatArrayContents(JNIEnv *env, jfloatArray ja, float* target, int* length = NULL);
void writeFloatArrayContents(JNIEnv *env, float* source, jfloatArray ja, int* length = NULL);


//bool convertString(JNIEnv *env, jstring js, std::string *s);
char *convertString(JNIEnv *env, jstring js, int *length=NULL);

char **convertStringArray(JNIEnv *env, jobjectArray jsa, int *length=NULL);
void deleteStringArray(char** &array, int length);

//std::string getToString(JNIEnv *env, jobject object);

void ThrowByName(JNIEnv *env, const char *name, const char *msg);

int initJNIUtils(JNIEnv *env);

extern jmethodID String_getBytes; // ()[B

/**
* Create an int array with the same size as the given java array,
* and store it in the given pointer. The caller must delete[] the
* created array. The fill-flag indicates whether the array should
* be initialized with the data from the given array
*/
bool initNative(JNIEnv *env, jintArray javaObject, int* &nativeObject, bool fill);

/**
* Release the given array by deleting it and setting the pointer to NULL.
* The writeBack flag indicates whether the data from the given array
* should be written into the given java array
*/
bool releaseNative(JNIEnv *env, int* &nativeObject, jintArray javaObject, bool writeBack);


#endif