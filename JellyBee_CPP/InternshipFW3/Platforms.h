#pragma once
#include <cstdio>
#ifndef WIN32
#define _ANDROID
#endif

#ifdef _ANDROID

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <unistd.h>

#include <android/log.h>
#define  LOG_TAG    "GAME"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define SKIP_ONE_LINE(f) if(f) { char s[256]; fgets(s, 256, f); }
#define SLEEP(milliSecond) sleep(milliSecond / 1000);

#else

#ifdef _WIN32
#include <GLES2\gl2.h>
#include <windows.h>
#include <EGL\egl.h>
#define LOGI(...) printf(__VA_ARGS__)
#define LOGE(...) printf(__VA_ARGS__)

#define SKIP_ONE_LINE(f) if(f) { fscanf(f, "%*[^\n]\n", NULL); }
#define SLEEP(milliSecond) Sleep(milliSecond);

const int SCREEN_W = 800;
const int SCREEN_H = 480;

#endif
#endif

#define SAFE_DEL(p) if (p) delete p; p = 0;
#define SAFE_DEL_ARRAY(p) if (p) delete []p; p = 0;

const int MAX_10 = 10;
const float PI = 3.14159265359f;
enum Direction { LEFT, RIGHT, UP, DOWN, FORWARD, BACKWARD };
enum StateStatus { ON_PAUSE, ON_RESUME, ON_GOING, FINISHED, GAME_OVER };
// enum ItemType { HONEY, HONEY_FAKE, HEART, MOUNTAIN_DEW };
enum Key { A = 65, D = 68, W = 87, S = 83, ARROW_LEFT = 37, ARROW_UP = 38, ARROW_RIGHT = 39, ARROW_DOWN = 40 };
enum MapType { BLOCK, PATH, TREE, END, HONEY, HONEY_FAKE, HEART, MOUNTAIN_DEW, FROG, SPIDER_WEB, TELE1 = 100, TELE2 = 200};
// enum EnemyType { FROG, SPIDER_WEB};
const float MAPFACTOR = 0.4f;
const int MAX_TEX = 5;

const int SIZE_OF_LINE = 255;
const int SIZE_OF_STRING = 100;
const int SIZE_OF_ID = 50;

const int BITMAPFONT_W_H = 512;
const int MAX_256 = 256;
enum MODEL_TYPE {NFG, OBJ};
const float DISTANCE = 8.0f;
const int NUM_OF_BUTTON_IN_MAIN_MENU = 7;
const int MAX_NUMBER_CHARACTERS = 10;