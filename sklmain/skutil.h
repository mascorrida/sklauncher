/**
 *
 * util.cpp �֗��֐��̊񂹏W��
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
// typedef�錾
//
typedef unsigned char UCHAR;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���g�^�C�v�錾
//

// �������̏�����
void MemoryClean();
// �X�N���[���T�C�Y���擾
RECT GetScreenSize();
// ini�t�@�C���̓ǂݍ��݁E��������
int GetIniInt(const string& IniPath, const string& SectionName, const string& KeyName);
string GetIniStr(const string& IniPath, const string& SectionName, const string& KeyName, const string& sDefault);
void PutIni(const string& IniPath, const string& SectionName, const string& KeyName, const string& strPut);
// SK�pGetIni��
bool GetIniBool(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault);
UCHAR GetIniUCHAR(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault);
char GetIniChar(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault);
// ������ɕϊ�
string CStr(int arg);
string CStr(bool arg);
string CStr(UCHAR arg);
string CStr(char arg);
// �J�����g�f�B���N�g���̎擾
string GetAppDir();
// �N�����̃G���[������
string openError(const int error);
// �|�b�v�A�b�v���j���[
void PopupMenu(HWND hWnd, LPCSTR lpMenuName);
// �f�X�N�g�b�v�̃L���v�`�����쐬
HDC CreateDeskCapDC();
// �Â�����
void DrawDark(HDC hDC, int darkType, int darkValue);
// -PI����PI�͈̔͂ɂ���
float TrimMPItoPI(float theta);
// �X�^�[�g�A�b�v�̃p�X���擾
string GetFolderPath(int nFolder);
// �V���[�g�J�b�g�t�@�C���̍쐬
HRESULT CreateShortcutFile(const string& filePath, const string& path, const string& dirPath, const string& parameter, const string& comment, INT showCmd, const string& iconPath, UINT iconIndex);
// �������S�u��
void StringReplaceAll(string& s, const string& sFrom, const string& sTo);
// �t�@�C�������t�B���^�����O
string FileNameFilter(const string& fileName);
// �\�����@�̖��̂��擾
string GetShowCmdName(int showCmd);
// �L�[�̖��̂��擾
string GetKeyName(int keyCode, char keyShift);
// SetForeGroundWindow�̒��J�o�[�W����
void SetForegroundWindow2(HWND hWnd);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// -PI����PI�͈̔͂ɂ���
//
float TrimMPItoPI(float theta)
{
	float ret = theta;

	// �͈͊O�̊ԃ��[�v
	while(ret < -PI || ret >= PI){
		if(ret < -PI)	ret += (float)(2 * PI);
		else			ret -= (float)(2 * PI);
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����������ꂢ��
//
void MemoryClean()
{
	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �X�N���[���T�C�Y���擾
//
RECT GetScreenSize()
{
	RECT rScreen;

    SystemParametersInfo(SPI_GETWORKAREA, 0, &rScreen, 0);
	return(rScreen);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ini�t�@�C�����琔�l�擾
//
int GetIniInt(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	UINT i;
	i = GetPrivateProfileInt(SectionName.c_str(), KeyName.c_str(), nDefault, IniPath.c_str());
    return((int)i);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ini�t�@�C�����當����ǂݍ���(���������ꍇ:�󕶎���)
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
// ini�t�@�C���ɏ�������
//
void PutIni(const string& IniPath, const string& SectionName, const string& KeyName, const string& strPut)
{
    WritePrivateProfileString(SectionName.c_str(), KeyName.c_str(), strPut.c_str(), IniPath.c_str());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ini�t�@�C������bool�擾
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
// ini�t�@�C������UCHAR�擾
//
UCHAR GetIniUCHAR(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	int i;
	i = GetIniInt(IniPath, SectionName, KeyName, nDefault);
    return( (UCHAR)i );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ini�t�@�C������char�擾
//
char GetIniChar(const string& IniPath, const string& SectionName, const string& KeyName, int nDefault)
{
	int i;
	i = GetIniInt(IniPath, SectionName, KeyName, nDefault);
    return( (char)i );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������ɕϊ�
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
// exe�t�@�C���̂���f�B���N�g���p�X�̎擾
//
string GetAppDir()
{
	DWORD dwRet;
	TCHAR path[_MAX_PATH];
	string s;

	// ���s���W���[���̃t���p�X�̎擾
	dwRet = GetModuleFileName(NULL, path, sizeof(path));
	if(dwRet){
		s.assign(path);
		s.assign(s, 0, s.find_last_of('\\'));
	}

	return s;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// exe�t�@�C���̃t���p�X�̎擾
//
string GetAppPath()
{
	DWORD dwRet;
	TCHAR path[_MAX_PATH];
	string s;

	// ���s���W���[���̃t���p�X�̎擾
	dwRet = GetModuleFileName(NULL, path, sizeof(path));
	if(dwRet){
		s.assign(path);
	}

	return s;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �N�����̃G���[������
//
string openError(const int error)
{
	string ret;

	switch(error){
    case ERROR_FILE_NOT_FOUND:
        ret = "�w�肳�ꂽ�t�@�C����������܂���";
        break;

    case ERROR_PATH_NOT_FOUND:
        ret = "�w�肳�ꂽ�p�X��������܂���";
        break;
        
    case ERROR_BAD_FORMAT:
        ret = "EXE�t�@�C�����s���ł�";
        break;
        
    case SE_ERR_ACCESSDENIED:
        ret = "�t�@�C���ւ̃A�N�Z�X�����ۂ���܂���";
        break;
        
    case SE_ERR_ASSOCINCOMPLETE:
        ret = "�t�@�C�����̊֘A�t�����s�\���ł�";
        break;
        
    case SE_ERR_DDEBUSY:
        ret = "����DDE�ϊ�����������Ă��邽�ߏ����ł��܂���";
        break;
        
    case SE_ERR_DDEFAIL:
        ret = "DDE�ϊ��Ɏ��s���܂���";
        break;
        
    case SE_ERR_DDETIMEOUT:
        ret = "DDE�ϊ��v�����Ԑ؂�ł�";
        break;
        
    case SE_ERR_DLLNOTFOUND:
        ret = "DLL��������܂���";
        break;
        
    case SE_ERR_NOASSOC:
        ret = "�t�@�C�����̊g���q�Ƃ��Ċ֘A�t����ꂽ�A�v���P�[�V����������܂���";
        break;
        
    case SE_ERR_OOM:
        ret = "�����ɏ\���ȃ�����������܂���";
        break;
        
    case SE_ERR_SHARE:
        ret = "���L�ᔽ�ł�";
        break;
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �|�b�v�A�b�v���j���[
//
void PopupMenu(HWND hWnd, LPCSTR lpMenuName)
{
	POINT pt;		// �}�E�X���W
	HMENU hMenu;	//  ���j���[�n���h��
	HINSTANCE hInstance;

	GetCursorPos(&pt);
	// �C���X�^���X�n���h���擾
	hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	// ���j���[�n���h���擾
	hMenu = LoadMenu(hInstance, lpMenuName);
	hMenu = GetSubMenu(hMenu, 0);
	// ���j���[�\��
	SetForegroundWindow2(hWnd);
	TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
	PostMessage(hWnd, WM_NULL, 0, 0);
	// ���
	DestroyMenu(hMenu);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �f�X�N�g�b�v�̃L���v�`�����폜
//
HDC CreateDeskCapDC()
{
	// ��ʃT�C�Y
	POINT pSc;
    pSc.x = GetSystemMetrics(SM_CXSCREEN);
    pSc.y = GetSystemMetrics(SM_CYSCREEN);

	// �f�X�N�g�b�v��DC�擾
	HDC hDC = GetDC(NULL);
	// �r�b�g�}�b�v�n���h�����쐬
	HBITMAP hbm = CreateCompatibleBitmap(hDC, pSc.x, pSc.y);
	// ���������DC���R�s�[
	HDC hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hbm);
	// ���������DC�ɃL���v�`���摜��`��
	BitBlt(hMemDC, 0, 0, pSc.x, pSc.y, hDC, 0, 0, SRCCOPY);

	ReleaseDC(NULL, hDC);	// hDskDC���
	// hbm�͍폜���Ȃ��iDrawDark�Ŏg���̂Łj

	return hMemDC;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DC���Â��`�悷��
//
void DrawDark(HDC hDC, int darkType, int darkValue)
{
	// �Â����Ȃ��Ȃ�I��
	if(darkType==0 || darkValue==0) return;

	// ��ʃT�C�Y
	UINT width = GetSystemMetrics(SM_CXSCREEN);
	UINT height = GetSystemMetrics(SM_CYSCREEN);

	// �摜���
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize =			sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biPlanes =		1;
	bmi.bmiHeader.biWidth =			width;
	bmi.bmiHeader.biHeight =		height;
	bmi.bmiHeader.biBitCount =		32;
	bmi.bmiHeader.biCompression =	0;
	bmi.bmiHeader.biSizeImage =		width * height * 4;
	bmi.bmiHeader.biClrUsed =		0;
	// �摜�̓��e
	RGBQUAD *pixel = new RGBQUAD[width * height];

	// ��̃r�b�g�}�b�v���쐬
	HBITMAP hbm = CreateCompatibleBitmap(hDC, width, height);
	// ��̂�I��(����Ńr�b�g�}�b�vohbm���ҏW�\��)
	HBITMAP ohbm = (HBITMAP)SelectObject(hDC, hbm);

	// pixel, bmi�ɉ摜�����擾
	GetDIBits(hDC, ohbm, 0, height, pixel, &bmi, 0);

	if(darkType==1){
		// �ȈՕ`�� //////////////////////

		if(darkValue>=0 && darkValue<13){
			// 0%
			return;

		}else if(darkValue>=13 && darkValue<38){
			// 25%
			// ��������
			// ��������
			// ��������
			// ��������
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
			// ��������
			// ��������
			// ��������
			// ��������
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
			// ��������
			// ��������
			// ��������
			// ��������
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
			// ��������
			// ��������
			// ��������
			// ��������
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
		// �ڍו`��

		float ratio = (float)(100-darkValue) / 100;		// �Â����銄��

		for(int i=0; i<height; i++){
			for(int j=0; j<width; j++){
				int index = i * width + j;
				pixel[index].rgbRed *= ratio;
				pixel[index].rgbBlue *= ratio;
				pixel[index].rgbGreen *= ratio;
			}
		}
	}

	// �r�b�g�}�b�v���Z�b�g
	SetDIBits(hDC, ohbm, 0, height, pixel, &bmi, 0);
	// �I��
	SelectObject(hDC, ohbm);

	DeleteObject(hbm);		// ��r�b�g�}�b�v�폜
	DeleteObject(ohbm);		// �L���v�`�������r�b�g�}�b�v���폜
	delete[] pixel;			// �摜�̓��e�폜
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����̃p�X���擾
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
// �V���[�g�J�b�g�t�@�C���̍쐬
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
// �������S�u��
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
// �t�@�C�������t�B���^�����O
//
string FileNameFilter(const string& fileName)
{
	string ret = fileName;
	StringReplaceAll(ret, "\\", "��");
	StringReplaceAll(ret, "/", "�^");
	StringReplaceAll(ret, ":", "�F");
	StringReplaceAll(ret, "*", "��");
	StringReplaceAll(ret, "?", "�H");
	StringReplaceAll(ret, "\"", "�h");
	StringReplaceAll(ret, "<", "��");
	StringReplaceAll(ret, ">", "��");
	StringReplaceAll(ret, "|", "�b");

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �\�����@�̖��̂��擾
//
string GetShowCmdName(int showCmd)
{
	string ret;
	if(showCmd == SW_SHOWNORMAL)		ret.append("�ʏ�");
	else if(showCmd == SW_MAXIMIZE)		ret.append("�ő剻");
	else if(showCmd == SW_MINIMIZE)		ret.append("�ŏ���");

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �L�[�̖��̂��擾
//
string GetKeyName(int keyCode, char keyShift)
{
	string mask, ret;

	// �V�t�g�L�[
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
		// ���̑�
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
		case VK_LEFT:		ret.append("��");
			break;
		case VK_UP:			ret.append("��");
			break;
		case VK_RIGHT:		ret.append("��");
			break;
		case VK_DOWN:		ret.append("��");
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
// SetForeGroundWindow�̒��J�o�[�W����
//
void SetForegroundWindow2(HWND hWnd)
{
	HWND hWndFor = GetForegroundWindow();							// ���݃��[�U�[����Ƃ��Ă���E�B���h�E���擾
	DWORD ThrdFor = GetWindowThreadProcessId(hWndFor, 0);			// �t�H�A�O���E���h�E�B���h�E���쐬�����X���b�h��ID���擾
	DWORD ThrdCur = GetCurrentThreadId();							// �ړI�̃E�B���h�E���쐬�����X���b�h��ID���擾
	AttachThreadInput(ThrdCur, ThrdFor, 1);							// ���݂̓��͏�Ԃ�ړI�̃X���b�h�ɃA�^�b�`
	int buf = 0;
	SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &buf, 0);	// ���݂�[�t�H�A�O���E���h ���b�N �^�C���A�E�g]�̐ݒ���擾
	SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0);	// �ݒ�� 0ms �ɕύX
	SetForegroundWindow(hWnd);										// �悤�₭�A�{���̏����̂��o�܂�
	SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &buf, 0);	// �ݒ�����ɖ߂��āc
	AttachThreadInput(ThrdCur, ThrdFor, 0);							// �f�^�b�`���Ă����܂�
}