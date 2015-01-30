#pragma once

union ColorDataConverter
{
    int iValue;
    struct
	{
		unsigned char r, g, b, a;
	};
};