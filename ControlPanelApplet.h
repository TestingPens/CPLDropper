#include "resource.h"		// main symbols
#include "cpl.h"

namespace ControlPanelApplet
{	
	class CControlPanelApplet : public CWinApp
	{
        public:
		 CControlPanelApplet();
		 static LONG APIENTRY CPlApplet(HWND hWnd, UINT uMsg, LONG lParam1, LONG lParam2);

		private:
		 static CControlPanelApplet * m_pThis;
		 LONG OnDblclk(HWND hWnd, UINT uAppNum, LONG lData);
		 LONG OnExit();
		 LONG OnGetCount();
		 LONG OnInit();
		 LONG OnInquire(UINT uAppNum, CPLINFO* pInfo);
		 LONG OnNewInquire(UINT uAppNum, NEWCPLINFO* pInfo);
		 LONG OnStop(UINT uAppNum, LONG lData);
	};
}