package com.learn.rxjava_library;

/**
 * @ProjectName: My Application
 * @Package: com.learn.rxjava_library
 * @Author: l.chen
 * @CreateDate: 2021/9/28 20:15
 * @Description:
 */
public interface Emitter<T> {

    void onNext(T t);

    void onError(Throwable t);

    void onComplete();
}
