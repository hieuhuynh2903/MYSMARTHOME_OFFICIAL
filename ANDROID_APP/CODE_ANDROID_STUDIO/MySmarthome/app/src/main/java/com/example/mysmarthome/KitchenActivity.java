package com.example.mysmarthome;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.ToggleButton;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class KitchenActivity extends AppCompatActivity {
    private ToggleButton toggleButton_Window;
    private ToggleButton toggleButton_Fan;
    private ToggleButton toggleButton_Light;

    private DatabaseReference firebase_light_kitchen = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference firebase_window_kitchen = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference firebase_fan_kitchen = FirebaseDatabase.getInstance().getReference();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kitchen);

        //Get data from Firebase to update switch status
        firebase_light_kitchen.child("stream/Kitchen/light").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.light_on_kitchen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_kitchen);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.light_on_kitchen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_kitchen);
                    switch_off.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        firebase_window_kitchen.child("stream/Kitchen/window").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.switch_on_window_kitchen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_window_kitchen);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.switch_on_window_kitchen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_window_kitchen);
                    switch_off.setVisibility(View.VISIBLE);

                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        firebase_fan_kitchen.child("stream/Kitchen/fan").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.switch_on_fan_kitchen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_fan_kitchen);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.switch_on_fan_kitchen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_fan_kitchen);
                    switch_off.setVisibility(View.VISIBLE);

                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        //Configure for window button
        toggleButton_Window = (ToggleButton) findViewById(R.id.btn_toggleWindow_Kitchen);
        toggleButton_Window.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView switch_on = findViewById(R.id.switch_on_window_kitchen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_window_kitchen);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send status of Kitchen's window to Firebase
                    firebase_window_kitchen.child("stream/Kitchen/window").setValue(true);
                }else{
                    ImageView switch_on = findViewById(R.id.switch_on_window_kitchen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_window_kitchen);
                    switch_off.setVisibility(View.VISIBLE);

                    //send status of Kitchen's window to Firebase
                    firebase_window_kitchen.child("stream/Kitchen/window").setValue(false);
                }
            }
        });

        //Configure for fan button
        toggleButton_Fan = (ToggleButton) findViewById(R.id.btn_toggleFan_Kitchen);
        toggleButton_Fan.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView switch_on = findViewById(R.id.switch_on_fan_kitchen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_fan_kitchen);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send status of Kitchen's fan to Firebase
                    firebase_window_kitchen.child("stream/Kitchen/fan").setValue(true);
                }else{
                    ImageView switch_on = findViewById(R.id.switch_on_fan_kitchen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.switch_off_fan_kitchen);
                    switch_off.setVisibility(View.VISIBLE);

                    //send status of Kitchen's fan to Firebase
                    firebase_window_kitchen.child("stream/Kitchen/fan").setValue(false);
                }
            }
        });

        //Configure for light button
        toggleButton_Light = (ToggleButton) findViewById(R.id.btn_toggleLight_Kitchen);
        toggleButton_Light.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView switch_on = findViewById(R.id.light_on_kitchen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_kitchen);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send status of Kitchen's light to Firebase
                    firebase_window_kitchen.child("stream/Kitchen/light").setValue(true);
                }else{
                    ImageView switch_on = findViewById(R.id.light_on_kitchen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_kitchen);
                    switch_off.setVisibility(View.VISIBLE);

                    //send status of Kitchen's light to Firebase
                    firebase_window_kitchen.child("stream/Kitchen/light").setValue(false);
                }
            }
        });
    }

}