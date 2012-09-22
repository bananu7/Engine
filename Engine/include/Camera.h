#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Singleton.h"
#include "Matrix4.h"
#include <functional>

// It stores only matrices - general purpose
class CCamera
{
protected:
	CMatrix4 m_ProjectionMatrix, m_ViewMatrix;

public:
	// ModelView
	static CMatrix4 CreateScale (float x, float y, float z);
	static CMatrix4 CreateRotation (float x, float y, float z);
	static inline CMatrix4 CreateRotation (CVector3 const& rot) { return CreateRotation(rot.X, rot.Y, rot.Z); }
	static CMatrix4 CreateTranslation(float x, float y, float z);
	static inline CMatrix4 CreateTranslation (CVector3 const& vec) { return CreateTranslation(vec.X, vec.Y, vec.Z); }

	// Projection
	static CMatrix4 CreateProjection(float fov, float ratio, float nearClip, float farClip);
	static CMatrix4 CreateModelview(CVector3 const& eye, CVector3 const& target);
	static CMatrix4 CreateOrtho(float left, float top, float right, float bottom, float znear, float zfar);
	static CMatrix4 CreateFrustum(float left, float right, float bottom, float top, float znear, float zfar);

	void SetProjectionMat(CMatrix4 const& mat4) { m_ProjectionMatrix = mat4; }
	inline CMatrix4 const& GetProjectionMat () const { return m_ProjectionMatrix; }
	inline CMatrix4 const& GetViewMat () const { return m_ViewMatrix ;}

	virtual void CalculateView();
	virtual void CalculateProjection();

	CCamera(void);
	~CCamera(void);
};

