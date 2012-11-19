#pragma once
#include "Typedefs.h"

class CVertexAttributeArray
{
private:
	unsigned int m_Id;

public:
	void Bind();
	void Draw(int startIx, int endIx);
	void EnableAttributeArray(uint32 index);

	static uint32 GetMaxVertexAttributes();

	CVertexAttributeArray(void);
	~CVertexAttributeArray(void);
};

