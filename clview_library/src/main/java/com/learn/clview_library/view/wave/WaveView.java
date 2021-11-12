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
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
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

    private int MAX_PROGRESS = 100;
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

    private Paint bgPaint;

    private Paint edgePaint;
    /**
     * 波浪的画布
     */
    private Canvas waveCanvas;

    private Bitmap waveBitmap;

    //水位上升的高度
    private float depth;

    private int currentProgress;

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
        waveHeight = array.getDimension(R.styleable.WaveView_waveHeight,100);
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
        wavePaint.setAntiAlias(true);
        wavePaint.setStyle(Paint.Style.FILL);
        wavePaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_IN));

        bgPaint = new Paint();
        bgPaint.setAntiAlias(true);
        bgPaint.setColor(waveBackgroudColor);
        bgPaint.setStyle(Paint.Style.FILL);

        edgePaint = new Paint();
        edgePaint.setAntiAlias(true);
        edgePaint.setColor(Color.BLUE);
        edgePaint.setStyle(Paint.Style.STROKE);
        edgePaint.setStrokeWidth(5);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        width = w;
        height = h;
        paintShader();
    }

    private void paintShader(){
        if(getWidth()<=0||getHeight()<=0){
            return;
        }
        waveBitmap = Bitmap.createBitmap(getWidth(),getHeight(), Bitmap.Config.ARGB_8888);
        waveCanvas = new Canvas(waveBitmap);

    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        depth = height * ((float) currentProgress/MAX_PROGRESS);
        waveCanvas.drawCircle(width/2,height/2,height/2,bgPaint);
        wavePath.reset();
        wavePath.moveTo(-width+dx,height - depth);
        //利用二阶贝塞尔曲线绘制
        for (int i=0;i<waveCount+1;i++){
            wavePath.rQuadTo(width / 4, -waveHeight, width / 2, 0);
            wavePath.rQuadTo(width / 4, waveHeight, width / 2, 0);
        }
        wavePath.lineTo(width,height);
        wavePath.lineTo(0,height);
        wavePath.close();
        waveCanvas.drawPath(wavePath,wavePaint);
        canvas.drawBitmap(waveBitmap,0,0,null);
        canvas.drawCircle(width/2,height/2,width/2,edgePaint);
    }

    public void startAnim(){
        ValueAnimator anim = ValueAnimator.ofFloat(0,1);
        anim.setDuration(2*1000);
        anim.setInterpolator(new LinearInterpolator());
        anim.setRepeatCount(ValueAnimator.INFINITE);
        anim.addUpdateListener(animation->{
            float va = (float)anim.getAnimatedValue();
            dx = (int) (width*va);
            postInvalidate();
        });
        anim.start();
    }

    public void setProgress(int progress){
        currentProgress = progress;
    }
}
