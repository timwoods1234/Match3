#ifndef __TUTIL_H__
#define __TUTIL_H__

//+-------------------------------------------------------
//| Contains common references for the rendering device
//| Version 1.003
//+-------------------------------------------------------

#ifndef __D3DX9MATH_H__
#include <d3dx9math.h>
#endif

#ifndef __TMathClasses_H__
#include "TMathClasses.h"
#endif

#ifndef _VECTOR_
#include <vector>
#endif

//+-----------------------------------------------------------------------
//| Vertex Structures
//+-----------------------------------------------------------------------
typedef enum TVERTEXID_TYPE
{
	VID_NONE,	// empty
	VID_UU,		// untransformed unlit, see VERTEX
	VID_UL,		// untransformed lit, see LVERTEX
	VID_UN,		// untransformed normal, see NVERTEX
} TVERTEXID;

typedef struct VERTEX_TYPE
{
	TVector3 position;
	TVector3 normal;
	TVector2 texture;

	static IDirect3DVertexDeclaration9* Decl;
} VERTEX;
#define FVF_VERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

typedef struct LVERTEX_TYPE
{
	TVector3	position;
	DWORD		Color;
	TVector2	texture;

	static IDirect3DVertexDeclaration9* Decl;
} LVERTEX;
#define FVF_LVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

typedef struct NVERTEX_TYPE
{
	TVector3 position;
	TVector3 normal;

	static IDirect3DVertexDeclaration9* Decl;
} NVERTEX;
#define FVF_NVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL )

LPDIRECT3DVERTEXDECLARATION9 GetVDecl(TVERTEXID vID);

//+-----------------------------------------------------------------------
//| Particle struct and VDECL
//+-----------------------------------------------------------------------
struct Particle
{
	TVector3	initialPos;
	TVector3	initialVelocity;
	float       initialSize; // In pixels.
	float       initialTime;
	float       lifeTime;
	float       mass;
	DWORD	    initialColor;

	static IDirect3DVertexDeclaration9* Decl;
};

//+-----------------------------------------------------------------------
//| Render States
//+-----------------------------------------------------------------------
typedef enum TD3DRENDERSTATE_TYPE
{
	RS_DEPTH_READWRITE,	// read and write depth buffer
	RS_DEPTH_READONLY,	// no writes to depth buffer
	RS_DEPTH_NONE,		// no reads or writes to depth buffer

	RS_SHADE_POINTS,	// render vertices as points
	RS_SHADE_TRIWIRE,	// render wireframe triangles
	RS_SHADE_HULLWIRE,	// render wireframe polygons
	RS_SHADE_SOLID,		// solid triangles

	RS_ZFUNC_LESSEQUAL,
	RS_ZFUNC_ALWAYS,

} TD3DRENDERSTATE;

//+-----------------------------------------------------------------------
//| Colors
//+-----------------------------------------------------------------------
const D3DXCOLOR		WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR		BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR		RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR		GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR		BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR		YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR		CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR		MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );
const D3DXCOLOR		BEACH_SAND( D3DCOLOR_XRGB(255, 249, 157) );
const D3DXCOLOR		DESERT_SAND( D3DCOLOR_XRGB(250, 205, 135) );
const D3DXCOLOR		LIGHTGREEN( D3DCOLOR_XRGB( 60, 184, 120) );
const D3DXCOLOR		PUREGREEN( D3DCOLOR_XRGB(  0, 166,  81) );
const D3DXCOLOR		DARKGREEN( D3DCOLOR_XRGB(  0, 114,  54) );
const D3DXCOLOR		LIGHT_YELLOW_GREEN( D3DCOLOR_XRGB(124, 197, 118) );
const D3DXCOLOR		PURE_YELLOW_GREEN( D3DCOLOR_XRGB( 57, 181,  74) );
const D3DXCOLOR		DARK_YELLOW_GREEN( D3DCOLOR_XRGB( 25, 123,  48) );
const D3DXCOLOR		LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
const D3DXCOLOR		DARKBROWN( D3DCOLOR_XRGB(115, 100,  87));

//+-----------------------------------------------------------------------
//| Materials & Lighting
//+-----------------------------------------------------------------------
__declspec(dllexport) D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

struct DirLight
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	TVector3 dirW;
};

struct SpotLight
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	TVector3 posW;
	TVector3 dirW;  
	float  spotPower;
};

//+-----------------------------------------------------------------------
//| Helper Methods & Structures
//+-----------------------------------------------------------------------
void InitLight(D3DLIGHT9 &light, D3DLIGHTTYPE ltType,
			   float x, float y, float z,
			   float r, float g, float b,
			   float range);

void BuildTriGrid(int numVertRows, int numVertCols,
				  float dx, float dz, 
				  const TVector3& center, 
				  std::vector<TVector3>& verts,
				  std::vector<DWORD>& indices);

#endif
