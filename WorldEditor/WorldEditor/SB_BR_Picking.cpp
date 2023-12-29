/*
Copyright (c) 2023 World Editor -- HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "stdafx.h"
#include "AB_App.h"
#include "SB_BR_Picking.h"

SB_BR_Picking::SB_BR_Picking(Ogre::SceneManager* sceneMgr)
{
    mSceneMgr = sceneMgr;

    mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
    if (NULL == mRaySceneQuery)
    {
        App->Say("No Ray Query");
        return;
    }
    // mRaySceneQuery->setSortByDistance(true,0);

    Pl_Entity_Name = "-- NA --";
    strcpy(FaceMaterial, " -- NA --");
    strcpy(Selected_Brush_Name, " -- NA --");
    strcpy(TextureName2, " -- NA2 --");

    Total_Vertices_count_Actual = -1;
    Total_index_count_Actual = -1;
    closest_distance = -1;
    pentity = NULL;

    Selected_Brush = NULL;
    Selected_Face = NULL;

    closest_distance = 0;

    Selected_Brush_Face_Count = 0;
    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = -1;
    SubMesh_Index = 0;
    Selected_Ok = 0;
    Left_MouseDown = 0;
    SubMesh_Index_Fault = 0;
    Local_Face = 0;
    Got_Mesh_Flag = 0;

    Real_Sub_Brush_Count = 0;
}

SB_BR_Picking::~SB_BR_Picking()
{
    if (mRaySceneQuery != NULL)
    {
        delete mRaySceneQuery;
    }
}

// *************************************************************************
// *		  Clear_Picking_Data:- Terry and Hazel Flanigan 2023	   	   *
// *************************************************************************
void SB_BR_Picking::Clear_Picking_Data()
{
    Pl_Entity_Name = "-- NA --";
    strcpy(FaceMaterial, " -- NA --");
    strcpy(Selected_Brush_Name, " -- NA --");
    strcpy(TextureName2, " -- NA2 --");

    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = -1;
    SubMesh_Index = 0;
    Selected_Ok = 0;

    pentity = NULL;

    App->CLSB_Grid->HitVertices[0] = Ogre::Vector3(0, 0, 0);
    App->CLSB_Grid->HitVertices[1] = Ogre::Vector3(0, 0, 0);
    App->CLSB_Grid->HitVertices[2] = Ogre::Vector3(0, 0, 0);

    App->CLSB_Grid->HitFaceUVs[0] = Ogre::Vector2(0, 0);
    App->CLSB_Grid->HitFaceUVs[1] = Ogre::Vector2(0, 0);
    App->CLSB_Grid->HitFaceUVs[2] = Ogre::Vector2(0, 0);
}

// *************************************************************************
// *		Mouse_Pick_Entity::Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_BR_Picking::Mouse_Pick_Entity()
{
   // App->Beep_Win();

    Clear_Picking_Data();
    Total_index_count = 0;
    // --------------------------------------------------------------

    rw = App->CLSB_BR_Render->mWindow;
    camera = App->CLSB_BR_Render->mCamera;


    ImGuiIO& io = ImGui::GetIO();

    tx = io.MousePos.x / (Ogre::Real)rw->getWidth();
    ty = io.MousePos.y / (Ogre::Real)rw->getHeight();

    Pl_Entity_Name = "-----";

    Ogre::uint32 queryMask = -1;
    Ogre::Vector3 result = Ogre::Vector3(0, 0, 0);
    Ogre::MovableObject* target = NULL;
    closest_distance = 0;

    App->CLSB_BR_Render->RB_RenderListener->Selected_Object_Name[0] = 0;

    Ogre::SceneNode* mNode;

    Ogre::Ray ray2 = camera->getCameraToViewportRay(tx, ty);
 
    if (raycast(ray2, result, target, closest_distance, queryMask))
    {

        App->Beep_Win();

        mNode = pentity->getParentSceneNode();
        Pl_Entity_Name = pentity->getName();

        char buff[255];
        char* pdest;

        strcpy(buff, Pl_Entity_Name.c_str());
        pdest = strstr(buff, "GDEnt_");
        if (pdest != NULL)
        {
            App->Beep_Win();
            return;
        }
        else
        {
            if (Left_MouseDown == 1)
            {
                App->Beep_Win();
                //// Test Marker in mesh Triangle
                //Render_Selection();
                //App->CLSB_Ogre->RenderListener->Show_Marker_Face = 1;

                //// Get Brush
                //int Index = App->CLSB_Model->Group[SubMesh_Index]->Face_Data[Local_Face].Brush_Index;
                //Select_Brush(Index, 1);

                //// Store Brush faces and render
                //App->CLSB_Mesh_Mgr->Store_Faces_Data();
                //App->CLSB_Ogre->RenderListener->Show_Brush_Faces = 1;

                //// Select Face and render
                //Real_Face_Index = App->CLSB_Model->Group[SubMesh_Index]->Face_Data[Local_Face].Face_Index + 1;
                //Select_Face_In_Brush(Real_Face_Index);
                //App->CLSB_Ogre->RenderListener->Show_Selected_Face = 1;

            }
        }
    }

}

// *************************************************************************
// *		        raycast:- Terry and Hazel Flanigan 2022		       	   *
// *************************************************************************
bool SB_BR_Picking::raycast(const Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject*& target, float& closest_distance, const Ogre::uint32 queryMask)
{
    Total_index_count_Actual = 0;
    Total_Vertices_count_Actual = 0;

    target = NULL;
    bool ParticleFound = 0;
    Pl_Entity_Name = "---------";

    if (mRaySceneQuery != NULL)
    {
        mRaySceneQuery->setRay(ray);
        mRaySceneQuery->setSortByDistance(true, 2);
        // mRaySceneQuery->setQueryMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
        mRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
        // execute the query, returns a vector of hits
        if (mRaySceneQuery->execute().size() <= 0)
        {
            // raycast did not hit an objects bounding box
            Selected_Ok = 0;
            Pl_Entity_Name = "---------";
            App->Say("Failed 1");
            return (false);
        }
    }
    else
    {
        App->Say("No Ray Query");
        Selected_Ok = 0;
        return (false);
    }

    closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult& query_result = mRaySceneQuery->getLastResults();


    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
            //strcpy(TextureName, query_result[0].movable->getMovableType().c_str());
            break;
        }

        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0) &&
            ParticleFound == 0)
        {
            // get the entity to check
            //strcpy(TextureName, "Entity");

            pentity = static_cast<Ogre::MovableObject*>(query_result[qr_idx].movable);
            App->Say(pentity->getName().c_str());
            char buff[255];
            char* pdest;
            strcpy(buff, pentity->getName().c_str());
            pdest = strstr(buff, "Ogre/MO");
            if (pdest != NULL)
            {
                //App->Say(pentity->getName().c_str());

               // if (Got_Mesh_Flag == 0)
                {
                    GetMeshInformation(((Ogre::Entity*)pentity)->getMesh(),
                        pentity->getParentNode()->_getDerivedPosition(),
                        pentity->getParentNode()->_getDerivedOrientation(),
                        pentity->getParentNode()->_getDerivedScale());

                    //Got_Mesh_Flag = 1;
                }

                // test for hitting individual triangles on the mesh
                bool new_closest_found = false;
                for (size_t i = 0; i < Total_index_count; i += 3)
                {
                    // check for a hit against this triangle
                    std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
                        vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);

                    // if it was a hit check if its the closest
                    if (hit.first)
                    {
                        if ((closest_distance < 0.0f) || (hit.second < closest_distance))
                        {
                            closest_distance = hit.second;

                            Face_Index = i;

                            SubMesh_Index = Sub_Mesh_Indexs[Face_Index];

                            bool test = Get_Material_Data();
                            if (test == 1)
                            {
                                Sub_Mesh_Count = Get_SubMesh_Count();
                                Total_index_count_Actual = Get_Total_Indices();
                                Total_Vertices_count_Actual = Get_Total_Vertices();

                                Local_Face = Get_Local_Face(SubMesh_Index);
                            }

                        }
                    }
                }

                delete[] vertices;
                delete[] indices;
                delete[] TextCords;
                delete[] Sub_Mesh_Indexs;

                // if we found a new closest raycast for this object, update the
                // closest_result before moving on to the next object.
                if (new_closest_found)
                {
                    target = pentity;

                    //Sub_Mesh_Count = ((Ogre::Entity*)pentity)->getMesh()->getNumSubMeshes();

                    closest_result = ray.getPoint(closest_distance);
                }
            }
        }

    }

    if (closest_distance >= 0.0f)
    {
        // raycast success
        result = closest_result;
        Selected_Ok = 1;
        App->Say("Ok");

        return (true);
    }
    else
    {
        // raycast failed
        Selected_Ok = 0;
        Pl_Entity_Name = "---------";
       App->Say("Failed 2");

        return (false);
    }
}

// *************************************************************************B
// *************************************************************************
void SB_BR_Picking::Render_Selection()
{
    if (SubMesh_Index_Fault == 0)
    {
        int A = App->CLSB_Model->Group[SubMesh_Index]->Face_Data[Local_Face].a;
        int B = App->CLSB_Model->Group[SubMesh_Index]->Face_Data[Local_Face].b;
        int C = App->CLSB_Model->Group[SubMesh_Index]->Face_Data[Local_Face].c;

        App->CLSB_Grid->HitVertices[0].x = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[A].x;
        App->CLSB_Grid->HitVertices[0].y = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[A].y;
        App->CLSB_Grid->HitVertices[0].z = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[A].z;

        App->CLSB_Grid->HitVertices[1].x = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[B].x;
        App->CLSB_Grid->HitVertices[1].y = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[B].y;
        App->CLSB_Grid->HitVertices[1].z = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[B].z;

        App->CLSB_Grid->HitVertices[2].x = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[C].x;
        App->CLSB_Grid->HitVertices[2].y = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[C].y;
        App->CLSB_Grid->HitVertices[2].z = App->CLSB_Model->Group[SubMesh_Index]->vertex_Data[C].z;


        /*App->CLSB_Grid->HitFaceUVs[0] = TextCords[Face_Index];
        App->CLSB_Grid->HitFaceUVs[1] = TextCords[Face_Index + 1];
        App->CLSB_Grid->HitFaceUVs[2] = TextCords[Face_Index + 2];*/

        // App->CLSB_Grid->Face_Update2();
        // App->CLSB_Grid->FaceNode->setVisible(true);
    }
}

// *************************************************************************
// *		  Get_Material_Data:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
bool SB_BR_Picking::Get_Material_Data()
{
    SubMesh_Index_Fault = 0;
    int test = ((Ogre::Entity*)pentity)->getMesh()->getNumSubMeshes();

    if (SubMesh_Index > test)
    {
        char buff[200];
        itoa(SubMesh_Index, buff, 10);
        App->Say("Sub Mesh Out of bounds", buff);
        SubMesh_Index_Fault = 1;
        return 0;
    }
    else
    {
        strcpy(FaceMaterial, ((Ogre::Entity*)pentity)->getMesh()->getSubMesh(SubMesh_Index)->getMaterialName().c_str());
        Ogre::MaterialPtr  MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(FaceMaterial));
        strcpy(TextureName2, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());
        SubMesh_Index_Fault = 0;
        return 1;
    }

    return 0;
}

// *************************************************************************
// *					      GetMeshInformation		              	   *
// *************************************************************************
void SB_BR_Picking::GetMeshInformation(const Ogre::MeshPtr mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    Total_vertex_count = Total_index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        // We only need to add the shared vertices once
        if (submesh->useSharedVertices)
        {
            if (!added_shared)
            {
                Total_vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            Total_vertex_count += submesh->vertexData->vertexCount;

        }

        // Add the indices
        Total_index_count += submesh->indexData->indexCount;

    }

    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[Total_vertex_count];
    indices = new Ogre::uint32[Total_index_count];
    TextCords = new Ogre::Vector2[Total_vertex_count];
    Sub_Mesh_Indexs = new Ogre::uint32[Total_vertex_count];

    memset(Sub_Mesh_Indexs, 0, Total_vertex_count);

    // App->Say_Int(Total_index_count);
    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if (submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));


            float* pReal;

            for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }

        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        Ogre::uint32* pLong = static_cast<Ogre::uint32*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

        if (use32bitindexes)
        {
            for (size_t k = 0; k < numTris * 3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<Ogre::uint32>(offset);
            }
        }
        else
        {
            for (size_t k = 0; k < numTris * 3; ++k)
            {
                indices[index_offset++] = static_cast<Ogre::uint32>(pShort[k]) +
                    static_cast<Ogre::uint32>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }

    // Texture Cords UVS
    int textoffsset = 0;

    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {

        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if (submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* texElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);

            Ogre::HardwareVertexBufferSharedPtr vbufText =
                vertex_data->vertexBufferBinding->getBuffer(texElem->getSource());

            byte* vertexText = (byte*)vbufText->lock(Ogre::HardwareBuffer::HBL_NORMAL);
            float* pRealText;

            for (ulong j = 0; j < vertex_data->vertexCount; ++j, vertexText += vbufText->getVertexSize())
            {
                texElem->baseVertexPointerToElement(vertexText, &pRealText);

                TextCords[textoffsset].x = pRealText[0];
                TextCords[textoffsset].y = pRealText[1];

                Sub_Mesh_Indexs[textoffsset] = i;

                textoffsset++;
            }

            vbufText->unlock();
        }
    }
}

// *************************************************************************
// *		Get_SubMesh_Count:- Terry and Hazel Flanigan 2023          	   *
// *************************************************************************
int SB_BR_Picking::Get_SubMesh_Count()
{
    return ((Ogre::Entity*)pentity)->getNumSubEntities();;
}

// *************************************************************************
// *		Get_Total_Indices:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int SB_BR_Picking::Get_Total_Indices()
{
    Ogre::MeshPtr mesh = ((Ogre::Entity*)pentity)->getMesh();

    int TotalIndices = 0;
    int Count = 0;
    int SubMeshes = mesh->getNumSubMeshes();

    while (Count < SubMeshes)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(Count);
        TotalIndices += submesh->indexData->indexCount;

        Count++;
    }

    return TotalIndices;
}

// *************************************************************************
// *		Get_Total_Vertices:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int SB_BR_Picking::Get_Total_Vertices()
{
    Ogre::MeshPtr mesh = ((Ogre::Entity*)pentity)->getMesh();

    bool added_shared = false;
    int TotalVertices = 0;
    int Count = 0;
    int SubMeshes = mesh->getNumSubMeshes();

    while (Count < SubMeshes)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(Count);

        if (submesh->useSharedVertices)
        {
            if (!added_shared)
            {
                TotalVertices += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            TotalVertices += submesh->vertexData->vertexCount;
        }

        Count++;
    }

    return TotalVertices;
}

// *************************************************************************
// *		    Get_Local_Face:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int SB_BR_Picking::Get_Local_Face(int SelectedGroup)
{
    Ogre::MeshPtr mesh = ((Ogre::Entity*)pentity)->getMesh();

    bool added_shared = false;
    int TotalFaces = 0;
    int Count = 0;
    int SubMeshes = mesh->getNumSubMeshes();

    while (Count < SelectedGroup)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(Count);
        TotalFaces += submesh->indexData->indexCount;

        Count++;
    }

    int Result = (Face_Index / 3) - (TotalFaces / 3);

    return Result;
}

// *************************************************************************
// *	        	Select_Brush:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_BR_Picking::Select_Brush(int Index, bool Clear)
{
    strcpy(Selected_Brush_Name, "-------");

    App->Get_Current_Document();

    Selected_Brush = NULL;

    int			c;
    geBoolean	bChanged = FALSE;

    c = App->CL_World->Get_Brush_Count();

    if (c > 0)
    {
        if (Clear == 1)
        {
            App->CLSB_Doc->ResetAllSelections();
            App->CLSB_Doc->UpdateSelected();
        }

        Selected_Brush = App->CL_World->Get_Brush_ByIndex(Index);

        // int FaceCount = Brush_GetNumFaces(Selected_Brush);

        strcpy(Selected_Brush_Name, App->CL_Brush->Brush_GetName(Selected_Brush));

        SelBrushList_Add(App->CLSB_Doc->pSelBrushes, Selected_Brush);

        Get_Brush_Data(Selected_Brush);
        App->CLSB_Doc->UpdateSelected();

        App->CL_TabsControl->Select_Brushes_Tab(0);
        App->CL_TabsGroups_Dlg->Get_Index(Selected_Brush);

        App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
        App->CLSB_TopTabs->Update_Dlg_Controls();


        if (Clear == 1)
        {
            /*App->CL_TabsControl->Select_Brushes_Tab(0);
            App->CL_TabsGroups_Dlg->Get_Index(Selected_Brush);

            App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
            App->CLSB_TopTabs->Update_Dlg_Controls();*/
        }

        //m_pDoc->DoBrushSelection( Selected_Brush, brushSelToggle) ;
        bChanged = GE_TRUE;
    }


    if (bChanged)
    {
        App->CLSB_Doc->UpdateSelected();
        App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);

    }
}

struct tag_FaceList
{
    int NumFaces;
    int Limit;
    Face** Faces;
    geBoolean Dirty;
    Box3d Bounds;
};

// *************************************************************************
// *         Get_Brush_Data:- Terry and Hazel Flanigan 2023                *
// *************************************************************************
void SB_BR_Picking::Get_Brush_Data(Brush* pBrush)
{
    Real_Brush_Type[0] = 0;
    Real_Sub_Brush_Count = 0;
    Real_Face_Count = 0;

    if (pBrush->Type == BRUSH_MULTI)
    {

        strcpy(Real_Brush_Type, "BRUSH_MULTI");
        Real_Sub_Brush_Count = BrushList_Count(pBrush->BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));

        BrushIterator bi;
        pBrush = BrushList_GetFirst(pBrush->BList, &bi);
        while (pBrush != NULL)
        {
            Real_Face_Count = Real_Face_Count + Brush_GetNumFaces(pBrush);
            pBrush = BrushList_GetNext(&bi);
        }

        return;
    }
    if (pBrush->Type == BRUSH_LEAF)
    {
        strcpy(Real_Brush_Type, "BRUSH_LEAF");
        Real_Sub_Brush_Count = 1;
        Real_Face_Count = pBrush->Faces->NumFaces;
        return;
    }
    if (pBrush->Type == BRUSH_CSG)
    {
        strcpy(Real_Brush_Type, "BRUSH_CSG");
        return;
    }

    Real_Face_Count = 0;
    strcpy(Real_Brush_Type, "Unknown");
}

// *************************************************************************
// *         Select_Face_In_Brush:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
void SB_BR_Picking::Select_Face_In_Brush(int Face_Index)
{
    Selected_Face = NULL;

    App->CLSB_Doc->ResetAllSelectedFaces();

    Selected_Brush_Face_Count = 0;

    App->CLSB_Doc->SelectAllFacesInBrushes();

    Selected_Brush_Face_Count = SelFaceList_GetSize(App->CLSB_Doc->pSelFaces);

    if (Face_Index > Selected_Brush_Face_Count)
    {
        App->Say("Face Index Out of Bounds");
        return;
    }


    Selected_Face = SelFaceList_GetFace(App->CLSB_Doc->pSelFaces, Face_Index - 1);

    App->CLSB_Doc->ResetAllSelectedFaces();

    Face_SetSelected(Selected_Face, GE_TRUE);

    SelFaceList_Add(App->CLSB_Doc->pSelFaces, Selected_Face);

    App->CLSB_Doc->UpdateSelected();
    App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);


    //App->Say_Int(nSelectedFaces);
}
