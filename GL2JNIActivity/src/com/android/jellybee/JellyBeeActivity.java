/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.jellybee;

import java.util.ArrayList;
import java.util.List;

import com.sensor.accelerometer.AccelerometerListener;
import com.sensor.accelerometer.AccelerometerManager;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo.State;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.DisplayMetrics;
import android.util.Log;

public class JellyBeeActivity extends Activity implements AccelerometerListener {

	public GL2JNIView mView;
	PowerManager powerManager;
	WakeLock wakeLock;

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		// if (checkConnection()) {
		// try {
		// Session currentSession = Session.getActiveSession();
		// if (currentSession == null
		// || currentSession.getState().isClosed()) {
		// Session session = new Session.Builder(this).build();
		// Session.setActiveSession(session);
		// currentSession = session;
		// }
		// if (!currentSession.isOpened()) {
		// // Ask for username and password
		// System.out.println("SESSION CLOSED");
		// OpenRequest op = new Session.OpenRequest(this);
		//
		// op.setLoginBehavior(SessionLoginBehavior.SUPPRESS_SSO);
		// op.setCallback(null);
		//
		// List<String> permissions = new ArrayList<String>();
		// permissions.add("publish_actions");
		// op.setPermissions(permissions);
		//
		// Session session = new Builder(this).build();
		// Session.setActiveSession(session);
		// session.openForPublish(op);
		//
		// }
		//
		// } catch (Exception e) {
		// e.printStackTrace();
		// }
		// }

		mView = new GL2JNIView(this, true, 8, 8);
		setContentView(mView);

		mView.setPreserveEGLContextOnPause(true);
		DisplayMetrics metrics = getResources().getDisplayMetrics();
		int densityDpi = (int) (metrics.density * 160f);
		GL2JNILib.setDensity(densityDpi);

		FacebookSharing.main_Activity = this;

		powerManager = (PowerManager) this
				.getSystemService(Context.POWER_SERVICE);
		wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK,
				"My Lock");
		wakeLock.acquire();
	}

	// /////////////
	/** CHECK NETWORK CONNECTION */
	public Boolean checkConnection() {
		ConnectivityManager conn = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		if (conn != null
				&& (conn.getNetworkInfo(1).getState() == State.CONNECTED)
				|| (conn.getNetworkInfo(0).getState() == State.CONNECTED)) {
			return true;
		} else {
			return false;
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		mView.onPause();
		wakeLock.release();

		// // Check Accelerometer is listening or not
		// if (AccelerometerManager.isListening()) {
		// // Stop Accelerometer Listening
		// AccelerometerManager.stopListening();
		// }
	}

	@Override
	protected void onResume() {
		super.onResume();

		SoundInGame.resumeAll();
		mView.onResume();
		wakeLock.acquire();

		// // Check device supported Accelerometer senssor or not
		// if (AccelerometerManager.isSupported(this)) {
		// // Start Accelerometer Listening
		// AccelerometerManager.startListening(this);
		// }
	}

	@Override
	public void onStop() {
		super.onStop();

		SoundInGame.pauseAll();
		wakeLock.release();

		// // Check Accelerometer is listening or not
		// if (AccelerometerManager.isListening()) {
		// // Stop Accelerometer Listening
		// AccelerometerManager.stopListening();
		// }
	}

	@Override
	protected void onDestroy() {

		super.onDestroy();
		// wakeLock.release();

		SoundInGame.stop();
		SoundInGame.destroy();
		GL2JNILib.destroy();

		// // Check Accelerometer is listening or not
		// if (AccelerometerManager.isListening()) {
		// // Stop Accelerometer Listening
		// AccelerometerManager.stopListening();
		// }

		GL2JNILib.destroy();
	}

	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		GL2JNILib.onBackPressed(true);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		FacebookSharing.onResult(requestCode, resultCode, data);
	}

	public static float ax, ay, az;

	@Override
	public void onAccelerationChanged(float x, float y, float z) {
		// Log.i("Sensor", "Service  x = " + x + " ; y = " + y + " ; z = " + z);
		ax = x;
		ay = y;
		az = z;
	}

	//
	@Override
	public void onShake(float force) {
		// Log.i("SENSOR", " lac ");
	}
}
