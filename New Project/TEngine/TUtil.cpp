#include "TUtil.h"
#include "SafeRelease.h"

//-------------------------------------------------------------------------------
IDirect3DVertexDeclaration9* VERTEX::Decl = 0;
IDirect3DVertexDeclaration9* LVERTEX::Decl = 0;
IDirect3DVertexDeclaration9* NVERTEX::Decl = 0;
IDirect3DVertexDeclaration9* Particle::Decl = 0;

//-------------------------------------------------------------------------------
D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

//-------------------------------------------------------------------------------
void InitLight(D3DLIGHT9 &light, D3DLIGHTTYPE ltType,
			   float x, float y, float z,
			   float r, float g, float b,
			   float range)
{
    ZeroMemory( &light, sizeof(D3DLIGHT9) );

    light.Type        = ltType;
    light.Diffuse.r   = r;
    light.Diffuse.g   = g;
    light.Diffuse.b   = b;

    TVector3 vecLightDirUnnormalized(x, y, z);
	vecLightDirUnnormalized.Normalize();
	light.Direction.x = vecLightDirUnnormalized.x;
	light.Direction.y = vecLightDirUnnormalized.y;
	light.Direction.z = vecLightDirUnnormalized.z;

    light.Position.x   = x;
    light.Position.y   = y;
    light.Position.z   = z;
    light.Range        = range;
} // InitLight

//-------------------------------------------------------------------------------
LPDIRECT3DVERTEXDECLARATION9 GetVDecl(TVERTEXID vID)
{
	switch (vID)
	{
	case VID_NONE:
		{ return NULL; } break;
	case VID_UU:
		{ return VERTEX::Decl; } break;
	case VID_UL:
		{ return LVERTEX::Decl; } break;
	case VID_UN:
		{ return NVERTEX::Decl; } break;
	}
	return NULL;
}

//-------------------------------------------------------------------------------
void BuildTriGrid(int numVertRows, int numVertCols,
				  float dx, float dz, 
				  const TVector3& center, 
				  std::vector<TVector3>& verts,
				  std::vector<DWORD>& indices)
{
	int numVertices = numVertRows*numVertCols;
	int numCellRows = numVertRows-1;
	int numCellCols = numVertCols-1;

	int numTris = numCellRows*numCellCols*2;

	float width = (float)numCellCols * dx;
	float depth = (float)numCellRows * dz;

	// build triangle grid
	verts.resize( numVertices );

	// build about center
	float xOffset = -width * 0.5f; 
	float zOffset =  depth * 0.5f;

	int k = 0;
	for(float i = 0; i < numVertRows; ++i)
	{
		for(float j = 0; j < numVertCols; ++j)
		{
			verts[k].x =  j * dx + xOffset;
			verts[k].z = -i * dz + zOffset;
			verts[k].y =  0.0f;

			// make sure middle of the grid is in the right place
			TMatrix T;
			T.Translate(center);
			T.TransformVector(&verts[k]);

			++k;
		}
	}

	// indices for each quad
	indices.resize(numTris * 3);

	k = 0;
	for(DWORD i = 0; i < (DWORD)numCellRows; ++i)
	{
		for(DWORD j = 0; j < (DWORD)numCellCols; ++j)
		{
			indices[k]     =   i   * numVertCols + j;
			indices[k + 1] =   i   * numVertCols + j + 1;
			indices[k + 2] = (i+1) * numVertCols + j;

			indices[k + 3] = (i+1) * numVertCols + j;
			indices[k + 4] =   i   * numVertCols + j + 1;
			indices[k + 5] = (i+1) * numVertCols + j + 1;

			k += 6;
		}
	}
}
