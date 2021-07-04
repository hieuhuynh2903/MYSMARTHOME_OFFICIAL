package com.example.mysmarthome;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.ToggleButton;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class BathroomActivity extends AppCompatActivity {
    private ToggleButton toggleButton_Light;

    //Define variable to send data to Firebase
    private DatabaseReference firebase_light_bathroom_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference firebase_lighthealth_bathroom_value = FirebaseDatabase.getInstance().getReference();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bathroom);

        //Get data from Firebase to update switch status
        firebase_light_bathroom_value.child("stream/Bathroom/light").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String firebae_getdata_light = snapshot.getValue().toString();
                String compared_data = "true";
                if(firebae_getdata_light == compared_data){
                    ImageView switch_on = findViewById(R.id.light_on_bathroom);
                    switch_on.setVisibility(View.VISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_bathroom);
                    switch_off.setVisibility(View.INVISIBLE);

                    new Handler().postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            firebase_lighthealth_bathroom_value.child("sensor/HALL").addListenerForSingleValueEvent(new ValueEventListener() {
                                @Override
                                public void onDataChange(DataSnapshot snapshot) {
                                    String firebase_gethalldata = snapshot.getValue().toString();
                                    if (firebase_gethalldata == "false"){
                                        ImageView img_lighthealth_bad = findViewById(R.id.lighthealth_bad_bathroom);
                                        img_lighthealth_bad.setVisibility(View.VISIBLE);
                                        ImageView img_lighthealth_good = findViewById(R.id.lighthealth_good_bathroom);
                                        img_lighthealth_good.setVisibility(View.INVISIBLE);
                                    }else {
                                        ImageView img_lighthealth_bad = findViewById(R.id.lighthealth_bad_bathroom);
                                        img_lighthealth_bad.setVisibility(View.INVISIBLE);
                                        ImageView img_lighthealth_good = findViewById(R.id.lighthealth_good_bathroom);
                                        img_lighthealth_good.setVisibility(View.VISIBLE);
                                    }
                                }

                                @Override
                                public void onCancelled(DatabaseError error) {

                                }
                            });
                        }
                    },5000);



                }else{
                    ImageView switch_on = findViewById(R.id.light_on_bathroom);
                    switch_on.setVisibility(View.INVISIBLE);
                    ImageView switch_off = findViewById(R.id.light_off_bathroom);
                    switch_off.setVisibility(View.VISIBLE);


                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });


        //Configure for Bathroom light button
        toggleButton_Light = (ToggleButton) findViewById(R.id.btn_toggleLight_bathroom);
        toggleButton_Light.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    ImageView light_on = findViewById(R.id.light_on_bathroom);
                    light_on.setVisibility(View.VISIBLE);
                    ImageView light_off = findViewById(R.id.light_off_bathroom);
                    light_off.setVisibility(View.INVISIBLE);

                    //send status of light to Firebase
                    firebase_light_bathroom_value.child("stream/Bathroom/light").setValue(true);
                }else{
                    ImageView light_on = findViewById(R.id.light_on_bathroom);
                    light_on.setVisibility(View.INVISIBLE);
                    ImageView light_off = findViewById(R.id.light_off_bathroom);
                    light_off.setVisibility(View.VISIBLE);

                    //send status of light to Firebase
                    firebase_light_bathroom_value.child("stream/Bathroom/light").setValue(false);



                }

            }
        });


    }
}