#### 1.1 为什么会选择用Rxjava这个框架，它比原生不用它区别在哪里

 优点1：简化逻辑,解耦了各个模块操作，单一化
 比如要嵌套请求的时候，这个时候用flatMap操作符就可以实现优雅的链式嵌套请求

 优点2：简化代码
 它的操作符封装了规则，我们用一个操作符就可以实现许多功能
 比如要打包网络请求，这个时候用zip就可以打包数据源

 优点3：操作符强大，可以实现各种功能
 flatmap解决嵌套回调的问题;mergeWith()可以把不同异步操作合并

 优点4：最方便的是线程切换

 优点5：错处处理
 只要有异常发生onError()一定会被调用，这极大的简化了错误处理。只需要在一个地方处理错误即可以

#### 1.2 rxjava中你了解有多少个操作符



#### 1.3 rxjava中这些操作符为什么能随意组合，而且还能够链式调用，这种链式调用和普通的建造者模式链式调用有什么不同



### 1.4 RxJava的subscribeOn只有第一次生效?

> 如果你理解了订阅的过程，其实该问题很好理解，subscribeOn是规定上游的observable在哪个线程中执行，如果我们执行多次的subscribeOn的话，从下游的observer到上游的observable的订阅过程，**最开始调用的subscribeOn返回的observable会把后面执行的subscribeOn返回的observable给覆盖了，因此我们感官的是只有第一次的subscribeOn能生效。**

那如何才能知道它实际在里面生效了呢，我们可以通过`doOnSubscribe`来监听切实发生线程切换了。

### 1.5 RxJava的observeOn多次调用哪个有效?

> 上面分析了observeOn是指定下游的observer在哪个线程中执行，所以这个更好理解，看observeOn下一个observer是哪一个，**所以多次调用observeOn肯定是最后一个observeOn控制有效。**

### 1.6 RxJava1.0、RxJava2.0、RxJava3.0有什么区别？

RxJava2.0相比于RxJava1.0

- 添加背压的策略Flowable
- 添加Observer的变体consumer
- ActionN 和 FuncN 改名（Action0 改名成Action，Action1改名成Consumer，而Action2改名成了BiConsumer，而Action3 - Action9都不再使用了，ActionN变成了Consumer<Object[]> 。Func改名成Function，Func2改名成BiFunction，Func3 - Func9 改名成 Function3 - Function9，FuncN 由 Function<Object[], R> 取代。）
- Observable.OnSubscribe 变成 ObservableOnSubscribe
- ObservableOnSubscribe 中使用 ObservableEmitter 发射数据给 Observer，在RxJava中使用Subscriber发射数据。
- Subscription 改名为 Disposable

RxJava3.0相比与RxJava2.0

- 提供Java 8 lambda友好的API
- 删除Maybe.toSingle(T)
- 删除Flowable.subscribe(4 args)
- 删除Observable.subscribe(4 args)
- 删除Single.toCompletable()