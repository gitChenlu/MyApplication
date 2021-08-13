package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import com.example.mylibrary.JniTest;
import com.learn.clview_library.view.CustomWaveView;
import com.learn.clview_library.view.dragbubble.DragBubbleView;
import com.learn.clview_library.view.wave.WaveView;

public class MainActivity extends AppCompatActivity {

    TextView text;
    CustomWaveView waveView;

    private int currentProgress = 0;
    private int maxProgress = 100;
    Handler handler = new Handler(Looper.getMainLooper()){
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);

            switch (msg.what){
                case 0:
                    waveView.start();
                    waveView.setCurrentProgress(currentProgress);
                    currentProgress ++;
                    break;
                default:
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //text = findViewById(R.id.text);
        /*DragBubbleView dragBubbleView = findViewById(R.id.drag_view);
        Button bt_reduction = findViewById(R.id.bt_reduction);
        dragBubbleView.setText("99+");
        bt_reduction.setOnClickListener(o->{
            dragBubbleView.reCreate();
        });*/
        DragBubbleView dragBubbleView = findViewById(R.id.drag_view);
        dragBubbleView.setText("10");
        Button bt_reduction = findViewById(R.id.bt_reduction);
        bt_reduction.setOnClickListener(o->{
            dragBubbleView.reset();
        });

        WaveView waveView = findViewById(R.id.wv);
        waveView.startAnim();

/*        waveView = (CustomWaveView)findViewById(R.id.custom_circle_wave_view);
        //设置圆的半径
        waveView.setRadius(100);
        //设置进度最大值
        waveView.setMaxProgress(maxProgress);
        //设置进度的当前值
        waveView.setCurrentProgress(currentProgress);
        //模拟下载。每个
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (currentProgress < maxProgress){
                    try {
                        Thread.sleep(100);
                        handler.sendEmptyMessage(0);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                }
            }
        }).start();*/
    }
}
