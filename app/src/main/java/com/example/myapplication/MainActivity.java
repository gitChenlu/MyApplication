package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Button;
import android.widget.TextView;

import com.example.mylibrary.JniTest;
import com.learn.clview_library.view.dragbubble.DragBubbleView;

public class MainActivity extends AppCompatActivity {

    TextView text;
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
    }
}
