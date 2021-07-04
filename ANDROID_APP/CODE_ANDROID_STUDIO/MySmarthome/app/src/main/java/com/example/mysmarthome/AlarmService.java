package com.example.mysmarthome;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Binder;
import android.os.IBinder;

import androidx.annotation.NonNull;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import static com.example.mysmarthome.HomeSceen.GAS_THRESH;

public class  AlarmService extends Service {

    private DatabaseReference Gas_value = FirebaseDatabase.getInstance().getReference();
    private DatabaseReference Fire_value = FirebaseDatabase.getInstance().getReference();
    private MediaPlayer mMediaPlayer;


    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        //throw new UnsupportedOperationException("Not yet implemented");
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        sendNotification();
        Gas_value.child("sensor/GAS").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                String gas_string = snapshot.getValue().toString();
                float gas_float = Float.parseFloat(gas_string);
                if(gas_float >= GAS_THRESH){
                    StartMusic();
                    sendGasNotification();
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
                if (firebase_getdata == compared_data){
                    StartMusic();
                    sendFlameNotification();
                }
            }

            @Override
            public void onCancelled(DatabaseError error) {

            }
        });
        return START_STICKY;
    }

    private void sendNotification() {
        Intent intent = new Intent(this, HomeSceen.class);
        PendingIntent  pendingIntent = PendingIntent.getActivity(this,0,intent,PendingIntent.FLAG_UPDATE_CURRENT);

        Notification notification = new Notification.Builder(this)
                .setContentTitle("MY SMARTHOME")
                .setContentText("is running")
                .setSmallIcon(R.drawable.ic_homeheart)
                .setContentIntent(pendingIntent)
                .build();
        startForeground(1,notification);
    }
    private void sendGasNotification() {
        Intent intent = new Intent(this, HomeSceen.class);
        PendingIntent  pendingIntent = PendingIntent.getActivity(this,0,intent,PendingIntent.FLAG_UPDATE_CURRENT);
        Notification notification = new Notification.Builder(this)
                .setContentTitle("MY SMARTHOME")
                .setContentText("Gas concentration is over")
                .setSmallIcon(R.drawable.ic_gas)
                .setContentIntent(pendingIntent)
                .build();
        startForeground(1,notification);
    }
    private void sendFlameNotification() {
        Intent intent = new Intent(this, HomeSceen.class);
        PendingIntent  pendingIntent = PendingIntent.getActivity(this,0,intent,PendingIntent.FLAG_UPDATE_CURRENT);
        Notification notification = new Notification.Builder(this)
                .setContentTitle("MY SMARTHOME")
                .setContentText("Fire! Fire! Fire!")
                .setSmallIcon(R.drawable.ic_emoji_fire)
                .setContentIntent(pendingIntent)
                .build();
        startForeground(1,notification);
    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        if(mMediaPlayer != null){
            mMediaPlayer.release();
            mMediaPlayer = null;
        }
    }

    public void StartMusic(){
        if(mMediaPlayer == null){
            mMediaPlayer = MediaPlayer.create(getApplicationContext(),R.raw.alarm);
        }
        mMediaPlayer.start();
    }
}