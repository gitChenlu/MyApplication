package com.learn.rxjava_library;

/**
 * @ProjectName: My Application
 * @Package: com.learn.rxjava_library
 * @Author: l.chen
 * @CreateDate: 2021/9/28 19:30
 * @Description:
 */
public interface Observer<T> {

    void onNext(T t);

    void onSubscribe();

    void onError(Throwable t);

    void onComplete();

}
