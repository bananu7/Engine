#include "ImageData.h"
#include "ResManager.h"

using std::string;

string CImageData::Load(ILoader & loader)
{		
	auto & Stream = loader.GetDataStream("main");
	if (Stream) {
		auto v = buffer_from_file(*Stream);
		m_Data = SOIL_load_image_from_memory
		(
			v.data(),
			v.size(),
			&m_Width, &m_Height, &m_Channels,
			SOIL_LOAD_AUTO
		);
	}
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
