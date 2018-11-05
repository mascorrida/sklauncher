/**
 *
 * skmain.cpp WinMain等の主要関数
 *
 */

#include <sys/stat.h>
#include "skutil.h"
#include "skoption.h"
#include "skitem.h"

// WM_MOUSEWHEELの定義
#ifndef WM_MOUSEWHEEL
#include "zmouse.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// typedef
//

typedef int (*HOOKDESKTOP)(HWND);		// デスクトップ左クリック用のフック
typedef int (*UNHOOKDESKTOP)(VOID);		// デスクトップ左クリック用のフック解除
typedef int (*HOOKRDBLCLICK)(HWND);		// 右ダブルクリック用のフック
typedef int (*UNHOOKRDBLCLICK)(VOID);	// 右ダブルクリック用のフック解除
typedef int (*HOOKMDBLCLICK)(HWND);		// ホイールダブルクリック用のフック
typedef int (*UNHOOKMDBLCLICK)(VOID);	// ホイールダブルクリック用のフック解除
typedef int (*HOOKROUNDMOUSE)(HWND);	// くるくるマウス用のフック
typedef int (*UNHOOKROUNDMOUSE)(VOID);	// くるくるマウス用のフック解除


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// パブリック定数
//

// ソフトの名称
#define APPNAME			"SK Launch"
// バージョン
#define VERSION			"version 1.1.0 β1"
// 著作権表記
#define COPYRIGHT		"(C) 2004-2005 ramuneya"
// ホットキーID
#define SKL_HOTKEY		"SKLHOTKEY"
// サポートURL
#define SUPPORT_URL		"http://hp.vector.co.jp/authors/VA030448/index.html"

// ウインドウクラスの名称
#define SZ_CLASS_NAME	"WindowClassName_SKL1.1"
// タスクトレイクリック
#define WM_TASKTRAY		(0x3000)

// シフトキー等のフラグ
#define MASK_NONE		(0)
#define MASK_SHIFT		(1)
#define MASK_CTRL		(2)
#define MASK_ALT		(4)

// ファイル名
#define INI_FILENAME	"skl.ini"
#define XML_FILENAME	"items.xml"
#define DLL_FILENAME	"sklutil.dll"

// アクションの種類
#define ACTION_FILEOPEN		(0)
#define ACTION_FOLDEROPEN	(1)
#define ACTION_HIDE			(2)
#define ACTION_LEFT			(3)
#define ACTION_RIGHT		(4)
#define ACTION_UP			(5)
#define ACTION_DOWN			(6)
#define ACTION_SHOW			(7)

// アニメーションの時間(millisecond)
#define ANIMATION_OUT_TIME			(550)
#define ANIMATION_IN_TIME			(200)
#define ANIMATION_HOR_TIME			(150)

// キューの最大数
#define MAX_QUEKEY		(2)

// タイマーID
#define TIMER_ITEM			(32750)
#define TIMER_GROUP1		(32751)
#define TIMER_GROUP2		(32752)
#define TIMER_SHOW			(32753)
#define TIMER_HIDE			(32754)
#define TIMER_FILEOPEN		(32755)
#define TIMER_FOLDEROPEN	(32756)

// ドラッグでアクションを起こす有効距離
#define DRAG_VALUE			(50)

#define MSG_OFFSET			(55)	// メッセージの表示位置オフセット
#define ICON_OFFSET			(16)	// アイコンの描画位置オフセット
#define LEVEL_SPACE_NUM		(2)		// XMLに出力する際のタブのスペース数

// オプションのインデックス
#define OPTION_ANIMTION		(0)
#define OPTION_MESSAGE		(1)
#define OPTION_INPUT		(2)
#define OPTION_SOUND		(3)
#define OPTION_ACTION		(4)


// デフォルトの値 /////////////////////////////////
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
#define DEFAULT_MsgFont				"ＭＳ Ｐゴシック"
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
// クラス
//

// キー操作
class Key{
public:
	// コンストラクタ
	Key(){
		nVirtKey = NULL;
		mask = MASK_NONE;
	}
	Key(int vk, char msk){
		nVirtKey = vk;
		mask = msk;
	}
	// デストラクタ
	virtual ~Key(){}

	// コピーコンストラクタ
	Key(const Key& other){
		nVirtKey = other.nVirtKey;
		mask = other.mask;
	}
	// 代入演算子
	const Key& operator=(const Key& other){
		nVirtKey = other.nVirtKey;
		mask = other.mask;
		return(*this);
	}

	// 同じ内容かどうか
	const bool equals(const Key& other) const{
		if(nVirtKey == other.nVirtKey && mask == other.mask)
			return true;
		else
			return false;
	}

	// メンバ
	int nVirtKey;
	char mask;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// パブリック変数
//

// グループアイテム
vector<SKGroupItem> GI;
// 今選択してるグループ
int iGI;
// 今表示しているか
bool bShow;

bool bDialog;				// ダイアログ表示中かどうか

RECT rScreen;				// スクリーンサイズ

HINSTANCE hInst;			// 自分ののハンドル
HINSTANCE hDllInst;			// sklutil.dllのハンドル
HANDLE hMutex;				// ミューテックスオブジェクトのハンドル

NOTIFYICONDATA m_nid;		// アイコンデータ

HDC hBkDC;					// バックバッファのDC
HBITMAP hBkBm;				// バックバッファのBITMAP
HFONT hFont;				// バックバッファのフォント
HDC hDskDC;					// スクリーンキャプチャのDC

HDC hCur;					// カーソル画像のDC
HBITMAP hBmpCur;			// カーソル画像のBITMAP
int curX, curY;				// カーソル画像の描画位置
int curOffsetX, curOffsetY;	// カーソル画像の描画のずれ位置
bool bCurPaint;				// カーソルを描画するか

HDC hMsg;					// メッセージ画像のDC
HBITMAP hBmpMsg;			// メッセージ画像のBITMAP
RECT msgArea;				// メッセージ画像の描画位置
int msgLength;				// メッセージ画像の幅
bool bMsgPaint;				// メッセージ画像をを描画するか
string sMsg;				// メッセージの内容

bool bActing;				// アクション中かどうか
queue<Key> queKey;			// 待ち中のキーリスト

bool bDrag;								// ドラッグしているかどうか
bool bMenu;								// メニューを表示しているかどうか
int mouseX, mouseY;						// マウスダウンした座標;

// アニメーション
bool autoR;					// 自動で半径を調整するか
UCHAR maxR;					// 最大の半径
UCHAR minR;					// 最小の半径
UCHAR iconItv;				// アイコン同士の間隔
UCHAR roteRad;				// アニメーションで動く角度
UCHAR smooth;				// 滑らかさ
UCHAR roteTime;				// アニメーション時間
UCHAR darkType;				// アニメ時に暗くする(0:暗くしない, 1:簡易描画, 2:詳細描画)
UCHAR darkValue;			// 暗くする濃度

// メッセージ
bool boolMsg;				// メッセージを表示するか
string msgStyle;			// メッセージの書式
UCHAR msgPos;				// メッセージの位置(0-上, 1-中心, 2-下)
UCHAR msgAlign;				// メッセージ文字の揃え位置(0-左, 1-中央, 2-右)
int msgColor;				// メッセージ文字の色
string msgFont;				// メッセージ文字のフォント
UCHAR msgFontSize;			// メッセージ文字のフォントサイズ

// キーボード・マウス
ATOM hotKeyID;							// ホットキーID
Key hotKey;								// ホットキー
vector< vector<Key> > shortKey;			// ショートカットキー(7*5)(0-起動, 1-フォルダ, 2-隠す, 3-左, 4-右, 5-上, 6-下)
bool shortKeyPriority;					// ジャンプ文字より優先するかどうか
bool boolAccessKey;						// アクセスキーの有効／無効

// SE
string SEHor;				// 横
string SEUp;				// 上
string SEDown;				// 下
string SEExe;				// 実行
bool boolSEHor;				// 効果音の有無
bool boolSEUp;
bool boolSEDown;
bool boolSEExe;

// 動作
UCHAR defaultPos;			// 表示した時の初期位置(0:以前と同じ, 1:先頭に戻る)
UCHAR hotKeyAction;			// 表示中にホットキーを押した時の動作
UCHAR centerPos;			// アニメーションの中心座標(0:画面中央, 1: 任意座標, 2:マウスの位置)
POINT center;				// 中心座標
UCHAR mouseL;				// 左クリック時のマウス動作	(0-なし, 1-起動, 2-フォルダ, 3-隠す, 4-メニュー)
UCHAR mouseM;				// 中クリック時のマウス動作	(0-なし, 1-起動, 2-フォルダ, 3-隠す, 4-メニュー)
UCHAR mouseR;				// 右クリック時のマウス動作	(0-なし, 1-起動, 2-フォルダ, 3-隠す, 4-メニュー)
bool boolInvRote;			// 左右の回転を逆にするか
bool boolDeskTopClick;		// デスクトップをダブルクリックしたときに起動するかどうか
bool boolRoundMouse;		// くるくるマウス
int intDBLClick;			// ダブルクリック(0-なし, 1-右, 2-中央)
//bool boolMouseCentering;	// 表示時にマウスを移動するか
//bool boolMouseCenteringMouseOnly;	// マウスからの起動のみ有効にするか

// アニメーション用変数
HWND timerHWnd;				// ウインドウハンドル
UINT timerID;				// タイマーID
UINT timerType;				// タイマーID
UINT timerCount;			// カウント
UINT timerMaxCount;			// 最大のカウント数
float timerR;				// 移動分動径
float timerTheta;			// 移動分方位角
bool timerOut;				// 外方向かどうか
bool timerClockwise;		// 時計回りかどうか
UINT timerIndex;			// 移動するアイテムのインデックス
UINT timerPerFrame;			// タイマーの間隔


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロトタイプ宣言
//

// ウインドウプロシージャ
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
// ウインドウクラスの登録
ATOM MyRegisterClass(HINSTANCE hInstance);
// インスタンス初期化（ウインドウ作成）
bool InitInstance(HINSTANCE hInstance, int nCmdShow);

// 初期化処理
bool Init(HWND hWnd);
// 終了処理
bool Unload(HWND hWnd);

// グラフィックス関連の初期化
bool InitGraphics(HWND hWnd);

// 変数の読み込み
void GetVar();
// 変数の書き込み
void SetVar(bool bDefault);
// XMLファイルから読み込み
bool GetFromXML();

// アイテムの表示
void SKShow(HWND hWnd);
// アイテムを隠す
void SKHide(HWND hWnd, bool animation);
// グループの変更＆アニメーション
void ChangeGroup(HWND hWnd, int index, bool bOut, bool bClockwise, bool bOnlyAnime);
// グループアニメーション
void AnimateGroup(HWND hWnd, bool bCenter, bool bOut, bool bClockwise, UINT uiTimerType);
// アイテムの変更＆アニメーション
void ChangeItem(HWND hWnd, int index, bool direction);
// アイテムアニメーション
void AnimateItem(HWND hWnd, UINT index, bool direction, bool clockwise);
// 描画
void DrawWindow(HWND hWnd);
// キーアクションの検索
void SKKeySearch(HWND hWnd, const Key& key);
// アクション
void SKAction(HWND hWnd, UCHAR type);
// ショートカットキー
bool ShortcutKey(HWND hWnd, const Key& key);
// ジャンプキー
bool JumpKey(HWND hWnd, const Key& key);
// アクセスキー
bool AccessKey(HWND hWnd, const Key& key);
// マウスアクション(0-なし, 1-起動, 2-フォルダ, 3-隠す, 4-メニュー)
void SKMouseAction(HWND hWnd, UCHAR type);
// タイマー用関数(タイマーID,予約,ユーザー定義,予約,予約)
void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
// スタートアップに登録
void CreateStartup(HWND hWnd);
// ショートカットファイルの出力
void CreateShortcutFiles(HWND hWnd);
// SKShortcutItemからショートカットファイルを作成
HRESULT CreateShortcutFileFromItem(const string& path, const SKShortcutItem& item);
// カーソルの描画位置を更新
void RenewCursor(HWND hWnd, HRGN invalidRgn);
// カーソルの描画領域を更新
void InvalidateCursor(HWND hWnd, HRGN invalidRgn);
// カーソルの描画
void DrawCursor(HWND hWnd, HDC hDC);
// メッセージの各種変数を更新
void RenewMessage(HWND hWnd, HDC hDC, HRGN invalidRgn);
// メッセージの内容を取得
string EncordMessage();
// メッセージ背景の描画領域を更新
void InvalidateMessage(HWND hWnd, HRGN invalidRgn);
// メッセージ背景の描画
void DrawMessage(HWND hWnd, HDC hDC);
// ホットキーの設定
bool SetHotKey(HWND hWnd, const string& strID, const Key& key);
// ホットキーの解除
void UnSetHotKey(HWND hWnd);
// バージョン情報
LRESULT CALLBACK DlgProcVer(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メイン関数
//

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	hInst = hInstance;
	// ウインドウクラスの登録
	MyRegisterClass(hInstance);
	// ウインドウのインスタンス初期化
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
// ウインドウクラスの登録
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
// インスタンスの初期化
//
bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	rScreen = GetScreenSize();	// スクリーンサイズを取得

	// ウインドウ作成
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
	if(!hWnd) return(FALSE);	// 失敗時はFALSE
	
    ShowWindow(hWnd, nCmdShow);	// 表示状態を設定
    UpdateWindow(hWnd);			// 更新

	return(TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロシージャ
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
	// 作成時
	case WM_CREATE:
		// 初期化失敗なら終了
		if(!Init(hWnd)) SendMessage(hWnd, WM_CLOSE, 0L, 0L);
		break;

	// 終了時
	case WM_DESTROY:
        if(Unload(hWnd)) PostQuitMessage(0);
        break;

	// ホットキー
	case WM_HOTKEY:
		// ダイアログ表示中なら中止
		if(bDialog) break;
		// 表示してなければ表示
		if(!bShow){
/*
			string buff;
			buff.append("WP:" + CStr((int)wParam) + ", LP:" + CStr((int)lParam));
			MessageBox(hWnd, buff.c_str(),"",MB_OK);
//*/
			SKAction(hWnd, ACTION_SHOW);
		}
		else if(!bActing){
			// 表示していればhotKeyActionで分岐
			switch(hotKeyAction){
			case 0: break;
			case 1: SKAction(hWnd, ACTION_HIDE);
				break;
			case 2: SKAction(hWnd, ACTION_FILEOPEN);
				break;
			}
		}
		break;

	// アクティブ・非アクティブ時
	case WM_ACTIVATE:
		// ダイアログのアクティブ・非アクティブなら関係なし
		if(bDialog) break;
		if(LOWORD(wParam) == WA_INACTIVE && bShow){
			// アクション中でなければ隠し
			if(!bActing)	SKAction(hWnd, ACTION_HIDE);
			// アクション中であればキューに追加
			else{
				while(!queKey.empty()) queKey.pop();	// キューをクリア
				key = shortKey.at(ACTION_HIDE).front();
				queKey.push(key);
			}
		}
		break;

	// メニュー
	case WM_COMMAND:
        switch (LOWORD(wParam)) {

		// 表示
        case IDM_MENU1_SHOW:
			SKAction(hWnd, ACTION_SHOW);
            break;

		// アイテムの編集
        case IDM_MENU1_ITEM:
			bDialog = true;
//			i = DialogBox(hInst, "IDD_ITEM", hWnd, (DLGPROC)DlgProcItem);
			bDialog = false;
			MemoryClean();
            break;

		// オプション
        case IDM_MENU1_OPTION:
			bDialog = true;
			i = DialogBox(hInst, "IDD_OPTION", hWnd, (DLGPROC)DlgProcOption);
			bDialog = false;

			MemoryClean();
            break;

		// スタートアップに登録
        case IDM_MENU1_STARTUP:
			CreateStartup(hWnd);
			MemoryClean();
            break;

		// ショートカットファイルの出力
        case IDM_MENU1_EXPORT:
			CreateShortcutFiles(hWnd);
			MemoryClean();
            break;

		// バージョン情報
        case IDM_MENU1_VERSION:
			bDialog = true;
			i = DialogBox(hInst, "IDD_VERSION", hWnd, (DLGPROC)DlgProcVer);
			bDialog = false;
			MemoryClean();
            break;

		// ヘルプ
        case IDM_MENU1_HELP:
			s.append(GetAppDir());
			s.append("\\doc\\index.html");
			i = (int)ShellExecute(0, "open", s.c_str(), "", "", SW_SHOWNORMAL);
			if(i<=32) MessageBox(timerHWnd, openError(i).c_str(), "ERROR", MB_OK);
			MemoryClean();
            break;

		// 作成元へ行く
        case IDM_MENU1_GOTO:
			i = (int)ShellExecute(0, "open", SUPPORT_URL, "", "", SW_SHOWNORMAL);
			if(i<=32) MessageBox(timerHWnd, openError(i).c_str(), "ERROR", MB_OK);
			MemoryClean();
            break;

		// 終了
        case IDM_MENU1_END:
            SendMessage(hWnd, WM_CLOSE, 0L, 0L);
            break;

		// アイテムの起動
        case IDM_MENU2_FILEOPEN:
			SKMouseAction(hWnd, 1);
            break;

		// フォルダを開く
        case IDM_MENU2_FOLDEROPEN:
			SKMouseAction(hWnd, 2);
            break;

		// 隠す
        case IDM_MENU2_HIDE:
			SKMouseAction(hWnd, 3);
            break;

        default:
            return (DefWindowProc(hWnd, msg, wParam, lParam));
        }
		bMenu = false;
		break;

	// マウスダウン
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

	// マウス移動
	case WM_MOUSEMOVE:
		// ドラッグ
		if(wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)){
			i = LOWORD(lParam) - mouseX;
			// 横方向に有効距離以上ドラッグしていたら
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
			// 縦方向に有効距離以上ドラッグしていたら
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

	// 左マウスアップ
	case WM_LBUTTONUP:
		if(bMenu){
			bMenu = false;
			break;
		}
		if(!bDrag && !bActing) SKMouseAction(hWnd, mouseL);
		break;

	// 中マウスアップ
	case WM_MBUTTONUP:
		if(!bMenu && !bDrag && !bActing) SKMouseAction(hWnd, mouseM);
		break;

	// 右マウスアップ
	case WM_RBUTTONUP:
		if(bMenu){
			bMenu = false;
			break;
		}
		if(!bDrag && !bActing) SKMouseAction(hWnd, mouseR);
		break;

	// ホイールを動かす
	case WM_MOUSEWHEEL:
		if(!bShow) break;
		i = (SHORT)HIWORD(wParam);
		bDrag=true;

		if(wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)){
			// クリックしながらならグループ
			if(!bActing && queKey.empty()){
				if(i>=0)	SKAction(hWnd, ACTION_UP);
				else		SKAction(hWnd, ACTION_DOWN);
			}

		}else{
			// 違うならアイテム(キューあり)
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

	// キーダウン
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if(!bShow) break;
		// 仮想キー
		key.nVirtKey = wParam;
		key.mask = MASK_NONE;
		// シフト
		if(GetKeyState(VK_SHIFT) < 0)	key.mask |= MASK_SHIFT;
		if(GetKeyState(VK_CONTROL) < 0)	key.mask |= MASK_CTRL;
		if(GetKeyState(VK_MENU) < 0)	key.mask |= MASK_ALT;

		if(!bActing && queKey.empty())		SKKeySearch(hWnd, key);
		else if(queKey.size() < MAX_QUEKEY)	queKey.push(key);
		break;

	// タスクトレイ
	case WM_TASKTRAY:
		if(bDialog) break;
		switch(lParam){

		// 左ダブルクリック
		case WM_LBUTTONDBLCLK:
			SKAction(hWnd, ACTION_SHOW);
			break;

		// 右クリック
		case WM_RBUTTONUP:
			bMenu = true;
			PopupMenu(hWnd, "IDR_MENU1");
			break;
		}

		break;

	// 描画時
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
// マウスアクション(0-なし, 1-起動, 2-フォルダ, 3-隠す, 4-メニュー)
//
void SKMouseAction(HWND hWnd, UCHAR type)
{
	int act = (int)type;

	if(act>=1 && act<=3)	SKAction(hWnd, act-1);			// 起動, フォルダ, 隠す
	else if(act == 4){
		bMenu = true;
		PopupMenu(hWnd, "IDR_MENU2");						// メニュー
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// キーアクションの検索
//
void SKKeySearch(HWND hWnd, const Key& key)
{
	if(shortKeyPriority){
		// ショートカットキーが優先
		if(ShortcutKey(hWnd, key))		return;
		else if(JumpKey(hWnd, key))		return;
		else							AccessKey(hWnd, key);
	}else{
		// ジャンプキーが優先
		if(JumpKey(hWnd, key))			return;
		else if(ShortcutKey(hWnd, key))	return;
		else							AccessKey(hWnd, key);

	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アクション
//
void SKAction(HWND hWnd, UCHAR type)
{
	switch(type){

	// 0-起動
	case ACTION_FILEOPEN:
		if(boolSEExe) sndPlaySound(SEExe.c_str(), SND_ASYNC | SND_NODEFAULT);

		SKHide(hWnd, true);
		timerType = TIMER_FILEOPEN;
		break;

	// 1-フォルダ
	case ACTION_FOLDEROPEN:
		if(boolSEExe) sndPlaySound(SEExe.c_str(), SND_ASYNC | SND_NODEFAULT);

		SKHide(hWnd, true);
		timerType = TIMER_FOLDEROPEN;

		break;

	// 2-隠す
	case ACTION_HIDE:
		if(boolSEUp) sndPlaySound(SEUp.c_str(), SND_ASYNC | SND_NODEFAULT);

		SKHide(hWnd, true);
		break;

	// 3,4-左右
	case ACTION_LEFT:
	case ACTION_RIGHT:
		if((type==ACTION_LEFT) ^ boolInvRote)
			ChangeItem(hWnd, GI.at(iGI).getItemIndex()+1, true);
		else
			ChangeItem(hWnd, GI.at(iGI).getItemIndex()-1, true);
		break;

	// 5-上
	case ACTION_UP:
		if(boolSEUp) sndPlaySound(SEUp.c_str(), SND_ASYNC | SND_NODEFAULT);

		ChangeGroup(hWnd, iGI-1, true, false, false);
		break;

	// 6-下
	case ACTION_DOWN:
		if(boolSEDown) sndPlaySound(SEDown.c_str(), SND_ASYNC | SND_NODEFAULT);

		ChangeGroup(hWnd, iGI+1, false, true, false);
		break;

	// 7-表示
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
// ショートカットキー
//
bool ShortcutKey(HWND hWnd, const Key& key)
{
	// 検索
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
// ジャンプキー
//
bool JumpKey(HWND hWnd, const Key& key)
{
	int i;

	// アイテムの検索
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

	// グループの検索
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
// アクセスキー
//
bool AccessKey(HWND hWnd, const Key& key)
{
	int size = GI.at(iGI).size();
	int stt = GI.at(iGI).getItemIndex();

	// 文字変換できるキーじゃなければ止める
	if(!((key.nVirtKey >= '0' && key.nVirtKey <= '9') ||
	     (key.nVirtKey >= 'A' && key.nVirtKey <= 'Z'))) return false;

	for(int i=stt+1; i!=stt; i++){
		if(i >= size){
			i = 0;
			if(i == stt) break;
		}

		// 頭文字を取得
		char sHead = GI.at(iGI).item(i).getCaption().at(0);
		// 大文字にする
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
// 終了処理
//
bool Unload(HWND hWnd)
{
	UNHOOKDESKTOP fpUnHookDesktop;
	UNHOOKRDBLCLICK fpUnHookRDBLClick;
	UNHOOKMDBLCLICK fpUnHookMDBLClick;
	UNHOOKROUNDMOUSE fpUnHookRoundMouse;

	// 表示していたら隠す
	if(bShow) SKHide(hWnd, false);

	// vector, listの削除
	shortKey.clear();
	GI.clear();

	// タスクトレイアイコンの削除
	Shell_NotifyIcon(NIM_DELETE, &m_nid);

	// バックバッファの削除
	DeleteObject(hBkBm);
	DeleteObject(hFont);
	DeleteDC(hBkDC);

	// カーソル画像の削除
	DeleteObject(hBmpCur);
	DeleteDC(hCur);

	// メッセージ画像の削除
	DeleteObject(hBmpMsg);
	DeleteDC(hMsg);

	// ホットキー解除
	UnSetHotKey(hWnd);

	// フック解除
	if(boolDeskTopClick || boolRoundMouse || (intDBLClick>0)){
		// デスクトップ左ダブルクリック
		if(boolDeskTopClick){
			fpUnHookDesktop = (UNHOOKDESKTOP)GetProcAddress(hDllInst, "UnHookDesktop");
			(*fpUnHookDesktop)();
		}
		// 右ダブルクリック
		if(intDBLClick == 1){
			fpUnHookRDBLClick = (UNHOOKRDBLCLICK)GetProcAddress(hDllInst, "UnHookRDBLClick");
			(*fpUnHookRDBLClick)();
		}
		// ホイールダブルクリック
		if(intDBLClick == 2){
			fpUnHookMDBLClick = (UNHOOKMDBLCLICK)GetProcAddress(hDllInst, "UnHookMDBLClick");
			(*fpUnHookMDBLClick)();
		}
		// くるくるマウス
		if(boolRoundMouse){
			fpUnHookRoundMouse = (UNHOOKROUNDMOUSE)GetProcAddress(hDllInst, "UnHookRoundMouse");
			(*fpUnHookRoundMouse)();
		}

		FreeLibrary(hDllInst);			// DLLの解放
	}

	// ミューテックス解放
	ReleaseMutex(hMutex);

	return(true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初期化処理
//
bool Init(HWND hWnd)
{
	HOOKDESKTOP fpHookDesktop;
	HOOKRDBLCLICK fpHookRDBLClick;
	HOOKMDBLCLICK fpHookMDBLClick;
	HOOKROUNDMOUSE fpHookRoundMouse;
	int ret;

	// ミューテックスの作成(２重起動防止用)
	hMutex = CreateMutex(NULL, TRUE, SZ_CLASS_NAME);
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) return false;

	GetVar();			// 変数の読み込み
	if(!GetFromXML()){	// アイテムの読み込み
		MessageBox(NULL, "xml-file reading failure!", APPNAME, MB_OK);
	}
	iGI = 0;				// グループを選択
	bActing = false;		// アクション中でない
	bDialog = false;		// ダイアログ非表示

	// ホットキーの設定
	hotKeyID = 0;
	if( (hotKey.nVirtKey==0) || (!SetHotKey(hWnd, SKL_HOTKEY, hotKey)) ){
		MessageBox(hWnd, "ホットキーの設定に失敗しました。\n終了します。", APPNAME, MB_OK);
		return false;
	}

	// グラフィックス関連の初期化
	if( !InitGraphics(hWnd) ){
		MessageBox(hWnd, "グラフィックスの初期化中にエラーが発生しました。\n終了します。", APPNAME, MB_OK);
		return false;
	}

	// フック
	if(boolDeskTopClick || boolRoundMouse || (intDBLClick > 0)){
		hDllInst = LoadLibrary(DLL_FILENAME);	// DLLのロード

		// デスクトップ左ダブルクリック
		if(boolDeskTopClick){
			fpHookDesktop = (HOOKDESKTOP)GetProcAddress(hDllInst, "HookDesktop");

			if(fpHookDesktop != NULL) ret = (*fpHookDesktop)(hWnd);
			if(fpHookDesktop == NULL || ret != 1){
				MessageBox(hWnd, "フックの設定に失敗しました。\n（デスクトップ左ダブルクリック機能）\n終了します。", APPNAME, MB_OK);
				return false;
			}
		}

		// 右ダブルクリック
		if(intDBLClick == 1){
			fpHookRDBLClick = (HOOKRDBLCLICK)GetProcAddress(hDllInst, "HookRDBLClick");

			if(fpHookRDBLClick != NULL) ret = (*fpHookRDBLClick)(hWnd);
			if(fpHookRDBLClick == NULL || ret != 1){
				MessageBox(hWnd, "フックの設定に失敗しました。\n（右ダブルクリック機能）\n終了します。", APPNAME, MB_OK);
				return false;
			}
		}else if(intDBLClick == 2){
			fpHookMDBLClick = (HOOKMDBLCLICK)GetProcAddress(hDllInst, "HookMDBLClick");

			if(fpHookMDBLClick != NULL) ret = (*fpHookMDBLClick)(hWnd);
			if(fpHookMDBLClick == NULL || ret != 1){
				MessageBox(hWnd, "フックの設定に失敗しました。\n（ホイールダブルクリック機能）\n終了します。", APPNAME, MB_OK);
				return false;
			}
		}

		// くるくるマウス
		if(boolRoundMouse){
			fpHookRoundMouse = (HOOKROUNDMOUSE)GetProcAddress(hDllInst, "HookRoundMouse");

			if(fpHookRoundMouse != NULL) ret = (*fpHookRoundMouse)(hWnd);
			if(fpHookRoundMouse == NULL || ret != 1){
				MessageBox(hWnd, "フックの設定に失敗しました。\n（くるくるマウス機能）\n終了します。", APPNAME, MB_OK);
				return false;
			}
		}
	}

	// タクストレイアイコンの追加
	strcpy(m_nid.szTip, "SK Launch");
	m_nid.uFlags =				NIF_ICON | NIF_TIP | NIF_MESSAGE;
	m_nid.hIcon =				LoadIcon((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), "IDI_ICON_TASKTRAY");
	m_nid.hWnd =				hWnd;
	m_nid.uID =					1;
	m_nid.uCallbackMessage =	WM_TASKTRAY;
	m_nid.cbSize =				sizeof(m_nid);
	Shell_NotifyIcon(NIM_ADD, &m_nid);

	MemoryClean();		// メモリ初期化
	return(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// グラフィックス関連の初期化
//
bool InitGraphics(HWND hWnd)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);	// インスタンスハンドルの取得
    HDC hDC=GetDC(hWnd);												// WindowのDCを取得
	BITMAP bmp;
	HFONT hFontOld;

	// バックバッファ
	POINT pSc;											// 画面サイズ
    pSc.x = GetSystemMetrics(SM_CXSCREEN);
    pSc.y = GetSystemMetrics(SM_CYSCREEN);

    hBkBm = CreateCompatibleBitmap(hDC, pSc.x, pSc.y);	// BITMAP作成
    hBkDC = CreateCompatibleDC(hDC);					// DC作成
    SelectObject(hBkDC, hBkBm);							// hBkDCにhBkBmをセット

	// フォント指定
	hFont = CreateFont(
		(int)msgFontSize,			// フォント高さ
		0,							// 文字幅
		0,							// テキストの角度
		0,							// ベースラインとｘ軸との角度
		FW_REGULAR,					// フォントの重さ（太さ）
		FALSE,						// イタリック体
		FALSE,						// アンダーライン
		FALSE,						// 打ち消し線
		SHIFTJIS_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力精度
		CLIP_DEFAULT_PRECIS,		// クリッピング精度
		PROOF_QUALITY | ANTIALIASED_QUALITY,	// 出力品質
		DEFAULT_PITCH | FF_MODERN,	// ピッチとファミリー
		msgFont.c_str());			// 書体名
	hFontOld = (HFONT)SelectObject(hBkDC, hFont);	// フォントの選択
	DeleteObject(hFontOld);							// 前のフォントの削除

	SetTextColor(hBkDC, msgColor);					// 文字色
	SetBkMode(hBkDC, TRANSPARENT);					// 背景は透明

	// カーソル画像
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

	// メッセージ画像
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

	// hDC解放
    ReleaseDC(hWnd, hDC);

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 変数の読み込み
//
void GetVar()
{
	string iniPath;
	iniPath = GetAppDir() + "\\" + INI_FILENAME;

	// iniファイルがない場合
	if( GetFileAttributes(iniPath.c_str()) == (DWORD)-1 )
		SetVar(true);

	center.x = GetIniInt(iniPath, "GENERAL", "CenterX", DEFAULT_CenterX);
	center.y = GetIniInt(iniPath, "GENERAL", "CenterY", DEFAULT_CenterY);
	// (0,0)なら画面中央
	if(center.x == DEFAULT_CenterX && center.y == DEFAULT_CenterY){
		center.x = (rScreen.right-rScreen.left) / 2;
		center.y = (rScreen.bottom-rScreen.top) / 2;
	}

	// アニメーション
	autoR = GetIniBool(iniPath, "GENERAL", "AutoR", DEFAULT_AutoR);				// 自動で半径を調整するか
	maxR = GetIniUCHAR(iniPath, "GENERAL", "MaxR", DEFAULT_MaxR);				// 最大の半径
	minR = GetIniUCHAR(iniPath, "GENERAL", "MinR", DEFAULT_MinR);				// 最小の半径
	iconItv = GetIniUCHAR(iniPath, "GENERAL", "IconItv", DEFAULT_IconItv);		// アイコン同士の間隔
	roteRad = GetIniUCHAR(iniPath, "GENERAL", "RoteRad", DEFAULT_RoteRad);		// アニメーションで動く角度
	smooth = GetIniUCHAR(iniPath, "GENERAL", "Smooth", DEFAULT_Smooth);			// 滑らかさ
	roteTime = GetIniUCHAR(iniPath, "GENERAL", "RoteTime", DEFAULT_RoteTime);	// アニメーション時間
	darkType = GetIniUCHAR(iniPath, "GENERAL", "DarkType", DEFAULT_DarkType);	// アニメ時に暗くする(0:暗くしない, 1:簡易, 2:高画質)
	darkValue = GetIniUCHAR(iniPath, "GENERAL", "DarkValue", DEFAULT_DarkValue);	// 暗くする濃度

	// メッセージ
	boolMsg = GetIniBool(iniPath, "GENERAL", "boolMsg", DEFAULT_boolMsg);
	msgPos = GetIniUCHAR(iniPath, "GENERAL", "MsgPos",	DEFAULT_MsgPos);
	msgStyle = GetIniStr(iniPath, "GENERAL", "MsgStyle", "DEFAULT_MsgStyle");
	msgAlign = GetIniUCHAR(iniPath, "GENERAL", "MsgAlign", DEFAULT_MsgAlign);
	msgColor = GetIniInt(iniPath, "GENERAL", "MsgColor", DEFAULT_MsgColor);
	msgFont = GetIniStr(iniPath, "GENERAL", "MsgFont", "DEFAULT_MsgFont");
	msgFontSize = GetIniUCHAR(iniPath, "GENERAL", "MsgFontSize", DEFAULT_MsgFontSize);

	// キーボード
	hotKey.nVirtKey = GetIniInt(iniPath, "GENERAL", "HotKeyCode", DEFAULT_HotKeyCode);		// ホットキー
	hotKey.mask = GetIniChar(iniPath, "GENERAL", "HotKeyShift", DEFAULT_HotKeyShift);	// ホットキーのシフト

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
	shortKeyPriority = GetIniBool(iniPath, "GENERAL", "ShortKeyPriority", DEFAULT_ShortKeyPriority);	// ジャンプ文字より優先するかどうか
	boolAccessKey = GetIniBool(iniPath, "GENERAL", "boolAccessKey", DEFAULT_boolAccessKey);				// アクセスキーの有効／無効

	mouseL = GetIniUCHAR(iniPath, "GENERAL", "MouseL", DEFAULT_MouseL);
	mouseM = GetIniUCHAR(iniPath, "GENERAL", "MouseM", DEFAULT_MouseM);
	mouseR = GetIniUCHAR(iniPath, "GENERAL", "MouseR", DEFAULT_MouseR);

	// SE
	SEHor = GetIniStr(iniPath, "GENERAL", "SEHor", "DEFAULT_SEHor");			// 横
	SEUp = GetIniStr(iniPath, "GENERAL", "SEUp", "DEFAULT_SEUp");				// 上
	SEDown = GetIniStr(iniPath, "GENERAL", "SEDown", "DEFAULT_SEDown");			// 下
	SEExe = GetIniStr(iniPath, "GENERAL", "SEExe", "DEFAULT_SEExe");			// 実行
	boolSEHor = GetIniBool(iniPath, "GENERAL", "boolSEHor", DEFAULT_boolSEHor);	// 効果音の有無
	boolSEUp = GetIniBool(iniPath, "GENERAL", "boolSEUp", DEFAULT_boolSEUp);
	boolSEDown = GetIniBool(iniPath, "GENERAL", "boolSEDown", DEFAULT_boolSEDown);
	boolSEExe = GetIniBool(iniPath, "GENERAL", "boolSEExe", DEFAULT_boolSEExe);

	// 動作
	defaultPos = GetIniUCHAR(iniPath, "GENERAL", "DefaultPos", DEFAULT_DefaultPos);						// 起動した時の初期位置
	hotKeyAction = GetIniUCHAR(iniPath, "GENERAL", "HotKeyAction", DEFAULT_HotKeyAction);				// 表示中にホットキーを押した時の動作
	boolDeskTopClick = GetIniBool(iniPath, "GENERAL", "boolDeskTopClick", DEFAULT_boolDeskTopClick);	// デスクトップをダブルクリックしたときに起動するかどうか
	boolRoundMouse = GetIniBool(iniPath, "GENERAL", "boolRoundMouse", DEFAULT_boolRoundMouse);			// くるくるマウス
	intDBLClick = GetIniInt(iniPath, "GENERAL", "intDBLClick", DEFAULT_intDBLClick);					// 左ダブルクリック
	centerPos = GetIniUCHAR(iniPath, "GENERAL", "CenterPos", DEFAULT_CenterPos);						// 起動した時の初期位置
	boolInvRote = GetIniBool(iniPath, "GENERAL", "boolInvRote", DEFAULT_boolInvRote);					// 回転を逆にする
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 変数の書き込み
//
void SetVar(bool bDefault)
{
	string iniPath;
	iniPath = GetAppDir() + "\\" + INI_FILENAME;

	if(bDefault){
		// デフォルト値の設定
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
		// 通常書き込み
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
// XMLファイルから読み込み
//
bool GetFromXML()
{
	string xmlPath;
	xmlPath = GetAppDir() + "\\" + XML_FILENAME;

	// XMLファイルのオープン
	FILE *fp;
	if ((fp = fopen(xmlPath.c_str(), "rb")) == NULL) return false;

	// サイズの取得(st.st_size)
	struct stat st;
	fstat(fileno(fp), &st);
	const long size = st.st_size;
	// データを取得
	char *data = new char[st.st_size];
	if( fread( data, sizeof(char), st.st_size, fp ) == 0) return false;

	// ファイルのクローズ
	fclose(fp);

	// グループアイテムの取得
	int pos1=0, pos2=0;
	string xmlData, buff;
	xmlData.assign(data);

	while(1){
		// pos1, pos1の設定
		pos1 = xmlData.find("<GroupItem>", pos2);
		if(pos1 == string::npos) break;			// ない場合終了
		buff = "</GroupItem>";
		pos2 = xmlData.find(buff, pos1);
		if(pos2 == string::npos) return false;	// ない場合失敗
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

	// グループがあるなら成功
	if(!GI.empty())	return true;
	else			return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムを表示
//
void SKShow(HWND hWnd)
{
	rScreen = GetScreenSize();				// 画面サイズを更新

	if(centerPos == (UCHAR)2){
		GetCursorPos(&center);
		int r = (int)GI.at(iGI).selectedItem().getR();
		// X座標の判定
		if(center.x < rScreen.left + r + ICON_OFFSET)	center.x = rScreen.left + r + ICON_OFFSET;
		else if(center.x > rScreen.right - r - ICON_OFFSET)	center.x = rScreen.right - r - ICON_OFFSET;
		// Y座標の判定
		switch(msgPos){
		case 0:					// MSG上
			if(center.y < rScreen.top + r + MSG_OFFSET + ICON_OFFSET)
				center.y = rScreen.top + r + MSG_OFFSET + ICON_OFFSET;
			else if(center.y > rScreen.bottom - r)
				center.y = rScreen.bottom - r;
			break;
		case 1:					// MSG中
			if(center.y < rScreen.top + r)
				center.y = rScreen.top + r;
			else if(center.y > rScreen.bottom - r)
				center.y = rScreen.bottom - r;
			break;
		case 2:					// MSG下
			if(center.y < rScreen.top + r)
				center.y = rScreen.top + r;
			else if(center.y > rScreen.bottom - r - MSG_OFFSET - ICON_OFFSET)
				center.y = rScreen.bottom - r - MSG_OFFSET - ICON_OFFSET;
			break;
		}
	}

//	SetTextColor(hBkDC, msgColor);					// 文字色の更新（時々黒になるから）
	hDskDC = CreateDeskCapDC();						// デスクトップキャプチャ
	DrawDark(hDskDC, 2, 50);						// 暗くする

	// 表示
	SetForegroundWindow2(hWnd);
	SetWindowPos(hWnd, HWND_TOPMOST, rScreen.left, rScreen.top, rScreen.right-rScreen.left, rScreen.bottom-rScreen.top, SWP_SHOWWINDOW);

	bShow = true;									// フラグＯＮ
	ChangeGroup(hWnd, NULL, false, true, true);		// 表示
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムを隠す
//
void SKHide(HWND hWnd, bool animation)
{
	// アニメーション
	if(animation) ChangeGroup(hWnd, NULL, true, true, true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// グループの変更＆アニメーション
//
void ChangeGroup(HWND hWnd, int index, bool bOut, bool bClockwise, bool bOnlyAnime)
{
	// アニメーションだけなら
	if(bOnlyAnime){
		if(!bOut)	AnimateGroup(hWnd, false, bOut, bClockwise, TIMER_SHOW);
		else		AnimateGroup(hWnd, false, bOut, bClockwise, TIMER_HIDE);

	}else{

		int i = (int)index;
		int size = (int)GI.size();

		// indexの修正
		while(i<0 || i>=size){
			if(i<0)				i += size;
			else if(i>=size)	i -= size;
			else break;
		}
		timerIndex = (UINT)i;

		// アニメーション
		AnimateGroup(hWnd, !bOut, bOut, bClockwise, TIMER_GROUP1);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// グループアニメーション
//
void AnimateGroup(HWND hWnd, bool bCenter, bool bOut, bool bClockwise, UINT uiTimerType)
{
	int fps;						// アニメーションのFPS
	int time;						// アニメーションする時間(ミリ秒)
	int frame;						// アニメーションのコマ数
	int perFrame;				// １コマあたりの時間(ミリ秒)

	int length;						// アニメーションする動径の(最大)距離
	if(rScreen.right-rScreen.left > rScreen.bottom-rScreen.top){
		length = (rScreen.right-rScreen.left);
		if(center.x*2 < length)	length -= center.x;
		else					length = center.x;
	}else{
		length = (rScreen.bottom-rScreen.top);
		if(center.y*2 < length)	length -= center.y;
		else					length = center.y;
	}

	// (smooth,FPS) = (0,10) ～ (100, 70)
	fps = (int)( (double)smooth * 0.6 + 10 );
	// (roteTime, time) = (0, 250%) ～ (100, 50%)
	time= 100 - (int)roteTime;
	time= (int)( (double)(time*time)*0.02 ) + 50;

	if(bCenter){
		// 中心 ～ 通常位置
		
		// frame, timerPerFrameの設定
		time = (int)((double)ANIMATION_IN_TIME*((double)time / 100));	// アニメーションの時間
		frame = (int)(fps * time / 1000);								// 何回描画するか
		if(frame < 2) frame = 2;
		perFrame = (int)(time / frame) - 10;							// １コマあたりの時間
		if(perFrame > 1)	timerPerFrame = perFrame;
		else				timerPerFrame = 1;

		// rの設定
		timerR = GI.at(iGI).selectedItem().getR() / frame;
		if(!bOut) timerR = -timerR;					// 内方向へならマイナス

		// thetaの設定
		timerTheta = 0;

	}else{
		// 通常位置 ～ 外

		// frame, timerPerFrameの設定
		time = (int)((double)ANIMATION_OUT_TIME*((double)time / 100));	// アニメーションの時間
		frame = (int)(fps * time / 1000);								// 何回描画するか
		if(frame < 2) frame = 2;
		perFrame = (int)(time / frame) - 10;							// １コマあたりの時間
		if(perFrame > 1)	timerPerFrame = perFrame;
		else				timerPerFrame = 1;

		// rの設定
		timerR = ((float)length - GI.at(iGI).selectedItem().getR()) / frame;
		if(!bOut) timerR = -timerR;							// 内方向へならマイナス

		// thetaの設定 ( roteRad,theta(全体) ) = (0,0)～(100,PI)
		timerTheta = (float)((double)roteRad * PI/100) / frame;
		if(!bClockwise) timerTheta = -timerTheta;			// 半時計回りならマイナス
	}

	// 通常位置に向かうアニメーションなら開始位置に移動
	if(!(bCenter^bOut)) GI.at(iGI).moveIcon(-(timerR*frame), -(timerTheta*frame));

	timerHWnd = hWnd;
	timerType = uiTimerType;
	timerCount = (UINT)0;
	timerMaxCount = (UINT)frame;
	timerOut = bOut;
	timerClockwise = bClockwise;
	bActing = true;

	bCurPaint = false;	// カーソルの描画設定
	bMsgPaint = false;	// メッセージの描画設定
	// 消す為に一度再描画
	HRGN invalidRgn = CreateRectRgn(0,0,0,0);
	InvalidateCursor(hWnd, invalidRgn);
	InvalidateMessage(hWnd, invalidRgn);
	InvalidateRgn(hWnd, invalidRgn, FALSE);
	DeleteObject(invalidRgn);

	// アニメーション開始
	timeBeginPeriod(1);
	timerID = timeSetEvent(timerPerFrame, 1, TimerProc, 0, TIME_ONESHOT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムの変更＆アニメーション
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

	// indexの修正
	while(i<0 || i>=size){
		if(i<0)				i += size;
		else if(i>=size)	i -= size;
		else break;
	}
	timerIndex = (UINT)i;

	// アニメーション
	AnimateItem(hWnd, timerIndex, direction, clockwise);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムアニメーション
//
void AnimateItem(HWND hWnd, UINT index, bool direction, bool clockwise)
{
	int fps;						// アニメーションのFPS
	int time;						// アニメーションする時間(ミリ秒)
	int frame;						// アニメーションのコマ数
	int perFrame;				// １コマあたりの時間(ミリ秒)

	// (smooth,FPS) = (0,10) ～ (100, 70)
	fps = (int)( (double)smooth * 0.6 + 10 );
	// (roteTime, time) = (0, 250%) ～ (100, 50%)
	time= 100 - (int)roteTime;
	time= (int)( (double)(time*time)*0.02 ) + 50;

	// frame, timerPerFrameの設定
	time = (int)((double)ANIMATION_HOR_TIME*((double)time / 100));	// アニメーションの時間
	frame = (int)(fps * time / 1000);								// 何回描画するか
	if(frame < 2) frame = 2;
	perFrame = (int)(time / frame) - 10;							// １コマあたりの時間
	if(perFrame > 1)	timerPerFrame = perFrame;
	else				timerPerFrame = 1;

	// rの設定
	timerR = 0;

	// thetaの設定
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

	bCurPaint = true;	// カーソルの描画設定
	bMsgPaint = true;	// メッセージの描画設定

	// アニメーション開始
	timeBeginPeriod(1);
	timerID = timeSetEvent(timerPerFrame, 1, TimerProc, 0, TIME_ONESHOT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 描画
//
void DrawWindow(HWND hWnd)
{
	// 描画領域のサイズ
	int width, height;
	width = rScreen.right-rScreen.left;
	height = rScreen.bottom-rScreen.top;

	PAINTSTRUCT ps;

	// 背景描画
	BitBlt(hBkDC, 0, 0, width, height, 
		hDskDC, rScreen.left, rScreen.top, SRCCOPY);
	// アイテム描画
	GI.at(iGI).drawIcon(hBkDC, &center);
	// カーソル描画
	if(bCurPaint) DrawCursor(hWnd, hBkDC);
	// メッセージ描画
	if(boolMsg && bMsgPaint) DrawMessage(hWnd, hBkDC);

	BeginPaint(hWnd, &ps);				// 描画開始

	// バックバッファをps.hdcに転送
	BitBlt(ps.hdc, 0, 0, width, height, hBkDC, 0, 0, SRCCOPY);

	EndPaint(hWnd, &ps);				// 描画終了
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// タイマー用関数(タイマーID,予約,ユーザー定義,予約,予約)
//
void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	HRGN invalidRgn;
	bool bKeyMore = false;
	int ret;
//*
	// タイマーを解除
	timeKillEvent(timerID);
	timeEndPeriod(1);
//*/
	timerCount++;

	// 再描画リージョンの初期化
	invalidRgn = CreateRectRgn(0,0,0,0);

	GI.at(iGI).moveDrawIcon(timerHWnd, invalidRgn, &center, true, 
		timerR, timerTheta);											// アイテム移動＆描画
	if(bCurPaint) InvalidateCursor(timerHWnd, invalidRgn);				// カーソル
	if(boolMsg && bMsgPaint) InvalidateMessage(timerHWnd, invalidRgn);	// メッセージ

	if(timerCount < timerMaxCount && timerHWnd != NULL){
		// 最後でないなら

		// 再描画
		InvalidateRgn(timerHWnd, invalidRgn, FALSE);
		DeleteObject(invalidRgn);
		UpdateWindow(timerHWnd);
//*
		// アニメーション再開
		timeBeginPeriod(1);
		timerID = timeSetEvent(timerPerFrame, 1, TimerProc, 0, TIME_ONESHOT);
//*/
	}else{
		// 最後なら
/*
		// タイマーを解除
		timeKillEvent(timerID);
		timeEndPeriod(1);
//*/
		// 何のタイマーかで分岐
		switch(timerType){
		// アイテム
		case TIMER_ITEM:
			// アイテムは移動後にSE
			if(boolSEHor) sndPlaySound(SEHor.c_str(), SND_ASYNC | SND_NODEFAULT);
			// indexを選択
			GI.at(iGI).setItemIndex(timerIndex);

			bActing = false;
			// メッセージの描画設定
			if(boolMsg){
				bMsgPaint = true;
				RenewMessage(timerHWnd, hBkDC, invalidRgn);
			}

			// 次のアクション検索
			bKeyMore = true;
			break;

		// グループ（１）
		case TIMER_GROUP1:
			// 位置の修正
			GI.at(iGI).moveDrawIcon(timerHWnd, invalidRgn, &center, false, 
				-(timerR*timerMaxCount), -(timerTheta*timerMaxCount));
			// (描画後に２のアニメーション)
			break;

		// グループ（２）
		case TIMER_GROUP2:
			bActing = false;
			// カーソルの描画設定
			bCurPaint = true;
			RenewCursor(timerHWnd, invalidRgn);

			// メッセージの描画設定
			if(boolMsg){
				bMsgPaint = true;
				RenewMessage(timerHWnd, hBkDC, invalidRgn);
			}

			// 次のアクション検索
			bKeyMore = true;
			break;

		// 表示
		case TIMER_SHOW:
			bActing = false;
			// カーソルの描画設定
			bCurPaint = true;
			RenewCursor(timerHWnd, invalidRgn);

			// メッセージの描画設定
			if(boolMsg){
				bMsgPaint = true;
				RenewMessage(timerHWnd, hBkDC, invalidRgn);
			}

			// 次のアクション検索
			bKeyMore = true;
			break;

		// 隠す
		case TIMER_HIDE:
		case TIMER_FILEOPEN:
		case TIMER_FOLDEROPEN:
			// 位置の修正
			GI.at(iGI).moveDrawIcon(timerHWnd, invalidRgn, &center, false, 
				-(timerR*timerMaxCount), -(timerTheta*timerMaxCount));
			bActing = false;

			// まだアクションがあるならクリア
			while(!queKey.empty()) queKey.pop();

			DeleteDC(hDskDC);					// hDskDC削除
			ShowWindow(timerHWnd, SW_HIDE);		// 表示の設定
			UpdateWindow(timerHWnd);			// 更新
			bShow = false;						// フラグＯＦＦ
			
			// TIMER_HIDE以外のOPEN系なら開く
			if(timerType != TIMER_HIDE){
				if(timerType == TIMER_FILEOPEN)
					ret = GI.at(iGI).selectedItem().fileOpen();
				else
					ret = GI.at(iGI).selectedItem().folderOpen();

				if(ret!=0) MessageBox(timerHWnd, openError(ret).c_str(), "ERROR", MB_OK);
			}

			// 表示を初期位置に戻すなら
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

		// 再描画
		InvalidateRgn(timerHWnd, invalidRgn, FALSE);
		DeleteObject(invalidRgn);
		UpdateWindow(timerHWnd);

		MemoryClean();

		// グループ１なら次に２のアニメーション
		if(timerType == TIMER_GROUP1){
			// indexを選択
			iGI = timerIndex;
			AnimateGroup(timerHWnd, timerOut, timerOut, timerClockwise, TIMER_GROUP2);
		}

		// まだアクションがあるなら
		if(bKeyMore && !queKey.empty()){
			// キー検索
			SKKeySearch(timerHWnd, queKey.front());
			// 先頭のキーを削除
			queKey.pop();
		}
/*
		// HWndをNULL
		timerHWnd = NULL;
//*/
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// スタートアップに登録
//
void CreateStartup(HWND hWnd)
{
	HRESULT ret;
	string msg = "";

	msg.append("以下のパス\n");
	msg.append(GetFolderPath(CSIDL_STARTUP) + "\n");
	msg.append("にショートカットファイルを作成し、Windows起動時に自動実行するようにします。\n\n");
	msg.append("よろしいですか？");

	if(MessageBox(hWnd, msg.c_str(), APPNAME, MB_OKCANCEL | MB_ICONQUESTION) == IDOK){

		string path = GetFolderPath(CSIDL_STARTUP) + "\\SK Launch.lnk";
		ret = CreateShortcutFile(path, GetAppPath(), GetAppDir(), "", "", SW_SHOWNORMAL, "", 0);

		if( SUCCEEDED(ret) )	MessageBox(hWnd, "ショートカットファイルを作成しました。", APPNAME, MB_OK);
		else					MessageBox(hWnd, "作成に失敗しました。", APPNAME, MB_OK);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ショートカットファイルの出力
//
void CreateShortcutFiles(HWND hWnd)
{
	// デスクトップの位置
	string dskPath = GetFolderPath(CSIDL_DESKTOP) + "\\SKL Export";
	string msg = "";

	msg.append("デスクトップへ次のパス\n");
	msg.append(dskPath + "\n");
	msg.append("のようにフォルダを作成し、そこへ全アイテムを\n");
	msg.append("ショートカットファイルとして出力します。\n\n");
	msg.append("よろしいですか？");

	if(MessageBox(hWnd, msg.c_str(), APPNAME, MB_OKCANCEL | MB_ICONQUESTION) == IDOK){
		// ルートフォルダ作成
		CreateDirectory( dskPath.c_str(), NULL );

		int gSize = GI.size();
		for(int i=0; i<gSize; i++){
			SKGroupItem& group = GI.at(i);
			string groupPath = dskPath + "\\" + FileNameFilter(group.getCaption());
			// フォルダ作成
			CreateDirectory(groupPath.c_str(), NULL);

			int iSize = group.size();
			for(int j=0; j<iSize; j++){
				// ショートカットファイル作成
				if( FAILED(CreateShortcutFileFromItem(groupPath, group.item(j))) ){
					MessageBox(hWnd, "失敗しました。", APPNAME, MB_OK);
					return;
				}
			}
		}
		MessageBox(hWnd, "成功しました。", APPNAME, MB_OK);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SKShortcutItemからショートカットファイルを作成
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
// カーソルの描画位置を更新
//
void RenewCursor(HWND hWnd, HRGN invalidRgn)
{
	// 描画位置
	curX = center.x - curOffsetX;
	curY = center.y - curOffsetY - (int)GI.at(iGI).item(0).getR();
	InvalidateCursor(hWnd, invalidRgn);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// カーソルの描画領域を更新
//
void InvalidateCursor(HWND hWnd, HRGN invalidRgn)
{
	HRGN addRgn = NULL;
	// 追加する範囲
	addRgn = CreateRectRgn(curX, curY, curX + curOffsetX*2, curY + curOffsetY*2);
	// 再描画範囲にプラス
	CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
	// 不要なリージョン削除
	DeleteObject(addRgn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// カーソルの描画
//
void DrawCursor(HWND hWnd, HDC hDC)
{
	// マスクの描画
	BitBlt(hDC, curX, curY, curOffsetX*2, curOffsetY*2, hCur, curOffsetX*2, 0, SRCAND);
	// 実際の絵の描画
	BitBlt(hDC, curX, curY, curOffsetX*2, curOffsetY*2, hCur, 0, 0, SRCPAINT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッセージの各種変数を更新
//
void RenewMessage(HWND hWnd, HDC hDC, HRGN invalidRgn)
{
	int ox = 0, oy;

	InvalidateMessage(hWnd, invalidRgn);

	sMsg = EncordMessage();

	// 文字の描画サイズ
	while(ox == 0 || msgArea.right > rScreen.right || msgArea.bottom > rScreen.right){
		msgArea.left = 0;
		msgArea.right = 0;
		msgArea.top = 0;
		msgArea.bottom = 0;
		DrawText(hDC, sMsg.c_str(), sMsg.length(), &msgArea, DT_CALCRECT);
		ox++;
		if(ox>1) break;
	}

	// lengthに合わせる
	msgArea.right += msgLength - (msgArea.right % msgLength);
	msgArea.bottom += msgLength - (msgArea.bottom % msgLength);

	// 描画位置
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
// メッセージの内容を取得
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
// メッセージ背景の描画領域を更新
//
void InvalidateMessage(HWND hWnd, HRGN invalidRgn)
{
	HRGN addRgn;
	// 追加する範囲
	addRgn = CreateRectRgn(msgArea.left, msgArea.top, msgArea.right, msgArea.bottom);
	// 再描画範囲にプラス
	CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
	// 不要なリージョン削除
	DeleteObject(addRgn);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メッセージ背景の描画
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

	// 角の描画

	// マスク
	BitBlt(hDC, (int)msgArea.left,	(int)msgArea.top,	msgLength, msgLength, hMsg, 3*msgLength, 0,		SRCAND);
	BitBlt(hDC, x2,					(int)msgArea.top,	msgLength, msgLength, hMsg, 5*msgLength, 0,		SRCAND);
	BitBlt(hDC, (int)msgArea.left,	y2,					msgLength, msgLength, hMsg, 3*msgLength, len2,	SRCAND);
	BitBlt(hDC, x2,					y2,					msgLength, msgLength, hMsg, 5*msgLength, len2,	SRCAND);
	// 実際の絵
	BitBlt(hDC, (int)msgArea.left,	(int)msgArea.top,	msgLength, msgLength, hMsg, 0,		0,		SRCPAINT);
	BitBlt(hDC, x2,					(int)msgArea.top,	msgLength, msgLength, hMsg, len2,	0,		SRCPAINT);
	BitBlt(hDC, (int)msgArea.left,	y2,					msgLength, msgLength, hMsg, 0,		len2,	SRCPAINT);
	BitBlt(hDC, x2,					y2,					msgLength, msgLength, hMsg, len2,	len2,	SRCPAINT);


	// 辺の描画（横）
	for(x=x1; x<x2; x+=msgLength){
		// マスク
		BitBlt(hDC, x, (int)msgArea.top,	msgLength, msgLength, hMsg, len4, 0,	SRCAND);
		BitBlt(hDC, x, y2,					msgLength, msgLength, hMsg, len4, len2, SRCAND);
		// 実際の絵
		BitBlt(hDC, x, (int)msgArea.top,	msgLength, msgLength, hMsg, msgLength, 0,		SRCPAINT);
		BitBlt(hDC, x, y2,					msgLength, msgLength, hMsg, msgLength, len2,	SRCPAINT);
	}

	// 辺の描画（縦）
	for(y=y1; y<y2; y+=msgLength){
		// マスク
		BitBlt(hDC, (int)msgArea.left,	y,	msgLength, msgLength, hMsg, 3*msgLength, msgLength, SRCAND);
		BitBlt(hDC, x2,					y,	msgLength, msgLength, hMsg, 5*msgLength, msgLength, SRCAND);
		// 実際の絵
		BitBlt(hDC, (int)msgArea.left,	y,	msgLength, msgLength, hMsg, 0,		msgLength, SRCPAINT);
		BitBlt(hDC, x2,					y,	msgLength, msgLength, hMsg, len2,	msgLength, SRCPAINT);
	}

	// 内部の描画
	for(x=x1; x<x2; x+=msgLength){
		for(y=y1; y<y2; y+=msgLength){
			// マスク
			BitBlt(hDC, x, y, msgLength, msgLength, hMsg, len4, msgLength, SRCAND);
			// 実際の絵
			BitBlt(hDC, x, y, msgLength, msgLength, hMsg, msgLength, msgLength, SRCPAINT);
		}
	}

	// 文字の描画
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
// ホットキーの設定
//
bool SetHotKey(HWND hWnd, const string& strID, const Key& key)
{
	int ret = 0;
	UINT mask = 0;

	// 既に登録されていたら解除
	if(hotKeyID != 0) UnSetHotKey(hWnd);

	if(key.mask & MASK_SHIFT) mask |= MOD_SHIFT;
	if(key.mask & MASK_CTRL) mask |= MOD_CONTROL;
	if(key.mask & MASK_ALT) mask |= MOD_ALT;

	// グローバルアトムの追加
	hotKeyID = GlobalAddAtom(strID.c_str());
	// ホットキーの設定
	if(hotKeyID != 0) ret = RegisterHotKey(hWnd, hotKeyID, mask, key.nVirtKey);

	if(ret == 0)	return false;
	else			return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ホットキーの解除
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
// バージョン情報
//
LRESULT CALLBACK DlgProcVer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	string s;

    switch (msg) {
		// 作成時
        case WM_INITDIALOG:
			s.append(APPNAME);
			s.append("\n");
			s.append(VERSION);
			s.append("\n");
			s.append(COPYRIGHT);
			SetDlgItemText( hWnd, IDC_LABEL, s.c_str() );
            break;

		// コントロール
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
