package com.example.mysmarthome;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity extends AppCompatActivity {
    private Button getsmart_button;
    private DatabaseReference helloworld = FirebaseDatabase.getInstance().getReference();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getsmart_button = (Button) findViewById(R.id.getsmart_button);
        getsmart_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, HomeSceen.class);
                startActivity(intent);
            }
        });

//        FirebaseDatabase database = FirebaseDatabase.getInstance();
//        DatabaseReference myRef = database.getReference("message");
//
//        myRef.setValue("Hello World");

        helloworld.child("message/Begin").setValue("Hi");
    }
}