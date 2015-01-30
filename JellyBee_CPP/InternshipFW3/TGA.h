#pragma once

char * LoadTGA( const char * szFileName, int * width, int * height, int * bpp );
char* GetPixels(const char* bufferTGA, const int & x, const int & y, const int & width, const int & height, const int & bpp);