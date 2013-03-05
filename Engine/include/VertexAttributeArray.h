#pragma once

namespace engine {

class CVertexAttributeArray
{
private:
	unsigned int m_Id;

public:
	void Bind();
	void Draw(int startIx, int endIx);
	void EnableAttributeArray(unsigned index);

	static unsigned GetMaxVertexAttributes();

	CVertexAttributeArray(void);
	~CVertexAttributeArray(void);
};

} // namespace engine