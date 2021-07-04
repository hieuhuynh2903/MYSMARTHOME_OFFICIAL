package com.example.mysmarthome;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.IBinder;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class HomeSceen extends AppCompatActivity {
    private Button btn_bedroom;
    private Button btn_kitchen;
    private Button btn_bathroom;
    private Button btn_livingroom;

    private ToggleButton togglebtn_Smartlight;
    private ToggleButton togglebtn_Dryingpole;
    private ToggleButton togglebtn_Gate;
    private ToggleButton togglebtn_Bell;

    //Declare variable to send data to Firebase
    private DatabaseReference Smartlight_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Dryingpole_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Gate_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Temp_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Humi_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Gas_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Fire_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Bell_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Rain_value = FirebaseDatabase.getInstance().getReference();


    private TextView temperature_Homescreen;
    private TextView humidity_Homescreen;
    private TextView gas_Homescreen;
    private TextView fire_Homescreen;

    private MediaPlayer mMediaPlayer;

    public static final float GAS_THRESH = 10;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_sceen);

        StarAlarmService();

        //Get data from Firebase to update sensor value
        Temp_value.child("sensor/DHT11/temp").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                temperature_Homescreen = (TextView) findViewById(R.id.temp_Homescreen);
                temperature_Homescreen.setText(firebase_getdata);
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Humi_value.child("sensor/DHT11/humi").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                humidity_Homescreen = (TextView) findViewById(R.id.humi_Homescreen);
                humidity_Homescreen.setText(firebase_getdata);
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Gas_value.child("sensor/GAS").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                gas_Homescreen = (TextView) findViewById(R.id.gas_Homescreen);
                gas_Homescreen.setText(firebase_getdata);
                float firebase_getfloatdata = Float.parseFloat(firebase_getdata);
                if (firebase_getfloatdata >= GAS_THRESH){
                    togglebtn_Bell.setChecked(true);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Fire_value.child("sensor/FLAME").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    fire_Homescreen = (TextView) findViewById(R.id.fire_Homescreeen);
                    fire_Homescreen.setTextColor(Color.RED);
                    fire_Homescreen.setText("YES");
                    togglebtn_Bell.setChecked(true);
                }else {
                    fire_Homescreen = (TextView) findViewById(R.id.fire_Homescreeen);
                    fire_Homescreen.setTextColor(Color.GREEN);
                    fire_Homescreen.setText("NO");
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Bell_value.child("stream/Homescreen/bell").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.bell_on_Homescreen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.bell_off_Homescreen);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.bell_on_Homescreen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.bell_off_Homescreen);
                    switch_off.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Rain_value.child("sensor/RAIN").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.rainy_Homescreen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.sunny_Homescreen);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.rainy_Homescreen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.sunny_Homescreen);
                    switch_off.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });


        //Get data from Firebase to update switch status
        Smartlight_value.child("stream/Homescreen/smartlight").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_value = "false";
                if(firebase_getdata == compared_value){
                    ImageView switch_off = findViewById(R.id.light_off_homescreen);
                    switch_off.setVisibility(View.VISIBLE);
                    ImageView switch_on = findViewById(R.id.light_on_homesceen);
                    switch_on.setVisibility(View.INVISIBLE);
                    //Toast.makeText(HomeSceen.this,"Smartlight off",Toast.LENGTH_SHORT).show();
                }else{
                    ImageView switch_off = findViewById(R.id.light_off_homescreen);
                    switch_off.setVisibility(View.INVISIBLE);
                    ImageView switch_on = findViewById(R.id.light_on_homesceen);
                    switch_on.setVisibility(View.VISIBLE);
                    //Toast.makeText(HomeSceen.this,"Smartlight on",Toast.LENGTH_SHORT).show();
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Dryingpole_value.child("stream/Homescreen/dryingpole").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.dryingpole_on);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.dryingpole_off);
                    switch_off.setVisibility(View.INVISIBLE);
                }else{
                    ImageView switch_on = findViewById(R.id.dryingpole_on);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.dryingpole_off);
                    switch_off.setVisibility(View.VISIBLE);
                }

            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        Gate_value.child("stream/Homescreen/gate").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebase_getdata = snapshot.getValue().toString();
                String compared_data = "true";
                if (firebase_getdata == compared_data){
                    ImageView switch_on = findViewById(R.id.gate_on);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.gate_off);
                    switch_off.setVisibility(View.INVISIBLE);
                }else {
                    ImageView switch_on = findViewById(R.id.gate_on);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.gate_off);
                    switch_off.setVisibility(View.VISIBLE);
                }

            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });

        /******************Jump to another room**********/
        btn_bedroom = (Button) findViewById(R.id.btn_bedroom);
        btn_bedroom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent_bedroom = new Intent(HomeSceen.this, BedroomActivity.class);
                startActivity(intent_bedroom);
            }
        });

        btn_bathroom = (Button) findViewById(R.id.btn_bathroom);
        btn_bathroom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent_bathroom = new Intent(HomeSceen.this, BathroomActivity.class);
                startActivity(intent_bathroom);
            }
        });

        btn_kitchen = (Button) findViewById(R.id.btn_kitchen);
        btn_kitchen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent_kitchen = new Intent(HomeSceen.this, KitchenActivity.class);
                startActivity(intent_kitchen);
            }
        });

        btn_livingroom = (Button) findViewById(R.id.btn_livingroom);
        btn_livingroom.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent_livingroom = new Intent(HomeSceen.this, LivingroomActivity.class);
                startActivity(intent_livingroom);
            }
        });

        /*****************Set device status*****************/
        togglebtn_Smartlight = (ToggleButton) findViewById(R.id.toggleButton_Smartlight);
        togglebtn_Smartlight.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView switch_on = findViewById(R.id.light_on_homesceen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_homescreen);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send value to Firebase
                    Smartlight_value.child("stream/Homescreen/smartlight").setValue(true);
                    Smartlight_value.child("stream/Livingroom/smartlight").setValue(true);

                }else{
                    ImageView switch_on = findViewById(R.id.light_on_homesceen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_homescreen);
                    switch_off.setVisibility(View.VISIBLE);

                    //send value to Firebase
                    Smartlight_value.child("stream/Homescreen/smartlight").setValue(false);
                    Smartlight_value.child("stream/Livingroom/smartlight").setValue(false);
                }
            }
        });

        togglebtn_Dryingpole = (ToggleButton) findViewById(R.id.toggleButton_Dryingpole);
        togglebtn_Dryingpole.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView switch_on = findViewById(R.id.dryingpole_on);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.dryingpole_off);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send value to Firebase
                    Smartlight_value.child("stream/Homescreen/dryingpole").setValue(true);
                }else{
                    ImageView switch_on = findViewById(R.id.dryingpole_on);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.dryingpole_off);
                    switch_off.setVisibility(View.VISIBLE);

                    //send value to Firebase
                    Smartlight_value.child("stream/Homescreen/dryingpole").setValue(false);
                }
            }
        });

        togglebtn_Gate = (ToggleButton) findViewById(R.id.toggleButton_Gate);
        togglebtn_Gate.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView switch_on = findViewById(R.id.gate_on);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.gate_off);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send value to Firebase
                    Smartlight_value.child("stream/Homescreen/gate").setValue(true);
                }else{
                    ImageView switch_on = findViewById(R.id.gate_on);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.gate_off);
                    switch_off.setVisibility(View.VISIBLE);

                    //send value to Firebase
                    Smartlight_value.child("stream/Homescreen/gate").setValue(false);
                }
            }
        });

        togglebtn_Bell = (ToggleButton) findViewById(R.id.toggleButton_Bell);
        togglebtn_Bell.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked){
                    ImageView switch_on = findViewById(R.id.bell_on_Homescreen);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.bell_off_Homescreen);
                    switch_off.setVisibility(View.INVISIBLE);

                    //send value to Firebase
                    Bell_value.child("stream/Homescreen/bell").setValue(true);

                    StartMusic_proactive();

                }else {
                    ImageView switch_on = findViewById(R.id.bell_on_Homescreen);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.bell_off_Homescreen);
                    switch_off.setVisibility(View.VISIBLE);

                    //send value to Firebase
                    Bell_value.child("stream/Homescreen/bell").setValue(false);

                    StopAlarmService();
                    StopMusic_proactive();

                }
            }
        });
    }

    private void StopMusic_proactive() {
        if(mMediaPlayer != null){
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
    }

    private void StartMusic_proactive() {
        if(mMediaPlayer == null){
            mMediaPlayer = MediaPlayer.create(getApplicationContext(),R.raw.alarm);
        }
        mMediaPlayer.start();

    }

    private void StarAlarmService() {
        Intent intent = new Intent(HomeSceen.this,AlarmService.class);
        startService(intent);
    }

    private void StopAlarmService() {
        Intent intent = new Intent(HomeSceen.this,AlarmService.class);
        stopService(intent);
    }

}