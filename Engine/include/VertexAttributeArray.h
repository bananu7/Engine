#pragma once
class CVertexAttributeArray
{
private:
	unsigned int m_Id;

public:
	void Bind();
	void Draw(int startIx, int endIx);

	CVertexAttributeArray(void);
	~CVertexAttributeArray(void);
};

