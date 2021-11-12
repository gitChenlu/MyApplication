package com.learn.kotlinstudy

/**
 * @ProjectName: My Application
 * @Package: com.learn.kotlinstudy
 * @Author: l.chen
 * @CreateDate: 2021-10-15 16:57
 * @Description:
 */
class Study1(
        var name:String,
        var age:Int
) {

    fun getStudyName():String {
        return name
    }

    constructor(name: String) : this(name,20)
}