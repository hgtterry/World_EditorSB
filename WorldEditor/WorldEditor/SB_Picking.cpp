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
#include "SB_Picking.h"

SB_Picking::SB_Picking(Ogre::SceneManager* sceneMgr)
{
    mSceneMgr = sceneMgr;

    mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
    if (NULL == mRaySceneQuery)
    {
        App->Say("No Ray Query");
        return;
    }
    mRaySceneQuery->setSortByDistance(true);

    pentity = NULL;

    closest_distance = 0;

    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = 0;
    SubMesh_Index = 0;
    Selected_Ok = 0;

    Local_Face = 0;
}

SB_Picking::~SB_Picking()
{
    if (mRaySceneQuery != NULL)
    {
        delete mRaySceneQuery;
    }
}

// *************************************************************************
// *		  Clear_Picking_Data:- Terry and Hazel Flanigan 2023	   	   *
// *************************************************************************
void SB_Picking::Clear_Picking_Data()
{
    Total_vertex_count = 0;
    Total_index_count = 0;
    Face_Index = 0;
    Sub_Mesh_Count = 0;
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
void SB_Picking::Mouse_Pick_Entity()
{
    Clear_Picking_Data();

    // --------------------------------------------------------------
    Ogre::RenderWindow* rw = App->CLSB_Ogre->mWindow;
    Ogre::Camera* camera = App->CLSB_Ogre->mCamera;

    ImGuiIO& io = ImGui::GetIO();

    Ogre::Real tx = io.MousePos.x / (Ogre::Real)rw->getWidth();
    Ogre::Real ty = io.MousePos.y / (Ogre::Real)rw->getHeight();

    Ogre::Ray ray = camera->getCameraToViewportRay(tx, ty);

    Pl_Entity_Name = "-----";

    Ogre::uint32 queryMask = -1;
    Ogre::Vector3 result = Ogre::Vector3(0, 0, 0);
    Ogre::MovableObject* target = NULL;
    closest_distance = 0;

    Pl_Entity_Name = " ";
    App->CLSB_Ogre->OgreListener->Selected_Object_Name[0] = 0;

    Ogre::SceneNode* mNode;

    Ogre::Ray ray2 = camera->getCameraToViewportRay(tx, ty);
    if (raycast(ray2, result, target, closest_distance, queryMask))
    {
  
        //App->Beep_Win();

        mNode = pentity->getParentSceneNode();
        Pl_Entity_Name = pentity->getName();

        char buff[255];
        strcpy(buff, Pl_Entity_Name.c_str());

        //App->CL_Vm_ImGui->Show_Object_Selection = 1;

        bool test = Ogre::StringUtil::match("Plane0", Pl_Entity_Name, true);
        if (test == 1)
        {
            //Pl_Entity_Name = "---------";
        }
        else
        {
            bool test = Ogre::StringUtil::match("Player_1", Pl_Entity_Name, true);
            if (test == 1)
            {
                //Pl_Entity_Name = "Player_1";

                return;
            }
            else
            {
                char* pdest;
                int IntNum = 0;

                strcpy(buff, Pl_Entity_Name.c_str());
                pdest = strstr(buff, "GDEnt_");
                if (pdest != NULL)
                {
                   // sscanf((buff + 6), "%i", &IntNum);

                   // App->SBC_Markers->MarkerBB_Addjust(IntNum);

                   // App->CLSB_Ogre->OgreListener->Selected_Entity_Index = IntNum;
                   // strcpy(App->CLSB_Ogre->OgreListener->Selected_Object_Name, App->SBC_Scene->V_Object[IntNum]->Mesh_Name);

                   // App->SBC_FileView->SelectItem(App->SBC_Scene->V_Object[App->CL_Ogre->OgreListener->Selected_Entity_Index]->FileViewItem);

                    return;

                }

                pdest = strstr(buff, "Area_");
                if (pdest != NULL)
                {
                    sscanf((buff + 5), "%i", &IntNum);

                    //App->SBC_Markers->MarkerBB_Addjust(IntNum);

                    App->CLSB_Ogre->OgreListener->Selected_Entity_Index = IntNum;
                   // strcpy(App->CLSB_Ogre->OgreListener->Selected_Object_Name, App->SBC_Scene->B_Area[IntNum]->Area_Name);

                    //App->SBC_FileView->SelectItem(App->SBC_Scene->B_Area[App->CL_Ogre->OgreListener->Selected_Entity_Index]->FileViewItem);

                    return;
                }

            }
     }

    }
    else
    {
       // Pl_Entity_Name = "---------";
        //strcpy(TextureName, "None 2");
        Selected_Ok = 0;
    }

}

// *************************************************************************
// *		        raycast:- Terry and Hazel Flanigan 2022		       	   *
// *************************************************************************
bool SB_Picking::raycast(const Ogre::Ray& ray, Ogre::Vector3& result, Ogre::MovableObject*& target, float& closest_distance, const Ogre::uint32 queryMask)
{
    Total_index_count_Actual = 0;
    Total_Vertices_count_Actual = 0;

	target = NULL;
	bool ParticleFound = 0;
	Pl_Entity_Name = "---------";
    
	if (mRaySceneQuery != NULL)
	{
		mRaySceneQuery->setRay(ray);
		mRaySceneQuery->setSortByDistance(true);
		// mRaySceneQuery->setQueryMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
		mRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
		// execute the query, returns a vector of hits
		if (mRaySceneQuery->execute().size() <= 0)
		{
			// raycast did not hit an objects bounding box
			Selected_Ok = 0;
			Pl_Entity_Name = "---------";
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

			// get the mesh information
			GetMeshInformation(((Ogre::Entity*)pentity)->getMesh(),
				pentity->getParentNode()->_getDerivedPosition(),
				pentity->getParentNode()->_getDerivedOrientation(),
				pentity->getParentNode()->_getDerivedScale());

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

						App->CLSB_Grid->HitVertices[0] = vertices[indices[i]];
						App->CLSB_Grid->HitVertices[1] = vertices[indices[i + 1]];
						App->CLSB_Grid->HitVertices[2] = vertices[indices[i + 2]];

						App->CLSB_Grid->Face_Update2();

						App->CLSB_Grid->HitFaceUVs[0] = TextCords[Face_Index];
						App->CLSB_Grid->HitFaceUVs[1] = TextCords[Face_Index + 1];
						App->CLSB_Grid->HitFaceUVs[2] = TextCords[Face_Index + 2];

						SubMesh_Index = Sub_Mesh_Indexs[Face_Index];

						Get_Material_Data();

                        Sub_Mesh_Count = Get_SubMesh_Count();
                        Total_index_count_Actual = Get_Total_Indices();
                        Total_Vertices_count_Actual = Get_Total_Vertices();

                        Local_Face = Get_Local_Face(SubMesh_Index);

						App->CLSB_Grid->FaceNode->setVisible(true);
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

	if (closest_distance >= 0.0f)
	{
		// raycast success
		result = closest_result;
		Selected_Ok = 1;
		return (true);
	}
	else
	{
		// raycast failed
		Selected_Ok = 0;
		Pl_Entity_Name = "---------";
		return (false);
	}
}

// *************************************************************************
// *		  Get_Material_Data:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
void SB_Picking::Get_Material_Data()
{
    int test = ((Ogre::Entity*)pentity)->getMesh()->getNumSubMeshes();

    if (SubMesh_Index > test)
    {
        App->Say("Sub Mesh Out of bounds");
    }
    else
    {
        strcpy(FaceMaterial, ((Ogre::Entity*)pentity)->getMesh()->getSubMesh(SubMesh_Index)->getMaterialName().c_str());
        Ogre::MaterialPtr  MatCurent = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(FaceMaterial));
        strcpy(TextureName2, MatCurent->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName().c_str());

    }
}

// *************************************************************************
// *					      GetMeshInformation		              	   *
// *************************************************************************
void SB_Picking::GetMeshInformation(const Ogre::MeshPtr mesh, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale)
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
    Sub_Mesh_Indexs = new Ogre::uint32[Total_index_count];

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
int SB_Picking::Get_SubMesh_Count()
{
    return ((Ogre::Entity*)pentity)->getNumSubEntities();;
}

// *************************************************************************
// *		Get_Total_Indices:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int SB_Picking::Get_Total_Indices()
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
int SB_Picking::Get_Total_Vertices()
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
int SB_Picking::Get_Local_Face(int SelectedGroup)
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
