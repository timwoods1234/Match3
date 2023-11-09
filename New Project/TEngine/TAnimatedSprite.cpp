#include "TGeneralLib.h"
#include "TError.h"
#include "TAnimatedSprite.h"

//-------------------------------------------------------------------------------
TAnimatedSprite::TAnimatedSprite()
:	m_fFrameTimer(0.0f),
	m_nCurrentFrame(0),
	m_nCurrentAnimation(0),
	m_bAnimationFinished(false)
{
	m_strFilename = "";
}

//-------------------------------------------------------------------------------
TAnimatedSprite::~TAnimatedSprite()
{
	m_frames.clear();
	m_animations.clear();
}

//-------------------------------------------------------------------------------
const TString& TAnimatedSprite::GetCurrentAnimation() const
{
	return m_animations[ m_nCurrentAnimation ].strName;
}

//-------------------------------------------------------------------------------
TTextureHandle TAnimatedSprite::GetCurrentFrame() const
{
	return m_frames[m_nCurrentFrame].GetTextureHandle();
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::SetCurrentFrameIndex(UINT nFrame)
{
	m_nCurrentFrame = nFrame;

	if (nFrame >= m_frames.size())
	{
		m_nCurrentFrame = m_frames.size() - 1;
	}
}

//-------------------------------------------------------------------------------
int TAnimatedSprite::GetCurrentFrameIndex() const
{
	return m_nCurrentFrame;
}

//-------------------------------------------------------------------------------
const Animation* TAnimatedSprite::GetAnimation(const TString& strName) const
{
	for (unsigned int i=0; i<m_animations.size(); ++i)
	{
		if (m_animations[i].strName == strName)
		{
			return &m_animations[i];
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::SetAnimation(const TString& strAnimation)
{
	// set animation
	for (unsigned int i=0; i<m_animations.size(); i++)
	{
		if(strAnimation == m_animations[i].strName)
		{
			m_nCurrentAnimation = i;

			// reset current frame (beginning of animation) & timer
			m_nCurrentFrame = m_animations[m_nCurrentAnimation].nStartFrame;
			m_fFrameTimer = 0.0f;

			m_bAnimationFinished = true;
		}
	}
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::SetAnimation(int index)
{
	if (index >= (int)m_animations.size())
	{
		return;
	}

	m_nCurrentAnimation = index;
	m_nCurrentFrame		= m_animations[m_nCurrentAnimation].nStartFrame;
	m_fFrameTimer		= 0.0f;
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::IncrementFrame()
{
	static const TString strLoop = "Loop";

	// advance animation by one frame
	++m_nCurrentFrame;

	if (m_nCurrentFrame > m_animations[m_nCurrentAnimation].nEndFrame)
	{
		m_bAnimationFinished = true;

		if (m_animations[m_nCurrentAnimation].strNextAnim == strLoop)
		{
			// start this animation again
			SetAnimation(m_nCurrentAnimation);
		}
		else
		{
			SetAnimation( m_animations[m_nCurrentAnimation].strNextAnim );
		}
	}
}

//-------------------------------------------------------------------------------
bool TAnimatedSprite::Load(const char *filename)
{
	FILE * file = NULL;
	fopen_s(&file, filename, "r");

	if (!file)
	{
		TDebugPrint(("[TAnimatedSprite::Load] failed to open %s", filename));
		return false;
	}

	char str[256];

	while(fgets(str, 256, file))
	{
		// fetch next line
		std::string line = str;		// convert to std::string (easier)
		line.erase( line.end()-1 );	// cut newline

		// split into parts
		std::vector<std::string> parts;
		StringTokenize(line, parts, " ");

		if (parts.size() < 0)
		{
			// empty line, ignore
		}
		else if (strncmp(parts[0].c_str(), "//", 2) == 0)
		{
			// comment line, ignore
		}
		else if (parts.size() != 5)
		{
			TDebugPrint(("[TAnimatedSprite::Load] line has bad format"));
		}
		else
		{
			Animation anim;
			anim.strName = parts[0].c_str();
			anim.strNextAnim = parts[2].c_str();
			anim.nStartFrame = m_frames.size();
			anim.fFrameRate = 1.0f / (float)atof(parts[4].c_str());

			int num_frames = atoi(parts[1].c_str());

			for (int i=0; i<num_frames; i++)
			{
				char frame_filename[256];
				sprintf_s(frame_filename, 256, "%s/%s%d.png", parts[3].c_str(), *(anim.strName), i);

				TSprite frame;
				if (frame.LoadTexture(frame_filename))
				{
					m_frames.push_back(frame);
				}
				else
				{
					TDebugPrint(("[TAnimatedSprite::Load] unable to load %s", frame_filename));
				}
			}

			anim.nEndFrame = m_frames.size() - 1;

			m_animations.push_back(anim);
		}
	}

	fclose(file);

	m_strFilename = filename;

	return true;
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::Update(float elapsedTime)
{
	m_fFrameTimer += elapsedTime;

	if (m_fFrameTimer > m_animations[m_nCurrentAnimation].fFrameRate)
	{
		m_fFrameTimer = 0.0f;
		IncrementFrame();
	}
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::Render(const TVector2 & position, float rotation, float scaleX, float scaleY)
{
	m_frames[ m_nCurrentFrame ].Render(position, rotation, scaleX, scaleY);
}

//-------------------------------------------------------------------------------
bool TAnimatedSprite::HasAnimationFinished() const
{
	return m_bAnimationFinished;
}

//-------------------------------------------------------------------------------
const TVector2& TAnimatedSprite::GetDimensions() const
{
	return m_frames[ m_nCurrentFrame ].GetDimensions();
}

//-------------------------------------------------------------------------------
void TAnimatedSprite::SetDimensions(const TVector2& dimensions)
{
	for (unsigned int i=0; i<m_frames.size(); ++i)
	{
		m_frames[i].SetDimensions(dimensions);
	}
}
