package com.learn.clview_library.view.dragbubble;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.ObjectAnimator;
import android.animation.PointFEvaluator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import androidx.annotation.Nullable;

import com.learn.clview_library.R;


/**
 * @ProjectName: My Application
 * @Package: com.example.myapplication.view2
 * @Author: l.chenlu
 * @CreateDate: 2021/7/5 16:40
 * @Description:
 */
public class DragBubbleView extends View {

    /**
     * 控件的宽高
     */
    private int width;
    private int height;
    /**
     * 气泡的半径
     */
    private float bubbleRadius;
    /**
     * 气泡的颜色
     */
    private int bubbleColor;

    /**
     * 气泡上的文字
     */
    private String text;
    /**
     * 气泡上文字的大小
     */
    private float textSize;
    /**
     * 气泡上文字的颜色
     */
    private int textColor;

    /**
     * 固定小圆的半径
     */
    private float littleCircleRadius;
    /**
     * 气泡的状态
     */
    private STATUS status = STATUS.BUBBLE_DEFAULT;

    /**
     * 两个圆心之前的直线距离
     */
    private float circleCenterDist;

    /**
     * 气泡的圆心X点
     */
    private float bubbleCircleX;
    /**
     * 气泡的圆心Y点
     */
    private float bubbleCircleY;
    /**
     * 固定小圆圆心的X点
     */
    private float littleCircleX;
    /**
     * 固定小圆圆心的Y点
     */
    private float littleCircleY;
    /**
     * 固定小圆贝塞尔曲线起点的X坐标
     */
    private float littleStartX;
    /**
     * 固定小圆贝塞尔曲线起点的Y坐标
     */
    private float littleStartY;
    /**
     * 固定小圆贝塞尔曲线终点的X坐标
     */
    private float littleEndX;
    /**
     * 固定小圆贝塞尔曲线终点的Y坐标
     */
    private float littleEndY;
    /**
     * 气泡贝塞尔曲线起点的X坐标
     */
    private float bubbleStartX;
    /**
     * 气泡贝塞尔曲线起点的Y坐标
     */
    private float bubbleStartY;
    /**
     * 气泡贝塞尔曲线终点的X坐标
     */
    private float bubbleEndX;
    /**
     * 气泡贝塞尔曲线终点的Y坐标
     */
    private float bubbleEndY;
    /**
     * 贝塞尔曲线控制点的X坐标
     */
    private float controlPointX;
    /**
     * 贝塞尔曲线控制点的Y坐标
     */
    private float controlPointY;
    /**
     * 文字Rect,用于计算文字的宽高
     */
    private Rect textRect;
    /**
     * 文字的画板
     */
    private Paint textPaint;
    /**
     * 气泡上的画板
     */
    private Paint bubblePaint;

    /**
     * 气泡消失动画
     */
    private Paint dismissPaint;

    /**
     * 贝塞尔曲线的path
     */
    private Path bubblePath;

    /**
     * 气泡拖拽消失的距离
     */
    private float dismissDist ;

    /**
     * 气泡消失动画的图片
     */
    private int[] dismissDrawable = new int[]{R.drawable.explosion_one,R.drawable.explosion_two,R.drawable.explosion_three,R.drawable.explosion_four,R.drawable.explosion_five};
    private Bitmap[] dismissBitmap;
    /**
     * 气泡消失动画播放第几张
     */
    private int drawableLen;
    /**
     * 是否播放消失动画
     */
    private boolean isPlayDismissAnim;
    /**
     * 气泡的状态
     * BUBBLE_DEFAULT：默认
     * BUBBLE_DRAG：可拖拽状态（默认状态下点击气泡拖拽变为BUBBLE_DRAG）
     * BUBBLE_MOVE：移动状态（松开拖拽气泡回弹）
     * BUBBLE_DISMISS：消失
     *
     */
    enum STATUS{
        BUBBLE_DEFAULT,BUBBLE_DRAG,BUBBLE_MOVE,BUBBLE_DISMISS
    }
    public DragBubbleView(Context context) {
        this(context,null);
    }

    public DragBubbleView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs,0);
    }

    public DragBubbleView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView(context,attrs,defStyleAttr);
    }

    private void initView(Context context,AttributeSet attrs,int defStyleAttr){
        TypedArray array = context.obtainStyledAttributes(attrs, R.styleable.DragBubbleView,defStyleAttr,0);
        bubbleRadius = array.getDimension(R.styleable.DragBubbleView_bubbleRadius,12);
        bubbleColor = array.getColor(R.styleable.DragBubbleView_bubbleColor,Color.RED);
        text = array.getString(R.styleable.DragBubbleView_text);
        textSize = array.getDimension(R.styleable.DragBubbleView_textSize,12);
        textColor = array.getColor(R.styleable.DragBubbleView_textColor,Color.WHITE);
        littleCircleRadius = bubbleRadius/2;
        dismissDist = bubbleRadius * 8;

        dismissBitmap = new Bitmap[dismissDrawable.length];
        for(int i=0;i<dismissDrawable.length;i++){
            dismissBitmap[i] = BitmapFactory.decodeResource(getResources(),dismissDrawable[i]);
        }
        initPaint();
    }

    private void initPaint(){
        textPaint = new Paint();
        textPaint.setAntiAlias(true);
        textPaint.setColor(textColor);
        textPaint.setTextSize(textSize);
        textRect = new Rect();

        bubblePaint = new Paint();
        bubblePaint.setAntiAlias(true);
        bubblePaint.setColor(bubbleColor);

        dismissPaint = new Paint();
        dismissPaint.setFilterBitmap(true);


        bubblePath = new Path();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        width = w;
        height = h;
        initCircle(w,h);
    }

    private void initCircle(int w,int h){
        bubbleCircleX = w/2;
        bubbleCircleY = h/2;
        littleCircleX = w/2;
        littleCircleY = h/2;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if(status== STATUS.BUBBLE_DRAG){
            canvas.drawCircle(littleCircleX,littleCircleY,littleCircleRadius,bubblePaint);
            controlPointX = (bubbleCircleX + littleCircleX) / 2;
            controlPointY = (bubbleCircleY + littleCircleY) / 2;
            float sin = (bubbleCircleX - littleCircleX) / circleCenterDist;
            float cos = (bubbleCircleY - littleCircleY) / circleCenterDist;
            littleStartX = littleCircleX - littleCircleRadius * cos;
            littleStartY = littleCircleY + littleCircleRadius * sin;
            bubbleStartX = bubbleCircleX - bubbleRadius * cos;
            bubbleStartY = bubbleCircleY + bubbleRadius * sin;
            bubbleEndX = bubbleCircleX + bubbleRadius * cos;
            bubbleEndY = bubbleCircleY - bubbleRadius * sin;
            littleEndX = littleCircleX + littleCircleRadius * cos;
            littleEndY = littleCircleY - littleCircleRadius * sin;

            bubblePath.reset();
            bubblePath.moveTo(littleStartX,littleStartY);
            bubblePath.quadTo(controlPointX,controlPointY,bubbleStartX,bubbleStartY);
            bubblePath.lineTo(bubbleEndX,bubbleEndY);
            bubblePath.quadTo(controlPointX,controlPointY,littleEndX,littleEndY);
            bubblePath.close();
            canvas.drawPath(bubblePath,bubblePaint);

        }
        if(status!= STATUS.BUBBLE_DISMISS){
            textPaint.getTextBounds(text,0,text.length(),textRect);
            canvas.drawCircle(bubbleCircleX,bubbleCircleY,bubbleRadius,bubblePaint);
            canvas.drawText(text,bubbleCircleX - textRect.width()/2,bubbleCircleY + textRect.height()/2,textPaint);
        }
        if(status== STATUS.BUBBLE_DISMISS&&isPlayDismissAnim&&drawableLen<dismissDrawable.length){
            RectF rectF = new RectF(bubbleCircleX - bubbleRadius,bubbleCircleY - bubbleRadius,bubbleCircleX + bubbleRadius,bubbleCircleY + bubbleRadius);
            canvas.drawBitmap(dismissBitmap[drawableLen],null,rectF,dismissPaint);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                if(status == STATUS.BUBBLE_DEFAULT){
                    getParent().requestDisallowInterceptTouchEvent(true);
                    float downX = event.getX();
                    float downY = event.getY();
                    if(downX>(bubbleCircleX - bubbleRadius)&&downX<(bubbleCircleX + bubbleRadius)&&downY>(bubbleCircleY - bubbleRadius)&&downY<(bubbleCircleY + bubbleRadius)){
                        status = STATUS.BUBBLE_DRAG;
                    }
                }
                break;
            case MotionEvent.ACTION_MOVE:
                if(status == STATUS.BUBBLE_DRAG){
                    getParent().requestDisallowInterceptTouchEvent(true);
                    bubbleCircleX = event.getX();
                    bubbleCircleY = event.getY();
                    circleCenterDist = (float) Math.hypot(bubbleCircleX - littleCircleX,bubbleCircleY - littleCircleY);
                    littleCircleRadius = bubbleRadius - circleCenterDist / 8 ;
                    if(circleCenterDist>dismissDist){
                        status = STATUS.BUBBLE_MOVE;
                    }
                    invalidate();
                }
                break;
            case MotionEvent.ACTION_UP:
                if(status != STATUS.BUBBLE_DISMISS){
                    getParent().requestDisallowInterceptTouchEvent(false);
                    if (circleCenterDist>dismissDist){
                        status = STATUS.BUBBLE_DISMISS;
                        //invalidate();
                        setDismissAnim();
                    }else {
                        setAnim();
                    }
                }

                break;
        }
        return true;
    }

    /**
     * 气泡回弹动画
     */
    private void setAnim(){
        ValueAnimator animator = ObjectAnimator.ofObject(new PointFEvaluator(),new PointF(bubbleCircleX,bubbleCircleY),new PointF(littleCircleX,littleCircleY));
        animator.setDuration(300);
        //自定义Interpolator差值器达到颤动效果
        animator.setInterpolator(input -> {
            float f = 0.571429f;
            return (float) (Math.pow(2, -4 * input) * Math.sin((input - f / 4) * (2 * Math.PI) / f) + 1);
        });
        //animator.setInterpolator(new OvershootInterpolator());
        animator.addUpdateListener(animation->{
            PointF pointF = (PointF) animation.getAnimatedValue();
            bubbleCircleX = pointF.x;
            bubbleCircleY = pointF.y;
            invalidate();
        });
        animator.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
                status = STATUS.BUBBLE_DEFAULT;
            }
        });
        animator.start();
    }

    /**
     * 气泡消失动画
     */
    private void setDismissAnim(){
        isPlayDismissAnim = true;
        ValueAnimator animator = ObjectAnimator.ofInt(0,dismissDrawable.length);
        animator.setDuration(800);
        animator.addUpdateListener(animation -> {
            drawableLen = (int) animation.getAnimatedValue();
            invalidate();
        });

        animator.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
                isPlayDismissAnim =false;
            }
        });
        animator.start();
    }

    public void reset(){
        initCircle(width,height);
        status = STATUS.BUBBLE_DEFAULT;
        invalidate();
    }

    public void setText(String str){
        text = str;
    }

    public void setText(int textId){
        text = getResources().getString(textId);
    }
}
