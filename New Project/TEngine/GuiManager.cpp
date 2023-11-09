#pragma warning (disable : 4244) // converting atof (double) to float

#include "SafeRelease.h"
#include "TMemory.h"
#include "TGeneralLib.h"
#include "TError.h"
#include "GuiItem.h"
#include "GuiManager.h"

#include "GuiText.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCursor.h"

//-------------------------------------------------------------------------------
GuiManager::GuiManager()
{

}

//-------------------------------------------------------------------------------
GuiManager::~GuiManager()
{
	Clear();
}

//-------------------------------------------------------------------------------
void GuiManager::Clear()
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		TSafeDelete(m_pItems[i]);
	}
	m_pItems.clear();
}

//-------------------------------------------------------------------------------
void GuiManager::Render()
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		m_pItems[i]->Render();
	}
}

//-------------------------------------------------------------------------------
GuiItem * GuiManager::GetSelectedItem(const TVector2 & position) const
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->IsSelected(position))
		{
			return m_pItems[i];
		}
	}
	return NULL;
}

//-------------------------------------------------------------------------------
void GuiManager::AddItem(GuiItem * pItem)
{
	m_pItems.push_back(pItem);
}

//-------------------------------------------------------------------------------
void GuiManager::RemoveItem(const TString& name)
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->GetName() == name)
		{
			TSafeDelete(m_pItems[i]);
			m_pItems.erase( m_pItems.begin() + i );
		}
	}
}

//-------------------------------------------------------------------------------
void GuiManager::SetPosition(const TString& name, const TVector2 & position)
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->GetName() == name)
		{
			m_pItems[i]->SetPosition(position);
		}
	}
}

//-------------------------------------------------------------------------------
void GuiManager::SetValue(const TString& name, const char* format, ...)
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->GetName() == name)
		{
			char	ch[256];
			char	*pArgs;

			pArgs = (char*) &format + sizeof(format);
			vsprintf_s(ch, 256, format, pArgs);

			m_pItems[i]->SetValue(ch);
		}
	}
}

//-------------------------------------------------------------------------------
GuiItem * GuiManager::GetButtonByName(const TString& name) const
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->GetName() == name)
			return m_pItems[i];
	}
	return NULL;
}

//-------------------------------------------------------------------------------
void GuiManager::SetVisible(const TString& name, bool b)
{
	for (unsigned int i=0; i<m_pItems.size(); ++i)
	{
		if (m_pItems[i]->GetName() == name)
			m_pItems[i]->SetVisible(b);
	}
}

//-------------------------------------------------------------------------------
void GuiManager::Update(const GuiCursor& cursor)
{
	GuiItem* pItem = NULL;
	TVector2 position = cursor.GetPosition();

	if (cursor.GetState() == Cursor_Pressed)
	{
		pItem = GetSelectedItem(position);

		if (pItem != NULL)
		{
			pItem->OnPress(position);
		}
	}
	else if (cursor.GetState() == Cursor_Released)
	{
		pItem = GetSelectedItem(position);

		pItem = GetSelectedItem(position);

		if (pItem != NULL)
		{
			pItem->OnRelease(position);
		}
	}
	else if (cursor.GetState() == Cursor_Down)
	{
		pItem = GetSelectedItem(position);

		if (pItem != NULL)
		{
			pItem->OnDrag(position);
		}
	}
}

//-------------------------------------------------------------------------------
bool GuiManager::LoadFile(const char* filename)
{
	FILE * pFile;
	fopen_s(&pFile, filename, "r");

	if (!pFile)
	{
		TDebugPrint(("[GuiManager::LoadFile] Failed to load %s", filename));
		return false;
	}

	// get number of items
	char str[128];
	unsigned int nLine = 1;
	bool bError;

	while (fgets(str, 128, pFile))
	{
		// fetch next line
		std::string line = str;		// convert to std::string (easier)
		line.erase( line.end()-1 );	// cut newline

		std::vector<std::string> parts;	// split into parts
		StringTokenize(line, parts, " ");

		bError = true;		// error unless we load the line successfully

		if (parts[0] == "button")
		{
			GuiButton* pButton = GuiButton::Create(parts);
			if (pButton)
			{
				pButton->SetSelectable(true);
				AddItem(pButton);
				bError = false;
			}
		}

		if (parts[0] == "image")
		{
			GuiButton* pButton = GuiButton::Create(parts);
			if (pButton)
			{
				pButton->SetSelectable(false);
				AddItem(pButton);
				bError = false;
			}
		}

		if (parts[0] == "text")
		{
			GuiText * pText = GuiText::Create(parts);
			if (pText)
			{
				AddItem(pText);
				bError = false;
			}
		}

		if (parts[0] == "slider")
		{
			GuiSlider* pSlider = GuiSlider::Create(parts);
			if (pSlider)
			{
				AddItem(pSlider);
				bError = false;
			}
		}

		if (bError)
		{
			TDebugPrint(("[GuiManager::LoadFile] error in %s, line %d", filename, nLine));
			TDebugPrint(("[GuiManager::LoadFile] line reads '%s'", line.c_str()));
		}

		++ nLine;
	} // while

	fclose(pFile);
	return true;
}

