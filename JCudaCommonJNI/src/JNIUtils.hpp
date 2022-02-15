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

template <typename JavaArray, typename JavaElement, typename NativeElement>
NativeElement* getArrayContentsGeneric(JNIEnv *env, JavaArray ja, int* length = NULL)
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
    JavaElement *a = (JavaElement*)env->GetPrimitiveArrayCritical(ja, NULL);
    if (a == NULL)
    {
        return NULL;
    }
    NativeElement *result = new NativeElement[len];
    if (result == NULL)
    {
        env->ReleasePrimitiveArrayCritical(ja, a, JNI_ABORT);
        return NULL;
    }
    for (int i = 0; i<len; i++)
    {
        result[i] = (NativeElement)a[i];
    }
    env->ReleasePrimitiveArrayCritical(ja, a, JNI_ABORT);
    return result;
}

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
template <typename JavaArrayType, typename JavaType, typename NativeType>
bool initNativeGeneric(JNIEnv *env, JavaArrayType &javaObject, NativeType* &nativeObject, bool fill)
{
    if (javaObject == NULL)
    {
        nativeObject = NULL;
        return true;
    }
    jsize length = env->GetArrayLength(javaObject);
    nativeObject = new NativeType[size_t(length)];
    if (nativeObject == NULL)
    {
        ThrowByName(env, "java/lang/OutOfMemoryError",
            "Out of memory during array creation");
        return false;
    }
    if (fill)
    {
        JavaType *primitiveArray =
            (JavaType*)env->GetPrimitiveArrayCritical(javaObject, NULL);
        if (primitiveArray == NULL)
        {
            delete[] nativeObject;
            nativeObject = NULL;
            return false;
        }
        for (jint i = 0; i < length; i++)
        {
            nativeObject[i] = (NativeType)primitiveArray[i];
        }
        env->ReleasePrimitiveArrayCritical(javaObject, primitiveArray, JNI_ABORT);
    }
    return true;
}

/**
* Release the given array by deleting it and setting the pointer to NULL.
* The writeBack flag indicates whether the data from the given array
* should be written into the given java array
*/
template <typename JavaType, typename JavaArrayType, typename NativeType>
bool releaseNativeGeneric(JNIEnv *env, NativeType* &nativeObject, JavaArrayType &javaObject, bool writeBack)
{
    if (javaObject == NULL)
    {
        delete[] nativeObject;
        nativeObject = NULL;
        return true;
    }
    if (writeBack)
    {
        jsize length = env->GetArrayLength(javaObject);
        JavaType *primitiveArray =
            (JavaType*)env->GetPrimitiveArrayCritical(javaObject, NULL);
        if (primitiveArray == NULL)
        {
            delete[] nativeObject;
            nativeObject = NULL;
            return false;
        }
        for (jint i = 0; i < length; i++)
        {
            primitiveArray[i] = (JavaType)nativeObject[i];
        }
        env->ReleasePrimitiveArrayCritical(javaObject, primitiveArray, 0);
    }
    delete[] nativeObject;
    nativeObject = NULL;
    return true;
}

bool initNative(JNIEnv* env, jintArray javaObject, int*& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, int*& nativeObject, jintArray javaObject, bool writeBack);

bool initNative(JNIEnv* env, jintArray javaObject, unsigned int*& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, unsigned int*& nativeObject, jintArray javaObject, bool writeBack);

bool initNative(JNIEnv* env, jlongArray javaObject, long*& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, long*& nativeObject, jlongArray javaObject, bool writeBack);

bool initNative(JNIEnv* env, jlongArray javaObject, unsigned long*& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, unsigned long*& nativeObject, jlongArray javaObject, bool writeBack);

bool initNative(JNIEnv* env, jlongArray javaObject, size_t*& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, size_t*& nativeObject, jlongArray javaObject, bool writeBack);

bool initNative(JNIEnv *env, jobjectArray javaObject, int** &nativeObject, bool fill);
bool releaseNative(JNIEnv *env, int** &nativeObject, jobjectArray javaObject, bool writeBack);

bool initNative(JNIEnv* env, jbyteArray javaObject, char*& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, char*& nativeObject, jbyteArray javaObject, bool writeBack);

bool initNative(JNIEnv* env, jobjectArray javaObject, char**& nativeObject, bool fill);
bool releaseNative(JNIEnv* env, char**& nativeObject, jobjectArray javaObject, bool writeBack);

#endif