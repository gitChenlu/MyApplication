package com.learn.rxjava_library;

/**
 * @ProjectName: My Application
 * @Package: com.learn.rxjava_library
 * @Author: l.chen
 * @CreateDate: 2021/9/28 19:40
 * @Description:
 */
public class ObservableCreate<T> extends Observable{

    ObservableOnSubscrible<T> source;
    public ObservableCreate(ObservableOnSubscrible<T> observableOnSubscrible){
        source = observableOnSubscrible;
    }

    @Override
    public void subscribleActual(Observer observer) {
        observer.onSubscribe();
        CreateEmitter emitter = new CreateEmitter(observer);
        source.subscrible(emitter);
    }

    public class CreateEmitter<T> implements Emitter<T>{

        Observer observer;

        public CreateEmitter(Observer<T> observer){
            this.observer = observer;
        }
        @Override
        public void onNext(T t) {
            observer.onNext(t);
        }

        @Override
        public void onError(Throwable t) {
            observer.onError(t);
        }

        @Override
        public void onComplete() {
            observer.onComplete();
        }
    }
}
