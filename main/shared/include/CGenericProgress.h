// CGenericProgress.h


//#include "CModalProgress.h"



//const	ResIDT	Progress_Dialog_Resource_ID = 138;



class CGenericProgress {
public:
	~CGenericProgress();
	void Initialize(long totalSteps);
	bool UpdateProgress(long thisStep);		// returns whether search was cancelled

protected:
//	CModalProgress	dlg;
	
};
