#include "APIEnvir.h"
#include "ACAPinc.h"

#include "ResourceIds.hpp"
#include "DGModule.hpp"
#include "StringConversion.hpp"
#include <iostream>
#include "Controls.hpp"
#include "TemplateDialog.hpp"

using namespace std;

static const GSResID AddOnInfoID = ID_ADDON_INFO;
static const Int32 AddOnNameID = 1;
static const Int32 AddOnDescriptionID = 2;

static const short AddOnMenuID = ID_ADDON_MENU;
static const Int32 AddOnCommandID = 1;

static GSErrCode MenuCommandHandler(const API_MenuParams* menuParams)
{
	switch (menuParams->menuItemRef.menuResID) {
	case AddOnMenuID:
		switch (menuParams->menuItemRef.itemIndex) {
		case AddOnCommandID:
		{
			Controls init(10, 20, 1);
			TemplateDialog dialog(init);
			dialog.Invoke();
		}
		break;
		}
		break;
	}
	return NoError;
}


API_AddonType __ACDLL_CALL CheckEnvironment(API_EnvirParams* envir)
{
	RSGetIndString(&envir->addOnInfo.name, AddOnInfoID, AddOnNameID, ACAPI_GetOwnResModule());
	RSGetIndString(&envir->addOnInfo.description, AddOnInfoID, AddOnDescriptionID, ACAPI_GetOwnResModule());

	return APIAddon_Normal;
}

GSErrCode __ACDLL_CALL RegisterInterface(void)
{
	return ACAPI_Register_Menu(AddOnMenuID, 0, MenuCode_Tools, MenuFlag_Default);
}

GSErrCode __ACENV_CALL Initialize(void)
{
	return ACAPI_Install_MenuHandler(AddOnMenuID, MenuCommandHandler);
}

GSErrCode __ACENV_CALL FreeData(void)
{
	return NoError;
}
