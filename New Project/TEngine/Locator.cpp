#include "Locator.h"

//-------------------------------------------------------------------------------
IAudio*			Locator::m_pAudio			= 0;
IInput*			Locator::m_pInput			= 0;
ITextPrinter*	Locator::m_pTextPrinter		= 0;
IRenderDevice*	Locator::m_pRenderDevice	= 0;
IRenderer*		Locator::m_pRenderer		= 0;

//-------------------------------------------------------------------------------
void Locator::RegisterAudio(IAudio * pAudio)
{
	m_pAudio = pAudio;
}

//-------------------------------------------------------------------------------
void Locator::RegisterInput(IInput * pInput)
{
	m_pInput = pInput;
}

//-------------------------------------------------------------------------------
void Locator::RegisterTextPrinter(ITextPrinter * pText)
{
	m_pTextPrinter = pText;
}

//-------------------------------------------------------------------------------
void Locator::RegisterRenderDevice(IRenderDevice * pRenderDevice)
{
	m_pRenderDevice = pRenderDevice;
}

//-------------------------------------------------------------------------------
void Locator::RegisterRenderer(IRenderer * pRenderer) 
{
	m_pRenderer = pRenderer;
}
