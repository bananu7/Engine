#pragma once

#ifdef ENGINE_USE_FBX

#include "VertexAttributeArray.h"
#include "VertexBuffer.h"
#include <fbxsdk.h>

class CFbxVboMesh
{
public:
    CFbxVboMesh();
    ~CFbxVboMesh();

    // Save up data into GPU buffers.
    bool Initialize(const FbxMesh * pMesh);

    // Update vertex positions for deformed meshes.
    void UpdateVertexPosition(const FbxMesh * pMesh, const FbxVector4 * pVertices);

    // Bind buffers, set vertex arrays, turn on lighting and texture.
    void BeginDraw();
    // Draw all the faces with specific material with given shading mode.
    void Draw(int pMaterialIndex) const;
    // Unbind buffers, reset vertex arrays, turn off lighting and texture.
    void EndDraw() const;

    // Get the count of material groups
    int GetSubMeshCount() const { return mSubMeshes.GetCount(); }

private:
	CVertexBuffer m_VertsVbo, m_NormalsVbo, m_TexCoordsVbo, m_IndexVbo;
	CVertexAttributeArray m_Vao;

    // For every material, record the offsets in every VBO and triangle counts
    struct SubMesh
    {
        SubMesh() : IndexOffset(0), TriangleCount(0) {}

        int IndexOffset;
        int TriangleCount;
    };

    FbxArray<SubMesh*> mSubMeshes;
    bool mHasNormal;
    bool mHasUV;
    bool mAllByControlPoint; // Save data in VBO by control point or by polygon vertex.
};

} // namespace engine

#endif