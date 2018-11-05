
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 定数
//
#define OPTION_ACTION	(100)
#define OPTION_ANIME	(101)
#define OPTION_MESSAGE	(102)
#define OPTION_INPUT	(103)
#define OPTION_SE		(104)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// パブリック変数
//


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロトタイプ宣言
//

// プロシージャ
LRESULT CALLBACK DlgProcAction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProcAnime(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProcMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProcInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgProcSE(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// オプションの初期化
bool InitOption(HWND hWnd, int id);
// デフォルトに戻す
bool SetDefaultOption(HWND hWnd, int id);
// ダイアログ作成
void CreateMyProp(HWND hWnd);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロシージャ（動作）
//
LRESULT CALLBACK DlgProcAction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
		// 作成時
        case WM_INITDIALOG:
			InitOption(hWnd, OPTION_ACTION);
			break;

		// コントロール
        case WM_COMMAND:
			switch (LOWORD(wParam)) {
			// 初期状態に戻す
			case IDC_DEFALUT_:
				SetDefaultOption(hWnd, OPTION_ACTION);
				break;

			default:
				return FALSE;
			}
			break;

		// 外からメッセージ
		case WM_NOTIFY:
			nmhdr = (NMHDR *)lp;
			switch(nmhdr->code){
			// OKボタン
			case PSN_APPLY:
				break;

			default:
				return FALSE;
			}
			break;

        default:
            return FALSE;
    }
    return TRUE;
}

// プロシージャ（アニメーション）
LRESULT CALLBACK DlgProcAnime(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
		// 作成時
        case WM_INITDIALOG:
			InitOption(hWnd, OPTION_ANIME);
			break;

		// コントロール
        case WM_COMMAND:
			switch (LOWORD(wParam)) {
			// 初期状態に戻す
			case IDC_DEFALUT_:
				SetDefaultOption(hWnd, OPTION_ANIME);
				break;

			default:
				return FALSE;
			}
			break;

		// 外からメッセージ
		case WM_NOTIFY:
			nmhdr = (NMHDR *)lp;
			switch(nmhdr->code){
			// OKボタン
			case PSN_APPLY:
				break;

			default:
				return FALSE;
			}
			break;

        default:
            return FALSE;
    }
    return TRUE;
}

// プロシージャ（メッセージ）
LRESULT CALLBACK DlgProcMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
		// 作成時
        case WM_INITDIALOG:
			InitOption(hWnd, OPTION_MESSAGE);
			break;

		// コントロール
        case WM_COMMAND:
			switch (LOWORD(wParam)) {
			// 初期状態に戻す
			case IDC_DEFALUT_:
				SetDefaultOption(hWnd, OPTION_MESSAGE);
				break;

			default:
				return FALSE;
			}
			break;

		// 外からメッセージ
		case WM_NOTIFY:
			nmhdr = (NMHDR *)lp;
			switch(nmhdr->code){
			// OKボタン
			case PSN_APPLY:
				break;

			default:
				return FALSE;
			}
			break;

        default:
            return FALSE;
    }
    return TRUE;
}

// プロシージャ（入力関連）
LRESULT CALLBACK DlgProcInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
		// 作成時
        case WM_INITDIALOG:
			InitOption(hWnd, OPTION_INPUT);
			break;

		// コントロール
        case WM_COMMAND:
			switch (LOWORD(wParam)) {
			// 初期状態に戻す
			case IDC_DEFALUT_:
				SetDefaultOption(hWnd, OPTION_INPUT);
				break;

			default:
				return FALSE;
			}
			break;

		// 外からメッセージ
		case WM_NOTIFY:
			nmhdr = (NMHDR *)lp;
			switch(nmhdr->code){
			// OKボタン
			case PSN_APPLY:
				break;

			default:
				return FALSE;
			}
			break;

        default:
            return FALSE;
    }
    return TRUE;
}

// プロシージャ（SE）
LRESULT CALLBACK DlgProcSE(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
		// 作成時
        case WM_INITDIALOG:
			InitOption(hWnd, OPTION_SE);
			break;

		// コントロール
        case WM_COMMAND:
			switch (LOWORD(wParam)) {
			// 初期状態に戻す
			case IDC_DEFALUT_:
				SetDefaultOption(hWnd, OPTION_SE);
				break;

			default:
				return FALSE;
			}
			break;

		// 外からメッセージ
		case WM_NOTIFY:
			nmhdr = (NMHDR *)lp;
			switch(nmhdr->code){
			// OKボタン
			case PSN_APPLY:
				break;

			default:
				return FALSE;
			}
			break;

        default:
            return FALSE;
    }
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ダイアログ作成
//
void CreateMyProp(HWND hWnd)
{
	HINSTANCE hInst;
	PROPSHEETPAGE psp;
	PROPSHEETHEADER psh;
	HPROPSHEETPAGE hpsp[5];

	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	psp.dwSize = sizeof(PROPSHEETPAGE);
	psp.dwFlags = PSP_DEFAULT;
	psp.hInstance = hInst;

	psp.pszTemplate = "IDD_OPTION_ACTION";
	psp.pfnDlgProc = (DLGPROC)DlgProcAction;
	hpsp[0] = CreatePropertySheetPage(&psp);

	psp.pszTemplate = "IDD_OPTION_ANIME";
	psp.pfnDlgProc = (DLGPROC)DlgProcAnime;
	hpsp[1] = CreatePropertySheetPage(&psp);

	psp.pszTemplate = "IDD_OPTION_INPUT";
	psp.pfnDlgProc = (DLGPROC)DlgProcInput;
	hpsp[2] = CreatePropertySheetPage(&psp);

	psp.pszTemplate = "IDD_OPTION_MESSAGE";
	psp.pfnDlgProc = (DLGPROC)DlgProcMessage;
	hpsp[3] = CreatePropertySheetPage(&psp);

	psp.pszTemplate = "IDD_OPTION_SE";
	psp.pfnDlgProc = (DLGPROC)DlgProcSE;
	hpsp[4] = CreatePropertySheetPage(&psp);

	memset(&psh, 0, sizeof(PROPSHEETHEADER));
	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_NOAPPLYNOW;
	psh.hInstance = hInst;
	psh.hwndParent = hWnd;
	psh.nPages = 5;
	psh.phpage = hpsp;
	psh.pszCaption = "オプション";
	(HWND)PropertySheet(&psh);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// オプションの初期化
//
bool InitOption(HWND hWnd, int id)
{
	int roundNum, roundTimeOut;
	bool bClockwise;

	switch(id){
	case OPTION_ACTION:
		// くるくるマウスの変数取得
		roundNum = GetIniBool(iniPath, "GENERAL", "RoundNum", DEFAULT_RoundNum);
		roundTimeOut = GetIniBool(iniPath, "GENERAL", "RoundTimeOut", DEFAULT_RoundTimeOut);
		bClockwise = GetIniBool(iniPath, "GENERAL", "bClockwise", DEFAULT_bClockwise);

		// 変数をコントロールにセット
		SendMessage(GetDlgItem(hDlgWnd, IDC_DEFAULTPOS), CB_SETCURSEL, (WPARAM)defaultPos, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_HOTKEYACTION), CB_SETCURSEL, (WPARAM)hotKeyAction, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_CENTERPOS), CB_SETCURSEL, (WPARAM)centerPos, 0L);
		if(centerPos==1){
			SetWindowText(GetDlgItem(hDlgWnd, IDC_CENTER_X), (LPCTSTR)CStr(center.x));
			SetWindowText(GetDlgItem(hDlgWnd, IDC_CENTER_X), (LPCTSTR)CStr(center.y));
		}else{
			SetWindowText(GetDlgItem(hDlgWnd, IDC_CENTER_X), (LPCTSTR)"0");
			SetWindowText(GetDlgItem(hDlgWnd, IDC_CENTER_X), (LPCTSTR)"0");
		}
		SendMessage(GetDlgItem(hDlgWnd, IDC_MOUSEL), CB_SETCURSEL, (WPARAM)mouseL, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_MOUSEM), CB_SETCURSEL, (WPARAM)mouseM, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_MOUSER), CB_SETCURSEL, (WPARAM)mouseR, 0L);

		if(boolInvRote)	SendMessage(GetDlgItem(hDlgWnd, IDC_INVROTE), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else			SendMessage(GetDlgItem(hDlgWnd, IDC_INVROTE), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		if(boolDeskTopClick)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLDESKTOPCLICK), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else					SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLDESKTOPCLICK), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		if(boolRoundMouse)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLROUNDMOUSE), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else				SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLROUNDMOUSE), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);

		// 1回～5回
		for(i=0; i<5; i++)
			SendMessage(GetDlgItem(hDlgWnd, IDC_ROUNDMOUSE_VALUE), LB_INSERTSTRING, (WPARAM)i, (LPCTSTR)CStr(i+1));
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROUNDMOUSE_VALUE), LB_SETCURSEL, (WPARAM)roundNum-1, 0L);
		// 500～2500ms　100単位
		for(i=0; i<21; i++)
			SendMessage(GetDlgItem(hDlgWnd, IDC_ROUNDMOUSE_TIMEOUT), LB_INSERTSTRING, (WPARAM)i, (LPCTSTR)CStr(i*100+500));
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROUNDMOUSE_TIMEOUT), LB_SETCURSEL, (WPARAM)((int)(roundTimeOut/100)-5), 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROUNDMOUSE_DIRECTION), CB_SETCURSEL, (WPARAM)bClockwise, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_INTDBLCLICK), CB_SETCURSEL, (WPARAM)intDBLClick, 0L);
		break;

	case OPTION_ANIME:
		// トラックバーの範囲指定
		SendMessage(GetDlgItem(hDlgWnd, IDC_MAXR), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_maxR, MAX_maxR));
		SendMessage(GetDlgItem(hDlgWnd, IDC_MINR), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_minR, MAX_minR));
		SendMessage(GetDlgItem(hDlgWnd, IDC_ICONITV), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_iconItv, MAX_iconItv));
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROTERAD), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_roteRad, MAX_roteRad));
		SendMessage(GetDlgItem(hDlgWnd, IDC_SMOOTH), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_smooth, MAX_smooth));
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROTETIME), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_roteTime, MAX_roteTime));
		SendMessage(GetDlgItem(hDlgWnd, IDC_DARKVALUE), TBM_SETRANGE, 0L, (LPARAM)MAKELONG(MIN_darkValue, MAX_darkValue));

		if(autoR)	SendMessage(GetDlgItem(hDlgWnd, IDC_AUTOR), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else		SendMessage(GetDlgItem(hDlgWnd, IDC_AUTOR), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_MAXR), TBM_SETPOS, (WPARAM)maxR, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_MINR), TBM_SETPOS, (WPARAM)minR, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_ICONITV), TBM_SETPOS, (WPARAM)iconItv, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROTERAD), TBM_SETPOS, (WPARAM)roteRad, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_SMOOTH), TBM_SETPOS, (WPARAM)smooth, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_ROTETIME), TBM_SETPOS, (WPARAM)roteTime, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_DARKTYPE), CB_SETCURSEL, (WPARAM)darkType, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_DARKVALUE), TBM_SETPOS, (WPARAM)darkValue, 0L);
		break;
	case OPTION_INPUT:
		SendMessage(GetDlgItem(hDlgWnd, IDC_KEY_COMBO), CB_SETCURSEL, (WPARAM)0, 0L);
		break;
	case OPTION_MESSAGE:
		if(boolMsg)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLMSG), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else		SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLMSG), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_MSGPOS), CB_SETCURSEL, (WPARAM)msgPos, 0L);
		SendMessage(GetDlgItem(hDlgWnd, IDC_MSGALIGN), CB_SETCURSEL, (WPARAM)msgAlign, 0L);
		SetWindowText(GetDlgItem(hDlgWnd, IDC_MSGSTYLE), (LPCTSTR)msgStyle.c_str()); 
		break;
	case OPTION_SE:
		if(boolSEHor)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEHOR), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else			SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEHOR), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		if(boolSEUp)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEUP), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else			SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEUP), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		if(boolSEDown)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEDOWN), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else			SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEDOWN), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		if(boolSEExe)	SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEEXE), BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
		else			SendMessage(GetDlgItem(hDlgWnd, IDC_BOOLSEEXE), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0L);
		SetWindowText(GetDlgItem(hDlgWnd, IDC_SEHOR), (LPCTSTR)SEHor.c_str());
		SetWindowText(GetDlgItem(hDlgWnd, IDC_SEUP), (LPCTSTR)SEUp.c_str());
		SetWindowText(GetDlgItem(hDlgWnd, IDC_SEDOWN), (LPCTSTR)SEDown.c_str());
		SetWindowText(GetDlgItem(hDlgWnd, IDC_SEEXE), (LPCTSTR)SEExe.c_str());
		break;
	}
	return true;
}
