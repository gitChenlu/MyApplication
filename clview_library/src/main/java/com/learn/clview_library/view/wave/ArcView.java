package com.learn.clview_library.view.wave;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.Nullable;

import com.learn.clview_library.R;

/**
 * @ProjectName: My Application
 * @Package: com.learn.clview_library.view.wave
 * @Author: l.chen
 * @CreateDate: 2021/8/16 18:00
 * @Description:
 */
public class ArcView extends View {

    private int MAX_PROGRESS = 100;

    private int width;
    private int height;
    private int circleColor;
    private int circleSize;
    private int arcColor;
    private int textColor;
    private int textSize;

    private int currentProgress;

    private Paint circlePaint,arcPaint,textPaint;
    public ArcView(Context context) {
        this(context,null);
    }

    public ArcView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs,0);
    }

    public ArcView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView(context,attrs);
        initPaint();
    }

    private void initView(Context context,AttributeSet attrs){
        TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.ArcView);
        circleColor = typedArray.getColor(R.styleable.ArcView_bg_circle_color, Color.GRAY);
        circleSize = typedArray.getInt(R.styleable.ArcView_circle_size,16);
        arcColor = typedArray.getColor(R.styleable.ArcView_arc_color,Color.RED);
        textColor = typedArray.getColor(R.styleable.ArcView_text_color,Color.BLUE );
        textSize = typedArray.getInt(R.styleable.ArcView_text_size,36);
    }

    private void initPaint(){
        circlePaint = new Paint();
        circlePaint.setColor(circleColor);
        circlePaint.setAntiAlias(true);
        circlePaint.setStyle(Paint.Style.STROKE);
        circlePaint.setStrokeWidth(circleSize);

        arcPaint = new Paint();
        arcPaint.setColor(arcColor);
        arcPaint.setAntiAlias(true);
        arcPaint.setStyle(Paint.Style.STROKE);
        arcPaint.setStrokeCap(Paint.Cap.ROUND);
        arcPaint.setStrokeWidth(circleSize);

        textPaint = new Paint();
        textPaint.setColor(textColor);
        textPaint.setTextSize(textSize);
        textPaint.setAntiAlias(true);
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
        int angle = (int) (360 * ((float)currentProgress/MAX_PROGRESS));
        canvas.drawCircle(width/2,height/2,width/2,circlePaint);
        RectF rectF = new RectF(0,0,width,height);
        canvas.drawArc(rectF,-90,angle,false,arcPaint);
        canvas.drawText(currentProgress+"%",width/2,height/2,textPaint);
    }

    public void setProgress(int progress){
        currentProgress = progress;
    }
}
