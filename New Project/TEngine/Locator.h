#ifndef __LOCATOR_H__
#define __LOCATOR_H__

//+-------------------------------------------------------
//| Service Locator for game components
//| Hides the implementation behind abstract interfaces
//| Version 1.001
//+-------------------------------------------------------

class IAudio;
class IInput;
class ITextPrinter;
class IRenderDevice;
class IRenderer;

class Locator
{
public:
	static void				RegisterAudio(IAudio * pAudio);
	static void				RegisterInput(IInput * pInput);
	static void				RegisterTextPrinter(ITextPrinter * pText);
	static void				RegisterRenderDevice(IRenderDevice * pRenderDevice);
	static void				RegisterRenderer(IRenderer * pRenderer);

	static inline IAudio *			GetAudio();
	static inline IInput *			GetInput();
	static inline ITextPrinter *	GetTextPrinter();
	static inline IRenderDevice *	GetRenderDevice();
	static inline IRenderer *		GetRenderer();

protected:
	static IAudio * m_pAudio;
	static IInput * m_pInput;
	static ITextPrinter * m_pTextPrinter;
	static IRenderDevice * m_pRenderDevice;
	static IRenderer * m_pRenderer;
};

#include "Locator.inl"

#endif
