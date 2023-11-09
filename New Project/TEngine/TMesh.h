#ifndef __TMESH_H__
#define __TMESH_H__

//+-------------------------------------------------------
//| Mesh wrapper class
//| Version 1.004
//| TODO: handle loading materials with no texture
//+-------------------------------------------------------

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __D3DX9_H__
#include <d3dx9.h>
#endif

#ifndef __TUTIL_H__
#include "TUtil.h"
#endif

#ifndef __TAABB_H__
#include "TAABB.h"
#endif

#ifndef __TSPHERE_H__
#include "TSphere.h"
#endif

#ifndef __TTEXTURE_H__
#include "TTexture.h"
#endif

#ifndef __TERROR_H__
#include "TError.h"
#endif

#ifndef __TMESHHANDLEDEFINED__
#define __TMESHHANDLEDEFINED__
#define TMeshHandle unsigned int
#endif

class TMesh
{
public:
	static bool			Load(const char* filename, TMeshHandle* nID, TVERTEXID nVID = VID_UU);
	static void			Unload();
	static bool			PreLoad(const char* filename, TVERTEXID nVID = VID_UU);
	static unsigned int GetAllocatedMemory();

	static inline TMesh* GetMesh(TMeshHandle nID);

	TMesh();
	~TMesh();

	void				Render();
	bool				LoadFile(const char* filename, TVERTEXID);
	void				Release();
	void				ComputeBounds(TAABB& box);
	void				ComputeBounds(TSphere& sphere);
	bool				SetTexture(unsigned int nSubset, const char* file);

	inline const TString&		GetFilename() const;
	inline DWORD				GetNumTriangles() const;
	inline DWORD				GetNumVertices() const;
	inline unsigned int			GetMemoryFootprint() const;
	inline void					GetAABB(TAABB& box) const;
	inline void					GetSphere(TSphere& sphere) const;
	inline D3DXMATERIAL &		GetMaterial(unsigned int nSubset) const;
	inline LPDIRECT3DTEXTURE9 & GetTexture(unsigned int nSubset) const;
	inline bool					SetTexture(unsigned int nSubset, TTextureHandle nTexture);
	inline void					DrawSubset(unsigned int nSubset);
	inline DWORD				GetNumMaterials() const;

private:
	static bool HasNormals(ID3DXMesh* pMesh);

	static std::vector<TMesh*> ms_pMeshes;
	static unsigned int ms_nAllocatedMemory;

	LPD3DXMESH			m_pMesh;
	TString				m_strFilename;
	UINT				m_nMemoryFootprint;

	DWORD				m_nNumMaterials;
	D3DXMATERIAL*		m_pMaterials;
	LPD3DXBUFFER		m_pbufMaterials;
	LPD3DXBUFFER		m_pAdjacencyBuffer;
	TTextureHandle*		m_pTextures;

	TAABB				m_aabbBounds;
	TSphere				m_sphereBounds;
}; // class TMesh

#include "TMesh.inl"

#endif
