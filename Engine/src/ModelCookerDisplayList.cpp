
#include "ModelCookerDisplayList.h"
#include "ModelData.h"
#include <GL/glew.h>

namespace engine {

void _ApplyMaterial (ModelData::Material const& material)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.Specular);
    //glColor4fv(TempMat.Diffuse);
    if (!material.Textures.empty())
    {
        int TextureUnit = 0;
        for (auto Tex = material.Textures.begin();
                Tex != material.Textures.end();
                ++Tex, ++TextureUnit)
        {
            glBindMultiTextureEXT(GL_TEXTURE0 + TextureUnit, GL_TEXTURE_2D, *Tex);
            glEnableIndexedEXT(GL_TEXTURE_2D, 0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }
}

bool ModelCookerDisplayList::Cook (const ModelData& modelData)
{
//    m_Components.clear();

    // Ka¿dy komponent to jedna displaylista

    auto const& Components = modelData.GetComponents();
    auto const& Materials = modelData.GetMaterials();
    auto const& Vertices = modelData.GetVertices();
    auto const& Normals = modelData.GetNormals();
    auto const& TexCoords = modelData.GetTexCoords();

    for (auto Cmp = Components.begin(); Cmp != Components.end(); ++Cmp)
    {
        // Ka¿dy komponent to 1 displaylista
        m_Components.push_back(CookedComponentDisplayList(Cmp->Center, glGenLists(1)));
        glNewList(m_Components.back().DisplayListNum, GL_COMPILE);
        
        for (auto Group = Cmp->Groups.begin(); Group != Cmp->Groups.end(); ++Group)
        {
            // Bindujemy materia³
            // W ramach jednej grupy u¿ywamy jednego materia³u

            _ApplyMaterial(Materials.at(Group->MaterialName));
            glBegin (GL_TRIANGLES);

            for (auto Face = Group->Faces.begin(); Face != Group->Faces.end(); ++Face)
            {
                for (int i = 0; i < 3; ++i)
                {
                    /*tex
                    norm
                    vert*/
                    auto const& TexCoord = TexCoords[Face->TexCIndices[i]];
                    glTexCoord2f(TexCoord.U, TexCoord.V);

                    ModelData::Vector3 const& Normal = Normals[Face->NormIndices[i]];
                    glNormal3f(Normal.X, Normal.Y, Normal.Z);

                    ModelData::Vector3 const& Vertex = Vertices[Face->VertIndices[i]];
                    glVertex3f(Vertex.X, Vertex.Y, Vertex.Z);    
                }
            }
            glEnd();
        }
        glEndList();
    }
    return true;
}

void ModelCookerDisplayList::Draw ()
{
    //glEnable(GL_TEXTURE_2D);
    //glColor3f (1.0f, 1.0f, 1.0f);
    //glCallList(m_ListNumber);
    //glMatrixMode(GL_MODELVIEW);
    //for (auto It = m_Components.begin(); It != m_Components.end(); ++It)
    //{
        // FIXME
        /*glPushMatrix();
        vec3 Temp = It->Center;
        glTranslatef(It->Center.X, It->Center.Y, It->Center.Z);
        It->Transform.ApplyToOpenGL();
        glTranslatef(-It->Center.X, -It->Center.Y, -It->Center.Z);
        glCallList(It->DisplayListNum);
        glPopMatrix();
        */
    //}
}

ModelCookerDisplayList::ModelCookerDisplayList(void)
{
}


ModelCookerDisplayList::~ModelCookerDisplayList(void)
{
}

} // namespace engine
