/**
 *
 * ShortcutItem.h ショートカットの（半抽象）クラス
 *
 */

#include <string>

using namespace std;

/**
 * 親クラス ShortcutItem
 */
class ShortcutItem
{
protected:
	// 内部メンバ /////////////////////////

	string m_caption;		// キャプション
	string m_path;			// パス
	string m_dirPath;		// 作業フォルダのパス
	string m_comment;		// コメント
	string m_parameter;		// コマンドライン引数

public:
	// Getter /////////////////////////

	const string getCaption() const;	// キャプション
	const string getPath() const;		// パス
	const string getDirPath() const;	// 作業フォルダのパス
	const string getComment() const;	// コメント
	const string getParameter() const;	// コマンドライン引数

	// Setter /////////////////////////

	void setCaption(const string& arg);		// キャプション
	void setPath(const string& arg);		// パス
	void setDirPath(const string& arg);		// 作業フォルダのパス
	void setComment(const string& arg);		// コメント
	void setParameter(const string& arg);	// コマンドライン引数
	
	// 公開メソッド /////////////////////////
	
	// コンストラクタ
	ShortcutItem(){}
	ShortcutItem::ShortcutItem
		(const string& caption, 
		 const string& path,
		 const string& dirPath,
		 const string& comment,
		 const string& parameter);
	// デストラクタ
	virtual ~ShortcutItem(){}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コンストラクタ(引数指定)
//
inline ShortcutItem::ShortcutItem
	(const string& caption, 
	 const string& path,
	 const string& dirPath,
	 const string& comment,
	 const string& parameter)
{
	setCaption(caption);
	setPath(path);
	setDirPath(dirPath);
	setComment(comment);
	setParameter(parameter);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// キャプション
inline const string ShortcutItem::getCaption() const
{
	return m_caption;
}

// パス
inline const string ShortcutItem::getPath() const
{
	return m_path;
}

// 作業フォルダのパス
inline const string ShortcutItem::getDirPath() const
{
	return m_dirPath;
}

// コメント
inline const string ShortcutItem::getComment() const
{
	return m_comment;
}

// コマンドライン引数
inline const string ShortcutItem::getParameter() const
{
	return m_parameter;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// キャプション
inline void ShortcutItem::setCaption(const string& arg)
{
	string s(arg);
	m_caption.assign(s);
}

// パス
inline void ShortcutItem::setPath(const string& arg)
{
	string s(arg);
	m_path.assign(s);
}

// 作業フォルダのパス
inline void ShortcutItem::setDirPath(const string& arg)
{
	string s(arg);
	m_dirPath.assign(s);
}

// コメント
inline void ShortcutItem::setComment(const string& arg)
{
	string s(arg);
	m_comment.assign(s);
}

// コマンドライン引数
inline void ShortcutItem::setParameter(const string& arg)
{
	string s(arg);
	m_parameter.assign(s);
}
