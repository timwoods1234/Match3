#include "TAABB.h"
#include "TMatrix.h"
#include "TMath.h"
#include "Collision.h"

//-------------------------------------------------------------------------------
void TAABB::ComputeVertices( TVector3 (&vertices)[8] ) const
{
	vertices[0].Set( m_min.x, m_min.y, m_min.z );
	vertices[1].Set( m_max.x, m_min.y, m_min.z );
	vertices[2].Set( m_max.x, m_max.y, m_min.z );
	vertices[3].Set( m_min.x, m_max.y, m_min.z );
	vertices[4].Set( m_min.x, m_min.y, m_max.z );
	vertices[5].Set( m_max.x, m_min.y, m_max.z );
	vertices[6].Set( m_max.x, m_max.y, m_max.z );
	vertices[7].Set( m_min.x, m_max.y, m_max.z );
}

//-------------------------------------------------------------------------------
void TAABB::Transform( const TMatrix& M )
{
	TVector3 c = GetCentre();		// Convert to center/extent representation.
	TVector3 e = GetExtent();

	M.TransformVector(&c);		// Transform center in usual way.

	TMatrix absM;				// Transform extent.
	absM.SetIdentity();
	absM.m[0][0] = fabsf(M.m[0][0]); absM.m[0][1] = fabsf(M.m[0][1]); absM.m[0][2] = fabsf(M.m[0][2]);
	absM.m[1][0] = fabsf(M.m[1][0]); absM.m[1][1] = fabsf(M.m[1][1]); absM.m[1][2] = fabsf(M.m[1][2]);
	absM.m[2][0] = fabsf(M.m[2][0]); absM.m[2][1] = fabsf(M.m[2][1]); absM.m[2][2] = fabsf(M.m[2][2]);
	absM.Invert();
	absM.Transpose();
	absM.TransformVector(&e);

	m_min = c - e;	// Convert back to TAABB representation.
	m_max = c + e;
/* ----------------
	// the rotation will change the volume

	// turn this into an obb and transform the eight points
	TVector3 v[ 8 ];
	ComputeVertices( v );
	M.TransformVectors( v, 8 );

	// now calculate the new min and max
	TVector3 Min( v[0] );
	TVector3 Max( v[0] );
	for( unsigned int i = 1; i < 8; ++i )
	{
		Min = TMin(v[i], Min);
		Max = TMax(v[i], Max);
	}
	m_min = Min;
	m_max = Max;
*/
}

//-------------------------------------------------------------------------------
bool TAABB::IsInside( const TVector3& p ) const
{
	return
		(p.x >= m_min.x) && (p.x <= m_max.x) &&
		(p.y >= m_min.y) && (p.y <= m_max.y) &&
		(p.z >= m_min.z) && (p.z <= m_max.z);
}

//-------------------------------------------------------------------------------
void TAABB::GetSize(TVector3& size)const
{
	size = m_max;
	size -= m_min;
}

//-------------------------------------------------------------------------------
TVector3 TAABB::GetCorner(unsigned int i) const
{
	// Bit 0 selects min.x vs. max.x
	// Bit 1 selects min.y vs. max.y
	// Bit 2 selects min.z vs. max.z
	return TVector3(
		(i & 1) ? m_max.x : m_min.x,
		(i & 2) ? m_max.y : m_min.y,
		(i & 4) ? m_max.z : m_min.z
	);
}

//-------------------------------------------------------------------------------
TVector3 TAABB::GetClosestPoint(const TVector3& p) const
{
	// "Push" p into the box, on each dimension

	TVector3 r;

	if		(p.x < m_min.x) { r.x = m_min.x; }
	else if (p.x > m_max.x) { r.x = m_max.x; }
	else					{ r.x = p.x;     }

	if		(p.y < m_min.y) { r.y = m_min.y; }
	else if (p.y > m_max.y) { r.y = m_max.y; }
	else					{ r.y = p.y;	 }

	if		(p.z < m_min.z) { r.z = m_min.z; }
	else if (p.z > m_max.z) { r.z = m_max.z; }
	else					{ r.z = p.z;	 }

	return r;
}