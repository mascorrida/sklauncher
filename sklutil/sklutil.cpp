#include	<windows.h>
#include	<windowsx.h>
#include	<shlobj.h>
#include	<stdio.h>
#include	<tchar.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// define
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DllExport extern "C" __declspec(dllexport)

// フックフラグ
#define HOOK_NONE				(0)
#define HOOK_DESKTOP			(1)
#define HOOK_RDBLCLICK			(2)
#define HOOK_MDBLCLICK			(4)
#define HOOK_ROUNDMOUSE			(8)

#define DEFAULT_ROUNDNUM		(2)
#define DEFAULT_ROUNDTIMEOUT	(1500)
#define DEFAULT_BCLOCKWISE		(1)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 共有変数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma data_seg(".sdata")
HINSTANCE hDllInst		= NULL;			// DLLインスタンス

HHOOK hHookDesktop		= NULL;			// フックアドレス
HHOOK hHookRDBLClick	= NULL;
HHOOK hHookMDBLClick	= NULL;
HHOOK hHookRoundMouse	= NULL;

HWND hMainWnd			= NULL;			// SKランチャのハンドル(FrmMain.hWnd)
ATOM hotKeyID			= NULL;			// SKランチャのホットキーID
int hookFlag			= HOOK_NONE;	// どのフックをしているか

// デスクトップ
HWND hSysViewWnd		= NULL;			// デスクトップのハンドル

// 右ダブルクリック
DWORD RDownTime			= 0;			// 右ボタンを押したときの時間
UINT RDownTimeOut		= 1000;			// ダブルクリック感知の時間
bool bRDBL				= false;		// マウスアップの判定

// ホイールダブルクリック
DWORD MDownTime			= 0;			// ホイールボタンを押したときの時間
UINT MDownTimeOut		= 1000;			// ダブルクリック感知の時間
bool bMDBL				= false;		// マウスアップの判定

// くるくるマウス
UINT roundNum			= DEFAULT_ROUNDNUM;		// 何回まわすか
UINT roundTimeOut		= DEFAULT_ROUNDTIMEOUT;	// くるくるマウスのタイムアウト
bool bClockwise			= DEFAULT_BCLOCKWISE;	// 時計回りかどうか
int roundSttDir			= -1;					// マウスの移動開始方向(L, UL, U, UR, R, DR, D, DL)
int roundNowDir			= -1;					// マウスの現在の移動方向(L, UL, U, UR, R, DR, D, DL)
UINT roundNowNum		= 0;					// 何回まわしたか
int mouseX				= 0;					// マウス座標
int mouseY				= 0;
DWORD roundTime			= 0;					// まわし始めた時間

#pragma data_seg()


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// プロトタイプ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ホットキーを送る
void SendHotKey();
// iniファイルからUINT取得
UINT GetIniUINT(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault);
// iniファイルからbool取得
bool GetIniBool(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault);
// exeファイルのあるディレクトリパスの取得
bool GetIniPath(char *iniPath);
// VBのDoEventsに相当
void DoEvents();

// デスクトップダブルクリック
DllExport int HookDesktop(HWND hWnd);
DllExport int UnHookDesktop(VOID);
LRESULT CALLBACK ProcDesktop(int nCode, WPARAM wParam, LPARAM lParam);
HWND FindSysListViewWnd(VOID);	// デスクトップのハンドル取得
// 右ダブルクリック
DllExport int HookRDBLClick(HWND hWnd);
DllExport int UnHookRDBLClick(VOID);
LRESULT CALLBACK ProcRDBLClick(int nCode, WPARAM wParam, LPARAM lParam);
//bool isTaskChild(HWND hWnd);	// タスクバーの子ウインドウかどうか
// 中央ダブルクリック
DllExport int HookMDBLClick(HWND hWnd);
DllExport int UnHookMDBLClick(VOID);
LRESULT CALLBACK ProcMDBLClick(int nCode, WPARAM wParam, LPARAM lParam);
// くるくるマウス
DllExport int HookRoundMouse(HWND hWnd);
DllExport int UnHookRoundMouse(VOID);
LRESULT CALLBACK ProcRoundMouse(int nCode, WPARAM wParam, LPARAM lParam);
void RoundReset();				// マウス移動フラグを全てオフ
bool EvalRound(int dir);		// くるくるマウスの判定


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// メイン
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
	hDllInst = hInstance;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ホットキーを送る
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendHotKey()
{
	SendMessage(hMainWnd, WM_HOTKEY, (short)hotKeyID, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルからUINT取得
//
UINT GetIniUINT(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault)
{
	UINT i;
	i = GetPrivateProfileInt(SectionName, KeyName, nDefault, IniPath);
    return(i);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルからbool取得
//
bool GetIniBool(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault)
{
	int i;
	i = GetIniUINT(IniPath, SectionName, KeyName, 10);
    if(i==10)	return (nDefault!=0);
	else		return (i!=0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// exeファイルのあるディレクトリパスの取得
//
bool GetIniPath(char *iniPath)
{
	DWORD ret;
	TCHAR path[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];

	// 実行モジュールのフルパスの取得
	ret = GetModuleFileName(NULL, path, sizeof(path));
	if(ret){
		// フルパスを分解
		_tsplitpath(path, drive, dir, NULL, NULL);

		// szPathに実行モジュールのパスを作成
		_tcscpy(path, drive);
		_tcscat(path, dir);
		_tcscat(path, "\\skl.ini");

		// iniPathに設定
		_tcscpy(iniPath, path);
		return true;
	}
	else return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック開始(デスクトップダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int HookDesktop(HWND hWnd)
{
	hHookDesktop = NULL;
	hSysViewWnd = NULL;
	if(hookFlag == HOOK_NONE){
		hMainWnd = hWnd;
		hotKeyID = GlobalFindAtom("SKLHOTKEY");
	}

	// デスクトップハンドルを取得
	hSysViewWnd = FindSysListViewWnd();
	if(hSysViewWnd == NULL) return 0;
	DWORD dwThread = GetWindowThreadProcessId( hSysViewWnd, NULL );
	// マウスフックをかける
	hHookDesktop = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcDesktop, hDllInst, dwThread );

	hookFlag |= HOOK_DESKTOP;

	if(hHookDesktop != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック終了(デスクトップダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookDesktop(VOID)
{
	int ret;

	// フック止め
	ret = UnhookWindowsHookEx(hHookDesktop);
	hHookDesktop = NULL;
	hSysViewWnd = NULL;
	if(hookFlag & HOOK_DESKTOP) hookFlag ^= HOOK_DESKTOP;

	if(hookFlag == HOOK_NONE){
		hMainWnd = NULL;
		hotKeyID = NULL;
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 盗ってきたメッセージのプロシージャ(デスクトップダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcDesktop(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookDesktop, nCode, wParam, lParam);

	// 左ボタンダブルクリック
	if(wParam == WM_LBUTTONDBLCLK){
		// アイコンを選択していない状態なら（何もないところでダブルクリック）
		if( ListView_GetSelectedCount(hSysViewWnd) == 0 ){
			// ホットキー押したよと送る
			SendHotKey();
			return 1;
		}
	}

	return CallNextHookEx(hHookDesktop, nCode, wParam, lParam);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// デスクトップのハンドル取得
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND FindSysListViewWnd(VOID)
{
	HWND hWnd;

	hWnd = FindWindow("Progman", "Program Manager");
	hWnd = FindWindowEx(hWnd, NULL, "SHELLDLL_DefView", NULL);
	hWnd = FindWindowEx(hWnd, NULL, "SysListView32", NULL);
	return hWnd;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック開始(右ダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int HookRDBLClick(HWND hWnd)
{
	hHookRDBLClick = NULL;
	if(hookFlag == HOOK_NONE){
		hMainWnd = hWnd;
		hotKeyID = GlobalFindAtom("SKLHOTKEY");
	}

	RDownTime = 0;
	RDownTimeOut = GetDoubleClickTime();
	bRDBL = false;

	// マウスフックをかける
	hHookRDBLClick = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcRDBLClick, hDllInst, NULL );

	hookFlag |= HOOK_RDBLCLICK;

	if(hHookRDBLClick != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック終了(右ダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookRDBLClick(VOID)
{
	int ret;

	// フック止め
	ret = UnhookWindowsHookEx(hHookRDBLClick);

	hHookRDBLClick = NULL;
	if(hookFlag & HOOK_RDBLCLICK) hookFlag ^= HOOK_RDBLCLICK;

	if(hookFlag == HOOK_NONE){
		hMainWnd = NULL;
		hotKeyID = NULL;
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 盗ってきたメッセージのプロシージャ(右ダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcRDBLClick(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookRDBLClick, nCode, wParam, lParam);

	switch(wParam){
	// 右ダブルクリック
	case WM_RBUTTONDBLCLK:
//	case WM_NCRBUTTONDBLCLK:
		bRDBL = false;
		RDownTime = 0;
/*
		MOUSEHOOKSTRUCT *mhs;
		mhs = (MOUSEHOOKSTRUCT *)lParam;
		if(isTaskChild(mhs->hwnd)) break;	// タスクバー上でなければ
//*/
		// ホットキー押したよと送る
		SendHotKey();
		return 1;
		break;

	// 右ボタンダウン
	case WM_RBUTTONDOWN:
//	case WM_NCRBUTTONDOWN:
		DWORD dwTime;
		dwTime = timeGetTime();
		RDownTime = dwTime;
		bRDBL = false;

		if(bRDBL){
			DWORD downItv = dwTime - RDownTime;
			// ダブルクリック感知時間内なら
			if(downItv < (DWORD)RDownTimeOut){
/*
				MOUSEHOOKSTRUCT *mhs;
				mhs = (MOUSEHOOKSTRUCT *)lParam;
				if(isTaskChild(mhs->hwnd)) break;
//*/
				// ホットキー押したよと送る
				SendHotKey();
				return 1;
			}
		}
		break;

	case WM_RBUTTONUP:
//	case WM_NCRBUTTONUP:
		bRDBL = true;
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
		RDownTime = 0;
		bRDBL = false;
		break;
	}

	return CallNextHookEx(hHookRDBLClick, nCode, wParam, lParam);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック開始(ホイールダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int HookMDBLClick(HWND hWnd)
{
	hHookMDBLClick = NULL;
	if(hookFlag == HOOK_NONE){
		hMainWnd = hWnd;
		hotKeyID = GlobalFindAtom("SKLHOTKEY");
	}

	RDownTime = 0;
	RDownTimeOut = GetDoubleClickTime();
	bRDBL = false;

	// マウスフックをかける
	hHookMDBLClick = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcMDBLClick, hDllInst, NULL );

	hookFlag |= HOOK_MDBLCLICK;

	if(hHookMDBLClick != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック終了(ホイールダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookMDBLClick(VOID)
{
	int ret;

	// フック止め
	ret = UnhookWindowsHookEx(hHookMDBLClick);

	hHookMDBLClick = NULL;
	if(hookFlag & HOOK_MDBLCLICK) hookFlag ^= HOOK_MDBLCLICK;

	if(hookFlag == HOOK_NONE){
		hMainWnd = NULL;
		hotKeyID = NULL;
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 盗ってきたメッセージのプロシージャ(ホイールダブルクリック)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcMDBLClick(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookMDBLClick, nCode, wParam, lParam);

	switch(wParam){
	// ホイールダブルクリック
	case WM_MBUTTONDBLCLK:
	case WM_NCMBUTTONDBLCLK:
		bMDBL = false;
		MDownTime = 0;
/*
		MOUSEHOOKSTRUCT *mhs;
		mhs = (MOUSEHOOKSTRUCT *)lParam;
		if(isTaskChild(mhs->hwnd)) break;	// タスクバー上でなければ
//*/
		// ホットキー押したよと送る
		SendHotKey();
		return 1;
		break;

	// ホイールボタンダウン
	case WM_MBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
		DWORD dwTime;
		dwTime = timeGetTime();
		MDownTime = dwTime;
		bMDBL = false;

		if(bMDBL){
			DWORD downItv = dwTime - MDownTime;
			// ダブルクリック感知時間内なら
			if(downItv < (DWORD)MDownTimeOut){
/*
				MOUSEHOOKSTRUCT *mhs;
				mhs = (MOUSEHOOKSTRUCT *)lParam;
				if(isTaskChild(mhs->hwnd)) break;
//*/
				// ホットキー押したよと送る
				SendHotKey();
				return 1;
			}
		}
		break;

	case WM_MBUTTONUP:
	case WM_NCMBUTTONUP:
		bMDBL = true;
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
		MDownTime = 0;
		bMDBL = false;
		break;
	}

	return CallNextHookEx(hHookMDBLClick, nCode, wParam, lParam);
}

/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// タスクバーの子ウインドウかどうか
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isTaskChild(HWND hWnd)
{
	HWND hTaskWnd;
	bool ret = false;
	HWND hParent = hWnd;

    hTaskWnd = FindWindow("Shell_TrayWnd", NULL);
	if(hTaskWnd == NULL) return false;

	// 5階層下まで見る
	for(int i=0; i<4 && hParent!=NULL; i++){
		hParent = GetParent(hParent);

		if(hParent == hTaskWnd){
//			MessageBox(NULL, "done!", "", MB_OK);
			ret = true;
			break;
		}
	}

	return ret;
}
//*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック開始(くるくるマウス)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int HookRoundMouse(HWND hWnd)
{
	hHookRoundMouse = NULL;
	if(hookFlag == HOOK_NONE){
		hMainWnd = hWnd;
		hotKeyID = GlobalFindAtom("SKLHOTKEY");
	}

	TCHAR iniPath[_MAX_PATH];

	if(GetIniPath(iniPath)){
		roundNum =		GetIniUINT(iniPath, "DLL GENERAL", "RoundNum", DEFAULT_ROUNDNUM);
		roundTimeOut =	GetIniUINT(iniPath, "DLL GENERAL", "RoundTimeOut", DEFAULT_ROUNDTIMEOUT);
		bClockwise =	GetIniBool(iniPath, "DLL GENERAL", "bClockwise", DEFAULT_BCLOCKWISE);

	}else{
		roundNum =		DEFAULT_ROUNDNUM;
		roundTimeOut =	DEFAULT_ROUNDTIMEOUT;
		bClockwise =	(DEFAULT_BCLOCKWISE != 0);
	}

	mouseX = 0;
	mouseY = 0;
	roundTime = 0;
	RoundReset();

	// マウスフックをかける
	hHookRoundMouse = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcRoundMouse, hDllInst, NULL );

	hookFlag |= HOOK_ROUNDMOUSE;

	if(hHookRoundMouse != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// フック終了(くるくるマウス)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookRoundMouse(VOID)
{
	int ret;

	// フック止め
	ret = UnhookWindowsHookEx(hHookRoundMouse);

	hHookRoundMouse = NULL;
	if(hookFlag & HOOK_ROUNDMOUSE) hookFlag ^= HOOK_ROUNDMOUSE;

	if(hookFlag == HOOK_NONE){
		hMainWnd = NULL;
		hotKeyID = NULL;
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 盗ってきたメッセージのプロシージャ(くるくるマウス)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcRoundMouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookRoundMouse, nCode, wParam, lParam);

	switch(wParam){

	// マウス移動
	case WM_MOVE:
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
		MOUSEHOOKSTRUCT *mhs;
		int dir;
		dir = -1;
		mhs = (MOUSEHOOKSTRUCT *)lParam;

		// 横方向
		if(mhs->pt.x - mouseX < 0)		dir = 0;
		else if(mhs->pt.x - mouseX > 0)	dir = 4;

		// 縦方向
		if(mhs->pt.y - mouseY < 0){
			if(dir == -1)		dir = 2;
			else if(dir == 0)	dir = 1;
			else				dir = 3;

		}else if(mhs->pt.y - mouseY > 0){
			if(dir == -1)		dir = 6;
			else if(dir == 0)	dir = 7;
			else				dir = 5;
		}

		// 座標更新
		mouseX = mhs->pt.x;
		mouseY = mhs->pt.y;

		// 判定
		if(EvalRound(dir)){
			// ホットキー押したよと送る
			SendHotKey();
			return 1;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		RoundReset();
		break;
	}

	return CallNextHookEx(hHookRoundMouse, nCode, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// マウス移動フラグを全てオフ(くるくるマウス)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RoundReset()
{
	roundSttDir = -1;
	roundNowDir = -1;
	roundNowNum = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// くるくるマウスの判定
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EvalRound(int dir)
{
	// 動いてないなら何もしない
	if(dir == -1){
		return false;
	}
	// スタートなら初期設定
	if(roundSttDir == -1){
		roundSttDir = dir;
		roundNowDir = dir;
		roundTime = timeGetTime();
		return false;
	}

	// 移動方向の変化を取得
	int move = dir-roundNowDir;
	// -3 ～ 4 に補正
	while(move < -3 || move > 4){
		if(move < -3)	move += 8;
		else			move -= 8;
	}

	if(move == 0){
		// 方向が変わってないなら何もしない
		return false;

	}else if( bClockwise && (move==1 || (move==2 && roundNowDir%2==1)) ){
		// 時計回りの正しい方向

		// １回転しているなら
		if( (roundSttDir > roundNowDir &&		roundSttDir <= roundNowDir+move) ||
			(roundSttDir+8 > roundNowDir &&	roundSttDir+8 <= roundNowDir+move) ){
			// タイムアウトなら出る
			if(timeGetTime() > roundTime + roundTimeOut){
				RoundReset();
				return false;
			}

			roundNowNum++;
			// 指定数回転で成功
			if(roundNowNum == roundNum){
				RoundReset();
				return true;
			}
		}
		roundNowDir = dir;

	}else if( !bClockwise && (move==-1 || (move==-2 && roundNowDir%2==1)) ){
		// 反時計回りの正しい方向

		// １回転しているなら
		if( (roundSttDir >= roundNowDir+move &&		roundSttDir < roundNowDir) ||
			(roundSttDir-8 >= roundNowDir+move &&	roundSttDir-8 < roundNowDir) ){
			// タイムアウトなら出る
			if(timeGetTime() > roundTime + roundTimeOut){
				RoundReset();
				return false;
			}

			roundNowNum++;
			// 指定数回転で成功
			if(roundNowNum == roundNum){
				RoundReset();
				return true;
			}
		}
		roundNowDir = dir;

	}else{
		// 間違った方向ならリセット
		RoundReset();
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VBのDoEventsに相当
//
void DoEvents()
{
	MSG    msg;
	if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
