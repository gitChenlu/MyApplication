package com.learn.kotlinstudy

import android.app.Activity
import android.os.Bundle

/**
 * @ProjectName: My Application
 * @Package: com.learn.kotlinstudy
 * @Author: l.chen
 * @CreateDate: 2021-10-15 17:15
 * @Description:
 */
class TestActivity : Activity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.support_simple_spinner_dropdown_item)

        var study = Study1("a")
        study.getStudyName()

        var t = Test()
        var color = t.getColor()
        var str = t.test()
    }
}