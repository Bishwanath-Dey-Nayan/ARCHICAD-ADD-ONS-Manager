#ifndef TEMPLATEDIALOG_HPP
#define TEMPLATEDIALOG_HPP

#include "DGModule.hpp"
#include "Controls.hpp"

class TemplateDialog : public DG::ModalDialog,
	public DG::PanelObserver,
	public DG::ButtonItemObserver,
	public DG::CompoundItemObserver
{
public:
	TemplateDialog(const Controls& controls);
	~TemplateDialog();
	const Controls& GetControls() const;

private:
	virtual void	PanelOpened(const DG::PanelOpenEvent& ev) override;
	virtual	void	PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted) override;
	virtual void	ButtonClicked(const DG::ButtonClickEvent& ev) override;
	void FillDatabasePopup();

	DG::Button		okButton;
	DG::Button		cancelButton;
	DG::PosIntEdit	scaleEdit;
	DG::PosIntEdit	layerEdit;
	DG::PopUp	PopupEdit;

	Controls controls;
};


#endif // !TEMPLATEDIALOG_HPP
