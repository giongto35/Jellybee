package com.android.jellybee;

import java.io.IOException;
import java.util.HashMap;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.SoundPool;
import android.media.SoundPool.OnLoadCompleteListener;
import android.net.Uri;
import android.os.Vibrator;
import android.util.Log;

public class SoundInGame {
	private static SoundPool soundPool;
	private static MediaPlayer musicPlayer = null;
	private static MediaPlayer soundPlayer = null;
	public static Context sound_context;
	private static int mHitStreamID, mBackGroundStreamID;
	private static HashMap<String, Integer> soundPoolMap;
	private static boolean isLoaded = false;
	private static int mStatus = 1;

	private static SoundInGame _instance = null;

	public static SoundInGame Instance() {
		if (_instance == null)
			_instance = new SoundInGame();
		return _instance;
	}

	// ==============================================================
	// ==============================================================
	public static native void SoundBridgeJ2C(SoundInGame obj);

	public static void init() {
		SoundBridgeJ2C(Instance());
		soundPool = new SoundPool(2, AudioManager.STREAM_MUSIC, 100);
		soundPoolMap = new HashMap<String, Integer>();

	}

	public static void initSound(String id, String filePath) {

		if (id.equals("block")) {
			if (soundPlayer != null) {
				try {
					if (soundPlayer.isPlaying())
						soundPlayer.stop();
					soundPlayer.release();
				} catch (Exception e) {
				}
			}

			soundPlayer = MediaPlayer
					.create(sound_context, Uri.parse(filePath));
			try {
				soundPlayer.prepare();
			} catch (IllegalStateException e) {
			} catch (IOException e) {
			}

			soundPlayer.setLooping(false);
			soundPlayer.setOnCompletionListener(new OnCompletionListener() {
				
				@Override
				public void onCompletion(MediaPlayer mp) {
					// TODO Auto-generated method stub
					mStatus = 1;
				}
			});
			
		} else {
			soundPoolMap.put(id, soundPool.load(filePath, 1));
			Log.i("GAME", "load sound " + id + " - " + filePath);
		}
	}

	public static void destroy() {
		Integer[] sounds = (Integer[]) soundPoolMap.values().toArray();
		for (int i = 0; i < soundPoolMap.size(); ++i) {
			soundPool.unload(soundPoolMap.get(sounds[i]));
		}
		soundPool.release();

		soundPoolMap.clear();
	}

	// ==============================================================
	// ==============================================================
	public static void playBackGround(String path) {
		// mBackGroundStreamID = soundPool.play(soundPoolMap.get(id), 1f, 1f, 1,
		// 5, 1f); // priority = 1
		if (musicPlayer != null) {
			try {
				if (musicPlayer.isPlaying())
					musicPlayer.stop();
				musicPlayer.release();
			} catch (Exception e) {
			}
		}

		musicPlayer = MediaPlayer.create(sound_context, Uri.parse(path));
		try {
			musicPlayer.prepare();
		} catch (IllegalStateException e) {
		} catch (IOException e) {
		}

		musicPlayer.setLooping(true);
		musicPlayer.start();

	}

	public static void playSound(String id) {
		if (id.equals("block"))
		{
			 Vibrator v = (Vibrator) sound_context.getSystemService(Context.VIBRATOR_SERVICE);
			 // Vibrate for 500 milliseconds
			 v.vibrate(50);
			 
			if (mStatus == 1)
			{
				mStatus = 0;
				if (soundPlayer.isPlaying())
					soundPlayer.stop();
				soundPlayer.start();
			}
		}
		else
		{
			mHitStreamID = soundPool.play(soundPoolMap.get(id), 1f, 1f, 1, 0, 1.0f);
		}
	}

	public static void pauseAll() {

		if (soundPool != null) {
			soundPool.autoPause();
		}

		if (musicPlayer != null) {
			try {
				if (musicPlayer.isPlaying())
					musicPlayer.pause();
			} catch (Exception e) {
			}
		}
		
		if (soundPlayer != null) {
			try {
				if (soundPlayer.isPlaying())
					soundPlayer.pause();
			} catch (Exception e) {
			}
		}
	}

	public static void resumeAll() {

		if (soundPool != null) {
			soundPool.autoResume();
		}

		if (musicPlayer != null) {
			try {
				musicPlayer.start();
			} catch (Exception e) {
			}
		}
		
		if (soundPlayer != null) {
			try {
				soundPlayer.start();
			} catch (Exception e) {
			}
		}
	}

	public static void stop() {
		// soundPool.stop(mBackGroundStreamID);
		soundPool.stop(mHitStreamID);
		
		if (musicPlayer != null) {
			try {
				if (musicPlayer.isPlaying())
					musicPlayer.stop();
				musicPlayer.release();
			} catch (Exception e) {
			}
		}
		
		if (soundPlayer != null) {
			try {
				if (soundPlayer.isPlaying())
					soundPlayer.stop();
				soundPlayer.release();
			} catch (Exception e) {
			}
		}
	}

	static {
		System.loadLibrary("gl2jni");
	}
}
