#include "TemplateDialog.hpp"
#include "ResourceIds.hpp"
#include "ACAPinc.h"
#include "GSRoot.hpp"
#include <string>
// from GSXML
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>

// from GSXMLUtils
#include "GSXMLDOMUtils.hpp"
#include "XMLUtils.h"

// from InputOutput
#include "File.hpp"
#include "FileTypeManager.hpp"

static const GS::UniString XMLTag_TemplateData = "TemplateData";
static const GS::UniString XMLTag_TemplateProperty = "Properties";
static const GS::UniString XMLTag_Name = "Name";
static const GS::UniString XMLTag_Val = "Value";


static GSErrCode WriteTemplateDataToXMLDocument(xercesc::DOMDocument*& document)
{
	GSErrCode err = NoError;

	document = xercesc::DOMImplementation::getImplementation()->createDocument(nullptr, UNISTR_TO_XMLCH_PTR(XMLTag_TemplateData), nullptr);
	if (DBERROR(document == nullptr))
		return Error;

	try {
		xercesc::DOMElement* rootTag = document->getDocumentElement();
		if (rootTag == nullptr)
			throw 1;

		xercesc::DOMElement* groupsElement = document->createElement(UNISTR_TO_XMLCH_PTR(XMLTag_TemplateProperty));
		GSXMLUtils::WriteNewLine(rootTag);
		GSXMLUtils::WriteTabs(rootTag, 1);
		rootTag->appendChild(groupsElement);

		//Template Manager
		xercesc::DOMElement* groupNameElement = document->createElement(UNISTR_TO_XMLCH_PTR(XMLTag_Name));
		GSXMLUtils::WriteNewLine(groupsElement);
		GSXMLUtils::WriteTabs(groupsElement, 2);
		groupsElement->appendChild(groupNameElement);
		groupNameElement->appendChild(document->createTextNode(UNISTR_TO_XMLCH_PTR(GS::UniString("Template Manager"))));

		xercesc::DOMElement* groupGuidElement = document->createElement(UNISTR_TO_XMLCH_PTR(XMLTag_Val));
		GSXMLUtils::WriteNewLine(groupsElement);
		GSXMLUtils::WriteTabs(groupsElement, 2);
		groupsElement->appendChild(groupGuidElement);
		groupGuidElement->appendChild(document->createTextNode(UNISTR_TO_XMLCH_PTR(GS::UniString())));

		//Graphic Override
		//xercesc::DOMElement* groupOverrideElement = document->createElement(UNISTR_TO_XMLCH_PTR(XMLTag_Name));
		//GSXMLUtils::WriteNewLine(groupsElement);
		//GSXMLUtils::WriteTabs(groupsElement, 2);
		//groupsElement->appendChild(groupOverrideElement);
		//groupOverrideElement->appendChild(document->createTextNode(UNISTR_TO_XMLCH_PTR(GS::UniString("GraphicOverrideIndex"))));

		//xercesc::DOMElement* overrideIndexElement = document->createElement(UNISTR_TO_XMLCH_PTR(XMLTag_Val));
		//GSXMLUtils::WriteNewLine(groupsElement);
		//GSXMLUtils::WriteTabs(groupsElement, 2);
		//groupsElement->appendChild(overrideIndexElement);
		//std::string OverrideIndex = std::to_string(tempData.GraphicOverrideIndex);
		//overrideIndexElement->appendChild(document->createTextNode(UNISTR_TO_XMLCH_PTR(GS::UniString(OverrideIndex.c_str()))));

		GSXMLUtils::WriteNewLine(groupsElement);
		GSXMLUtils::WriteTabs(groupsElement, 1);

		GSXMLUtils::WriteNewLine(rootTag);

	}
	catch (...) {
		err = Error;
	}

	if (err != NoError) {
		document->release();
		document = nullptr;
	}

	return err;
}

static GSErrCode ExportAllBuiltInPropertiesToXMLFile()
{
	GSErrCode err = NoError;
	xercesc::DOMDocument* document = nullptr;

	err = WriteTemplateDataToXMLDocument(document);
	if (err != NoError)
		return err;

	DG::FileDialog dialog(DG::FileDialog::Save);

	dialog.SetTitle("Select destination file");

	FTM::FileTypeManager manager("XML files");
	FTM::FileType type(nullptr, "xml", 0, 0, 0);
	FTM::TypeID id = FTM::FileTypeManager::SearchForType(type);

	if (id == FTM::UnknownType) {
		id = manager.AddType(type);
	}

	dialog.AddFilter(id);

	IO::Location location;
	if (dialog.Invoke()) {
		location = dialog.GetSelectedFile();
	}
	else {
		return Cancel;
	}

	IO::File xmlFile(location, IO::File::Create);
	err = xmlFile.Open(IO::File::WriteEmptyMode, IO::AccessDeniedIsError);

	if (err != NoError) {
		document->release();
		return Error;
	}
	err = FormatXMLDoc(document, &xmlFile) ? NoError : Error;

	document->release();
	xmlFile.Close();

	return NoError;
}

enum DialogResourceIds
{
	MazeDialogResourceId = ID_ADDON_DLG,
	OKButtonId = 1,
	CancelButtonId = 2,
	DrawingId = 3,
	TemplateSettingsTextId = 4,
	scaleTextId = 5,
	scaleEditId = 6,
	layerTextId = 7,
	layerEditId = 8,
	separator2Id = 9,
	PopupId = 10
};

static GS::UniString AddOnClientOnlyObject("ClientOnlyObject");

TemplateDialog::TemplateDialog(const Controls& controls):
	DG::ModalDialog(ACAPI_GetOwnResModule(), MazeDialogResourceId, ACAPI_GetOwnResModule()),
	okButton(GetReference(), OKButtonId),
	cancelButton(GetReference(), CancelButtonId),
	scaleEdit(GetReference(), scaleEditId),
	layerEdit(GetReference(), layerEditId),
	PopupEdit(GetReference(), PopupId),
	controls(controls)
{
	AttachToAllItems(*this);
	Attach(*this);
	FillDatabasePopup();
}

TemplateDialog::~TemplateDialog()
{
	Detach(*this);
	DetachFromAllItems(*this);
}

const Controls& TemplateDialog::GetControls() const
{
	return controls;
}

void TemplateDialog::PanelOpened(const DG::PanelOpenEvent&)
{
	scaleEdit.SetValue(controls.scale);
	layerEdit.SetValue(controls.layer);
}

void TemplateDialog::PanelCloseRequested(const DG:: PanelCloseRequestEvent & ev, bool*)
{
	if (ev.IsAccepted())
	{
		controls.scale = scaleEdit.GetValue();
		controls.layer = layerEdit.GetValue();
		controls.item = PopupEdit.GetSelectedItem();
	}
}

void TemplateDialog::ButtonClicked(const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource() == &okButton)
	{
		ExportAllBuiltInPropertiesToXMLFile();
		PostCloseRequest(DG::ModalDialog::Accept);
	}
	else if (ev.GetSource() == &cancelButton)
	{
		PostCloseRequest(DG::ModalDialog::Cancel);
	}
}


void TemplateDialog::FillDatabasePopup()
{
	//ListLayers();
	PopupEdit.DeleteItem(DG::PopUp::AllItems);
	for (unsigned short index = 0; index < 10; index++) {
		PopupEdit.AppendItem();

		GS::UniString bufUstr = "Item " + index;

		PopupEdit.SetItemText(DG::PopUp::BottomItem, bufUstr);
		PopupEdit.SetItemValue(DG::PopUp::BottomItem, index);
	}
	//PopupEdit.SelectItem(1);
}
