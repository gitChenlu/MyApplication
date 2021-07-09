#include <jni.h>
#include <string>
#include "test1.h"
using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_mylibrary_JniTest_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL Java_com_example_mylibrary_JniTest_jniTest(
        JNIEnv* env,
jobject,int i) {
std::string hello = "this is test!";
return i+2;
}

/*
 @brief: 根据不同的糖尿病类型，将血糖值分类为极低(-2)、低(-1)、正常(0)、高(1)、极高(2)，我称之为血糖水平编码。
 @param:
		input:
			glucose：血糖值,float
			type：糖尿病类型。int,0：正常；1：1、2型糖尿病；2：妊娠糖；3：老年糖；4：自定义；默认正常。
			low:低血糖阈值（自定义值，仅在type为4时启用）,float
			high:高血糖阈值（自定义值，仅在type为4时启用）,float
 @ret: tir code,值为{-2，-1,0,1,2},int
*/
extern "C" JNIEXPORT jint JNICALL  Java_com_example_mylibrary_JniTest_test(JNIEnv* env,
                                                     jobject,float glucose, int type = 0, float low = 3.9, float high = 7.8)
{
    //	glucose = round(glucose * 10) / 10.0;
    int code = 0;
    switch (type)
    {
        case 0:
        {
            //			if (glucose <= 3.9)
            if (glucose - 3.9 < 0.0005)
                code = -1;
                //			else if (glucose < 7.8)
            else if (glucose - 7.8 < -0.0005)
                code = 0;
            else
                code = 1;
            break;
        }
        case 1:
        {
            //			if (glucose < 3.0)
            if (glucose - 3.0 < -0.005)
                code = -2;
                //			else if (glucose < 3.9)
            else if (glucose - 3.9 < -0.005)
                code = -1;
                //			else if (glucose <= 10.0)
            else if (glucose - 10.0 < 0.005)
                code = 0;
                //			else if (glucose <= 13.9)
            else if (glucose - 13.9 < 0.005)
                code = 1;
            else
                code = 2;
            break;
        }
        case 2:
        {
            //			if (glucose < 3.0)
            if (glucose - 3.0 < -0.005)
                code = -2;
                //			else if (glucose < 3.5)
            else if (glucose - 3.5 < -0.005)
                code = -1;
                //			else if (glucose <= 7.8)
            else if (glucose - 7.8 < 0.005)
                code = 0;
            else
                code = 1;
            break;
        }
        case 3:
        {
            //			if (glucose < 3.9)
            if (glucose - 3.9 < -0.005)
                code = -1;
                //			else if (glucose <= 10.0)
            else if (glucose - 10.0 < 0.005)
                code = 0;
                //			else if (glucose <= 13.9)
            else if (glucose - 13.9 < 0.005)
                code = 1;
            else
                code = 2;
            break;
        }
        case 4:
        {
            //			if(glucose <= low)
            if (glucose - low < 0.005)
                code = -1;
                //			else if (glucose < high)
            else if (glucose - high < -0.005)
                code = 0;
            else
                code = 1;
            break;
        }
        default:
        {
            //			if (glucose <= 3.9)
            if (glucose - 3.9 < 0.0005)
                code = -1;
                //			else if (glucose < 7.8)
            else if (glucose - 7.8 < -0.0005)
                code = 0;
            else
                code = 1;
            break;
        }
    }
    return code;
}

/*
@brief: 计算均值。
@param:
	input:
		bg_array：血糖值数组，float*
		N:数组长度，int
@ret: 血糖均值,float
*/

extern "C" JNIEXPORT jfloat JNICALL Java_com_example_mylibrary_JniTest_calcMean(JNIEnv* env,
                                                                                 jobject,jfloatArray array, jint N)
{
    float mean_value = 0;
    float sum_value = 0;
    float *bg_array = env ->GetFloatArrayElements(array,NULL);
    //	int N = length(bg);
    for (int i = 0; i < N; i++)
    {
        sum_value += bg_array[i];
    }
    if (sum_value != 0)
        mean_value = sum_value / N;
    return mean_value;
}


extern "C" JNIEXPORT jint JNICALL Java_com_example_mylibrary_JniTest_calcStd(JNIEnv* env,
                                                                                jobject,jfloatArray array, jint n)
{
    /*float mean_value = 0;
    float *bg_array = env->GetFloatArrayElements(array,NULL);
    mean_value = calc_std(*bg_array,n);*/
    int result = 2;

    return result;
}
