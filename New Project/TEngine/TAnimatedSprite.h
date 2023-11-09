#ifndef __TANIMATEDSPRITE_H__
#define __TANIMATEDSPRITE_H__

//+-------------------------------------------------------
//| Wrapper class for 2D animation.
//| Version 1.002
//+-------------------------------------------------------

#include <vector>

#ifndef __TSTRING_H__
#include "TString.h"
#endif

#ifndef __TSprite_H__
#include "TSprite.h"
#endif

struct Animation
{
	TString		strName;
	int			nStartFrame;
	int			nEndFrame;
	TString		strNextAnim;	// "Loop" = loop
	float		fFrameRate;
};

class TAnimatedSprite
{
public:
	TAnimatedSprite();
	virtual ~TAnimatedSprite();

	const TString& GetCurrentAnimation() const;
	void SetAnimation(const TString& strAnimation);
	const Animation* GetAnimation(const TString& strName) const;

	// returns the actual frame, not the index
	TTextureHandle GetCurrentFrame() const;

	int GetCurrentFrameIndex() const;
	void SetCurrentFrameIndex(UINT nFrame);

	const TVector2& GetDimensions() const;
	void SetDimensions(const TVector2& dimensions);

	// IncrementFrame() increments m_nCurrentFrame by one
	// also swaps animation if the current animation finishes
	void IncrementFrame();
	bool HasAnimationFinished() const;

	bool Load(const char* filename);
	void Update(float elapsedTime);
	void Render(const TVector2 & position, float rotation=0.0, float scaleX=1.0, float scaleY=1.0);
private:
	int m_nCurrentFrame;
	int m_nCurrentAnimation;

	float m_fFrameTimer;
	bool  m_bAnimationFinished;

	std::vector<Animation> m_animations;
	std::vector<TSprite> m_frames;

	TString m_strFilename;

	void SetAnimation(int index);
};

#endif