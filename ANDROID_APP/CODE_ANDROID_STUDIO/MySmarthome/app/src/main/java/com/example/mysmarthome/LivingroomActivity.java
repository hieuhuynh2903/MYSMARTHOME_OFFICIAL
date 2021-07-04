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

public class LivingroomActivity extends AppCompatActivity {

    private ToggleButton toggleButton_light_livingroom;
    private ToggleButton toggleButton_garagelight_livingroom;

    private DatabaseReference firebase_smartlight_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference firebase_human_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference firebase_garagelight_value = FirebaseDatabase.getInstance().getReference();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_livingroom);

        //Get data from Firebase to update switch status
        firebase_smartlight_value.child("stream/Homescreen/smartlight").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange( DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String comparedata = "true";
                if(firebase_getdata == comparedata){
                    ImageView switch_on = findViewById(R.id.light_on_livingroom);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_livingroom);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.light_on_livingroom);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_livingroom);
                    switch_off.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        firebase_human_value.child("sensor/PIR").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange( DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String comparedata = "true";
                if(firebase_getdata == comparedata){
                    ImageView switch_on = findViewById(R.id.human_yes_livingroom);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.human_no_livingroom);
                    switch_off.setVisibility(View.INVISIBLE);

                    firebase_human_value.child("stream/Livingroom/human").setValue(true);
                }else {
                    ImageView switch_on = findViewById(R.id.human_yes_livingroom);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.human_no_livingroom);
                    switch_off.setVisibility(View.VISIBLE);

                    firebase_human_value.child("stream/Livingroom/human").setValue(false);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        firebase_garagelight_value.child("stream/Livingroom/garagelight").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                if(firebase_getdata == "true"){
                    toggleButton_garagelight_livingroom.setChecked(true);
                }else{
                    toggleButton_garagelight_livingroom.setChecked(false);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        //Configure for smart light button
        toggleButton_light_livingroom = (ToggleButton) findViewById(R.id.btn_togglesmartlight_livingroom);
        toggleButton_light_livingroom.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked == true){
                    ImageView switch_on = findViewById(R.id.light_on_livingroom);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_livingroom);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send status of smart light to Firebase
                    firebase_smartlight_value.child("stream/Homescreen/smartlight").setValue(true);
                    firebase_smartlight_value.child("stream/Livingroom/smartlight").setValue(true);
                }else {
                    ImageView switch_on = findViewById(R.id.light_on_livingroom);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_livingroom);
                    switch_off.setVisibility(View.VISIBLE);

                    //send status of smart light to Firebase
                    firebase_smartlight_value.child("stream/Homescreen/smartlight").setValue(false);
                    firebase_smartlight_value.child("stream/Livingroom/smartlight").setValue(false);
                }
            }
        });

        toggleButton_garagelight_livingroom = (ToggleButton) findViewById(R.id.btn_togglegaragelight_livingroom);
        toggleButton_garagelight_livingroom.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked == true){
                    ImageView switch_on = findViewById(R.id.garagelight_on);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.garagelight_off);
                    switch_off.setVisibility(View.INVISIBLE);

                    firebase_garagelight_value.child("stream/Livingroom/garagelight").setValue(true);
                }else{
                    ImageView switch_on = findViewById(R.id.garagelight_on);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.garagelight_off);
                    switch_off.setVisibility(View.VISIBLE);

                    firebase_garagelight_value.child("stream/Livingroom/garagelight").setValue(false);
                }
            }
        });

    }

}