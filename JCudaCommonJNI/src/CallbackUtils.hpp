/*
 * JCuda - Java bindings for NVIDIA CUDA driver and runtime API
 *
 * Copyright (c) 2009-2017 Marco Hutter - http://www.jcuda.org
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

#include <jni.h>

/**
 * A structure containing the information about the arguments that have
 * been passed to establish a callback method. A pointer to this structure
 * will be passed as the *userData to the respective function. The
 * function will then use the data from the given structure
 * to call the Java callback method.
 */
typedef struct
{
    /**
    * A global reference to the strea that was given (may be NULL)
    */
    jobject globalStream;

    /**
    * A global reference to the callback function object that was given
    */
    jobject globalJavaCallbackObject;

    /**
    * A global reference to the userData that was given (may be NULL)
    */
    jobject globalUserData;

} CallbackInfo;


CallbackInfo* initCallbackInfo(JNIEnv *env, jobject hStream, jobject javaCallbackObject, jobject userData);
void deleteCallbackInfo(JNIEnv *env, CallbackInfo* &callbackInfo);
void finishCallback(JNIEnv *env);
