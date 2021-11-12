package com.learn.rxjava_library;

/**
 * @ProjectName: My Application
 * @Package: com.learn.rxjava_library
 * @Author: l.chen
 * @CreateDate: 2021/9/28 19:35
 * @Description:
 */
public abstract class Observable<T> implements ObservableSource{

    @Override
    public void subscribleObserver(Observer observer) {
        subscribleActual(observer);
    }

    public abstract void subscribleActual(Observer observer);

    public static<T> Observable create(ObservableOnSubscrible<T> observable){
        return new ObservableCreate(observable);
    }

}
