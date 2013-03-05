#pragma once
#include "Camera.h"
#include <glm/glm.hpp>

namespace engine {

// Kamera do ustawiania celu i pozycji
class CCameraSimple:
	public CCamera
{
public:
	glm::vec3 Target, Position;
	
	void CalculateView();
};

// Kamera do latania jak przy komendzie 'fly' w FPS-ach
class CCameraFly :
	public CCamera
{
public:
	// Kierunek, w jaki patrzymy
	// X - obrot wokol osi Y - w radianach
	// Y - obrot wokol osi X
	glm::vec2 LookDir;
	glm::vec3 Position;

	void CalculateView();
	void Fly (float len);
	void Strafe (float left);
};

class CCameraTrack :
	public CCamera
{
public:
	glm::vec3 Target;
	float Direction, Radius;

	void CalculateView();
};

// Kamera do obracania sie wokol obiektu - dobra to testow i podgladow
class CCameraSpherical :
	public CCamera
{
public:
};

// Kamera, ktora automatycznie ustala wysokosc, na jakiej sie znajdujemy
// na podstawie terenu
class CCameraWalk
{
	// sampler do ziemi
public:
};

} // namespace engine