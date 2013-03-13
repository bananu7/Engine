
#include "Misc.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>

namespace engine {

using namespace std;
#pragma warning(disable: 4244)

char *TextFileRead(const char *fn) 
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) 
	{
		fp = fopen(fn,"rt");

		if (fp != NULL) 
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}

	return content;
}
//---------------------------------------------
int Rnd (float f)
{
	return int(f+0.5); // Szybsze 
}
//---------------------------------------------
float DotPart (float f)
{
	 return f-int(f); // Mniej pamieci zajmuje
}
//---------------------------------------------
float Average (float a, float b)
{
   return (a+b) / 2;
}          

//---------------------------------------------
int Random (int range_min, int range_max)
{
	return (rand()%(range_max-range_min))+range_min;
}

float Random ()
{
	return rand() / (float)(RAND_MAX+1); // (0,1)
}
    
//================================================================
std::string ToLower (const string& in)
{
	string Temp;
	for (auto It = in.begin(); It != in.end(); It++)
	{
		const char TempC = *It;
		if (TempC >= 'A' && TempC <= 'Z')
			Temp += (char)(TempC) + 'a'-'A';
		else
			Temp += (char)(TempC);
	}
	return Temp;
}
//================================================================

} // namespace engine
