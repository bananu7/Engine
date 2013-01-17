
#include "ImageData.h"
#include "ResManager.h"
#include <string>
#include <SOIL.h>

using std::string;

string CImageData::Load(ILoader const& loadParams)
{		
	string Path;
	if (!loadParams.GetParam("path", Path))
		return string("Missing loading param 'path'");
	Path = CResManager::GetSingleton()->GetResourcePath() + Path;

	m_Data = SOIL_load_image
	(
		Path.c_str(),
		&m_Width, &m_Height, &m_Channels,
		SOIL_LOAD_AUTO
	);
	return (m_Data != 0) ? string() : string("Error with loading image");
}

void CImageData::Unload ()
{
	delete m_Data;
	m_Data = NULL;
}

CImageData::CImageData(void) :
	m_Data(NULL),
	m_Height(0),
	m_Width(0),
	m_Channels(0)
{
}

CImageData::~CImageData(void)
{
	Unload();
}
