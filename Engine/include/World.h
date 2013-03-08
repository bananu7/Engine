#pragma once
#include <string>
#include <list>
#include <map>

namespace engine {

class GameObject;

class World
{
private:
	static const float UPDATE_INTERVAL;

protected:
	std::map<unsigned, GameObject*> m_Objects;

	void _LoadMap();

public:

	void Load();
//	std::vector<SUpdateData> Update ();
	
	void Draw();

	GameObject* GetObjectById (unsigned id);
	void AddObject (GameObject* obj);

	World();
	virtual ~World();
};

} // namespace engine