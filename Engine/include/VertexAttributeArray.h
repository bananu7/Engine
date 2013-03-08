#pragma once

namespace engine {

class VertexAttributeArray
{
private:
	unsigned int m_Id;

public:
	void Bind();
	void Draw(int startIx, int endIx);
	void EnableAttributeArray(unsigned index);

	static unsigned GetMaxVertexAttributes();

	VertexAttributeArray(void);
	~VertexAttributeArray(void);
};

} // namespace engine