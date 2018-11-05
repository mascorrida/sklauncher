/**
 *
 * GroupItem.h ショートカットのグループの（半抽象）クラス
 *
 */

#include <string>
#include <vector>
#include "ShortcutItem.h"

using namespace std;

/**
 * 親クラス GroupItem
 */
class GroupItem
{
protected:
	// 内部メンバ /////////////////////////

	string m_caption;		// キャプション
	string m_comment;		// コメント

public:
	// Getter /////////////////////////

	const string getCaption() const;	// キャプション
	const string getComment() const;	// コメント

	// Setter /////////////////////////

	void setCaption(const string& arg);		// キャプション
	void setComment(const string& arg);		// コメント
	
	// 公開メソッド /////////////////////////
	
	// コンストラクタ
	GroupItem(){}
	// デストラクタ
	virtual ~GroupItem(){}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// キャプション
inline const string GroupItem::getCaption() const
{
	return m_caption;
}

// コメント
inline const string GroupItem::getComment() const
{
	return m_comment;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// キャプション
inline void GroupItem::setCaption(const string& arg)
{
	string s(arg);
	m_caption.assign(s);
}

// コメント
inline void GroupItem::setComment(const string& arg)
{
	string s(arg);
	m_comment.assign(s);
}
