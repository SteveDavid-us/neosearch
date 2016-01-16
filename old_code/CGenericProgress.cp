// CGenericProgress.cp


#include "CGenericProgress.h"


CGenericProgress::~CGenericProgress()
{
	dlg.EndModal();
}


void CGenericProgress::Initialize(long totalSteps)
{
	if (dlg.SetupDialog(Progress_Dialog_Resource_ID) != noErr)
	{
		SysBeep(1);
		return;
	}

	dlg.SetProgressBar(4/*kTest1ProgressBarItem*/);

	dlg.SetCurrentState(100);
	dlg.SetStateSpace(totalSteps);
	dlg.BeginModal();

}


Boolean CGenericProgress::UpdateProgress(long thisStep)
{
	short progress = dlg.ProcessModal();
	if (progress != kDialogContinues) {
		dlg.EndModal();
		return TRUE;
	}

	dlg.SetCurrentStateValue(thisStep);
	return FALSE;
}
