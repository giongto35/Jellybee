package com.android.gl2jni;

import java.io.IOException;
import java.util.HashMap;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;
import android.net.Uri;
import android.util.Log;

public class SoundInGame {
	private static SoundPool soundPool;
	private static MediaPlayer musicPlayer = null;
	public static Context sound_context;
	private static int mHitStreamID, mBackGroundStreamID;
	private static HashMap<String, Integer> soundPoolMap;
	private static boolean isLoaded = false;

	private static SoundInGame _instance = null;

	public static SoundInGame Instance() {
		if (_instance == null)
			_instance = new SoundInGame();
		return _instance;
	}

	// ==============================================================
	// ==============================================================
	public static native void BridgeJ2C(SoundInGame obj);

	public static void init() {
		BridgeJ2C(Instance());
		soundPool = new SoundPool(2, AudioManager.STREAM_MUSIC, 0);
		soundPoolMap = new HashMap<String, Integer>();
	}

	public static void initSound(String id, String filePath) {
		soundPoolMap.put(id, soundPool.load(filePath, 1));
		Log.i("GAME", "load sound " + id + " - " + filePath);
	}

	public static void destroy() {
		Integer[] sounds = (Integer[]) soundPoolMap.values().toArray();
		for (int i = 0; i < soundPoolMap.size(); ++i) {
			soundPool.unload(soundPoolMap.get(sounds[i]));
		}

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
		mHitStreamID = soundPool.play(soundPoolMap.get(id), 1f, 1f, 0, 0, 1f); 
																																		// 0
	}

	public static void pauseAll() {
		soundPool.autoPause();
		if (musicPlayer != null)
    	{
    		try
    		{
    		if (musicPlayer.isPlaying())
    			musicPlayer.pause();
    		}
    		catch (Exception e) {
			}
    	}
	}

	public static void resumeAll() {
		soundPool.autoResume();
		if (musicPlayer != null)
    	{
    		try
    		{
    		if (musicPlayer.isPlaying())
    			musicPlayer.start();
    		}
    		catch (Exception e) {
			}
    	}
	}

	public static void stop() {
		// soundPool.stop(mBackGroundStreamID);
		soundPool.stop(mHitStreamID);
		if (musicPlayer != null)
    	{
    		try
    		{
    		if (musicPlayer.isPlaying())
    			musicPlayer.stop();
    		musicPlayer.release();
    		}
    		catch (Exception e) {
			}
    	}
	}

	static {
		System.loadLibrary("gl2jni");
	}
}
