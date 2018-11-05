/**
 *
 * util.cpp 便利関数の寄せ集め
 *
 */

#include <windows.h>
#include <string>
#include <queue>
#include <shlobj.h>
#include "SKGroupItem.h"
#include "resource.h"

#if !defined(SPI_GETFOREGROUNDLOCKTIMEOUT)
#define SPI_GETFOREGROUNDLOCKTIMEOUT 0x2000
#define SPI_SETFOREGROUNDLOCKTIMEOUT 0x2001
#endif

using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// typedef宣言
//
typedef unsigned char UCHAR;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロトタイプ宣言
//

// メモリの初期化
void MemoryClean();
// スクリーンサイズを取得
RECT GetScreenSize();
// iniファイルの読み込み・書き込み
int GetIniInt(const string& IniPath, const string& SectionName, const string& KeyName);
string GetIniStr(const string& IniPath, const string& SectionName, const string& KeyName, const string& sDefault);
void PutIni(const string& IniPath, const string& SectionName, const string& KeyName, const string& strPut);
// SK用GetIni↓
bool GetIniBool(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault);
UCHAR GetIniUCHAR(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault);
char GetIniChar(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault);
// 文字列に変換
string CStr(int arg);
string CStr(bool arg);
string CStr(UCHAR arg);
string CStr(char arg);
// カレントディレクトリの取得
string GetAppDir();
// 起動時のエラー文字列
string openError(const int error);
// ポップアップメニュー
void PopupMenu(HWND hWnd, LPCSTR lpMenuName);
// デスクトップのキャプチャを作成
HDC CreateDeskCapDC();
// 暗くする
void DrawDark(HDC hDC, int darkType, int darkValue);
// -PIからPIの範囲にする
float TrimMPItoPI(float theta);
// スタートアップのパスを取得
string GetFolderPath(int nFolder);
// ショートカットファイルの作成
HRESULT CreateShortcutFile(const string& filePath, const string& path, const string& dirPath, const string& parameter, const string& comment, INT showCmd, const string& iconPath, UINT iconIndex);
// 文字列を全置換
void StringReplaceAll(string& s, const string& sFrom, const string& sTo);
// ファイル名をフィルタリング
string FileNameFilter(const string& fileName);
// 表示方法の名称を取得
string GetShowCmdName(int showCmd);
// キーの名称を取得
string GetKeyName(int keyCode, char keyShift);
// SetForeGroundWindowの丁寧バージョン
void SetForegroundWindow2(HWND hWnd);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// -PIからPIの範囲にする
//
float TrimMPItoPI(float theta)
{
	float ret = theta;

	// 範囲外の間ループ
	while(ret < -PI || ret >= PI){
		if(ret < -PI)	ret += (float)(2 * PI);
		else			ret -= (float)(2 * PI);
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// メモリをきれいに
//
void MemoryClean()
{
	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// スクリーンサイズを取得
//
RECT GetScreenSize()
{
	RECT rScreen;

    SystemParametersInfo(SPI_GETWORKAREA, 0, &rScreen, 0);
	return(rScreen);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルから数値取得
//
int GetIniInt(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	UINT i;
	i = GetPrivateProfileInt(SectionName.c_str(), KeyName.c_str(), nDefault, IniPath.c_str());
    return((int)i);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルから文字列読み込み(無かった場合:空文字列)
//
string GetIniStr(const string& IniPath, const string& SectionName, const string& KeyName, const string& sDefault)
{
    char strData[256];
	
    GetPrivateProfileString(SectionName.c_str(), KeyName.c_str(), sDefault.c_str(), strData, 256, IniPath.c_str());
	
	return strData;
//    return( Mid(strData, 1, InStr(1, strData, vbNullChar) - 1) );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルに書き込み
//
void PutIni(const string& IniPath, const string& SectionName, const string& KeyName, const string& strPut)
{
    WritePrivateProfileString(SectionName.c_str(), KeyName.c_str(), strPut.c_str(), IniPath.c_str());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルからbool取得
//
bool GetIniBool(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	int i;
	i = GetIniInt(IniPath, SectionName, KeyName, -10);
    if(i==-10)	return (nDefault!=0);
	else		return (i!=0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルからUCHAR取得
//
UCHAR GetIniUCHAR(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	int i;
	i = GetIniInt(IniPath, SectionName, KeyName, nDefault);
    return( (UCHAR)i );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// iniファイルからchar取得
//
char GetIniChar(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	int i;
	i = GetIniInt(IniPath, SectionName, KeyName, nDefault);
    return( (char)i );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 文字列に変換
//
string CStr(int arg)
{
	char str[256];
	sprintf(str, "%d", arg);
	return str;
}

string CStr(bool arg)
{
	if(arg)	return "1";
	else	return "0";
}

string CStr(UCHAR arg)
{
	return CStr((int)arg);
}

string CStr(char arg)
{
	return CStr((int)arg);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// exeファイルのあるディレクトリパスの取得
//
string GetAppDir()
{
	DWORD dwRet;
	TCHAR path[_MAX_PATH];
	string s;

	// 実行モジュールのフルパスの取得
	dwRet = GetModuleFileName(NULL, path, sizeof(path));
	if(dwRet){
		s.assign(path);
		s.assign(s, 0, s.find_last_of('\\'));
	}

	return s;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// exeファイルのフルパスの取得
//
string GetAppPath()
{
	DWORD dwRet;
	TCHAR path[_MAX_PATH];
	string s;

	// 実行モジュールのフルパスの取得
	dwRet = GetModuleFileName(NULL, path, sizeof(path));
	if(dwRet){
		s.assign(path);
	}

	return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 起動時のエラー文字列
//
string openError(const int error)
{
	string ret;

	switch(error){
    case ERROR_FILE_NOT_FOUND:
        ret = "指定されたファイルが見つかりません";
        break;

    case ERROR_PATH_NOT_FOUND:
        ret = "指定されたパスが見つかりません";
        break;
        
    case ERROR_BAD_FORMAT:
        ret = "EXEファイルが不正です";
        break;
        
    case SE_ERR_ACCESSDENIED:
        ret = "ファイルへのアクセスが拒否されました";
        break;
        
    case SE_ERR_ASSOCINCOMPLETE:
        ret = "ファイル名の関連付けが不十分です";
        break;
        
    case SE_ERR_DDEBUSY:
        ret = "他のDDE変換が処理されているため処理できません";
        break;
        
    case SE_ERR_DDEFAIL:
        ret = "DDE変換に失敗しました";
        break;
        
    case SE_ERR_DDETIMEOUT:
        ret = "DDE変換要求時間切れです";
        break;
        
    case SE_ERR_DLLNOTFOUND:
        ret = "DLLが見つかりません";
        break;
        
    case SE_ERR_NOASSOC:
        ret = "ファイル名の拡張子として関連付けられたアプリケーションがありません";
        break;
        
    case SE_ERR_OOM:
        ret = "処理に十分なメモリがありません";
        break;
        
    case SE_ERR_SHARE:
        ret = "共有違反です";
        break;
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ポップアップメニュー
//
void PopupMenu(HWND hWnd, LPCSTR lpMenuName)
{
	POINT pt;		// マウス座標
	HMENU hMenu;	//  メニューハンドル
	HINSTANCE hInstance;

	GetCursorPos(&pt);
	// インスタンスハンドル取得
	hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	// メニューハンドル取得
	hMenu = LoadMenu(hInstance, lpMenuName);
	hMenu = GetSubMenu(hMenu, 0);
	// メニュー表示
	SetForegroundWindow2(hWnd);
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
	PostMessage(hWnd, WM_NULL, 0, 0);
	// 解放
	DestroyMenu(hMenu);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// デスクトップのキャプチャを削除
//
HDC CreateDeskCapDC()
{
	// 画面サイズ
	POINT pSc;
    pSc.x = GetSystemMetrics(SM_CXSCREEN);
    pSc.y = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップのDC取得
	HDC hDC = GetDC(NULL);
	// ビットマップハンドルを作成
	HBITMAP hbm = CreateCompatibleBitmap(hDC, pSc.x, pSc.y);
	// メモリ上にDCをコピー
	HDC hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hbm);
	// メモリ上のDCにキャプチャ画像を描画
	BitBlt(hMemDC, 0, 0, pSc.x, pSc.y, hDC, 0, 0, SRCCOPY);

	ReleaseDC(NULL, hDC);	// hDskDC解放
	// hbmは削除しない（DrawDarkで使うので）

	return hMemDC;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DCを暗く描画する
//
void DrawDark(HDC hDC, int darkType, int darkValue)
{
	// 暗くしないなら終了
	if(darkType==0 || darkValue==0) return;

	// 画面サイズ
	UINT width = GetSystemMetrics(SM_CXSCREEN);
	UINT height = GetSystemMetrics(SM_CYSCREEN);

	// 画像情報
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize =			sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biPlanes =		1;
	bmi.bmiHeader.biWidth =			width;
	bmi.bmiHeader.biHeight =		height;
	bmi.bmiHeader.biBitCount =		32;
	bmi.bmiHeader.biCompression =	0;
	bmi.bmiHeader.biSizeImage =		width * height * 4;
	bmi.bmiHeader.biClrUsed =		0;
	// 画像の内容
	RGBQUAD *pixel = new RGBQUAD[width * height];

	// 空のビットマップを作成
	HBITMAP hbm = CreateCompatibleBitmap(hDC, width, height);
	// 空のを選択(これでビットマップohbmが編集可能に)
	HBITMAP ohbm = (HBITMAP)SelectObject(hDC, hbm);

	// pixel, bmiに画像情報を取得
	GetDIBits(hDC, ohbm, 0, height, pixel, &bmi, 0);

	if(darkType==1){
		// 簡易描画 //////////////////////

		if(darkValue>=0 && darkValue<13){
			// 0%
			return;

		}else if(darkValue>=13 && darkValue<38){
			// 25%
			// ■□■□
			// □□□□
			// ■□■□
			// □□□□
			for(int i=0; i<height; i++){
				for(int j=0; j<width; j++){
					if((i&1) & (j&1)){
						int index = i * width + j;
						pixel[index].rgbRed = 0;
						pixel[index].rgbBlue = 0;
						pixel[index].rgbGreen = 0;
					}
				}
			}

		}else if(darkValue>=38 && darkValue<63){
			// 50%
			// ■□■□
			// □■□■
			// ■□■□
			// □■□■
			for(int i=0; i<height; i++){
				for(int j=0; j<width; j++){
					if((i&1) ^ (j&1)){
						int index = i * width + j;
						pixel[index].rgbRed = 0;
						pixel[index].rgbBlue = 0;
						pixel[index].rgbGreen = 0;
					}
				}
			}

		}else if(darkValue>=63 && darkValue<88){
			// 75%
			// □■□■
			// ■■■■
			// □■□■
			// ■■■■
			for(int i=0; i<height; i++){
				for(int j=0; j<width; j++){
					if(!(i&1) | !(j&1)){
						int index = i * width + j;
						pixel[index].rgbRed = 0;
						pixel[index].rgbBlue = 0;
						pixel[index].rgbGreen = 0;
					}
				}
			}

		}else{
			// 100%
			// ■■■■
			// ■■■■
			// ■■■■
			// ■■■■
			for(int i=0; i<height; i++){
				for(int j=0; j<width; j++){
					int index = i * width + j;
					pixel[index].rgbRed = 0;
					pixel[index].rgbBlue = 0;
					pixel[index].rgbGreen = 0;
				}
			}
		}

	}else if(darkType==2){
		// 詳細描画

		float ratio = (float)(100-darkValue) / 100;		// 暗くする割合

		for(int i=0; i<height; i++){
			for(int j=0; j<width; j++){
				int index = i * width + j;
				pixel[index].rgbRed *= ratio;
				pixel[index].rgbBlue *= ratio;
				pixel[index].rgbGreen *= ratio;
			}
		}
	}

	// ビットマップをセット
	SetDIBits(hDC, ohbm, 0, height, pixel, &bmi, 0);
	// 選択
	SelectObject(hDC, ohbm);

	DeleteObject(hbm);		// 空ビットマップ削除
	DeleteObject(ohbm);		// キャプチャしたビットマップも削除
	delete[] pixel;			// 画像の内容削除
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 特殊のパスを取得
//
string GetFolderPath(int nFolder)
{
	LPITEMIDLIST pidl;
	LPMALLOC pMalloc;

	if( SHGetMalloc(&pMalloc) != NOERROR ) return "";
	if( SHGetSpecialFolderLocation(NULL, nFolder, &pidl) != S_OK ) return "";

	char path[MAX_PATH+1];
	if( SHGetPathFromIDList(pidl, path) == FALSE ) return "";
	pMalloc->Free(pidl);

	return path;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ショートカットファイルの作成
//
HRESULT CreateShortcutFile(const string& filePath,
						   const string& path,
						   const string& dirPath,
						   const string& parameter,
						   const string& comment,
						   INT showCmd,
						   const string& iconPath,
						   UINT iconIndex)
{
    IShellLink *ShellLink;
    HRESULT hRes;

    CoInitialize(NULL);
    
    hRes = CoCreateInstance(
               CLSID_ShellLink,
               NULL,
               CLSCTX_INPROC_SERVER,
               IID_IShellLink,
               (void **)&ShellLink
           );

           
    if(SUCCEEDED(hRes)){

        IPersistFile * PersistFile;

        hRes = ShellLink->QueryInterface(
                        IID_IPersistFile,
                        (void **)&PersistFile
                    );
        
        if(SUCCEEDED(hRes)){
            wchar_t wfilename[MAX_PATH+1];
            MultiByteToWideChar(
                        CP_ACP,0,filePath.c_str(),-1,
                        wfilename,MAX_PATH+1
                    );

            ShellLink->SetPath(path.c_str());
            ShellLink->SetArguments(parameter.c_str());
            ShellLink->SetWorkingDirectory(dirPath.c_str());
            ShellLink->SetDescription(comment.c_str());
            ShellLink->SetShowCmd(showCmd);
			if(iconPath.compare("") != 0)
				ShellLink->SetIconLocation(iconPath.c_str(), (int)iconIndex);

            hRes = PersistFile->Save(wfilename,TRUE);
            
            PersistFile->Release();
        }
        ShellLink->Release();
    }
    CoUninitialize();
    return hRes;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 文字列を全置換
//
void StringReplaceAll(string& s, const string& sFrom, const string& sTo)
{
	int i;

	do{
		i = s.find(sFrom, 0);
		if(i != string::npos) s.replace(i, sFrom.length(), sTo);

	}while(i != string::npos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ファイル名をフィルタリング
//
string FileNameFilter(const string& fileName)
{
	string ret = fileName;
	StringReplaceAll(ret, "\\", "￥");
	StringReplaceAll(ret, "/", "／");
	StringReplaceAll(ret, ":", "：");
	StringReplaceAll(ret, "*", "＊");
	StringReplaceAll(ret, "?", "？");
	StringReplaceAll(ret, "\"", "”");
	StringReplaceAll(ret, "<", "＜");
	StringReplaceAll(ret, ">", "＞");
	StringReplaceAll(ret, "|", "｜");

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 表示方法の名称を取得
//
string GetShowCmdName(int showCmd)
{
	string ret;
	if(showCmd == SW_SHOWNORMAL)		ret.append("通常");
	else if(showCmd == SW_MAXIMIZE)		ret.append("最大化");
	else if(showCmd == SW_MINIMIZE)		ret.append("最小化");

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// キーの名称を取得
//
string GetKeyName(int keyCode, char keyShift)
{
	string mask, ret;

	// シフトキー
	if(keyShift & 1)	mask.append("Shift + ");
	if(keyShift & 2)	mask.append("Ctrl + ");
	if(keyShift & 4)	mask.append("Alt + ");

	if(keyCode >= 'A' && keyCode <= 'Z'){
		// AtoZ
		ret.append(1, (char)keyCode);

	}else if(keyCode >= '0' && keyCode <= '9'){
		// 0-9
		ret.append(1, (char)(keyCode));

	}else if(keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9){
		// Numpad0-9
		ret.append("NumPad" + CStr(keyCode - VK_NUMPAD0));

	}else{
		// その他
		switch(keyCode){
		case VK_BACK:		ret.append("BackSpace");
			break;
		case VK_TAB:		ret.append("Tab");
			break;
		case VK_CLEAR:		ret.append("Clear");
			break;
		case VK_RETURN:		ret.append("Enter");
			break;
		case VK_PAUSE:		ret.append("Pause");
			break;
		case VK_CAPITAL:	ret.append("CapsLock");
			break;
		case VK_KANA:		ret.append("Kana");
			break;
		case VK_JUNJA:		ret.append("Junja");
			break;
		case VK_FINAL:		ret.append("Final");
			break;
		case VK_KANJI:		ret.append("Kanji");
			break;
		case VK_ESCAPE:		ret.append("Esc");
			break;
		case VK_CONVERT:	ret.append("Convert");
			break;
		case VK_NONCONVERT:	ret.append("NonConvert");
			break;
		case VK_ACCEPT:		ret.append("Accept");
			break;
		case VK_MODECHANGE:	ret.append("ModeChange");
			break;
		case VK_SPACE:		ret.append("Space");
			break;
		case VK_PRIOR:		ret.append("PageUp");
			break;
		case VK_NEXT:		ret.append("PageDown");
			break;
		case VK_END:		ret.append("End");
			break;
		case VK_HOME:		ret.append("Home");
			break;
		case VK_LEFT:		ret.append("←");
			break;
		case VK_UP:			ret.append("↑");
			break;
		case VK_RIGHT:		ret.append("→");
			break;
		case VK_DOWN:		ret.append("↓");
			break;
		case VK_SELECT:		ret.append("Select");
			break;
		case VK_PRINT:		ret.append("Print");
			break;
		case VK_EXECUTE:	ret.append("Execute");
			break;
		case VK_SNAPSHOT:	ret.append("PrintScreen");
			break;
		case VK_INSERT:		ret.append("Insert");
			break;
		case VK_DELETE:		ret.append("Delete");
			break;
		case VK_HELP:		ret.append("Help");
			break;
		case VK_LWIN:
		case VK_RWIN:		ret.append("Windows");
			break;
		case VK_APPS:		ret.append("Apps");
			break;
		case VK_MULTIPLY:	ret.append("NumPad*");
			break;
		case VK_ADD:		ret.append("NumPad+");
			break;
		case VK_SEPARATOR:	ret.append("NumPadEnter");
			break;
		case VK_SUBTRACT:	ret.append("NumPad-");
			break;
		case VK_DECIMAL:	ret.append("NumPadDot");
			break;
		case VK_DIVIDE:		ret.append("NumPad/");
			break;
		case VK_F1:			ret.append("F1");
			break;
		case VK_F2:			ret.append("F2");
			break;
		case VK_F3:			ret.append("F3");
			break;
		case VK_F4:			ret.append("F4");
			break;
		case VK_F5:			ret.append("F5");
			break;
		case VK_F6:			ret.append("F6");
			break;
		case VK_F7:			ret.append("F7");
			break;
		case VK_F8:			ret.append("F8");
			break;
		case VK_F9:			ret.append("F9");
			break;
		case VK_F10:		ret.append("F10");
			break;
		case VK_F11:		ret.append("F11");
			break;
		case VK_F12:		ret.append("F12");
			break;
		case VK_F13:		ret.append("F13");
			break;
		case VK_F14:		ret.append("F14");
			break;
		case VK_F15:		ret.append("F15");
			break;
		case VK_F16:		ret.append("F16");
			break;
		case VK_F17:		ret.append("F17");
			break;
		case VK_F18:		ret.append("F18");
			break;
		case VK_F19:		ret.append("F19");
			break;
		case VK_F20:		ret.append("F20");
			break;
		case VK_F21:		ret.append("F21");
			break;
		case VK_F22:		ret.append("F22");
			break;
		case VK_F23:		ret.append("F23");
			break;
		case VK_F24:		ret.append("F24");
			break;
		case VK_NUMLOCK:	ret.append("NumLock");
			break;
		case VK_SCROLL:		ret.append("ScrollLock");
			break;
		}
	}
	if(ret.compare("") == 0)	return "";
	else						return mask + ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SetForeGroundWindowの丁寧バージョン
//
void SetForegroundWindow2(HWND hWnd)
{
	HWND hWndFor = GetForegroundWindow();							// 現在ユーザーが作業しているウィンドウを取得
	DWORD ThrdFor = GetWindowThreadProcessId(hWndFor, 0);			// フォアグラウンドウィンドウを作成したスレッドのIDを取得
	DWORD ThrdCur = GetCurrentThreadId();							// 目的のウィンドウを作成したスレッドのIDを取得
	AttachThreadInput(ThrdCur, ThrdFor, 1);							// 現在の入力状態を目的のスレッドにアタッチ
	int buf = 0;
	SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &buf, 0);	// 現在の[フォアグラウンド ロック タイムアウト]の設定を取得
	SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0);	// 設定を 0ms に変更
	SetForegroundWindow(hWnd);										// ようやく、本命の処理のお出まし
	SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &buf, 0);	// 設定を元に戻して…
	AttachThreadInput(ThrdCur, ThrdFor, 0);							// デタッチしておしまい
}