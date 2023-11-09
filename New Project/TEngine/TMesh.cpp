#pragma warning (disable : 4129)	// unrecognized character '/' in TMesh::Load

#include "TMesh.h"
#include "SafeRelease.h"
#include "IRenderDevice.h"
#include "Locator.h"
#include "TError.h"
#include "TTexture.h"
#include "TAABB.h"
#include "TSphere.h"
#include "TMemory.h"

//-------------------------------------------------------------------------------
std::vector<TMesh*> TMesh::ms_pMeshes;
unsigned int TMesh::ms_nAllocatedMemory;

//-------------------------------------------------------------------------------
bool TMesh::Load(const char* filename, TMeshHandle* nID, TVERTEXID nVID)
{
	// don't load the same mesh twice
	for (int i=0; i<(int)ms_pMeshes.size(); i++)
	{
		if (ms_pMeshes[i]->GetFilename() == filename)
		{
			(*nID) = i;
			return true;
		}
	}

	TMesh *pNewMesh = TNew TMesh();

	if (!(pNewMesh->LoadFile(filename, nVID)))
	{
		free(pNewMesh);
		(*nID) = -1;

		TDebugPrint(("[TMesh::Load] Failed to load %s", filename));

		return false;
	}

	ms_pMeshes.push_back(pNewMesh);
	(*nID) = ms_pMeshes.size() - 1;

	ms_nAllocatedMemory += pNewMesh->GetMemoryFootprint();

	return true;
}

//-------------------------------------------------------------------------------
void TMesh::Unload()
{
	for (unsigned int i=0; i<ms_pMeshes.size(); ++i)
	{
		TSafeDelete(ms_pMeshes[i]);
	}

	ms_pMeshes.clear();
}

//-------------------------------------------------------------------------------
bool TMesh::PreLoad(const char* filename, TVERTEXID nVID)
{
	// open file
	FILE* f;
	fopen_s(&f, filename, "r");

	if (!f)
	{
		TDebugPrint(("[TMesh::PreLoad] Couldn't open file %s", filename));
		return false;
	}

	// read lines until EOF
	char str[1024];

	int nLine = 0;
	unsigned int temp = 0;

	while (fgets(str, 1024, f))
	{
		// fetch next line
		std::string string = str;		// convert to std::string (easier)
		string.erase( string.end()-1 );	// cut newline

		if (!Load(string.c_str(), &temp, nVID))
		{
			TDebugPrint(("[TMesh::PreLoad]: preload failed (%s)", string.c_str()));
		}

		++nLine;
	}

	fclose(f);
	f = NULL;

	return true;
}

//-------------------------------------------------------------------------------
unsigned int TMesh::GetAllocatedMemory()
{
	return ms_nAllocatedMemory;
}

//-------------------------------------------------------------------------------
TMesh::TMesh()
:	m_nMemoryFootprint(0),
	m_pMesh(0)
{

}

//-------------------------------------------------------------------------------
TMesh::~TMesh()
{
	Release();
}

//-------------------------------------------------------------------------------
bool TMesh::LoadFile(const char* filename, TVERTEXID vID)
{
    m_pAdjacencyBuffer					= NULL;
    LPD3DXBUFFER	pMtrlBuffer			= NULL;
	ID3DXMesh*		meshSys				= NULL;	// system mesh
	m_strFilename						= filename;

	// Load mesh from file
	if(FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, 
								Locator::GetRenderDevice()->GetDevice(),
								&m_pAdjacencyBuffer,
								&m_pbufMaterials, NULL, 
								&m_nNumMaterials, &meshSys)))
	{
		TDebugPrint(("[TMesh::Load] Failed to load mesh %s", filename));
		return false;
	}

	// Convert to vertex format if required
	if (vID != VID_NONE)
	{
		D3DVERTEXELEMENT9 elements[64];
		UINT numElements = 0;
		GetVDecl(vID)->GetDeclaration(elements, &numElements);

		ID3DXMesh* temp = NULL;

		meshSys->CloneMesh(D3DXMESH_SYSTEMMEM,
			elements, Locator::GetRenderDevice()->GetDevice(), &temp);
		SafeRelease(meshSys);
		meshSys = temp;
	}

	// Generate normals
	if(!HasNormals(meshSys))
	{
      D3DXComputeNormals(meshSys, 0);
	}

    // Optimize the mesh for performance
    meshSys->Optimize( D3DXMESH_MANAGED | 
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)m_pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL,
						&m_pMesh);
	SafeRelease(meshSys);

	// Materials & Textures
	m_pMaterials = (D3DXMATERIAL*)m_pbufMaterials->GetBufferPointer();
	m_pTextures  = new UINT[m_nNumMaterials];

	// Make sure texture is loaded from the right path
	std::string strFileName = filename;

	if (strFileName.rfind("\/") == std::string::npos)
	{
		strFileName = "";
	}
	else
	{
		strFileName = strFileName.substr(0, strFileName.rfind("\/")+1);
	}

	for( unsigned int i=0; i < m_nNumMaterials; i++ )
	{
		bool bTextureLoaded = false;

		if (m_pMaterials[i].pTextureFilename)
		{
			std::string texFileName = strFileName;
			texFileName.append(m_pMaterials[i].pTextureFilename);

			if (SetTexture(i, texFileName.c_str()))
			{
				bTextureLoaded = true;
			}
		}

		if (!bTextureLoaded)
		{
			TTextureHandle blank = 0;	// this is a white, blank texture
			SetTexture(i, blank);
		}
	}

	m_nMemoryFootprint = m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices();

	ComputeBounds(m_aabbBounds);
	ComputeBounds(m_sphereBounds);

	return true;
}

//-------------------------------------------------------------------------------
void TMesh::Render()
{
	for(unsigned int i = 0; i < m_nNumMaterials; i++ )
	{
		m_pMesh->DrawSubset(i);
	}
}

//-------------------------------------------------------------------------------
void TMesh::Release()
{
	free(m_pTextures);

	SafeRelease(m_pMesh);
	SafeRelease(m_pbufMaterials);
	SafeRelease(m_pAdjacencyBuffer);
}

//-------------------------------------------------------------------------------
bool TMesh::SetTexture(unsigned int nSubset, const char* file)
{
	return TTexture::Load(file, &m_pTextures[nSubset]);
}

//-------------------------------------------------------------------------------
void TMesh::ComputeBounds(TAABB& box)
{
	TVector3 min, max;
	VERTEX* v = 0;

	m_pMesh->LockVertexBuffer(0, (void**)&v);

	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)&v[0].position, m_pMesh->GetNumVertices(),
		sizeof(VERTEX), (D3DXVECTOR3*)&min, (D3DXVECTOR3*)&max);

	m_pMesh->UnlockVertexBuffer();

	box.SetMin(min);
	box.SetMax(max);
}

//-------------------------------------------------------------------------------
void TMesh::ComputeBounds(TSphere& sphere)
{
	TVector3 centre;
	float fRadius;
	VERTEX* v = 0;

	m_pMesh->LockVertexBuffer(0, (void**)&v);

	D3DXComputeBoundingSphere((D3DXVECTOR3*)&v[0].position, m_pMesh->GetNumVertices(),
		sizeof(VERTEX), (D3DXVECTOR3*)&centre, &fRadius);

	m_pMesh->UnlockVertexBuffer();

	sphere.SetCentre(centre);
	sphere.SetRadius(fRadius);
}

//-------------------------------------------------------------------------------
bool TMesh::HasNormals(ID3DXMesh* pMesh)
{
	bool bHasNormals = false;

	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(elems);
	
	for(int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		// Reached D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED, 0,0,0}?
		if(elems[i].Stream == 0xff)
		{
			break;
		}

		if( elems[i].Type == D3DDECLTYPE_FLOAT3 &&
			elems[i].Usage == D3DDECLUSAGE_NORMAL &&
			elems[i].UsageIndex == 0 )
		{
			bHasNormals = true;
			break;
		}
	}
	return bHasNormals;
}
