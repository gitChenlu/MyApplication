package com.learn.rxjava_library;

/**
 * @ProjectName: My Application
 * @Package: com.learn.rxjava_library
 * @Author: l.chen
 * @CreateDate: 2021/9/28 20:12
 * @Description:
 */
public interface ObservableOnSubscrible<T> {

    void subscrible(Emitter<T> emitter);

}
