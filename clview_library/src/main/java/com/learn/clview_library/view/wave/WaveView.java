package com.learn.clview_library.view.wave;

import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.animation.LinearInterpolator;

import androidx.annotation.Nullable;

import com.learn.clview_library.R;

/**
 * @ProjectName: My Application
 * @Package: com.learn.clview_library.view.wave
 * @Author: l.chenlu
 * @CreateDate: 2021/7/12 14:55
 * @Description:
 */
public class WaveView extends View {

    private float width;
    private float height;
    private Paint wavePaint;
    private Path wavePath;
    private int dx;
    public WaveView(Context context) {
        this(context,null);
    }

    public WaveView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs,0);
    }

    public WaveView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
        initPaint();
    }

    private void initView(){
        wavePath = new Path();
    }

    private void initPaint(){
        wavePaint = new Paint();
        wavePaint.setColor(Color.GREEN);
        wavePaint.setStyle(Paint.Style.FILL);
    }
    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        width = w;
        height = h;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        wavePath.reset();
        Log.d("jimilog ","jimilog onDraw dx="+dx);
        wavePath.moveTo(-width+dx,0);
        //利用二阶贝塞尔曲线绘制
        wavePath.rQuadTo(width / 4, -150, width / 2, 0);
        wavePath.rQuadTo(width / 4, 150, width / 2, 0);
        wavePath.rQuadTo(width / 4, -150, width / 2, 0);
        wavePath.rQuadTo(width / 4, 150, width / 2, 0);
        //wavePath.quadTo(width / 4, -150, width / 2, 0);
        //wavePath.quadTo(width * 3 / 4, 150, width , 0);
        wavePath.lineTo(width,height);
        wavePath.lineTo(0,height);
        wavePath.close();
        canvas.drawPath(wavePath,wavePaint);
    }

    public void startAnim(){
        ValueAnimator anim = ValueAnimator.ofFloat(0,1);
        anim.setDuration(2000);
        anim.setInterpolator(new LinearInterpolator());
        anim.setRepeatCount(ValueAnimator.INFINITE);
        anim.addUpdateListener(animation->{
            float va = (float)anim.getAnimatedValue();
            dx = (int) (width*va);
            postInvalidate();
        });
        anim.start();
    }
}
