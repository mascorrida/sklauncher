#include	<windows.h>
#include	<windowsx.h>
#include	<shlobj.h>
#include	<stdio.h>
#include	<tchar.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// define
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DllExport extern "C" __declspec(dllexport)

// �t�b�N�t���O
#define HOOK_NONE				(0)
#define HOOK_DESKTOP			(1)
#define HOOK_RDBLCLICK			(2)
#define HOOK_MDBLCLICK			(4)
#define HOOK_ROUNDMOUSE			(8)

#define DEFAULT_ROUNDNUM		(2)
#define DEFAULT_ROUNDTIMEOUT	(1500)
#define DEFAULT_BCLOCKWISE		(1)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���L�ϐ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma data_seg(".sdata")
HINSTANCE hDllInst		= NULL;			// DLL�C���X�^���X

HHOOK hHookDesktop		= NULL;			// �t�b�N�A�h���X
HHOOK hHookRDBLClick	= NULL;
HHOOK hHookMDBLClick	= NULL;
HHOOK hHookRoundMouse	= NULL;

HWND hMainWnd			= NULL;			// SK�����`���̃n���h��(FrmMain.hWnd)
ATOM hotKeyID			= NULL;			// SK�����`���̃z�b�g�L�[ID
int hookFlag			= HOOK_NONE;	// �ǂ̃t�b�N�����Ă��邩

// �f�X�N�g�b�v
HWND hSysViewWnd		= NULL;			// �f�X�N�g�b�v�̃n���h��

// �E�_�u���N���b�N
DWORD RDownTime			= 0;			// �E�{�^�����������Ƃ��̎���
UINT RDownTimeOut		= 1000;			// �_�u���N���b�N���m�̎���
bool bRDBL				= false;		// �}�E�X�A�b�v�̔���

// �z�C�[���_�u���N���b�N
DWORD MDownTime			= 0;			// �z�C�[���{�^�����������Ƃ��̎���
UINT MDownTimeOut		= 1000;			// �_�u���N���b�N���m�̎���
bool bMDBL				= false;		// �}�E�X�A�b�v�̔���

// ���邭��}�E�X
UINT roundNum			= DEFAULT_ROUNDNUM;		// ����܂킷��
UINT roundTimeOut		= DEFAULT_ROUNDTIMEOUT;	// ���邭��}�E�X�̃^�C���A�E�g
bool bClockwise			= DEFAULT_BCLOCKWISE;	// ���v��肩�ǂ���
int roundSttDir			= -1;					// �}�E�X�̈ړ��J�n����(L, UL, U, UR, R, DR, D, DL)
int roundNowDir			= -1;					// �}�E�X�̌��݂̈ړ�����(L, UL, U, UR, R, DR, D, DL)
UINT roundNowNum		= 0;					// ����܂킵����
int mouseX				= 0;					// �}�E�X���W
int mouseY				= 0;
DWORD roundTime			= 0;					// �܂킵�n�߂�����

#pragma data_seg()


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �v���g�^�C�v
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �z�b�g�L�[�𑗂�
void SendHotKey();
// ini�t�@�C������UINT�擾
UINT GetIniUINT(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault);
// ini�t�@�C������bool�擾
bool GetIniBool(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault);
// exe�t�@�C���̂���f�B���N�g���p�X�̎擾
bool GetIniPath(char *iniPath);
// VB��DoEvents�ɑ���
void DoEvents();

// �f�X�N�g�b�v�_�u���N���b�N
DllExport int HookDesktop(HWND hWnd);
DllExport int UnHookDesktop(VOID);
LRESULT CALLBACK ProcDesktop(int nCode, WPARAM wParam, LPARAM lParam);
HWND FindSysListViewWnd(VOID);	// �f�X�N�g�b�v�̃n���h���擾
// �E�_�u���N���b�N
DllExport int HookRDBLClick(HWND hWnd);
DllExport int UnHookRDBLClick(VOID);
LRESULT CALLBACK ProcRDBLClick(int nCode, WPARAM wParam, LPARAM lParam);
//bool isTaskChild(HWND hWnd);	// �^�X�N�o�[�̎q�E�C���h�E���ǂ���
// �����_�u���N���b�N
DllExport int HookMDBLClick(HWND hWnd);
DllExport int UnHookMDBLClick(VOID);
LRESULT CALLBACK ProcMDBLClick(int nCode, WPARAM wParam, LPARAM lParam);
// ���邭��}�E�X
DllExport int HookRoundMouse(HWND hWnd);
DllExport int UnHookRoundMouse(VOID);
LRESULT CALLBACK ProcRoundMouse(int nCode, WPARAM wParam, LPARAM lParam);
void RoundReset();				// �}�E�X�ړ��t���O��S�ăI�t
bool EvalRound(int dir);		// ���邭��}�E�X�̔���


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���C��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
	hDllInst = hInstance;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �z�b�g�L�[�𑗂�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendHotKey()
{
	SendMessage(hMainWnd, WM_HOTKEY, (short)hotKeyID, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ini�t�@�C������UINT�擾
//
UINT GetIniUINT(LPCTSTR IniPath, LPCTSTR SectionName, LPCTSTR KeyName, INT nDefault)
{
	UINT i;
	i = GetPrivateProfileInt(SectionName, KeyName, nDefault, IniPath);
    return(i);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ini�t�@�C������bool�擾
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
// exe�t�@�C���̂���f�B���N�g���p�X�̎擾
//
bool GetIniPath(char *iniPath)
{
	DWORD ret;
	TCHAR path[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];

	// ���s���W���[���̃t���p�X�̎擾
	ret = GetModuleFileName(NULL, path, sizeof(path));
	if(ret){
		// �t���p�X�𕪉�
		_tsplitpath(path, drive, dir, NULL, NULL);

		// szPath�Ɏ��s���W���[���̃p�X���쐬
		_tcscpy(path, drive);
		_tcscat(path, dir);
		_tcscat(path, "\\skl.ini");

		// iniPath�ɐݒ�
		_tcscpy(iniPath, path);
		return true;
	}
	else return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �t�b�N�J�n(�f�X�N�g�b�v�_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int HookDesktop(HWND hWnd)
{
	hHookDesktop = NULL;
	hSysViewWnd = NULL;
	if(hookFlag == HOOK_NONE){
		hMainWnd = hWnd;
		hotKeyID = GlobalFindAtom("SKLHOTKEY");
	}

	// �f�X�N�g�b�v�n���h�����擾
	hSysViewWnd = FindSysListViewWnd();
	if(hSysViewWnd == NULL) return 0;
	DWORD dwThread = GetWindowThreadProcessId( hSysViewWnd, NULL );
	// �}�E�X�t�b�N��������
	hHookDesktop = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcDesktop, hDllInst, dwThread );

	hookFlag |= HOOK_DESKTOP;

	if(hHookDesktop != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �t�b�N�I��(�f�X�N�g�b�v�_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookDesktop(VOID)
{
	int ret;

	// �t�b�N�~��
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
// �����Ă������b�Z�[�W�̃v���V�[�W��(�f�X�N�g�b�v�_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcDesktop(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookDesktop, nCode, wParam, lParam);

	// ���{�^���_�u���N���b�N
	if(wParam == WM_LBUTTONDBLCLK){
		// �A�C�R����I�����Ă��Ȃ���ԂȂ�i�����Ȃ��Ƃ���Ń_�u���N���b�N�j
		if( ListView_GetSelectedCount(hSysViewWnd) == 0 ){
			// �z�b�g�L�[��������Ƒ���
			SendHotKey();
			return 1;
		}
	}

	return CallNextHookEx(hHookDesktop, nCode, wParam, lParam);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �f�X�N�g�b�v�̃n���h���擾
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
// �t�b�N�J�n(�E�_�u���N���b�N)
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

	// �}�E�X�t�b�N��������
	hHookRDBLClick = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcRDBLClick, hDllInst, NULL );

	hookFlag |= HOOK_RDBLCLICK;

	if(hHookRDBLClick != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �t�b�N�I��(�E�_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookRDBLClick(VOID)
{
	int ret;

	// �t�b�N�~��
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
// �����Ă������b�Z�[�W�̃v���V�[�W��(�E�_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcRDBLClick(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookRDBLClick, nCode, wParam, lParam);

	switch(wParam){
	// �E�_�u���N���b�N
	case WM_RBUTTONDBLCLK:
//	case WM_NCRBUTTONDBLCLK:
		bRDBL = false;
		RDownTime = 0;
/*
		MOUSEHOOKSTRUCT *mhs;
		mhs = (MOUSEHOOKSTRUCT *)lParam;
		if(isTaskChild(mhs->hwnd)) break;	// �^�X�N�o�[��łȂ����
//*/
		// �z�b�g�L�[��������Ƒ���
		SendHotKey();
		return 1;
		break;

	// �E�{�^���_�E��
	case WM_RBUTTONDOWN:
//	case WM_NCRBUTTONDOWN:
		DWORD dwTime;
		dwTime = timeGetTime();
		RDownTime = dwTime;
		bRDBL = false;

		if(bRDBL){
			DWORD downItv = dwTime - RDownTime;
			// �_�u���N���b�N���m���ԓ��Ȃ�
			if(downItv < (DWORD)RDownTimeOut){
/*
				MOUSEHOOKSTRUCT *mhs;
				mhs = (MOUSEHOOKSTRUCT *)lParam;
				if(isTaskChild(mhs->hwnd)) break;
//*/
				// �z�b�g�L�[��������Ƒ���
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
// �t�b�N�J�n(�z�C�[���_�u���N���b�N)
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

	// �}�E�X�t�b�N��������
	hHookMDBLClick = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcMDBLClick, hDllInst, NULL );

	hookFlag |= HOOK_MDBLCLICK;

	if(hHookMDBLClick != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �t�b�N�I��(�z�C�[���_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookMDBLClick(VOID)
{
	int ret;

	// �t�b�N�~��
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
// �����Ă������b�Z�[�W�̃v���V�[�W��(�z�C�[���_�u���N���b�N)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcMDBLClick(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookMDBLClick, nCode, wParam, lParam);

	switch(wParam){
	// �z�C�[���_�u���N���b�N
	case WM_MBUTTONDBLCLK:
	case WM_NCMBUTTONDBLCLK:
		bMDBL = false;
		MDownTime = 0;
/*
		MOUSEHOOKSTRUCT *mhs;
		mhs = (MOUSEHOOKSTRUCT *)lParam;
		if(isTaskChild(mhs->hwnd)) break;	// �^�X�N�o�[��łȂ����
//*/
		// �z�b�g�L�[��������Ƒ���
		SendHotKey();
		return 1;
		break;

	// �z�C�[���{�^���_�E��
	case WM_MBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
		DWORD dwTime;
		dwTime = timeGetTime();
		MDownTime = dwTime;
		bMDBL = false;

		if(bMDBL){
			DWORD downItv = dwTime - MDownTime;
			// �_�u���N���b�N���m���ԓ��Ȃ�
			if(downItv < (DWORD)MDownTimeOut){
/*
				MOUSEHOOKSTRUCT *mhs;
				mhs = (MOUSEHOOKSTRUCT *)lParam;
				if(isTaskChild(mhs->hwnd)) break;
//*/
				// �z�b�g�L�[��������Ƒ���
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
// �^�X�N�o�[�̎q�E�C���h�E���ǂ���
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isTaskChild(HWND hWnd)
{
	HWND hTaskWnd;
	bool ret = false;
	HWND hParent = hWnd;

    hTaskWnd = FindWindow("Shell_TrayWnd", NULL);
	if(hTaskWnd == NULL) return false;

	// 5�K�w���܂Ō���
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
// �t�b�N�J�n(���邭��}�E�X)
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

	// �}�E�X�t�b�N��������
	hHookRoundMouse = SetWindowsHookEx( WH_MOUSE, (HOOKPROC)ProcRoundMouse, hDllInst, NULL );

	hookFlag |= HOOK_ROUNDMOUSE;

	if(hHookRoundMouse != NULL)	return 1;
	else						return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �t�b�N�I��(���邭��}�E�X)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DllExport int UnHookRoundMouse(VOID)
{
	int ret;

	// �t�b�N�~��
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
// �����Ă������b�Z�[�W�̃v���V�[�W��(���邭��}�E�X)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK ProcRoundMouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) 
		return CallNextHookEx(hHookRoundMouse, nCode, wParam, lParam);

	switch(wParam){

	// �}�E�X�ړ�
	case WM_MOVE:
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
		MOUSEHOOKSTRUCT *mhs;
		int dir;
		dir = -1;
		mhs = (MOUSEHOOKSTRUCT *)lParam;

		// ������
		if(mhs->pt.x - mouseX < 0)		dir = 0;
		else if(mhs->pt.x - mouseX > 0)	dir = 4;

		// �c����
		if(mhs->pt.y - mouseY < 0){
			if(dir == -1)		dir = 2;
			else if(dir == 0)	dir = 1;
			else				dir = 3;

		}else if(mhs->pt.y - mouseY > 0){
			if(dir == -1)		dir = 6;
			else if(dir == 0)	dir = 7;
			else				dir = 5;
		}

		// ���W�X�V
		mouseX = mhs->pt.x;
		mouseY = mhs->pt.y;

		// ����
		if(EvalRound(dir)){
			// �z�b�g�L�[��������Ƒ���
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
// �}�E�X�ړ��t���O��S�ăI�t(���邭��}�E�X)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RoundReset()
{
	roundSttDir = -1;
	roundNowDir = -1;
	roundNowNum = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���邭��}�E�X�̔���
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool EvalRound(int dir)
{
	// �����ĂȂ��Ȃ牽�����Ȃ�
	if(dir == -1){
		return false;
	}
	// �X�^�[�g�Ȃ珉���ݒ�
	if(roundSttDir == -1){
		roundSttDir = dir;
		roundNowDir = dir;
		roundTime = timeGetTime();
		return false;
	}

	// �ړ������̕ω����擾
	int move = dir-roundNowDir;
	// -3 �` 4 �ɕ␳
	while(move < -3 || move > 4){
		if(move < -3)	move += 8;
		else			move -= 8;
	}

	if(move == 0){
		// �������ς���ĂȂ��Ȃ牽�����Ȃ�
		return false;

	}else if( bClockwise && (move==1 || (move==2 && roundNowDir%2==1)) ){
		// ���v���̐���������

		// �P��]���Ă���Ȃ�
		if( (roundSttDir > roundNowDir &&		roundSttDir <= roundNowDir+move) ||
			(roundSttDir+8 > roundNowDir &&	roundSttDir+8 <= roundNowDir+move) ){
			// �^�C���A�E�g�Ȃ�o��
			if(timeGetTime() > roundTime + roundTimeOut){
				RoundReset();
				return false;
			}

			roundNowNum++;
			// �w�萔��]�Ő���
			if(roundNowNum == roundNum){
				RoundReset();
				return true;
			}
		}
		roundNowDir = dir;

	}else if( !bClockwise && (move==-1 || (move==-2 && roundNowDir%2==1)) ){
		// �����v���̐���������

		// �P��]���Ă���Ȃ�
		if( (roundSttDir >= roundNowDir+move &&		roundSttDir < roundNowDir) ||
			(roundSttDir-8 >= roundNowDir+move &&	roundSttDir-8 < roundNowDir) ){
			// �^�C���A�E�g�Ȃ�o��
			if(timeGetTime() > roundTime + roundTimeOut){
				RoundReset();
				return false;
			}

			roundNowNum++;
			// �w�萔��]�Ő���
			if(roundNowNum == roundNum){
				RoundReset();
				return true;
			}
		}
		roundNowDir = dir;

	}else{
		// �Ԉ���������Ȃ烊�Z�b�g
		RoundReset();
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VB��DoEvents�ɑ���
//
void DoEvents()
{
	MSG    msg;
	if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
