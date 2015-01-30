package com.android.jellybee;

import java.io.File;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.opengles.GL10;

import com.facebook.Request;
import com.facebook.Request.Callback;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;
import com.facebook.Session.Builder;
import com.facebook.Session.OpenRequest;
import com.facebook.model.GraphUser;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.opengl.GLException;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class FacebookSharing {

	public static Activity main_Activity;
	private static FacebookSharing _instance = null;
	public static GL10 mGl = null;

	public static Session currentSession = null;
	public static Bitmap mScreeshot = null;
	public static boolean isLoaded = false;
	private static File outputFile;

	public static FacebookSharing Instance() {
		if (_instance == null)
			_instance = new FacebookSharing();
		return _instance;
	}

	// ==============================================================
	// ==============================================================
	public static native void FBSocialBridgeJ2C(FacebookSharing obj);

	public static void init() {
		FBSocialBridgeJ2C(Instance());
	}

	public static void onResult(int requestCode, int resultCode, Intent data) {
		if (Session.getActiveSession() != null)
			Session.getActiveSession().onActivityResult(main_Activity,
					requestCode, resultCode, data);

		Session currentSession = Session.getActiveSession();
		if (currentSession == null || currentSession.getState().isClosed()) {
			Session session = new Session.Builder(
					main_Activity.getApplicationContext()).build();
			Session.setActiveSession(session);
			currentSession = session;
		}

		if (currentSession.isOpened()) {
			Session.openActiveSession(main_Activity, true,
					new Session.StatusCallback() {

						@SuppressWarnings("deprecation")
						@Override
						public void call(final Session session,
								SessionState state, Exception exception) {

							if (session.isOpened()) {
								Log.i("GAME", "FB Login Successfully");
								isLoaded = true;
								publishFeedDialog();
								// ////////////////////////////////////////////////////////////////////////
								Request.executeMeRequestAsync(session,
										new Request.GraphUserCallback() {

											@Override
											public void onCompleted(
													GraphUser user,
													Response response) {
												if (user != null) {

													Log.i("GAME", "Hello "
															+ user.getName()
															+ "!");

													String access_token = session
															.getAccessToken();
													String firstName = user
															.getFirstName();
													String fb_user_id = user
															.getId();

													System.out
															.println("Facebook Access token: "
																	+ access_token);
													System.out
															.println("First Name:"
																	+ firstName);
													System.out
															.println("FB USER ID: "
																	+ fb_user_id);

												}
											}
										});

							}
						}
					});
		}
	}

	static int isDone = 0;

	public static int publishFeedDialog() {
		try {
			currentSession = Session.getActiveSession();
			if (currentSession == null || currentSession.getState().isClosed()) {
				Session session = new Session.Builder(main_Activity).build();
				Session.setActiveSession(session);
				currentSession = session;
			}
			// final Bundle params = new Bundle();
			// params.putString("name", "JELLY BEE");
			// params.putString("caption", "Let's play with us!");
			// params.putString("description",
			// "TEST SHARING SCREENSHOT INGAME");
			// params.putString("link",
			// "https://developers.facebook.com/android");
			// params.putString("picture",
			// "http://www.upanh.biz/images/2014/08/21/abc.jpg");

			if (!currentSession.isOpened()) {
				// Ask for username and password
				System.out.println("SESSION CLOSED");
				OpenRequest op = new Session.OpenRequest(main_Activity);

				op.setLoginBehavior(SessionLoginBehavior.SUPPRESS_SSO);
				op.setCallback(null);

				List<String> permissions = new ArrayList<String>();
				permissions.add("publish_actions");
				op.setPermissions(permissions);

				Session session = new Builder(main_Activity).build();
				Session.setActiveSession(session);
				session.openForPublish(op);
			}

			else if (currentSession.isOpened()) {
				main_Activity.runOnUiThread(new Runnable() {
					@Override
					public void run() {
						// TODO Auto-generated method stub
						// WebDialog feedDialog = (new
						// WebDialog.FeedDialogBuilder(
						// main_Activity, currentSession, params))
						// .setOnCompleteListener(null).build();
						// feedDialog.show();
						// makeScreenShot();

						if (mScreeshot == null) {
							Log.i("GAME", "null screen shot");
						}

						else {
							Request req = Request.newUploadPhotoRequest(
									currentSession, mScreeshot, new Callback() {
										public void onCompleted(
												Response response) {
											if (response != null) {
												Toast.makeText(main_Activity, "Share screen shot successfully!", Toast.LENGTH_SHORT).show();
												Log.i("GAME",
														"share screen shot successfully!");
												isDone = 1;
											} else {
												
												Toast.makeText(main_Activity, "Cannot share screen shot!", Toast.LENGTH_SHORT).show();
												Log.i("GAME",
														"can not share photo!");
												isDone = 0;
											}
										}
									});
							Bundle par = req.getParameters();
							par.putString("name", "JELLY BEE");
							par.putString("caption", "Let's play with us!");
							par.putString("description",
									"TEST SHARING SCREENSHOT INGAME");
							par.putString("link",
									"https://developers.facebook.com/android");
							
							Toast.makeText(main_Activity, "Sharing screen shot, please wait!", Toast.LENGTH_SHORT).show();
						
							req.executeAndWait();

						}
					}
				});

			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return isDone;
	}

	static {
		System.loadLibrary("gl2jni");
	}

	public static void makeScreenShot() {
		createBitmapFromGLSurface(0, 0, GL2JNIView.width, GL2JNIView.height,
				mGl);
	}

	public static void createBitmapFromGLSurface(int x, int y, int w, int h,
			GL10 gl) throws OutOfMemoryError {
		int bitmapBuffer[] = new int[w * h];
		int bitmapSource[] = new int[w * h];
		IntBuffer intBuffer = IntBuffer.wrap(bitmapBuffer);
		intBuffer.position(0);

		try {
			gl.glReadPixels(x, y, w, h, GL10.GL_RGBA, GL10.GL_UNSIGNED_BYTE,
					intBuffer);
			int offset1, offset2;
			for (int i = 0; i < h; i++) {
				offset1 = i * w;
				offset2 = (h - i - 1) * w;
				for (int j = 0; j < w; j++) {
					int texturePixel = bitmapBuffer[offset1 + j];
					int blue = (texturePixel >> 16) & 0xff;
					int red = (texturePixel << 16) & 0x00ff0000;
					int pixel = (texturePixel & 0xff00ff00) | red | blue;
					bitmapSource[offset2 + j] = pixel;
				}
			}
		} catch (GLException e) {
			return;
		}

		mScreeshot = Bitmap.createBitmap(bitmapSource, w, h,
				Bitmap.Config.ARGB_8888);
	}
}