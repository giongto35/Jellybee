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

package com.android.gl2jni;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;


public class GL2JNIActivity extends Activity {

	GL2JNIView mView;
	PowerManager powerManager;
	WakeLock wakeLock;

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		mView = new GL2JNIView(getApplication(), true, 8, 8);
		setContentView(mView);
		
		powerManager = (PowerManager)this.getSystemService(Context.POWER_SERVICE);
		wakeLock = powerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK, "My Lock");
		wakeLock.acquire();
	}

	@Override
	protected void onPause() {
		super.onPause();
		mView.onPause();
		wakeLock.release();
		// Check Accelerometer is listening or not
//		if (AccelerometerManager.isListening()) {
//			// Stop Accelerometer Listening
//			AccelerometerManager.stopListening();
//		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		mView.onResume();
		wakeLock.acquire();

		// Check device supported Accelerometer senssor or not
//		if (AccelerometerManager.isSupported(this)) {
//
//			// Start Accelerometer Listening
//			AccelerometerManager.startListening(this);
//		}
	}

	@Override
	public void onStop() {
		super.onStop();
		SoundInGame.destroy();
		GL2JNILib.destroy();
		
		wakeLock.release();
		
		// Check Accelerometer is listening or not
//		if (AccelerometerManager.isListening()) {
//			// Stop Accelerometer Listening
//			AccelerometerManager.stopListening();
//		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		//wakeLock.release();
		
		// Check Accelerometer is listening or not
//		if (AccelerometerManager.isListening()) {
//			// Stop Accelerometer Listening
//			AccelerometerManager.stopListening();
//		}
		
		GL2JNILib.destroy();
	}
	
	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		// kiem tra dang o state nao
		//...
		//GL2JNILib.onKey(27, true);
		
	}

//	@Override
//	public void onAccelerationChanged(float x, float y, float z) {
//		// Log.i("Sensor", "Service  x = " + x + " ; y = " + y + " ; z = " + z);
//		
//	}
//
//	@Override
//	public void onShake(float force) {
//		Log.i("SENSOR", " lac ");
//	}
}
