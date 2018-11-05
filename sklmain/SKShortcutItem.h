/**
 *
 * SKShortcutItem.h SKランチャ用ショートカットクラス
 *
 */

//#include "ShortcutItem.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 定数
//

// 円周率
#define PI				(3.141593)
// 円周率
#define ICON_NOTFOUND	"notfound.ico"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロトタイプ宣言
//

string subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd);

// skutil.hの関数↓ ////////////////////////////////////////

// カレントディレクトリの取得
string GetAppDir();


/**
 * ショートカットアイテムクラス SKShortcutItem
 */
class SKShortcutItem: public ShortcutItem
{
private:
	// 内部メンバ /////////////////////////

	// キャプション
	// パス
	// 作業フォルダのパス
	// コメント
	// コマンドライン引数

	INT m_showCmd;			// 表示方法
	string m_iconPath;		// アイコンのパス
	UINT m_iconIndex;		// アイコンの位置

	float m_r;				// 動径
	float m_theta;			// 方位角(0～2PI)

	int m_keyChar;			// ジャンプキー
	char m_keyShift;		// ジャンプキーシフト

	HICON m_hIcon;			// アイコンのハンドル

	// 内部メソッド /////////////////////////

	// 角度を調整
	const float trim0to2PI(float theta) const;
	const float trimMPItoPI(float theta) const;
	// getFromXMLのサブルーチン
	const string subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const;
	// アイコンを設定
	const bool setIcon();
	const HICON getIconFromFile(const string& path, UINT index, bool bIconFile) const;

public:
	// Getter /////////////////////////

	const INT getShowCmd() const;		// 表示方法
	const string& getIconPath() const;	// アイコンのパス
	const UINT getIconIndex() const;	// アイコンの位置
	const HICON getHIcon() const;		// アイコンのハンドル
	const int getKeyChar() const;		// ジャンプキー
	const char getKeyShift() const;		// ジャンプキーシフト
	const float getR() const;			// 動径
	const float getTheta() const;		// 方位角

	// Setter /////////////////////////

	void setShowCmd(const INT arg);			// 表示方法
	void setPath(const string& arg);		// パス
	void setIconPath(const string& arg);	// アイコンのパス
	void setIconIndex(const UINT arg);		// アイコンの位置
	void setKeyChar(const int arg);			// ジャンプキー
	void setKeyShift(const char arg);		// ジャンプキーシフト


	// 公開メソッド /////////////////////////

	// コンストラクタ
	SKShortcutItem();
	// デストラクタ
	virtual ~SKShortcutItem();

    // コピーコンストラクタ
    SKShortcutItem(const SKShortcutItem& other);
    // 代入演算子
    SKShortcutItem& operator=(const SKShortcutItem& other);

	// 位置を取得
	const int getX() const;
	const int getY() const;

	// 位置を設定
	const void setR(float r);
	const void addR(float r);
	const void setTheta(float theta);
	const void addTheta(float theta);
	const void setLoacation(float r, float theta);
	const void addLoacation(float r, float theta);

	// ファイルを開く
	const int fileOpen() const;
	const int folderOpen() const;
	
	// ファイルが存在するか調べる
	const bool fileExist() const;
	
	// アイコンファイルが存在するか調べる
	const bool iconExist() const;

	// XMLから読み込む
	const bool getFromXML(const string& data);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コンストラクタ
//
inline SKShortcutItem::SKShortcutItem()
{
	// string型は全て空文字列で初期化される

	m_showCmd = SW_SHOWNORMAL;	// 表示方法
	m_iconIndex = 0;			// アイコンの位置

	m_r = 0;					// 動径
	m_theta = 0;				// 方位角(0～2PI)

	m_keyChar = 0;				// ジャンプキー
	m_keyShift = 0;				// ジャンプキーシフト

	m_hIcon = NULL;				// アイコンのハンドル
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// デストラクタ
//
inline SKShortcutItem::~SKShortcutItem()
{
	// アイコンの削除　
	if(iconExist()) DestroyIcon(m_hIcon);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コピーコンストラクタ
//
inline SKShortcutItem::SKShortcutItem(const SKShortcutItem& other)
{
	string caption(other.getCaption());
	m_caption.assign(caption);

	string path(other.getPath());
	m_path.assign(path);

	string dirPath(other.getDirPath());
	m_dirPath.assign(dirPath);
	
	string comment(other.getComment());
	m_comment.assign(comment);
	
	string parameter(other.getParameter());
	m_parameter.assign(parameter);
	
	m_showCmd = other.getShowCmd();
	
	string iconPath(other.getIconPath());
	m_iconPath.assign(iconPath);
	m_iconIndex = other.getIconIndex();

	m_keyChar = other.getKeyChar();
	m_keyShift = other.getKeyShift();

	m_r = other.getR();
	m_theta = other.getTheta();

	setIcon();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 代入演算子
//
inline SKShortcutItem& SKShortcutItem::operator=(const SKShortcutItem& other)
{
	string caption(other.getCaption());
	m_caption.assign(caption);

	string path(other.getPath());
	m_path.assign(path);

	string dirPath(other.getDirPath());
	m_dirPath.assign(dirPath);
	
	string comment(other.getComment());
	m_comment.assign(comment);
	
	string parameter(other.getParameter());
	m_parameter.assign(parameter);
	
	m_showCmd = other.getShowCmd();
	
	string iconPath(other.getIconPath());
	m_iconPath.assign(iconPath);
	
	m_iconIndex = other.getIconIndex();
	m_keyChar = other.getKeyChar();
	m_keyShift = other.getKeyShift();

	setIcon();
	return(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 表示方法
inline const INT SKShortcutItem::getShowCmd() const
{
	return m_showCmd;
}

// アイコンのパス
inline const string& SKShortcutItem::getIconPath() const
{
	return m_iconPath;
	m_r;
}

// アイコンの位置
inline const UINT SKShortcutItem::getIconIndex() const
{
	return m_iconIndex;
}

// アイコンのハンドル
inline const HICON SKShortcutItem::getHIcon() const
{
	return m_hIcon;
}

// ジャンプキー
inline const int SKShortcutItem::getKeyChar() const
{
	return m_keyChar;
}

// ジャンプキーシフト
inline const char SKShortcutItem::getKeyShift() const
{
	return m_keyShift;
}

// 動径
inline const float SKShortcutItem::getR() const
{
	return m_r;
}

// 方位角
inline const float SKShortcutItem::getTheta() const
{
	return m_theta;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 表示方法
inline void SKShortcutItem::setShowCmd(const INT arg)
{
	m_showCmd = arg;
}

// パス
inline void SKShortcutItem::setPath(const string& arg)
{
	string s(arg);
	m_path.assign(s);
	setIcon();
}

// アイコンのパス
inline void SKShortcutItem::setIconPath(const string& arg)
{
	string s(arg);
	m_iconPath.assign(s);
	setIcon();
}

// アイコンの位置
inline void SKShortcutItem::setIconIndex(const UINT arg)
{
	m_iconIndex = arg;
	setIcon();
}

// ジャンプキー
inline void SKShortcutItem::setKeyChar(const int arg)
{
	m_keyChar = arg;
}

// ジャンプキーシフト
inline void SKShortcutItem::setKeyShift(const char arg)
{
	m_keyShift = arg;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 位置を取得
//
inline const int SKShortcutItem::getX() const
{
	float x = m_r * cos(m_theta) + 0.5;
	return (int)x;
}

inline const int SKShortcutItem::getY() const
{
	float y = m_r * sin(m_theta) + 0.5;
	return (int)y;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 位置を設定
//
inline const void SKShortcutItem::setR(float r)
{
	if(r < 0)	m_r = 0;
	else		m_r = r;
}

inline const void SKShortcutItem::addR(float r)
{
	if(m_r + r < 0)	m_r = 0;
	else			m_r += r;
}

inline const void SKShortcutItem::setTheta(float theta)
{
	m_theta = trim0to2PI(theta);
}

inline const void SKShortcutItem::addTheta(float theta)
{
	m_theta = trim0to2PI(m_theta+theta);
}

inline const void SKShortcutItem::setLoacation(float r, float theta)
{
	setR(r);
	setTheta(theta);
}

inline const void SKShortcutItem::addLoacation(float r, float theta)
{
	addR(r);
	addTheta(theta);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 0から2PIの範囲にする
//
inline const float SKShortcutItem::trim0to2PI(float theta) const
{
	float ret = theta;

	// 範囲外の間ループ
	while(ret < 0 || ret >= 2 * PI){
		if(ret < 0)	ret += (float)(2 * PI);
		else		ret -= (float)(2 * PI);
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// -PIからPIの範囲にする
//
inline const float SKShortcutItem::trimMPItoPI(float theta) const
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
// ファイルを開く(正常:0, エラー:0以外のエラー定数)
//
inline const int SKShortcutItem::fileOpen() const
{
	int ret;
	ret = (int)ShellExecute(0, NULL, m_path.c_str(), m_parameter.c_str(), m_dirPath.c_str(), m_showCmd);
	
	if(ret<=32)	return ret;
	else		return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ファイルを開く(正常:0, エラー:0以外のエラー定数)
//
inline const int SKShortcutItem::folderOpen() const
{
	int ret;
	ret = (int)ShellExecute(0, NULL, m_dirPath.c_str(), m_parameter.c_str(), m_dirPath.c_str(), SW_SHOWNORMAL);
	
	if(ret<=32)	return ret;
	else		return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ファイルが存在するか調べる
//
inline const bool SKShortcutItem::fileExist() const
{
	return( GetFileAttributes(m_path.c_str()) != (DWORD)-1 );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイコンファイルが存在するか調べる
//
inline const bool SKShortcutItem::iconExist() const
{
	if(m_path.compare("") != 0)
		if(GetFileAttributes(m_path.c_str()) != (DWORD)-1)
			return true;
		else
			return false;
	else
		if(GetFileAttributes(m_iconPath.c_str()) != (DWORD)-1)
			return true;
		else
			return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XMLから読み込む
//
inline const bool SKShortcutItem::getFromXML(const string& data)
{
	UINT posStt, posEnd;
	string buff;

	// posStt, posEndの設定
	posStt = data.find("<ShortcutItem>", 0);
	if(posStt == string::npos) return false;	// ない場合失敗
	buff = "</ShortcutItem>";
	posEnd = data.find(buff, 0);
	if(posEnd == string::npos) return false;	// ない場合失敗
	posEnd += buff.length();

	// キャプション
	buff = subStrXML("Caption", data, posStt, posEnd);
	if(buff != "")	m_caption = buff;
	else			return false;		// 必須項目
	// パス
	buff = subStrXML("Path", data, posStt, posEnd);
	if(buff != "")	m_path = buff;
	else			return false;		// 必須項目
	// コメント
	buff = subStrXML("Comment", data, posStt, posEnd);
	if(buff != "")	m_comment = buff;
	else			m_comment = "";
	// 作業パス
	buff = subStrXML("DirPath", data, posStt, posEnd);
	if(buff != "")	m_dirPath = buff;
	else			m_dirPath = "";
	// コマンドライン
	buff = subStrXML("Param", data, posStt, posEnd);
	if(buff != "")	m_parameter = buff;
	else			m_parameter = "";
	// 表示方法
	buff = subStrXML("ShowCmd", data, posStt, posEnd);
	if(buff != "")	m_showCmd = atoi(buff.c_str());
	else			m_showCmd = 0;
	// アイコンのパス
	buff = subStrXML("IconPath", data, posStt, posEnd);
	if(buff != "")	m_iconPath = buff;
	else			m_iconPath = "";
	// アイコンの位置
	buff = subStrXML("IconIndex", data, posStt, posEnd);
	if(buff != "")	m_iconIndex = atoi(buff.c_str());
	else			m_iconIndex = 0;
	// ジャンプキー
	buff = subStrXML("KeyChar", data, posStt, posEnd);
	if(buff != "")	m_keyChar = atoi(buff.c_str());
	else			m_keyChar = 0;
	// ジャンプキーシフト
	buff = subStrXML("KeyShift", data, posStt, posEnd);
	if(buff != "")	m_keyShift = atoi(buff.c_str());
	else			m_keyShift = 0;

	return true;	// 成功
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// getFromXMLのサブルーチン
//
inline const string SKShortcutItem::subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const
{
	UINT pos1, pos2;

	// {<word} と {>} の検索
	pos1 = data.find("<" + word, posStt);
	if(pos1 == string::npos) return "";	// ないなら失敗
	pos2 = data.find(">", pos1);
	if(pos2 == string::npos) return "";	// ないなら失敗

	// {<word>～</word>} かを判断
	if( data.substr(pos2-1, 1).compare("/") != 0 && pos2 < posEnd){
		pos1 = pos2 + 1;
		pos2 = data.find("</" + word + ">", pos1);
		
		if(pos1 == pos2) return "";	// 空文字列なら失敗

		return data.substr(pos1, pos2 - pos1);

	}else return "";
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイコンを設定
//
inline const bool SKShortcutItem::setIcon()
{
	if( iconExist() ){
		// アイコンファイルがあるなら
		DestroyIcon(m_hIcon);		// 今のアイコン削除
		if(m_iconPath.compare("") == 0){
			// アイコンファイルが指定されていないなら
			m_hIcon = getIconFromFile(m_path.c_str(), NULL, false);
			
		}else{
			// アイコンファイルが指定されているなら
			m_hIcon = getIconFromFile(m_iconPath.c_str(), m_iconIndex, true);
		}
		return true;

	}else{
		// アイコンファイルがないならNOT FOUNDアイコン
		string s;
		s = GetAppDir() + "\\" + ICON_NOTFOUND;
        m_hIcon = getIconFromFile(s, 0, true);
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ファイルからアイコンを取得
//
inline const HICON SKShortcutItem::getIconFromFile(const string& path, UINT index, bool bIconFile) const
{
	if(bIconFile){
		// アイコンファイル(exe,ico,dll,icl)なら
		HICON hIcon;
		ExtractIconEx(path.c_str(), (int)index, &hIcon, NULL, 1);
		return hIcon;

	}else{
		// アイコンファイルでないなら
		SHFILEINFOA fi;
		SHGetFileInfo(path.c_str(), FILE_ATTRIBUTE_ARCHIVE, &fi, sizeof(fi), SHGFI_ICON | SHGFI_LARGEICON);
		return fi.hIcon;
	}
}
