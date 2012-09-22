#pragma once

template <class T>
class CSingleton abstract
{
private:
	CSingleton(CSingleton& other);
	CSingleton& operator=(CSingleton& other);
protected:
	explicit CSingleton() {};
public:
	static T* GetSingleton()
	{
		static T Instance;
		return &Instance;
	}
};