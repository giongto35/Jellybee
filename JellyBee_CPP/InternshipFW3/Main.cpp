#include "Platforms.h"
#include "Timer.h"
#include "ScreenResolution.h"
#include "Application.h"
#include <math.h>
#include "NameSpace.h"


#ifdef _WIN32
#include "ESContext.h"
#include <vld.h>
#else
#ifdef _ANDROID
#include <jni.h>		
#endif
#endif

// APPLICATION
Application *application = NULL;

InputManager *im = NULL;

void Init()
{
	application = Application::GetInstance();
	application->Init();

	im = InputManager::GetInstance();
}

void Update()
{
	//LOGI("\nFrame time: %f (s)", Timer::FrameTime());
	application->Update();

}

void Draw()
{
	application->Draw();
}

void Destroy()
{
	Application::DestroyInstance();
}

#ifdef _WIN32
void OnKey(int keyCode, bool isKeyDown)
{
	im->OnKey(keyCode, isKeyDown);			
}


void OnPointer(int type, int x, int y)
{
	if (type == 2) // dragging
	{
		im->OnDrag(x, y);
	}
	else 
	{
		im->OnPointer(type, x, y);
	}
}

int main()
{	
	ScreenResolution::GetInstance()->Init(SCREEN_W, SCREEN_H);
	InitWindow(SCREEN_W, SCREEN_H);
	
	RegisterInitFunc(Init);
	RegisterDrawFunc(Draw);
	RegisterUpdateFunc(Update);	
	RegisterDestroyFunc(Destroy);
	RegisterKeyFunc(OnKey);
	RegisterMouseFunc(OnPointer);

	WinLoop();
	return 0;
}
#else

#ifdef _ANDROID
static void printGLString(const char *name, GLenum s) 
{
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

bool setupGraphics(int w, int h) 
{
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
   
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

extern "C" 
{
    JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_destroy(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onKey(JNIEnv * env, jobject obj, jint keyCode, jboolean isKeyDown);
	JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onPointer(JNIEnv * env, jobject obj, jint type, jint x, jint y);
	JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onDrag(JNIEnv * env, jobject obj, jint x, jint y);
	JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onBackPressed(JNIEnv * env, jobject obj, jboolean isPressed);
	JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_setDensity(JNIEnv * env, jobject obj, jint density);
};


JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	ScreenResolution::GetInstance()->Init(width, height);
	LOGI("%d - %d", ScreenResolution::GetInstance()->m_screenWidth, ScreenResolution::GetInstance()->m_screenHeight);
    setupGraphics(width, height);
	Init();
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_step(JNIEnv * env, jobject obj)
{
	Timer::Tick();
	Update();
	Draw();
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_destroy(JNIEnv * env, jobject obj)
{
	Destroy();
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onKey(JNIEnv * env, jobject obj, jint keyCode, jboolean isKeyDown)
{
	im->OnKey(keyCode, isKeyDown);
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onPointer(JNIEnv * env, jobject obj, jint type, jint x, jint y)
{
	im->OnPointer(type, x, y);
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onDrag(JNIEnv * env, jobject obj, jint x, jint y)
{
	im->OnDrag(x, y);
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_onBackPressed(JNIEnv * env, jobject obj, jboolean isPressed)
{
	Setting::isBackButtonPressed = isPressed;
}

JNIEXPORT void JNICALL Java_com_android_jellybee_GL2JNILib_setDensity(JNIEnv * env, jobject obj, jint density)
{
	ScreenResolution::GetInstance()->SetDensityDpi(density);
	LOGI("Density dpi = %d\n", density);
}

#endif

#endif