#pragma once

struct SOption
{
	union
	{
		float		Float;
		int			Int;
	} Number;

	std::string String;

	SOption () { }
	SOption (float f) { Number.Float = f; }
	SOption (int i) { Number.Int = i; }
	SOption (std::string s) { String = s; }

	operator float() { return Number.Float; }
	operator int() { return Number.Int; }
	operator std::string() { return String; }
};
