package com.learn.clview_library.view.wave;

import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Shader;
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

    /**
     * 控件的宽度
     */
    private float width;
    /**
     * 控件的高度
     */
    private float height;
    /**
     *
     */
    private Paint wavePaint;
    private Path wavePath;
    private int dx;

    /**
     * 波浪的高度
     */
    private float waveHeight;
    /**
     * 波浪的个数
     */
    private int waveCount;
    /**
     * 波浪的颜色
     */
    private int waveColor;
    /**
     * 背景颜色
     */
    private int waveBackgroudColor;
    /**
     * 背景形状
     */
    private int waveBackgroudShape;

    /**
     * 波浪相对控件的高度
     */
    private float bgHeight;

    public WaveView(Context context) {
        this(context,null);
    }

    public WaveView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs,0);
    }

    public WaveView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView(context,attrs,defStyleAttr);
        initPaint();
    }

    private void initView(Context context, AttributeSet attrs,int def){
        TypedArray array = context.obtainStyledAttributes(attrs,R.styleable.WaveView);
        waveHeight = array.getDimension(R.styleable.WaveView_waveHeight,150);
        waveColor = array.getColor(R.styleable.WaveView_waveColor,Color.GRAY);
        waveCount = array.getInteger(R.styleable.WaveView_waveCount,1);
        waveBackgroudColor = array.getColor(R.styleable.WaveView_waveBackgroudColor,Color.WHITE);
        waveBackgroudShape = array.getInteger(R.styleable.WaveView_waveBackgroudShape,0);
        bgHeight = array.getDimension(R.styleable.WaveView_bgHeight,0);
        wavePath = new Path();
    }

    private void initPaint(){
        wavePaint = new Paint();
        wavePaint.setColor(waveColor);
        wavePaint.setStyle(Paint.Style.FILL);

    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        width = w;
        height = h;
        paintShader();
    }

    private void paintShader(){
        Bitmap bitmap = Bitmap.createBitmap(getWidth(),getHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        wavePath.reset();
        wavePath.moveTo(-width+dx,waveHeight);
        //利用二阶贝塞尔曲线绘制
        for (int i=0;i<waveCount+1;i++){
            wavePath.rQuadTo(width / 4, -waveHeight, width / 2, 0);
            wavePath.rQuadTo(width / 4, waveHeight, width / 2, 0);
        }
        //wavePath.quadTo(width / 4, -150, width / 2, 0);
        //wavePath.quadTo(width * 3 / 4, 150, width , 0);
        wavePath.lineTo(width,height);
        wavePath.lineTo(0,height);
        wavePath.close();
        canvas.drawPath(wavePath,wavePaint);
        BitmapShader shader = new BitmapShader(bitmap, Shader.TileMode.CLAMP, Shader.TileMode.CLAMP);
        wavePaint.setShader(shader);
    }
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawCircle(width/2,height/2,height/2,wavePaint);
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
