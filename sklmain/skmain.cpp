/**
 *
 * skmain.cpp WinMain���̎�v�֐�
 *
 */

#include <sys/stat.h>
#include "skutil.h"
#include "skoption.h"
#include "skitem.h"

// WM_MOUSEWHEEL�̒�`
#ifndef WM_MOUSEWHEEL
#include "zmouse.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// typedef
//

typedef int (*HOOKDESKTOP)(HWND);		// �f�X�N�g�b�v���N���b�N�p�̃t�b�N
typedef int (*UNHOOKDESKTOP)(VOID);		// �f�X�N�g�b�v���N���b�N�p�̃t�b�N����
typedef int (*HOOKRDBLCLICK)(HWND);		// �E�_�u���N���b�N�p�̃t�b�N
typedef int (*UNHOOKRDBLCLICK)(VOID);	// �E�_�u���N���b�N�p�̃t�b�N����
typedef int (*HOOKMDBLCLICK)(HWND);		// �z�C�[���_�u���N���b�N�p�̃t�b�N
typedef int (*UNHOOKMDBLCLICK)(VOID);	// �z�C�[���_�u���N���b�N�p�̃t�b�N����
typedef int (*HOOKROUNDMOUSE)(HWND);	// ���邭��}�E�X�p�̃t�b�N
typedef int (*UNHOOKROUNDMOUSE)(VOID);	// ���邭��}�E�X�p�̃t�b�N����


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �p�u���b�N�萔
//

// �\�t�g�̖���
#define APPNAME			"SK Launch"
// �o�[�W����
#define VERSION			"version 1.1.0 ��1"
// ���쌠�\�L
#define COPYRIGHT		"(C) 2004-2005 ramuneya"
// �z�b�g�L�[ID
#define SKL_HOTKEY		"SKLHOTKEY"
// �T�|�[�gURL
#define SUPPORT_URL		"http://hp.vector.co.jp/authors/VA030448/index.html"

// �E�C���h�E�N���X�̖���
#define SZ_CLASS_NAME	"WindowClassName_SKL1.1"
// �^�X�N�g���C�N���b�N
#define WM_TASKTRAY		(0x3000)

// �V�t�g�L�[���̃t���O
#define MASK_NONE		(0)
#define MASK_SHIFT		(1)
#define MASK_CTRL		(2)
#define MASK_ALT		(4)

// �t�@�C����
#define INI_FILENAME	"skl.ini"
#define XML_FILENAME	"items.xml"
#define DLL_FILENAME	"sklutil.dll"

// �A�N�V�����̎��
#define ACTION_FILEOPEN		(0)
#define ACTION_FOLDEROPEN	(1)
#define ACTION_HIDE			(2)
#define ACTION_LEFT			(3)
#define ACTION_RIGHT		(4)
#define ACTION_UP			(5)
#define ACTION_DOWN			(6)
#define ACTION_SHOW			(7)

// �A�j���[�V�����̎���(millisecond)
#define ANIMATION_OUT_TIME			(550)
#define ANIMATION_IN_TIME			(200)
#define ANIMATION_HOR_TIME			(150)

// �L���[�̍ő吔
#define MAX_QUEKEY		(2)

// �^�C�}�[ID
#define TIMER_ITEM			(32750)
#define TIMER_GROUP1		(32751)
#define TIMER_GROUP2		(32752)
#define TIMER_SHOW			(32753)
#define TIMER_HIDE			(32754)
#define TIMER_FILEOPEN		(32755)
#define TIMER_FOLDEROPEN	(32756)

// �h���b�O�ŃA�N�V�������N�����L������
#define DRAG_VALUE			(50)

#define MSG_OFFSET			(55)	// ���b�Z�[�W�̕\���ʒu�I�t�Z�b�g
#define ICON_OFFSET			(16)	// �A�C�R���̕`��ʒu�I�t�Z�b�g
#define LEVEL_SPACE_NUM		(2)		// XML�ɏo�͂���ۂ̃^�u�̃X�y�[�X��

// �I�v�V�����̃C���f�b�N�X
#define OPTION_ANIMTION		(0)
#define OPTION_MESSAGE		(1)
#define OPTION_INPUT		(2)
#define OPTION_SOUND		(3)
#define OPTION_ACTION		(4)


// �f�t�H���g�̒l /////////////////////////////////
#define DEFAULT_CenterX				(0)
#define DEFAULT_CenterY				(0)

#define DEFAULT_AutoR				(1)
#define DEFAULT_MaxR				(100)
#define DEFAULT_MinR				(0)
#define DEFAULT_IconItv				(10)
#define DEFAULT_Smooth				(50)
#define DEFAULT_RoteRad				(50)
#define DEFAULT_RoteTime			(50)
#define DEFAULT_DarkType			(2)
#define DEFAULT_DarkValue			(50)

#define DEFAULT_boolMsg				(1)
#define DEFAULT_MsgPos				(0)
#define DEFAULT_MsgStyle			"[%GROUP_CAPTION%]%RETURN%%RETURN%%ITEM_CAPTION%"
#define DEFAULT_MsgAlign			(1)
#define DEFAULT_MsgColor			(16777215)
#define DEFAULT_MsgFont				"�l�r �o�S�V�b�N"
#define DEFAULT_MsgFontSize			(12)

#define DEFAULT_HotKeyCode			(VK_SPACE)
#define DEFAULT_HotKeyShift			(MASK_ALT)

#define DEFAULT_ShortKeyNum0		(1)
#define DEFAULT_ShortKeyCode00		(VK_RETURN)
#define DEFAULT_ShortKeyShift00		(MASK_NONE)
#define DEFAULT_ShortKeyNum1		(1)
#define DEFAULT_ShortKeyCode10		(VK_RETURN)
#define DEFAULT_ShortKeyShift10		(MASK_CTRL)
#define DEFAULT_ShortKeyNum2		(1)
#define DEFAULT_ShortKeyCode20		(VK_ESCAPE)
#define DEFAULT_ShortKeyShift20		(MASK_NONE)
#define DEFAULT_ShortKeyNum3		(1)
#define DEFAULT_ShortKeyCode30		(VK_LEFT)
#define DEFAULT_ShortKeyShift30		(MASK_NONE)
#define DEFAULT_ShortKeyNum4		(1)
#define DEFAULT_ShortKeyCode40		(VK_RIGHT)
#define DEFAULT_ShortKeyShift40		(MASK_NONE)
#define DEFAULT_ShortKeyNum5		(1)
#define DEFAULT_ShortKeyCode50		(VK_UP)
#define DEFAULT_ShortKeyShift50		(MASK_NONE)
#define DEFAULT_ShortKeyNum6		(1)
#define DEFAULT_ShortKeyCode60		(VK_DOWN)
#define DEFAULT_ShortKeyShift60		(MASK_NONE)
#define DEFAULT_ShortKeyPriority	(1)
#define DEFAULT_boolAccessKey		(1)

#define DEFAULT_SEHor				"hor.wav"
#define DEFAULT_SEUp				"up.wav"
#define DEFAULT_SEDown				"down.wav"
#define DEFAULT_SEExe				"hor.wav"
#define DEFAULT_boolSEHor			(1)
#define DEFAULT_boolSEUp			(1)
#define DEFAULT_boolSEDown			(1)
#define DEFAULT_boolSEExe			(1)

#define DEFAULT_MouseL				(1)
#define DEFAULT_MouseM				(1)
#define DEFAULT_MouseR				(4)
#define DEFAULT_DefaultPos			(0)
#define DEFAULT_HotKeyAction		(0)
#define DEFAULT_boolDeskTopClick	(0)
#define DEFAULT_boolRoundMouse		(0)
#define DEFAULT_intDBLClick			(0)
#define DEFAULT_CenterPos			(0)
#define DEFAULT_boolInvRote			(0)

#define DEFAULT_RoundNum			(2)
#define DEFAULT_RoundTimeOut		(1500)
#define DEFAULT_bClockwise			(1)

#define MIN_MaxR				(0)
#define MIN_MinR				(0)
#define MIN_IconItv				(5)
#define MIN_Smooth				(0)
#define MIN_RoteRad				(0)
#define MIN_RoteTime			(50)
#define MIN_DarkValue			(0)
#define MAX_MaxR				(300)
#define MAX_MinR				(0)
#define MAX_IconItv				(50)
#define MAX_Smooth				(100)
#define MAX_RoteRad				(100)
#define MAX_RoteTime			(100)
#define MAX_DarkValue			(100)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �N���X
//

// �L�[����
class Key{
public:
	// �R���X�g���N�^
	Key(){
		nVirtKey = NULL;
		mask = MASK_NONE;
	}
	Key(int vk, char msk){
		nVirtKey = vk;
		mask = msk;
	}
	// �f�X�g���N�^
	virtual ~Key(){}

	// �R�s�[�R���X�g���N�^
	Key(const Key& other){
		nVirtKey = other.nVirtKey;
		mask = other.mask;
	}
	// ������Z�q
	const Key& operator=(const Key& other){
		nVirtKey = other.nVirtKey;
		mask = other.mask;
		return(*this);
	}

	// �������e���ǂ���
	const bool equals(const Key& other) const{
		if(nVirtKey == other.nVirtKey && mask == other.mask)
			return true;
		else
			return false;
	}

	// �����o
	int nVirtKey;
	char mask;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �p�u���b�N�ϐ�
//

// �O���[�v�A�C�e��
vector<SKGroupItem> GI;
// ���I�����Ă�O���[�v
int iGI;
// ���\�����Ă��邩
bool bShow;

bool bDialog;				// �_�C�A���O�\�������ǂ���

RECT rScreen;				// �X�N���[���T�C�Y

HINSTANCE hInst;			// �����̂̃n���h��
HINSTANCE hDllInst;			// sklutil.dll�̃n���h��
HANDLE hMutex;				// �~���[�e�b�N�X�I�u�W�F�N�g�̃n���h��

NOTIFYICONDATA m_nid;		// �A�C�R���f�[�^

HDC hBkDC;					// �o�b�N�o�b�t�@��DC
HBITMAP hBkBm;				// �o�b�N�o�b�t�@��BITMAP
HFONT hFont;				// �o�b�N�o�b�t�@�̃t�H���g
HDC hDskDC;					// �X�N���[���L���v�`����DC

HDC hCur;					// �J�[�\���摜��DC
HBITMAP hBmpCur;			// �J�[�\���摜��BITMAP
int curX, curY;				// �J�[�\���摜�̕`��ʒu
int curOffsetX, curOffsetY;	// �J�[�\���摜�̕`��̂���ʒu
bool bCurPaint;				// �J�[�\����`�悷�邩

HDC hMsg;					// ���b�Z�[�W�摜��DC
HBITMAP hBmpMsg;			// ���b�Z�[�W�摜��BITMAP
RECT msgArea;				// ���b�Z�[�W�摜�̕`��ʒu
int msgLength;				// ���b�Z�[�W�摜�̕�
bool bMsgPaint;				// ���b�Z�[�W�摜����`�悷�邩
string sMsg;				// ���b�Z�[�W�̓��e

bool bActing;				// �A�N�V���������ǂ���
queue<Key> queKey;			// �҂����̃L�[���X�g

bool bDrag;								// �h���b�O���Ă��邩�ǂ���
bool bMenu;								// ���j���[��\�����Ă��邩�ǂ���
int mouseX, mouseY;						// �}�E�X�_�E���������W;

// �A�j���[�V����
bool autoR;					// �����Ŕ��a�𒲐����邩
UCHAR maxR;					// �ő�̔��a
UCHAR minR;					// �ŏ��̔��a
UCHAR iconItv;				// �A�C�R�����m�̊Ԋu
UCHAR roteRad;				// �A�j���[�V�����œ����p�x
UCHAR smooth;				// ���炩��
UCHAR roteTime;				// �A�j���[�V��������
UCHAR darkType;				// �A�j�����ɈÂ�����(0:�Â����Ȃ�, 1:�ȈՕ`��, 2:�ڍו`��)
UCHAR darkValue;			// �Â�����Z�x

// ���b�Z�[�W
bool boolMsg;				// ���b�Z�[�W��\�����邩
string msgStyle;			// ���b�Z�[�W�̏���
UCHAR msgPos;				// ���b�Z�[�W�̈ʒu(0-��, 1-���S, 2-��)
UCHAR msgAlign;				// ���b�Z�[�W�����̑����ʒu(0-��, 1-����, 2-�E)
int msgColor;				// ���b�Z�[�W�����̐F
string msgFont;				// ���b�Z�[�W�����̃t�H���g
UCHAR msgFontSize;			// ���b�Z�[�W�����̃t�H���g�T�C�Y

// �L�[�{�[�h�E�}�E�X
ATOM hotKeyID;							// �z�b�g�L�[ID
Key hotKey;								// �z�b�g�L�[
vector< vector<Key> > shortKey;			// �V���[�g�J�b�g�L�[(7*5)(0-�N��, 1-�t�H���_, 2-�B��, 3-��, 4-�E, 5-��, 6-��)
bool shortKeyPriority;					// �W�����v�������D�悷�邩�ǂ���
bool boolAccessKey;						// �A�N�Z�X�L�[�̗L���^����

// SE
string SEHor;				// ��
string SEUp;				// ��
string SEDown;				// ��
string SEExe;				// ���s
bool boolSEHor;				// ���ʉ��̗L��
bool boolSEUp;
bool boolSEDown;
bool boolSEExe;

// ����
UCHAR defaultPos;			// �\���������̏����ʒu(0:�ȑO�Ɠ���, 1:�擪�ɖ߂�)
UCHAR hotKeyAction;			// �\�����Ƀz�b�g�L�[�����������̓���
UCHAR centerPos;			// �A�j���[�V�����̒��S���W(0:��ʒ���, 1: �C�Ӎ��W, 2:�}�E�X�̈ʒu)
POINT center;				// ���S���W
UCHAR mouseL;				// ���N���b�N���̃}�E�X����	(0-�Ȃ�, 1-�N��, 2-�t�H���_, 3-�B��, 4-���j���[)
UCHAR mouseM;				// ���N���b�N���̃}�E�X����	(0-�Ȃ�, 1-�N��, 2-�t�H���_, 3-�B��, 4-���j���[)
UCHAR mouseR;				// �E�N���b�N���̃}�E�X����	(0-�Ȃ�, 1-�N��, 2-�t�H���_, 3-�B��, 4-���j���[)
bool boolInvRote;			// ���E�̉�]���t�ɂ��邩
bool boolDeskTopClick;		// �f�X�N�g�b�v���_�u���N���b�N�����Ƃ��ɋN�����邩�ǂ���
bool boolRoundMouse;		// ���邭��}�E�X
int intDBLClick;			// �_�u���N���b�N(0-�Ȃ�, 1-�E, 2-����)
//bool boolMouseCentering;	// �\�����Ƀ}�E�X���ړ����邩
//bool boolMouseCenteringMouseOnly;	// �}�E�X����̋N���̂ݗL���ɂ��邩

// �A�j���[�V�����p�ϐ�
HWND timerHWnd;				// �E�C���h�E�n���h��
UINT timerID;				// �^�C�}�[ID
UINT timerType;				// �^�C�}�[ID
UINT timerCount;			// �J�E���g
UINT timerMaxCount;			// �ő�̃J�E���g��
float timerR;				// �ړ������a
float timerTheta;			// �ړ������ʊp
bool timerOut;				// �O�������ǂ���
bool timerClockwise;		// ���v��肩�ǂ���
UINT timerIndex;			// �ړ�����A�C�e���̃C���f�b�N�X
UINT timerPerFrame;			// �^�C�}�[�̊Ԋu


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���g�^�C�v�錾
//

// �E�C���h�E�v���V�[�W��
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
// �E�C���h�E�N���X�̓o�^
ATOM MyRegisterClass(HINSTANCE hInstance);
// �C���X�^���X�������i�E�C���h�E�쐬�j
bool InitInstance(HINSTANCE hInstance, int nCmdShow);

// ����������
bool Init(HWND hWnd);
// �I������
bool Unload(HWND hWnd);

// �O���t�B�b�N�X�֘A�̏�����
bool InitGraphics(HWND hWnd);

// �ϐ��̓ǂݍ���
void GetVar();
// �ϐ��̏�������
void SetVar(bool bDefault);
// XML�t�@�C������ǂݍ���
bool GetFromXML();

// �A�C�e���̕\��
void SKShow(HWND hWnd);
// �A�C�e�����B��
void SKHide(HWND hWnd, bool animation);
// �O���[�v�̕ύX���A�j���[�V����
void ChangeGroup(HWND hWnd, int index, bool bOut, bool bClockwise, bool bOnlyAnime);
// �O���[�v�A�j���[�V����
void AnimateGroup(HWND hWnd, bool bCenter, bool bOut, bool bClockwise, UINT uiTimerType);
// �A�C�e���̕ύX���A�j���[�V����
void ChangeItem(HWND hWnd, int index, bool direction);
// �A�C�e���A�j���[�V����
void AnimateItem(HWND hWnd, UINT index, bool direction, bool clockwise);
// �`��
void DrawWindow(HWND hWnd);
// �L�[�A�N�V�����̌���
void SKKeySearch(HWND hWnd, const Key& key);
// �A�N�V����
void SKAction(HWND hWnd, UCHAR type);
// �V���[�g�J�b�g�L�[
bool ShortcutKey(HWND hWnd, const Key& key);
// �W�����v�L�[
bool JumpKey(HWND hWnd, const Key& key);
// �A�N�Z�X�L�[
bool AccessKey(HWND hWnd, const Key& key);
// �}�E�X�A�N�V����(0-�Ȃ�, 1-�N��, 2-�t�H���_, 3-�B��, 4-���j���[)
void SKMouseAction(HWND hWnd, UCHAR type);
// �^�C�}�[�p�֐�(�^�C�}�[ID,�\��,���[�U�[��`,�\��,�\��)
void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
// �X�^�[�g�A�b�v�ɓo�^
void CreateStartup(HWND hWnd);
// �V���[�g�J�b�g�t�@�C���̏o��
void CreateShortcutFiles(HWND hWnd);
// SKShortcutItem����V���[�g�J�b�g�t�@�C�����쐬
HRESULT CreateShortcutFileFromItem(const string& path, const SKShortcutItem& item);
// �J�[�\���̕`��ʒu���X�V
void RenewCursor(HWND hWnd, HRGN invalidRgn);
// �J�[�\���̕`��̈���X�V
void InvalidateCursor(HWND hWnd, HRGN invalidRgn);
// �J�[�\���̕`��
void DrawCursor(HWND hWnd, HDC hDC);
// ���b�Z�[�W�̊e��ϐ����X�V
void RenewMessage(HWND hWnd, HDC hDC, HRGN invalidRgn);
// ���b�Z�[�W�̓��e���擾
string EncordMessage();
// ���b�Z�[�W�w�i�̕`��̈���X�V
void InvalidateMessage(HWND hWnd, HRGN invalidRgn);
// ���b�Z�[�W�w�i�̕`��
void DrawMessage(HWND hWnd, HDC hDC);
// �z�b�g�L�[�̐ݒ�
bool SetHotKey(HWND hWnd, const string& strID, const Key& key);
// �z�b�g�L�[�̉���
void UnSetHotKey(HWND hWnd);
// �o�[�W�������
LRESULT CALLBACK DlgProcVer(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���C���֐�
//

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	hInst = hInstance;
	// �E�C���h�E�N���X�̓o�^
	MyRegisterClass(hInstance);
	// �E�C���h�E�̃C���X�^���X������
	if( !InitInstance(hInstance, SW_HIDE) ) return FALSE;

	MSG msg;

    while( GetMessage(&msg, NULL, 0, 0) ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return msg.wParam;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �E�C���h�E�N���X�̓o�^
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS myProg;
	myProg.style			= CS_HREDRAW | CS_VREDRAW;
	myProg.lpfnWndProc		= WinProc;
	myProg.cbClsExtra		= 0;
	myProg.cbWndExtra		= 0;
	myProg.hInstance		= hInstance;
	myProg.hIcon			= NULL;
	myProg.hCursor			= LoadCursor(NULL, IDC_ARROW);
//	myProg.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	myProg.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	myProg.lpszMenuName		= NULL;
	myProg.lpszClassName	= SZ_CLASS_NAME;
	
	return( RegisterClass(&myProg) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �C���X�^���X�̏�����
//
bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	rScreen = GetScreenSize();	// �X�N���[���T�C�Y���擾

	// �E�C���h�E�쐬
	HWND hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		SZ_CLASS_NAME,
		"SK Launch",
		WS_POPUP,
		rScreen.left,
		rScreen.top,
		rScreen.right-rScreen.left,
		rScreen.bottom-rScreen.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	if(!hWnd) return(FALSE);	// ���s����FALSE
	
    ShowWindow(hWnd, nCmdShow);	// �\����Ԃ�ݒ�
    UpdateWindow(hWnd);			// �X�V

	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���V�[�W��
//
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i;
	Key key;
	string s;

    switch (msg) {
/*
	case WM_USER+16:
		s.append(CStr((int)wParam) + ", " + CStr((int)lParam));
		SetWindowText(hWnd, s.c_str());
		break;
//*/
	// �쐬��
	case WM_CREATE:
		// ���������s�Ȃ�I��
		if(!Init(hWnd)) SendMessage(hWnd, WM_CLOSE, 0L, 0L);
		break;

	// �I����
	case WM_DESTROY:
        if(Unload(hWnd)) PostQuitMessage(0);
        break;

	// �z�b�g�L�[
	case WM_HOTKEY:
		// �_�C�A���O�\�����Ȃ璆�~
		if(bDialog) break;
		// �\�����ĂȂ���Ε\��
		if(!bShow){
/*
			string buff;
			buff.append("WP:" + CStr((int)wParam) + ", LP:" + CStr((int)lParam));
			MessageBox(hWnd, buff.c_str(),"",MB_OK);
//*/
			SKAction(hWnd, ACTION_SHOW);
		}
		else if(!bActing){
			// �\�����Ă����hotKeyAction�ŕ���
			switch(hotKeyAction){
			case 0: break;
			case 1: SKAction(hWnd, ACTION_HIDE);
				break;
			case 2: SKAction(hWnd, ACTION_FILEOPEN);
				break;
			}
		}
		break;

	// �A�N�e�B�u�E��A�N�e�B�u��
	case WM_ACTIVATE:
		// �_�C�A���O�̃A�N�e�B�u�E��A�N�e�B�u�Ȃ�֌W�Ȃ�
		if(bDialog) break;
		if(LOWORD(wParam) == WA_INACTIVE && bShow){
			// �A�N�V�������łȂ���ΉB��
			if(!bActing)	SKAction(hWnd, ACTION_HIDE);
			// �A�N�V�������ł���΃L���[�ɒǉ�
			else{
				while(!queKey.empty()) queKey.pop();	// �L���[���N���A
				key = shortKey.at(ACTION_HIDE).front();
				queKey.push(key);
			}
		}
		break;

	// ���j���[
	case WM_COMMAND:
        switch (LOWORD(wParam)) {

		// �\��
        case IDM_MENU1_SHOW:
			SKAction(hWnd, ACTION_SHOW);
            break;

		// �A�C�e���̕ҏW
        case IDM_MENU1_ITEM:
			bDialog = true;
//			i = DialogBox(hInst, "IDD_ITEM", hWnd, (DLGPROC)DlgProcItem);
			bDialog = false;
			MemoryClean();
            break;

		// �I�v�V����
        case IDM_MENU1_OPTION:
			bDialog = true;
			i = DialogBox(hInst, "IDD_OPTION", hWnd, (DLGPROC)DlgProcOption);
			bDialog = false;

			MemoryClean();
            break;

		// �X�^�[�g�A�b�v�ɓo�^
        case IDM_MENU1_STARTUP:
			CreateStartup(hWnd);
			MemoryClean();
            break;

		// �V���[�g�J�b�g�t�@�C���̏o��
        case IDM_MENU1_EXPORT:
			CreateShortcutFiles(hWnd);
			MemoryClean();
            break;

		// �o�[�W�������
        case IDM_MENU1_VERSION:
			bDialog = true;
			i = DialogBox(hInst, "IDD_VERSION", hWnd, (DLGPROC)DlgProcVer);
			bDialog = false;
			MemoryClean();
            break;

		// �w���v
        case IDM_MENU1_HELP:
			s.append(GetAppDir());
			s.append("\\doc\\index.html");
			i = (int)ShellExecute(0, "open", s.c_str(), "", "", SW_SHOWNORMAL);
			if(i<=32) MessageBox(timerHWnd, openError(i).c_str(), "ERROR", MB_OK);
			MemoryClean();
            break;

		// �쐬���֍s��
        case IDM_MENU1_GOTO:
			i = (int)ShellExecute(0, "open", SUPPORT_URL, "", "", SW_SHOWNORMAL);
			if(i<=32) MessageBox(timerHWnd, openError(i).c_str(), "ERROR", MB_OK);
			MemoryClean();
            break;

		// �I��
        case IDM_MENU1_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;

		// �A�C�e���̋N��
        case IDM_MENU2_FILEOPEN:
			SKMouseAction(hWnd, 1);
            break;

		// �t�H���_���J��
        case IDM_MENU2_FOLDEROPEN:
			SKMouseAction(hWnd, 2);
            break;

		// �B��
        case IDM_MENU2_HIDE:
			SKMouseAction(hWnd, 3);
            break;

        default:
            return (DefWindowProc(hWnd, msg, wParam, lParam));
        }
		bMenu = false;
		break;

	// �}�E�X�_�E��
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		i = 0;
		if(wParam & MK_LBUTTON) i++;
		if(wParam & MK_MBUTTON) i++;
		if(wParam & MK_RBUTTON) i++;
		if(i==1)	bDrag = false;
		else		bDrag = true;

		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		break;

	// �}�E�X�ړ�
	case WM_MOUSEMOVE:
		// �h���b�O
		if(wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)){
			i = LOWORD(lParam) - mouseX;
			// �������ɗL�������ȏ�h���b�O���Ă�����
			if(abs(i) > DRAG_VALUE){
				bDrag = true;
				mouseX = LOWORD(lParam);
				mouseY = HIWORD(lParam);

				if(!bActing){
					if(i<0)	SKAction(hWnd, ACTION_LEFT);
					else	SKAction(hWnd, ACTION_RIGHT);
				}

				break;
			}

			i = HIWORD(lParam) - mouseY;
			// �c�����ɗL�������ȏ�h���b�O���Ă�����
			if(abs(i) > DRAG_VALUE){
				bDrag = true;
				mouseX = LOWORD(lParam);
				mouseY = HIWORD(lParam);

				if(!bActing){
					if(i<0)	SKAction(hWnd, ACTION_UP);
					else	SKAction(hWnd, ACTION_DOWN);
				}

				break;
			}
		}
		break;

	// ���}�E�X�A�b�v
	case WM_LBUTTONUP:
		if(bMenu){
			bMenu = false;
			break;
		}
		if(!bDrag && !bActing) SKMouseAction(hWnd, mouseL);
		break;

	// ���}�E�X�A�b�v
	case WM_MBUTTONUP:
		if(!bMenu && !bDrag && !bActing) SKMouseAction(hWnd, mouseM);
		break;

	// �E�}�E�X�A�b�v
	case WM_RBUTTONUP:
		if(bMenu){
			bMenu = false;
			break;
		}
		if(!bDrag && !bActing) SKMouseAction(hWnd, mouseR);
		break;

	// �z�C�[���𓮂���
	case WM_MOUSEWHEEL:
		if(!bShow) break;
		i = (SHORT)HIWORD(wParam);
		bDrag=true;

		if(wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)){
			// �N���b�N���Ȃ���Ȃ�O���[�v
			if(!bActing && queKey.empty()){
				if(i>=0)	SKAction(hWnd, ACTION_UP);
				else		SKAction(hWnd, ACTION_DOWN);
			}

		}else{
			// �Ⴄ�Ȃ�A�C�e��(�L���[����)
			if(!bActing && queKey.empty()){
				if(i>=0)	SKAction(hWnd, ACTION_LEFT);
				else		SKAction(hWnd, ACTION_RIGHT);

			}else if(queKey.size() < MAX_QUEKEY){
				if(i>=0)	key = shortKey.at(ACTION_LEFT).front();
				else		key = shortKey.at(ACTION_RIGHT).front();
				queKey.push(key);
			}
		}
		break;

	// �L�[�_�E��
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if(!bShow) break;
		// ���z�L�[
		key.nVirtKey = wParam;
		key.mask = MASK_NONE;
		// �V�t�g
		if(GetKeyState(VK_SHIFT) < 0)	key.mask |= MASK_SHIFT;
		if(GetKeyState(VK_CONTROL) < 0)	key.mask |= MASK_CTRL;
		if(GetKeyState(VK_MENU) < 0)	key.mask |= MASK_ALT;

		if(!bActing && queKey.empty())		SKKeySearch(hWnd, key);
		else if(queKey.size() < MAX_QUEKEY)	queKey.push(key);
		break;

	// �^�X�N�g���C
	case WM_TASKTRAY:
		if(bDialog) break;
		switch(lParam){

		// ���_�u���N���b�N
		case WM_LBUTTONDBLCLK:
			SKAction(hWnd, ACTION_SHOW);
			break;

		// �E�N���b�N
		case WM_RBUTTONUP:
			bMenu = true;
			PopupMenu(hWnd, "IDR_MENU1");
			break;
		}

		break;

	// �`�掞
	case WM_PAINT:
		DrawWindow(hWnd);
	    break;

    default:
        return( DefWindowProc(hWnd, msg, wParam, lParam) );
    }
    return (0L);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �}�E�X�A�N�V����(0-�Ȃ�, 1-�N��, 2-�t�H���_, 3-�B��, 4-���j���[)
//
void SKMouseAction(HWND hWnd, UCHAR type)
{
	int act = (int)type;

	if(act>=1 && act<=3)	SKAction(hWnd, act-1);			// �N��, �t�H���_, �B��
	else if(act == 4){
		bMenu = true;
		PopupMenu(hWnd, "IDR_MENU2");						// ���j���[
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �L�[�A�N�V�����̌���
//
void SKKeySearch(HWND hWnd, const Key& key)
{
	if(shortKeyPriority){
		// �V���[�g�J�b�g�L�[���D��
		if(ShortcutKey(hWnd, key))		return;
		else if(JumpKey(hWnd, key))		return;
		else							AccessKey(hWnd, key);
	}else{
		// �W�����v�L�[���D��
		if(JumpKey(hWnd, key))			return;
		else if(ShortcutKey(hWnd, key))	return;
		else							AccessKey(hWnd, key);

	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�N�V����
//
void SKAction(HWND hWnd, UCHAR type)
{
	switch(type){

	// 0-�N��
	case ACTION_FILEOPEN:
		if(boolSEExe) sndPlaySound(SEExe.c_str(), SND_ASYNC | SND_NODEFAULT);

		SKHide(hWnd, true);
		timerType = TIMER_FILEOPEN;
		break;

	// 1-�t�H���_
	case ACTION_FOLDEROPEN:
		if(boolSEExe) sndPlaySound(SEExe.c_str(), SND_ASYNC | SND_NODEFAULT);

		SKHide(hWnd, true);
		timerType = TIMER_FOLDEROPEN;

		break;

	// 2-�B��
	case ACTION_HIDE:
		if(boolSEUp) sndPlaySound(SEUp.c_str(), SND_ASYNC | SND_NODEFAULT);

		SKHide(hWnd, true);
		break;

	// 3,4-���E
	case ACTION_LEFT:
	case ACTION_RIGHT:
		if((type==ACTION_LEFT) ^ boolInvRote)
			ChangeItem(hWnd, GI.at(iGI).getItemIndex()+1, true);
		else
			ChangeItem(hWnd, GI.at(iGI).getItemIndex()-1, true);
		break;

	// 5-��
	case ACTION_UP:
		if(boolSEUp) sndPlaySound(SEUp.c_str(), SND_ASYNC | SND_NODEFAULT);

		ChangeGroup(hWnd, iGI-1, true, false, false);
		break;

	// 6-��
	case ACTION_DOWN:
		if(boolSEDown) sndPlaySound(SEDown.c_str(), SND_ASYNC | SND_NODEFAULT);

		ChangeGroup(hWnd, iGI+1, false, true, false);
		break;

	// 7-�\��
	case ACTION_SHOW:
		if(!bShow){
			if(boolSEUp) sndPlaySound(SEUp.c_str(), SND_ASYNC | SND_NODEFAULT);
			SKShow(hWnd);
		}
		break;
	}

	MemoryClean();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �V���[�g�J�b�g�L�[
//
bool ShortcutKey(HWND hWnd, const Key& key)
{
	// ����
	for(int i=0; i<7; i++){
		int size=shortKey.at(i).size();

		for(int j=0; j<size; j++){
			if( key.equals(shortKey.at(i).at(j)) ){
				SKAction(hWnd, i);
				return true;
			}
		}
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �W�����v�L�[
//
bool JumpKey(HWND hWnd, const Key& key)
{
	int i;

	// �A�C�e���̌���
	int size = GI.at(iGI).size();
	for(i=0; i<size; i++){
//		const SKShortcutItem& item = GI.at(iGI).item(i);

		if( key.equals(Key(GI.at(iGI).item(i).getKeyChar(), GI.at(iGI).item(i).getKeyShift())) ){
			if(i != GI.at(iGI).getItemIndex())
				ChangeItem(hWnd, i, false);
			else
				SKAction(hWnd, ACTION_FILEOPEN);

			return true;
		}
	}

	// �O���[�v�̌���
	size = GI.size();
	for(i=0; i<size; i++){
//		const SKGroupItem& group = GI.at(i);

		if( i != iGI && 
		    key.equals(Key(GI.at(i).getKeyChar(), GI.at(i).getKeyShift())) ){

			if(boolSEDown) sndPlaySound(SEDown.c_str(), SND_ASYNC | SND_NODEFAULT);
			ChangeGroup(hWnd, i, false, true, false);
			return true;
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�N�Z�X�L�[
//
bool AccessKey(HWND hWnd, const Key& key)
{
	int size = GI.at(iGI).size();
	int stt = GI.at(iGI).getItemIndex();

	// �����ϊ��ł���L�[����Ȃ���Ύ~�߂�
	if(!((key.nVirtKey >= '0' && key.nVirtKey <= '9') ||
	     (key.nVirtKey >= 'A' && key.nVirtKey <= 'Z'))) return false;

	for(int i=stt+1; i!=stt; i++){
		if(i >= size){
			i = 0;
			if(i == stt) break;
		}

		// ���������擾
		char sHead = GI.at(iGI).item(i).getCaption().at(0);
		// �啶���ɂ���
		if(sHead >= 'a' && sHead <= 'z') sHead += 'A'-'a';

		if( key.nVirtKey == sHead ){
			ChangeItem(hWnd, i, false);
			return true;
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �I������
//
bool Unload(HWND hWnd)
{
	UNHOOKDESKTOP fpUnHookDesktop;
	UNHOOKRDBLCLICK fpUnHookRDBLClick;
	UNHOOKMDBLCLICK fpUnHookMDBLClick;
	UNHOOKROUNDMOUSE fpUnHookRoundMouse;

	// �\�����Ă�����B��
	if(bShow) SKHide(hWnd, false);

	// vector, list�̍폜
	shortKey.clear();
	GI.clear();

	// �^�X�N�g���C�A�C�R���̍폜
	Shell_NotifyIcon(NIM_DELETE, &m_nid);

	// �o�b�N�o�b�t�@�̍폜
	DeleteObject(hBkBm);
	DeleteObject(hFont);
	DeleteDC(hBkDC);

	// �J�[�\���摜�̍폜
	DeleteObject(hBmpCur);
	DeleteDC(hCur);

	// ���b�Z�[�W�摜�̍폜
	DeleteObject(hBmpMsg);
	DeleteDC(hMsg);

	// �z�b�g�L�[����
	UnSetHotKey(hWnd);

	// �t�b�N����
	if(boolDeskTopClick || boolRoundMouse || (intDBLClick>0)){
		// �f�X�N�g�b�v���_�u���N���b�N
		if(boolDeskTopClick){
			fpUnHookDesktop = (UNHOOKDESKTOP)GetProcAddress(hDllInst, "UnHookDesktop");
			(*fpUnHookDesktop)();
		}
		// �E�_�u���N���b�N
		if(intDBLClick == 1){
			fpUnHookRDBLClick = (UNHOOKRDBLCLICK)GetProcAddress(hDllInst, "UnHookRDBLClick");
			(*fpUnHookRDBLClick)();
		}
		// �z�C�[���_�u���N���b�N
		if(intDBLClick == 2){
			fpUnHookMDBLClick = (UNHOOKMDBLCLICK)GetProcAddress(hDllInst, "UnHookMDBLClick");
			(*fpUnHookMDBLClick)();
		}
		// ���邭��}�E�X
		if(boolRoundMouse){
			fpUnHookRoundMouse = (UNHOOKROUNDMOUSE)GetProcAddress(hDllInst, "UnHookRoundMouse");
			(*fpUnHookRoundMouse)();
		}

		FreeLibrary(hDllInst);			// DLL�̉��
	}

	// �~���[�e�b�N�X���
	ReleaseMutex(hMutex);

	return(true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����������
//
bool Init(HWND hWnd)
{
	HOOKDESKTOP fpHookDesktop;
	HOOKRDBLCLICK fpHookRDBLClick;
	HOOKMDBLCLICK fpHookMDBLClick;
	HOOKROUNDMOUSE fpHookRoundMouse;
	int ret;

	// �~���[�e�b�N�X�̍쐬(�Q�d�N���h�~�p)
	hMutex = CreateMutex(NULL, TRUE, SZ_CLASS_NAME);
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) return false;

	GetVar();			// �ϐ��̓ǂݍ���
	if(!GetFromXML()){	// �A�C�e���̓ǂݍ���
		MessageBox(NULL, "xml-file reading failure!", APPNAME, MB_OK);
	}
	iGI = 0;				// �O���[�v��I��
	bActing = false;		// �A�N�V�������łȂ�
	bDialog = false;		// �_�C�A���O��\��

	// �z�b�g�L�[�̐ݒ�
	hotKeyID = 0;
	if( (hotKey.nVirtKey==0) || (!SetHotKey(hWnd, SKL_HOTKEY, hotKey)) ){
		MessageBox(hWnd, "�z�b�g�L�[�̐ݒ�Ɏ��s���܂����B\n�I�����܂��B", APPNAME, MB_OK);
		return false;
	}

	// �O���t�B�b�N�X�֘A�̏�����
	if( !InitGraphics(hWnd) ){
		MessageBox(hWnd, "�O���t�B�b�N�X�̏��������ɃG���[���������܂����B\n�I�����܂��B", APPNAME, MB_OK);
		return false;
	}

	// �t�b�N
	if(boolDeskTopClick || boolRoundMouse || (intDBLClick > 0)){
		hDllInst = LoadLibrary(DLL_FILENAME);	// DLL�̃��[�h

		// �f�X�N�g�b�v���_�u���N���b�N
		if(boolDeskTopClick){
			fpHookDesktop = (HOOKDESKTOP)GetProcAddress(hDllInst, "HookDesktop");

			if(fpHookDesktop != NULL) ret = (*fpHookDesktop)(hWnd);
			if(fpHookDesktop == NULL || ret != 1){
				MessageBox(hWnd, "�t�b�N�̐ݒ�Ɏ��s���܂����B\n�i�f�X�N�g�b�v���_�u���N���b�N�@�\�j\n�I�����܂��B", APPNAME, MB_OK);
				return false;
			}
		}

		// �E�_�u���N���b�N
		if(intDBLClick == 1){
			fpHookRDBLClick = (HOOKRDBLCLICK)GetProcAddress(hDllInst, "HookRDBLClick");

			if(fpHookRDBLClick != NULL) ret = (*fpHookRDBLClick)(hWnd);
			if(fpHookRDBLClick == NULL || ret != 1){
				MessageBox(hWnd, "�t�b�N�̐ݒ�Ɏ��s���܂����B\n�i�E�_�u���N���b�N�@�\�j\n�I�����܂��B", APPNAME, MB_OK);
				return false;
			}
		}else if(intDBLClick == 2){
			fpHookMDBLClick = (HOOKMDBLCLICK)GetProcAddress(hDllInst, "HookMDBLClick");

			if(fpHookMDBLClick != NULL) ret = (*fpHookMDBLClick)(hWnd);
			if(fpHookMDBLClick == NULL || ret != 1){
				MessageBox(hWnd, "�t�b�N�̐ݒ�Ɏ��s���܂����B\n�i�z�C�[���_�u���N���b�N�@�\�j\n�I�����܂��B", APPNAME, MB_OK);
				return false;
			}
		}

		// ���邭��}�E�X
		if(boolRoundMouse){
			fpHookRoundMouse = (HOOKROUNDMOUSE)GetProcAddress(hDllInst, "HookRoundMouse");

			if(fpHookRoundMouse != NULL) ret = (*fpHookRoundMouse)(hWnd);
			if(fpHookRoundMouse == NULL || ret != 1){
				MessageBox(hWnd, "�t�b�N�̐ݒ�Ɏ��s���܂����B\n�i���邭��}�E�X�@�\�j\n�I�����܂��B", APPNAME, MB_OK);
				return false;
			}
		}
	}

	// �^�N�X�g���C�A�C�R���̒ǉ�
	strcpy(m_nid.szTip, "SK Launch");
	m_nid.uFlags =				NIF_ICON | NIF_TIP | NIF_MESSAGE;
	m_nid.hIcon =				LoadIcon((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), "IDI_ICON_TASKTRAY");
	m_nid.hWnd =				hWnd;
	m_nid.uID =					1;
	m_nid.uCallbackMessage =	WM_TASKTRAY;
	m_nid.cbSize =				sizeof(m_nid);
	Shell_NotifyIcon(NIM_ADD, &m_nid);

	MemoryClean();		// ������������
	return(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �O���t�B�b�N�X�֘A�̏�����
//
bool InitGraphics(HWND hWnd)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);	// �C���X�^���X�n���h���̎擾
    HDC hDC=GetDC(hWnd);												// Window��DC���擾
	BITMAP bmp;
	HFONT hFontOld;

	// �o�b�N�o�b�t�@
	POINT pSc;											// ��ʃT�C�Y
    pSc.x = GetSystemMetrics(SM_CXSCREEN);
    pSc.y = GetSystemMetrics(SM_CYSCREEN);

    hBkBm = CreateCompatibleBitmap(hDC, pSc.x, pSc.y);	// BITMAP�쐬
    hBkDC = CreateCompatibleDC(hDC);					// DC�쐬
    SelectObject(hBkDC, hBkBm);							// hBkDC��hBkBm���Z�b�g

	// �t�H���g�w��
	hFont = CreateFont(
		(int)msgFontSize,			// �t�H���g����
		0,							// ������
		0,							// �e�L�X�g�̊p�x
		0,							// �x�[�X���C���Ƃ����Ƃ̊p�x
		FW_REGULAR,					// �t�H���g�̏d���i�����j
		FALSE,						// �C�^���b�N��
		FALSE,						// �A���_�[���C��
		FALSE,						// �ł�������
		SHIFTJIS_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x
		CLIP_DEFAULT_PRECIS,		// �N���b�s���O���x
		PROOF_QUALITY | ANTIALIASED_QUALITY,	// �o�͕i��
		DEFAULT_PITCH | FF_MODERN,	// �s�b�`�ƃt�@�~���[
		msgFont.c_str());			// ���̖�
	hFontOld = (HFONT)SelectObject(hBkDC, hFont);	// �t�H���g�̑I��
	DeleteObject(hFontOld);							// �O�̃t�H���g�̍폜

	SetTextColor(hBkDC, msgColor);					// �����F
	SetBkMode(hBkDC, TRANSPARENT);					// �w�i�͓���

	// �J�[�\���摜
	string path = GetAppDir();
	path.append("\\cur.bmp");
	hBmpCur = (HBITMAP)LoadImage(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hBmpCur == NULL) return false;

	GetObject(hBmpCur, sizeof(BITMAP), &bmp);
	curOffsetX = (int)(bmp.bmWidth / 4);
	curOffsetY = (int)(bmp.bmHeight / 2);

	hCur = CreateCompatibleDC(hDC);
	SelectObject(hCur, hBmpCur);
	curX = 0;
	curY = 0;
	bCurPaint = false;

	// ���b�Z�[�W�摜
	path = GetAppDir();
	path.append("\\msg.bmp");
	hBmpMsg = (HBITMAP)LoadImage(hInst, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hBmpMsg == NULL) return false;

	GetObject(hBmpMsg, sizeof(BITMAP), &bmp);
	msgLength = (int)(bmp.bmHeight / 3);

	hMsg = CreateCompatibleDC(hDC);
	SelectObject(hMsg, hBmpMsg);
	bMsgPaint = false;
	msgArea.left = 0;
	msgArea.right = 1;
	msgArea.top = 1;
	msgArea.bottom = 1;

	// hDC���
    ReleaseDC(hWnd, hDC);

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ϐ��̓ǂݍ���
//
void GetVar()
{
	string iniPath;
	iniPath = GetAppDir() + "\\" + INI_FILENAME;

	// ini�t�@�C�����Ȃ��ꍇ
	if( GetFileAttributes(iniPath.c_str()) == (DWORD)-1 )
		SetVar(true);

	center.x = GetIniInt(iniPath, "GENERAL", "CenterX", DEFAULT_CenterX);
	center.y = GetIniInt(iniPath, "GENERAL", "CenterY", DEFAULT_CenterY);
	// (0,0)�Ȃ��ʒ���
	if(center.x == DEFAULT_CenterX && center.y == DEFAULT_CenterY){
		center.x = (rScreen.right-rScreen.left) / 2;
		center.y = (rScreen.bottom-rScreen.top) / 2;
	}

	// �A�j���[�V����
	autoR = GetIniBool(iniPath, "GENERAL", "AutoR", DEFAULT_AutoR);				// �����Ŕ��a�𒲐����邩
	maxR = GetIniUCHAR(iniPath, "GENERAL", "MaxR", DEFAULT_MaxR);				// �ő�̔��a
	minR = GetIniUCHAR(iniPath, "GENERAL", "MinR", DEFAULT_MinR);				// �ŏ��̔��a
	iconItv = GetIniUCHAR(iniPath, "GENERAL", "IconItv", DEFAULT_IconItv);		// �A�C�R�����m�̊Ԋu
	roteRad = GetIniUCHAR(iniPath, "GENERAL", "RoteRad", DEFAULT_RoteRad);		// �A�j���[�V�����œ����p�x
	smooth = GetIniUCHAR(iniPath, "GENERAL", "Smooth", DEFAULT_Smooth);			// ���炩��
	roteTime = GetIniUCHAR(iniPath, "GENERAL", "RoteTime", DEFAULT_RoteTime);	// �A�j���[�V��������
	darkType = GetIniUCHAR(iniPath, "GENERAL", "DarkType", DEFAULT_DarkType);	// �A�j�����ɈÂ�����(0:�Â����Ȃ�, 1:�Ȉ�, 2:���掿)
	darkValue = GetIniUCHAR(iniPath, "GENERAL", "DarkValue", DEFAULT_DarkValue);	// �Â�����Z�x

	// ���b�Z�[�W
	boolMsg = GetIniBool(iniPath, "GENERAL", "boolMsg", DEFAULT_boolMsg);
	msgPos = GetIniUCHAR(iniPath, "GENERAL", "MsgPos",	DEFAULT_MsgPos);
	msgStyle = GetIniStr(iniPath, "GENERAL", "MsgStyle", "DEFAULT_MsgStyle");
	msgAlign = GetIniUCHAR(iniPath, "GENERAL", "MsgAlign", DEFAULT_MsgAlign);
	msgColor = GetIniInt(iniPath, "GENERAL", "MsgColor", DEFAULT_MsgColor);
	msgFont = GetIniStr(iniPath, "GENERAL", "MsgFont", "DEFAULT_MsgFont");
	msgFontSize = GetIniUCHAR(iniPath, "GENERAL", "MsgFontSize", DEFAULT_MsgFontSize);

	// �L�[�{�[�h
	hotKey.nVirtKey = GetIniInt(iniPath, "GENERAL", "HotKeyCode", DEFAULT_HotKeyCode);		// �z�b�g�L�[
	hotKey.mask = GetIniChar(iniPath, "GENERAL", "HotKeyShift", DEFAULT_HotKeyShift);	// �z�b�g�L�[�̃V�t�g

	for(int i=0; i<7; i++){
		// ShortKeyNumX
		string sNum = "ShortKeyNum";
		sNum.append(CStr(i));
		int num = GetIniInt(iniPath, "GENERAL", sNum, 0);
		if(num > 5) num = 5;

		if(num > 0){
			vector<Key> vKey;
			shortKey.push_back(vKey);
		}

		for(int j=0; j<num; j++){
			// shortKeyCodeXX
			string sCode = "ShortKeyCode";
			sCode.append(CStr(i));
			sCode.append(CStr(j));
			// shortKeyShiftXX
			string sShift = "ShortKeyShift";
			sShift.append(CStr(i));
			sShift.append(CStr(j));
			Key key;
			key.nVirtKey = GetIniInt(iniPath, "GENERAL", sCode, 0);
			key.mask = GetIniChar(iniPath, "GENERAL", sShift, MASK_NONE);
			shortKey.at(i).push_back( key );
		}
	}
	shortKeyPriority = GetIniBool(iniPath, "GENERAL", "ShortKeyPriority", DEFAULT_ShortKeyPriority);	// �W�����v�������D�悷�邩�ǂ���
	boolAccessKey = GetIniBool(iniPath, "GENERAL", "boolAccessKey", DEFAULT_boolAccessKey);				// �A�N�Z�X�L�[�̗L���^����

	mouseL = GetIniUCHAR(iniPath, "GENERAL", "MouseL", DEFAULT_MouseL);
	mouseM = GetIniUCHAR(iniPath, "GENERAL", "MouseM", DEFAULT_MouseM);
	mouseR = GetIniUCHAR(iniPath, "GENERAL", "MouseR", DEFAULT_MouseR);

	// SE
	SEHor = GetIniStr(iniPath, "GENERAL", "SEHor", "DEFAULT_SEHor");			// ��
	SEUp = GetIniStr(iniPath, "GENERAL", "SEUp", "DEFAULT_SEUp");				// ��
	SEDown = GetIniStr(iniPath, "GENERAL", "SEDown", "DEFAULT_SEDown");			// ��
	SEExe = GetIniStr(iniPath, "GENERAL", "SEExe", "DEFAULT_SEExe");			// ���s
	boolSEHor = GetIniBool(iniPath, "GENERAL", "boolSEHor", DEFAULT_boolSEHor);	// ���ʉ��̗L��
	boolSEUp = GetIniBool(iniPath, "GENERAL", "boolSEUp", DEFAULT_boolSEUp);
	boolSEDown = GetIniBool(iniPath, "GENERAL", "boolSEDown", DEFAULT_boolSEDown);
	boolSEExe = GetIniBool(iniPath, "GENERAL", "boolSEExe", DEFAULT_boolSEExe);

	// ����
	defaultPos = GetIniUCHAR(iniPath, "GENERAL", "DefaultPos", DEFAULT_DefaultPos);						// �N���������̏����ʒu
	hotKeyAction = GetIniUCHAR(iniPath, "GENERAL", "HotKeyAction", DEFAULT_HotKeyAction);				// �\�����Ƀz�b�g�L�[�����������̓���
	boolDeskTopClick = GetIniBool(iniPath, "GENERAL", "boolDeskTopClick", DEFAULT_boolDeskTopClick);	// �f�X�N�g�b�v���_�u���N���b�N�����Ƃ��ɋN�����邩�ǂ���
	boolRoundMouse = GetIniBool(iniPath, "GENERAL", "boolRoundMouse", DEFAULT_boolRoundMouse);			// ���邭��}�E�X
	intDBLClick = GetIniInt(iniPath, "GENERAL", "intDBLClick", DEFAULT_intDBLClick);					// ���_�u���N���b�N
	centerPos = GetIniUCHAR(iniPath, "GENERAL", "CenterPos", DEFAULT_CenterPos);						// �N���������̏����ʒu
	boolInvRote = GetIniBool(iniPath, "GENERAL", "boolInvRote", DEFAULT_boolInvRote);					// ��]���t�ɂ���
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ϐ��̏�������
//
void SetVar(bool bDefault)
{
	string iniPath;
	iniPath = GetAppDir() + "\\" + INI_FILENAME;

	if(bDefault){
		// �f�t�H���g�l�̐ݒ�
		PutIni(iniPath, "GENERAL", "CenterX", CStr(DEFAULT_CenterX));
		PutIni(iniPath, "GENERAL", "CenterY", CStr(DEFAULT_CenterX));

		PutIni(iniPath, "GENERAL", "AutoR",		CStr(DEFAULT_AutoR));
		PutIni(iniPath, "GENERAL", "MaxR",		CStr(DEFAULT_MaxR));
		PutIni(iniPath, "GENERAL", "MinR",		CStr(DEFAULT_MinR));
		PutIni(iniPath, "GENERAL", "IconItv",	CStr(DEFAULT_IconItv));
		PutIni(iniPath, "GENERAL", "Smooth",	CStr(DEFAULT_Smooth));
		PutIni(iniPath, "GENERAL", "RoteRad",	CStr(DEFAULT_RoteRad));
		PutIni(iniPath, "GENERAL", "RoteTime",	CStr(DEFAULT_RoteTime));
		PutIni(iniPath, "GENERAL", "DarkType",	CStr(DEFAULT_DarkType));
		PutIni(iniPath, "GENERAL", "DarkValue",	CStr(DEFAULT_DarkValue));

		PutIni(iniPath, "GENERAL", "boolMsg",		CStr(DEFAULT_boolMsg));
		PutIni(iniPath, "GENERAL", "MsgPos",		CStr(DEFAULT_MsgPos));
		PutIni(iniPath, "GENERAL", "MsgStyle",		DEFAULT_MsgStyle);
		PutIni(iniPath, "GENERAL", "MsgAlign",		CStr(DEFAULT_MsgAlign));
		PutIni(iniPath, "GENERAL", "MsgColor",		CStr(DEFAULT_MsgColor));
		PutIni(iniPath, "GENERAL", "MsgFont",		DEFAULT_MsgFont);
		PutIni(iniPath, "GENERAL", "MsgFontSize",	CStr(DEFAULT_MsgFontSize));

		PutIni(iniPath, "GENERAL", "HotKeyCode",	CStr(DEFAULT_HotKeyCode));
		PutIni(iniPath, "GENERAL", "HotKeyShift",	CStr(DEFAULT_HotKeyShift));

		PutIni(iniPath, "GENERAL", "ShortKeyNum0",		CStr(DEFAULT_ShortKeyNum0));
		PutIni(iniPath, "GENERAL", "ShortKeyCode00",	CStr(DEFAULT_ShortKeyCode00));
		PutIni(iniPath, "GENERAL", "ShortKeyShift00",	CStr(DEFAULT_ShortKeyShift00));
		PutIni(iniPath, "GENERAL", "ShortKeyNum1",		CStr(DEFAULT_ShortKeyNum1));
		PutIni(iniPath, "GENERAL", "ShortKeyCode10",	CStr(DEFAULT_ShortKeyCode10));
		PutIni(iniPath, "GENERAL", "ShortKeyShift10",	CStr(DEFAULT_ShortKeyShift10));
		PutIni(iniPath, "GENERAL", "ShortKeyNum2",		CStr(DEFAULT_ShortKeyNum2));
		PutIni(iniPath, "GENERAL", "ShortKeyCode20",	CStr(DEFAULT_ShortKeyCode20));
		PutIni(iniPath, "GENERAL", "ShortKeyShift20",	CStr(DEFAULT_ShortKeyShift20));
		PutIni(iniPath, "GENERAL", "ShortKeyNum3",		CStr(DEFAULT_ShortKeyNum3));
		PutIni(iniPath, "GENERAL", "ShortKeyCode30",	CStr(DEFAULT_ShortKeyCode30));
		PutIni(iniPath, "GENERAL", "ShortKeyShift30",	CStr(DEFAULT_ShortKeyShift30));
		PutIni(iniPath, "GENERAL", "ShortKeyNum4",		CStr(DEFAULT_ShortKeyNum4));
		PutIni(iniPath, "GENERAL", "ShortKeyCode40",	CStr(DEFAULT_ShortKeyCode40));
		PutIni(iniPath, "GENERAL", "ShortKeyShift40",	CStr(DEFAULT_ShortKeyShift40));
		PutIni(iniPath, "GENERAL", "ShortKeyNum5",		CStr(DEFAULT_ShortKeyNum5));
		PutIni(iniPath, "GENERAL", "ShortKeyCode50",	CStr(DEFAULT_ShortKeyCode50));
		PutIni(iniPath, "GENERAL", "ShortKeyShift50",	CStr(DEFAULT_ShortKeyShift50));
		PutIni(iniPath, "GENERAL", "ShortKeyNum6",		CStr(DEFAULT_ShortKeyNum6));
		PutIni(iniPath, "GENERAL", "ShortKeyCode60",	CStr(DEFAULT_ShortKeyCode60));
		PutIni(iniPath, "GENERAL", "ShortKeyShift60",	CStr(DEFAULT_ShortKeyShift60));
		PutIni(iniPath, "GENERAL", "ShortKeyPriority",	CStr(DEFAULT_ShortKeyPriority));
		PutIni(iniPath, "GENERAL", "boolAccessKey",		CStr(DEFAULT_boolAccessKey));

		PutIni(iniPath, "GENERAL", "SEHor",			DEFAULT_SEHor);
		PutIni(iniPath, "GENERAL", "SEUp",			DEFAULT_SEUp);
		PutIni(iniPath, "GENERAL", "SEDown",		DEFAULT_SEDown);
		PutIni(iniPath, "GENERAL", "SEExe",			DEFAULT_SEExe);
		PutIni(iniPath, "GENERAL", "boolSEHor",		CStr(DEFAULT_boolSEHor));
		PutIni(iniPath, "GENERAL", "boolSEUp",		CStr(DEFAULT_boolSEUp));
		PutIni(iniPath, "GENERAL", "boolSEDown",	CStr(DEFAULT_boolSEDown));
		PutIni(iniPath, "GENERAL", "boolSEExe",		CStr(DEFAULT_boolSEExe));

		PutIni(iniPath, "GENERAL", "DefaultPos",		CStr(DEFAULT_DefaultPos));
		PutIni(iniPath, "GENERAL", "HotKeyAction",		CStr(DEFAULT_HotKeyAction));
		PutIni(iniPath, "GENERAL", "boolDeskTopClick",	CStr(DEFAULT_boolDeskTopClick));
		PutIni(iniPath, "GENERAL", "boolRoundMouse",	CStr(DEFAULT_boolRoundMouse));
		PutIni(iniPath, "GENERAL", "intDBLClick",		CStr(DEFAULT_intDBLClick));
		PutIni(iniPath, "GENERAL", "centerPos",			CStr(DEFAULT_CenterPos));
		PutIni(iniPath, "GENERAL", "boolInvRote",		CStr(DEFAULT_boolInvRote));
	}else{
		// �ʏ폑������
		PutIni(iniPath, "GENERAL", "CenterX", CStr(center.x));
		PutIni(iniPath, "GENERAL", "CenterY", CStr(center.y));

		PutIni(iniPath, "GENERAL", "AutoR",		CStr(autoR));
		PutIni(iniPath, "GENERAL", "MaxR",		CStr(maxR));
		PutIni(iniPath, "GENERAL", "MinR",		CStr(minR));
		PutIni(iniPath, "GENERAL", "IconItv",	CStr(iconItv));
		PutIni(iniPath, "GENERAL", "Smooth",	CStr(smooth));
		PutIni(iniPath, "GENERAL", "RoteRad",	CStr(roteRad));
		PutIni(iniPath, "GENERAL", "RoteTime",	CStr(roteTime));

		PutIni(iniPath, "GENERAL", "boolMsg",		CStr(boolMsg));
		PutIni(iniPath, "GENERAL", "MsgPos",		CStr(msgPos));
		PutIni(iniPath, "GENERAL", "MsgStyle",		msgStyle);
		PutIni(iniPath, "GENERAL", "MsgAlign",		CStr(msgAlign));
		PutIni(iniPath, "GENERAL", "MsgColor",		CStr(msgColor));
		PutIni(iniPath, "GENERAL", "MsgFont",		msgFont);
		PutIni(iniPath, "GENERAL", "MsgFontSize",	CStr(msgFontSize));

		PutIni(iniPath, "GENERAL", "HotKeyCode",	CStr(hotKey.nVirtKey));
		PutIni(iniPath, "GENERAL", "HotKeyShift",	CStr(hotKey.mask));

		for(int i=0; i<7; i++){
			int num=shortKey.at(i).size();
			string sNum = "ShortKeyNum";
			sNum.append(CStr(i));
			PutIni(iniPath, "GENERAL", sNum, CStr(num));

			for(int j=0; j<num; j++){
				string sCode = "ShortKeyCode";
				sCode.append(CStr(i));
				sCode.append(CStr(j));
				PutIni(iniPath, "GENERAL", sCode, CStr(shortKey.at(i).at(j).nVirtKey));
				string sShift = "ShortKeyShift";
				sShift.append(CStr(i));
				sShift.append(CStr(j));
				PutIni(iniPath, "GENERAL", sShift, CStr(shortKey.at(i).at(j).mask));
			}
		}
		PutIni(iniPath, "GENERAL", "ShortKeyPriority",	CStr(shortKeyPriority));
		PutIni(iniPath, "GENERAL", "boolAccessKey",		CStr(boolAccessKey));

		PutIni(iniPath, "GENERAL", "SEHor",			SEHor);
		PutIni(iniPath, "GENERAL", "SEUp",			SEUp);
		PutIni(iniPath, "GENERAL", "SEDown",		SEDown);
		PutIni(iniPath, "GENERAL", "SEExe",			SEExe);
		PutIni(iniPath, "GENERAL", "boolSEHor",		CStr(boolSEHor));
		PutIni(iniPath, "GENERAL", "boolSEUp",		CStr(boolSEUp));
		PutIni(iniPath, "GENERAL", "boolSEDown",	CStr(boolSEDown));
		PutIni(iniPath, "GENERAL", "boolSEExe",		CStr(boolSEExe));

		PutIni(iniPath, "GENERAL", "DefaultPos",		CStr(defaultPos));
		PutIni(iniPath, "GENERAL", "HotKeyAction",		CStr(hotKeyAction));
		PutIni(iniPath, "GENERAL", "boolDeskTopClick",	CStr(boolDeskTopClick));
		PutIni(iniPath, "GENERAL", "boolRoundMouse",	CStr(boolRoundMouse));
		PutIni(iniPath, "GENERAL", "intDBLClick",		CStr(intDBLClick));
		PutIni(iniPath, "GENERAL", "centerPos",			CStr(centerPos));
		PutIni(iniPath, "GENERAL", "boolInvRote",		CStr(boolInvRote));
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XML�t�@�C������ǂݍ���
//
bool GetFromXML()
{
	string xmlPath;
	xmlPath = GetAppDir() + "\\" + XML_FILENAME;

	// XML�t�@�C���̃I�[�v��
	FILE *fp;
	if ((fp = fopen(xmlPath.c_str(), "rb")) == NULL) return false;

	// �T�C�Y�̎擾(st.st_size)
	struct stat st;
	fstat(fileno(fp), &st);
	const long size = st.st_size;
	// �f�[�^���擾
	char *data = new char[st.st_size];
	if( fread( data, sizeof(char), st.st_size, fp ) == 0) return false;

	// �t�@�C���̃N���[�Y
	fclose(fp);

	// �O���[�v�A�C�e���̎擾
	int pos1=0, pos2=0;
	string xmlData, buff;
	xmlData.assign(data);

	while(1){
		// pos1, pos1�̐ݒ�
		pos1 = xmlData.find("<GroupItem>", pos2);
		if(pos1 == string::npos) break;			// �Ȃ��ꍇ�I��
		buff = "</GroupItem>";
		pos2 = xmlData.find(buff, pos1);
		if(pos2 == string::npos) return false;	// �Ȃ��ꍇ���s
		pos2 += buff.length();

		SKGroupItem group;
		if( !group.getFromXML(xmlData.substr(pos1, pos2-pos1)) ){
			GI.clear();
			return false;
		}
		group.orderIcon(autoR, minR, maxR, iconItv);
		GI.push_back(group);
	}

	delete[] data;

	// �O���[�v������Ȃ琬��
	if(!GI.empty())	return true;
	else			return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e����\��
//
void SKShow(HWND hWnd)
{
	rScreen = GetScreenSize();				// ��ʃT�C�Y���X�V

	if(centerPos == (UCHAR)2){
		GetCursorPos(&center);
		int r = (int)GI.at(iGI).selectedItem().getR();
		// X���W�̔���
		if(center.x < rScreen.left + r + ICON_OFFSET)	center.x = rScreen.left + r + ICON_OFFSET;
		else if(center.x > rScreen.right - r - ICON_OFFSET)	center.x = rScreen.right - r - ICON_OFFSET;
		// Y���W�̔���
		switch(msgPos){
		case 0:					// MSG��
			if(center.y < rScreen.top + r + MSG_OFFSET + ICON_OFFSET)
				center.y = rScreen.top + r + MSG_OFFSET + ICON_OFFSET;
			else if(center.y > rScreen.bottom - r)
				center.y = rScreen.bottom - r;
			break;
		case 1:					// MSG��
			if(center.y < rScreen.top + r)
				center.y = rScreen.top + r;
			else if(center.y > rScreen.bottom - r)
				center.y = rScreen.bottom - r;
			break;
		case 2:					// MSG��
			if(center.y < rScreen.top + r)
				center.y = rScreen.top + r;
			else if(center.y > rScreen.bottom - r - MSG_OFFSET - ICON_OFFSET)
				center.y = rScreen.bottom - r - MSG_OFFSET - ICON_OFFSET;
			break;
		}
	}

//	SetTextColor(hBkDC, msgColor);					// �����F�̍X�V�i���X���ɂȂ邩��j
	hDskDC = CreateDeskCapDC();						// �f�X�N�g�b�v�L���v�`��
	DrawDark(hDskDC, 2, 50);						// �Â�����

	// �\��
	SetForegroundWindow2(hWnd);
	SetWindowPos(hWnd, HWND_TOPMOST, rScreen.left, rScreen.top, rScreen.right-rScreen.left, rScreen.bottom-rScreen.top, SWP_SHOWWINDOW);

	bShow = true;									// �t���O�n�m
	ChangeGroup(hWnd, NULL, false, true, true);		// �\��
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e�����B��
//
void SKHide(HWND hWnd, bool animation)
{
	// �A�j���[�V����
	if(animation) ChangeGroup(hWnd, NULL, true, true, true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �O���[�v�̕ύX���A�j���[�V����
//
void ChangeGroup(HWND hWnd, int index, bool bOut, bool bClockwise, bool bOnlyAnime)
{
	// �A�j���[�V���������Ȃ�
	if(bOnlyAnime){
		if(!bOut)	AnimateGroup(hWnd, false, bOut, bClockwise, TIMER_SHOW);
		else		AnimateGroup(hWnd, false, bOut, bClockwise, TIMER_HIDE);

	}else{

		int i = (int)index;
		int size = (int)GI.size();

		// index�̏C��
		while(i<0 || i>=size){
			if(i<0)				i += size;
			else if(i>=size)	i -= size;
			else break;
		}
		timerIndex = (UINT)i;

		// �A�j���[�V����
		AnimateGroup(hWnd, !bOut, bOut, bClockwise, TIMER_GROUP1);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �O���[�v�A�j���[�V����
//
void AnimateGroup(HWND hWnd, bool bCenter, bool bOut, bool bClockwise, UINT uiTimerType)
{
	int fps;						// �A�j���[�V������FPS
	int time;						// �A�j���[�V�������鎞��(�~���b)
	int frame;						// �A�j���[�V�����̃R�}��
	int perFrame;				// �P�R�}������̎���(�~���b)

	int length;						// �A�j���[�V�������铮�a��(�ő�)����
	if(rScreen.right-rScreen.left > rScreen.bottom-rScreen.top){
		length = (rScreen.right-rScreen.left);
		if(center.x*2 < length)	length -= center.x;
		else					length = center.x;
	}else{
		length = (rScreen.bottom-rScreen.top);
		if(center.y*2 < length)	length -= center.y;
		else					length = center.y;
	}

	// (smooth,FPS) = (0,10) �` (100, 70)
	fps = (int)( (double)smooth * 0.6 + 10 );
	// (roteTime, time) = (0, 250%) �` (100, 50%)
	time= 100 - (int)roteTime;
	time= (int)( (double)(time*time)*0.02 ) + 50;

	if(bCenter){
		// ���S �` �ʏ�ʒu
		
		// frame, timerPerFrame�̐ݒ�
		time = (int)((double)ANIMATION_IN_TIME*((double)time / 100));	// �A�j���[�V�����̎���
		frame = (int)(fps * time / 1000);								// ����`�悷�邩
		if(frame < 2) frame = 2;
		perFrame = (int)(time / frame) - 10;							// �P�R�}������̎���
		if(perFrame > 1)	timerPerFrame = perFrame;
		else				timerPerFrame = 1;

		// r�̐ݒ�
		timerR = GI.at(iGI).selectedItem().getR() / frame;
		if(!bOut) timerR = -timerR;					// �������ւȂ�}�C�i�X

		// theta�̐ݒ�
		timerTheta = 0;

	}else{
		// �ʏ�ʒu �` �O

		// frame, timerPerFrame�̐ݒ�
		time = (int)((double)ANIMATION_OUT_TIME*((double)time / 100));	// �A�j���[�V�����̎���
		frame = (int)(fps * time / 1000);								// ����`�悷�邩
		if(frame < 2) frame = 2;
		perFrame = (int)(time / frame) - 10;							// �P�R�}������̎���
		if(perFrame > 1)	timerPerFrame = perFrame;
		else				timerPerFrame = 1;

		// r�̐ݒ�
		timerR = ((float)length - GI.at(iGI).selectedItem().getR()) / frame;
		if(!bOut) timerR = -timerR;							// �������ւȂ�}�C�i�X

		// theta�̐ݒ� ( roteRad,theta(�S��) ) = (0,0)�`(100,PI)
		timerTheta = (float)((double)roteRad * PI/100) / frame;
		if(!bClockwise) timerTheta = -timerTheta;			// �����v���Ȃ�}�C�i�X
	}

	// �ʏ�ʒu�Ɍ������A�j���[�V�����Ȃ�J�n�ʒu�Ɉړ�
	if(!(bCenter^bOut)) GI.at(iGI).moveIcon(-(timerR*frame), -(timerTheta*frame));

	timerHWnd = hWnd;
	timerType = uiTimerType;
	timerCount = (UINT)0;
	timerMaxCount = (UINT)frame;
	timerOut = bOut;
	timerClockwise = bClockwise;
	bActing = true;

	bCurPaint = false;	// �J�[�\���̕`��ݒ�
	bMsgPaint = false;	// ���b�Z�[�W�̕`��ݒ�
	// �����ׂɈ�x�ĕ`��
	HRGN invalidRgn = CreateRectRgn(0,0,0,0);
	InvalidateCursor(hWnd, invalidRgn);
	InvalidateMessage(hWnd, invalidRgn);
	InvalidateRgn(hWnd, invalidRgn, FALSE);
	DeleteObject(invalidRgn);

	// �A�j���[�V�����J�n
	timeBeginPeriod(1);
	timerID = timeSetEvent(timerPerFrame, 1, TimerProc, 0, TIME_ONESHOT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���̕ύX���A�j���[�V����
//
void ChangeItem(HWND hWnd, int index, bool direction)
{
	int i = index;
	int size = GI.at(iGI).size();
	bool clockwise;

	if(direction){
		if(index < (int)GI.at(iGI).getItemIndex())
			clockwise = true;
		else
			clockwise = false;
	}

	// index�̏C��
	while(i<0 || i>=size){
		if(i<0)				i += size;
		else if(i>=size)	i -= size;
		else break;
	}
	timerIndex = (UINT)i;

	// �A�j���[�V����
	AnimateItem(hWnd, timerIndex, direction, clockwise);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���A�j���[�V����
//
void AnimateItem(HWND hWnd, UINT index, bool direction, bool clockwise)
{
	int fps;						// �A�j���[�V������FPS
	int time;						// �A�j���[�V�������鎞��(�~���b)
	int frame;						// �A�j���[�V�����̃R�}��
	int perFrame;				// �P�R�}������̎���(�~���b)

	// (smooth,FPS) = (0,10) �` (100, 70)
	fps = (int)( (double)smooth * 0.6 + 10 );
	// (roteTime, time) = (0, 250%) �` (100, 50%)
	time= 100 - (int)roteTime;
	time= (int)( (double)(time*time)*0.02 ) + 50;

	// frame, timerPerFrame�̐ݒ�
	time = (int)((double)ANIMATION_HOR_TIME*((double)time / 100));	// �A�j���[�V�����̎���
	frame = (int)(fps * time / 1000);								// ����`�悷�邩
	if(frame < 2) frame = 2;
	perFrame = (int)(time / frame) - 10;							// �P�R�}������̎���
	if(perFrame > 1)	timerPerFrame = perFrame;
	else				timerPerFrame = 1;

	// r�̐ݒ�
	timerR = 0;

	// theta�̐ݒ�
	if(direction){
		timerTheta = GI.at(iGI).selectedItem().getTheta() - GI.at(iGI).item(index).getTheta();
		if(clockwise && timerTheta<=0)			timerTheta = timerTheta + 2*PI;
		else if(!clockwise && timerTheta>=0)	timerTheta = timerTheta - 2*PI;

	}else{
		timerTheta = TrimMPItoPI(GI.at(iGI).selectedItem().getTheta() - GI.at(iGI).item(index).getTheta());
	}
	timerTheta /= frame;

	timerHWnd = hWnd;
	timerType = TIMER_ITEM;
	timerCount = (UINT)0;
	timerMaxCount = (UINT)frame;
	bActing = true;

	bCurPaint = true;	// �J�[�\���̕`��ݒ�
	bMsgPaint = true;	// ���b�Z�[�W�̕`��ݒ�

	// �A�j���[�V�����J�n
	timeBeginPeriod(1);
	timerID = timeSetEvent(timerPerFrame, 1, TimerProc, 0, TIME_ONESHOT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �`��
//
void DrawWindow(HWND hWnd)
{
	// �`��̈�̃T�C�Y
	int width, height;
	width = rScreen.right-rScreen.left;
	height = rScreen.bottom-rScreen.top;

	PAINTSTRUCT ps;

	// �w�i�`��
	BitBlt(hBkDC, 0, 0, width, height, 
		hDskDC, rScreen.left, rScreen.top, SRCCOPY);
	// �A�C�e���`��
	GI.at(iGI).drawIcon(hBkDC, &center);
	// �J�[�\���`��
	if(bCurPaint) DrawCursor(hWnd, hBkDC);
	// ���b�Z�[�W�`��
	if(boolMsg && bMsgPaint) DrawMessage(hWnd, hBkDC);

	BeginPaint(hWnd, &ps);				// �`��J�n

	// �o�b�N�o�b�t�@��ps.hdc�ɓ]��
	BitBlt(ps.hdc, 0, 0, width, height, hBkDC, 0, 0, SRCCOPY);

	EndPaint(hWnd, &ps);				// �`��I��
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �^�C�}�[�p�֐�(�^�C�}�[ID,�\��,���[�U�[��`,�\��,�\��)
//
void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	HRGN invalidRgn;
	bool bKeyMore = false;
	int ret;
//*
	// �^�C�}�[������
	timeKillEvent(timerID);
	timeEndPeriod(1);
//*/
	timerCount++;

	// �ĕ`�惊�[�W�����̏�����
	invalidRgn = CreateRectRgn(0,0,0,0);

	GI.at(iGI).moveDrawIcon(timerHWnd, invalidRgn, &center, true, 
		timerR, timerTheta);											// �A�C�e���ړ����`��
	if(bCurPaint) InvalidateCursor(timerHWnd, invalidRgn);				// �J�[�\��
	if(boolMsg && bMsgPaint) InvalidateMessage(timerHWnd, invalidRgn);	// ���b�Z�[�W

	if(timerCount < timerMaxCount && timerHWnd != NULL){
		// �Ō�łȂ��Ȃ�

		// �ĕ`��
		InvalidateRgn(timerHWnd, invalidRgn, FALSE);
		DeleteObject(invalidRgn);
		UpdateWindow(timerHWnd);
//*
		// �A�j���[�V�����ĊJ
		timeBeginPeriod(1);
		timerID = timeSetEvent(timerPerFrame, 1, TimerProc, 0, TIME_ONESHOT);
//*/
	}else{
		// �Ō�Ȃ�
/*
		// �^�C�}�[������
		timeKillEvent(timerID);
		timeEndPeriod(1);
//*/
		// ���̃^�C�}�[���ŕ���
		switch(timerType){
		// �A�C�e��
		case TIMER_ITEM:
			// �A�C�e���͈ړ����SE
			if(boolSEHor) sndPlaySound(SEHor.c_str(), SND_ASYNC | SND_NODEFAULT);
			// index��I��
			GI.at(iGI).setItemIndex(timerIndex);

			bActing = false;
			// ���b�Z�[�W�̕`��ݒ�
			if(boolMsg){
				bMsgPaint = true;
				RenewMessage(timerHWnd, hBkDC, invalidRgn);
			}

			// ���̃A�N�V��������
			bKeyMore = true;
			break;

		// �O���[�v�i�P�j
		case TIMER_GROUP1:
			// �ʒu�̏C��
			GI.at(iGI).moveDrawIcon(timerHWnd, invalidRgn, &center, false, 
				-(timerR*timerMaxCount), -(timerTheta*timerMaxCount));
			// (�`���ɂQ�̃A�j���[�V����)
			break;

		// �O���[�v�i�Q�j
		case TIMER_GROUP2:
			bActing = false;
			// �J�[�\���̕`��ݒ�
			bCurPaint = true;
			RenewCursor(timerHWnd, invalidRgn);

			// ���b�Z�[�W�̕`��ݒ�
			if(boolMsg){
				bMsgPaint = true;
				RenewMessage(timerHWnd, hBkDC, invalidRgn);
			}

			// ���̃A�N�V��������
			bKeyMore = true;
			break;

		// �\��
		case TIMER_SHOW:
			bActing = false;
			// �J�[�\���̕`��ݒ�
			bCurPaint = true;
			RenewCursor(timerHWnd, invalidRgn);

			// ���b�Z�[�W�̕`��ݒ�
			if(boolMsg){
				bMsgPaint = true;
				RenewMessage(timerHWnd, hBkDC, invalidRgn);
			}

			// ���̃A�N�V��������
			bKeyMore = true;
			break;

		// �B��
		case TIMER_HIDE:
		case TIMER_FILEOPEN:
		case TIMER_FOLDEROPEN:
			// �ʒu�̏C��
			GI.at(iGI).moveDrawIcon(timerHWnd, invalidRgn, &center, false, 
				-(timerR*timerMaxCount), -(timerTheta*timerMaxCount));
			bActing = false;

			// �܂��A�N�V����������Ȃ�N���A
			while(!queKey.empty()) queKey.pop();

			DeleteDC(hDskDC);					// hDskDC�폜
			ShowWindow(timerHWnd, SW_HIDE);		// �\���̐ݒ�
			UpdateWindow(timerHWnd);			// �X�V
			bShow = false;						// �t���O�n�e�e
			
			// TIMER_HIDE�ȊO��OPEN�n�Ȃ�J��
			if(timerType != TIMER_HIDE){
				if(timerType == TIMER_FILEOPEN)
					ret = GI.at(iGI).selectedItem().fileOpen();
				else
					ret = GI.at(iGI).selectedItem().folderOpen();

				if(ret!=0) MessageBox(timerHWnd, openError(ret).c_str(), "ERROR", MB_OK);
			}

			// �\���������ʒu�ɖ߂��Ȃ�
			if(defaultPos == 1){
				iGI = 0;
				int size = GI.size();
				for(int i=0; i<size; i++){
					GI.at(i).moveIcon(0, GI.at(i).selectedItem().getTheta() - GI.at(i).item(0).getTheta());
					GI.at(i).setItemIndex(0);
				}
			}
			break;
		}

		// �ĕ`��
		InvalidateRgn(timerHWnd, invalidRgn, FALSE);
		DeleteObject(invalidRgn);
		UpdateWindow(timerHWnd);

		MemoryClean();

		// �O���[�v�P�Ȃ玟�ɂQ�̃A�j���[�V����
		if(timerType == TIMER_GROUP1){
			// index��I��
			iGI = timerIndex;
			AnimateGroup(timerHWnd, timerOut, timerOut, timerClockwise, TIMER_GROUP2);
		}

		// �܂��A�N�V����������Ȃ�
		if(bKeyMore && !queKey.empty()){
			// �L�[����
			SKKeySearch(timerHWnd, queKey.front());
			// �擪�̃L�[���폜
			queKey.pop();
		}
/*
		// HWnd��NULL
		timerHWnd = NULL;
//*/
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �X�^�[�g�A�b�v�ɓo�^
//
void CreateStartup(HWND hWnd)
{
	HRESULT ret;
	string msg = "";

	msg.append("�ȉ��̃p�X\n");
	msg.append(GetFolderPath(CSIDL_STARTUP) + "\n");
	msg.append("�ɃV���[�g�J�b�g�t�@�C�����쐬���AWindows�N�����Ɏ������s����悤�ɂ��܂��B\n\n");
	msg.append("��낵���ł����H");

	if(MessageBox(hWnd, msg.c_str(), APPNAME, MB_OKCANCEL | MB_ICONQUESTION) == IDOK){

		string path = GetFolderPath(CSIDL_STARTUP) + "\\SK Launch.lnk";
		ret = CreateShortcutFile(path, GetAppPath(), GetAppDir(), "", "", SW_SHOWNORMAL, "", 0);

		if( SUCCEEDED(ret) )	MessageBox(hWnd, "�V���[�g�J�b�g�t�@�C�����쐬���܂����B", APPNAME, MB_OK);
		else					MessageBox(hWnd, "�쐬�Ɏ��s���܂����B", APPNAME, MB_OK);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �V���[�g�J�b�g�t�@�C���̏o��
//
void CreateShortcutFiles(HWND hWnd)
{
	// �f�X�N�g�b�v�̈ʒu
	string dskPath = GetFolderPath(CSIDL_DESKTOP) + "\\SKL Export";
	string msg = "";

	msg.append("�f�X�N�g�b�v�֎��̃p�X\n");
	msg.append(dskPath + "\n");
	msg.append("�̂悤�Ƀt�H���_���쐬���A�����֑S�A�C�e����\n");
	msg.append("�V���[�g�J�b�g�t�@�C���Ƃ��ďo�͂��܂��B\n\n");
	msg.append("��낵���ł����H");

	if(MessageBox(hWnd, msg.c_str(), APPNAME, MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
		// ���[�g�t�H���_�쐬
		CreateDirectory( dskPath.c_str(), NULL );

		int gSize = GI.size();
		for(int i=0; i<gSize; i++){
			SKGroupItem& group = GI.at(i);
			string groupPath = dskPath + "\\" + FileNameFilter(group.getCaption());
			// �t�H���_�쐬
			CreateDirectory(groupPath.c_str(), NULL);

			int iSize = group.size();
			for(int j=0; j<iSize; j++){
				// �V���[�g�J�b�g�t�@�C���쐬
				if( FAILED(CreateShortcutFileFromItem(groupPath, group.item(j))) ){
					MessageBox(hWnd, "���s���܂����B", APPNAME, MB_OK);
					return;
				}
			}
		}
		MessageBox(hWnd, "�������܂����B", APPNAME, MB_OK);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SKShortcutItem����V���[�g�J�b�g�t�@�C�����쐬
//
HRESULT CreateShortcutFileFromItem(const string& path, const SKShortcutItem& item)
{
	string lnkPath = "";
	lnkPath.append(path);
	lnkPath.append("\\" + FileNameFilter(item.getCaption()) + ".lnk");

	return CreateShortcutFile(
		lnkPath, item.getPath(), item.getDirPath(),
		item.getParameter(), item.getComment(), item.getShowCmd(),
		item.getIconPath(), item.getIconIndex());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �J�[�\���̕`��ʒu���X�V
//
void RenewCursor(HWND hWnd, HRGN invalidRgn)
{
	// �`��ʒu
	curX = center.x - curOffsetX;
	curY = center.y - curOffsetY - (int)GI.at(iGI).item(0).getR();
	InvalidateCursor(hWnd, invalidRgn);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �J�[�\���̕`��̈���X�V
//
void InvalidateCursor(HWND hWnd, HRGN invalidRgn)
{
	HRGN addRgn = NULL;
	// �ǉ�����͈�
	addRgn = CreateRectRgn(curX, curY, curX + curOffsetX*2, curY + curOffsetY*2);
	// �ĕ`��͈͂Ƀv���X
	CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
	// �s�v�ȃ��[�W�����폜
	DeleteObject(addRgn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �J�[�\���̕`��
//
void DrawCursor(HWND hWnd, HDC hDC)
{
	// �}�X�N�̕`��
	BitBlt(hDC, curX, curY, curOffsetX*2, curOffsetY*2, hCur, curOffsetX*2, 0, SRCAND);
	// ���ۂ̊G�̕`��
	BitBlt(hDC, curX, curY, curOffsetX*2, curOffsetY*2, hCur, 0, 0, SRCPAINT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�Z�[�W�̊e��ϐ����X�V
//
void RenewMessage(HWND hWnd, HDC hDC, HRGN invalidRgn)
{
	int ox = 0, oy;

	InvalidateMessage(hWnd, invalidRgn);

	sMsg = EncordMessage();

	// �����̕`��T�C�Y
	while(ox == 0 || msgArea.right > rScreen.right || msgArea.bottom > rScreen.right){
		msgArea.left = 0;
		msgArea.right = 0;
		msgArea.top = 0;
		msgArea.bottom = 0;
		DrawText(hDC, sMsg.c_str(), sMsg.length(), &msgArea, DT_CALCRECT);
		ox++;
		if(ox>1) break;
	}

	// length�ɍ��킹��
	msgArea.right += msgLength - (msgArea.right % msgLength);
	msgArea.bottom += msgLength - (msgArea.bottom % msgLength);

	// �`��ʒu
	ox = (int)center.x - (int)(msgArea.right/2);
	oy = (int)center.y + (msgPos-1) * (GI.at(iGI).item(0).getR() + MSG_OFFSET) - (int)msgArea.bottom + ICON_OFFSET;
	msgArea.left += ox - msgLength;
	msgArea.right += ox + msgLength;
	msgArea.top += oy - msgLength;
	msgArea.bottom += oy + msgLength;

	InvalidateMessage(hWnd, invalidRgn);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�Z�[�W�̓��e���擾
//
string EncordMessage()
{
	string ret(msgStyle);
	StringReplaceAll(ret, "%RETURN%",			"\n");
	StringReplaceAll(ret, "%SPACE%",			" ");
	StringReplaceAll(ret, "%GROUP_CAPTION%",	GI.at(iGI).getCaption());
	StringReplaceAll(ret, "%GROUP_COMMENT%",	GI.at(iGI).getComment());
	StringReplaceAll(ret, "%GROUP_KEYCHAR%",	GetKeyName(GI.at(iGI).getKeyChar(), GI.at(iGI).getKeyShift()));
	StringReplaceAll(ret, "%ITEM_CAPTION%",		GI.at(iGI).selectedItem().getCaption());
	StringReplaceAll(ret, "%ITEM_COMMENT%",		GI.at(iGI).selectedItem().getComment());
	StringReplaceAll(ret, "%ITEM_PATH%",		GI.at(iGI).selectedItem().getPath());
	StringReplaceAll(ret, "%ITEM_DIRPATH%",		GI.at(iGI).selectedItem().getDirPath());
	StringReplaceAll(ret, "%ITEM_PARAMETER%",	GI.at(iGI).selectedItem().getParameter());
	StringReplaceAll(ret, "%ITEM_SHOWCMD%",		GetShowCmdName(GI.at(iGI).selectedItem().getShowCmd()));
	StringReplaceAll(ret, "%ITEM_KEYCHAR%",		GetKeyName(GI.at(iGI).selectedItem().getKeyChar(), GI.at(iGI).selectedItem().getKeyShift()));

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�Z�[�W�w�i�̕`��̈���X�V
//
void InvalidateMessage(HWND hWnd, HRGN invalidRgn)
{
	HRGN addRgn;
	// �ǉ�����͈�
	addRgn = CreateRectRgn(msgArea.left, msgArea.top, msgArea.right, msgArea.bottom);
	// �ĕ`��͈͂Ƀv���X
	CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
	// �s�v�ȃ��[�W�����폜
	DeleteObject(addRgn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���b�Z�[�W�w�i�̕`��
//
void DrawMessage(HWND hWnd, HDC hDC)
{
	int x, y, x1, y1, x2, y2, len2, len4;
	UINT format;

	x1 = (int)msgArea.left + msgLength;
	y1 = (int)msgArea.top + msgLength;
	x2 = (int)msgArea.right - msgLength;
	y2 = (int)msgArea.bottom - msgLength;
	len2 = 2*msgLength;
	len4 = 4*msgLength;

	// �p�̕`��

	// �}�X�N
	BitBlt(hDC, (int)msgArea.left,	(int)msgArea.top,	msgLength, msgLength, hMsg, 3*msgLength, 0,		SRCAND);
	BitBlt(hDC, x2,					(int)msgArea.top,	msgLength, msgLength, hMsg, 5*msgLength, 0,		SRCAND);
	BitBlt(hDC, (int)msgArea.left,	y2,					msgLength, msgLength, hMsg, 3*msgLength, len2,	SRCAND);
	BitBlt(hDC, x2,					y2,					msgLength, msgLength, hMsg, 5*msgLength, len2,	SRCAND);
	// ���ۂ̊G
	BitBlt(hDC, (int)msgArea.left,	(int)msgArea.top,	msgLength, msgLength, hMsg, 0,		0,		SRCPAINT);
	BitBlt(hDC, x2,					(int)msgArea.top,	msgLength, msgLength, hMsg, len2,	0,		SRCPAINT);
	BitBlt(hDC, (int)msgArea.left,	y2,					msgLength, msgLength, hMsg, 0,		len2,	SRCPAINT);
	BitBlt(hDC, x2,					y2,					msgLength, msgLength, hMsg, len2,	len2,	SRCPAINT);


	// �ӂ̕`��i���j
	for(x=x1; x<x2; x+=msgLength){
		// �}�X�N
		BitBlt(hDC, x, (int)msgArea.top,	msgLength, msgLength, hMsg, len4, 0,	SRCAND);
		BitBlt(hDC, x, y2,					msgLength, msgLength, hMsg, len4, len2, SRCAND);
		// ���ۂ̊G
		BitBlt(hDC, x, (int)msgArea.top,	msgLength, msgLength, hMsg, msgLength, 0,		SRCPAINT);
		BitBlt(hDC, x, y2,					msgLength, msgLength, hMsg, msgLength, len2,	SRCPAINT);
	}

	// �ӂ̕`��i�c�j
	for(y=y1; y<y2; y+=msgLength){
		// �}�X�N
		BitBlt(hDC, (int)msgArea.left,	y,	msgLength, msgLength, hMsg, 3*msgLength, msgLength, SRCAND);
		BitBlt(hDC, x2,					y,	msgLength, msgLength, hMsg, 5*msgLength, msgLength, SRCAND);
		// ���ۂ̊G
		BitBlt(hDC, (int)msgArea.left,	y,	msgLength, msgLength, hMsg, 0,		msgLength, SRCPAINT);
		BitBlt(hDC, x2,					y,	msgLength, msgLength, hMsg, len2,	msgLength, SRCPAINT);
	}

	// �����̕`��
	for(x=x1; x<x2; x+=msgLength){
		for(y=y1; y<y2; y+=msgLength){
			// �}�X�N
			BitBlt(hDC, x, y, msgLength, msgLength, hMsg, len4, msgLength, SRCAND);
			// ���ۂ̊G
			BitBlt(hDC, x, y, msgLength, msgLength, hMsg, msgLength, msgLength, SRCPAINT);
		}
	}

	// �����̕`��
	RECT range;
	range.left =	x1;
	range.right =	x2;
	range.top =		y1;
	range.bottom =	y2;

	format = DT_NOCLIP | DT_VCENTER;
	if(msgAlign  == 0 )			format |= DT_LEFT;
	else if(msgAlign  == 1 )	format |= DT_CENTER;
	else						format |= DT_RIGHT;

	DrawText(hDC, sMsg.c_str(), -1, &range, format);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �z�b�g�L�[�̐ݒ�
//
bool SetHotKey(HWND hWnd, const string& strID, const Key& key)
{
	int ret = 0;
	UINT mask = 0;

	// ���ɓo�^����Ă��������
	if(hotKeyID != 0) UnSetHotKey(hWnd);

	if(key.mask & MASK_SHIFT) mask |= MOD_SHIFT;
	if(key.mask & MASK_CTRL) mask |= MOD_CONTROL;
	if(key.mask & MASK_ALT) mask |= MOD_ALT;

	// �O���[�o���A�g���̒ǉ�
	hotKeyID = GlobalAddAtom(strID.c_str());
	// �z�b�g�L�[�̐ݒ�
	if(hotKeyID != 0) ret = RegisterHotKey(hWnd, hotKeyID, mask, key.nVirtKey);

	if(ret == 0)	return false;
	else			return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �z�b�g�L�[�̉���
//
void UnSetHotKey(HWND hWnd)
{
	if(hotKeyID == 0) return;
	UnregisterHotKey(hWnd, hotKeyID);
	GlobalDeleteAtom(hotKeyID);

	hotKeyID = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �o�[�W�������
//
LRESULT CALLBACK DlgProcVer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	string s;

    switch (msg) {
		// �쐬��
        case WM_INITDIALOG:
			s.append(APPNAME);
			s.append("\n");
			s.append(VERSION);
			s.append("\n");
			s.append(COPYRIGHT);
			SetDlgItemText( hWnd, IDC_LABEL, s.c_str() );
            break;

		// �R���g���[��
        case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDOK:
				EndDialog(hWnd, IDOK);
				break;

			default:
				return FALSE;
			}

        default:
            return FALSE;
    }
    return TRUE;
}
