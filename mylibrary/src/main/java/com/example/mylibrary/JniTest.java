package com.example.mylibrary;

/**
 * @ProjectName: My Application
 * @Package: com.example.mylibrary
 * @Author: l.chenlu
 * @CreateDate: 2021/5/19
 * @Description:
 */
public class JniTest {
    static {
        System.loadLibrary("native-lib");
    }
    public static native String stringFromJNI();
    public static native String jniTest();
    public static native int test(float glucose,int type,float low,float high);
    public static native float calcMean(float[] array,int high);
    public static native float calcStd(float[] array,int high);
}
