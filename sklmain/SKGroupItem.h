/**
 *
 * SKGroupItem.h SKランチャ用グループクラス
 *
 */

#include "GroupItem.h"
#include "SKShortcutItem.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 定数
//

#define PI				(3.141593)	// 円周率
#define ICON_OFFSET		(16)		// アイコンの描画位置オフセット
#define LEVEL_SPACE_NUM	(2)			// XMLに出力する際のタブのスペース数


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// プロトタイプ宣言
//

// XML用のタグを取得
string convertTag(string key, string value, int level);


/**
 * グループクラス SKGroupItem
 */
class SKGroupItem: public GroupItem {
private:
	// 内部メンバ /////////////////////////

	// キャプション
	// コメント
	// アイテム
	
	vector<SKShortcutItem> m_item;	// アイテム
	UINT m_index;					// 選択しているアイテムインデックス
	int m_keyChar;					// ジャンプキー
	char m_keyShift;				// ジャンプキーシフト

	// 内部メソッド /////////////////////////
	
	// getFromXMLのサブルーチン
	const string subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const;

public:
	// Getter /////////////////////////

	const UINT getItemIndex() const;					// 選択しているアイテムインデックス
	const int getKeyChar() const;						// ジャンプキー
	const char getKeyShift() const;						// ジャンプキーシフト
	const vector<SKShortcutItem>& getItem() const;		// アイテム

	// Setter /////////////////////////

	void setItemIndex(const UINT arg);	// 選択しているアイテムインデックス
	void setKeyChar(const int arg);		// ジャンプキー
	void setKeyShift(const char arg);	// ジャンプキーシフト

	
	// 公開メソッド /////////////////////////

	// コンストラクタ
	SKGroupItem();
	// デストラクタ
	~SKGroupItem();

	// コピーコンストラクタ
	SKGroupItem(const SKGroupItem& other);
	// 代入演算子
	SKGroupItem& operator=(const SKGroupItem& other);
	
	// アイテム取得
	const SKShortcutItem& item(const int i) const;
	
	// 選択しているアイテム取得
	const SKShortcutItem& selectedItem() const;
	
	// アイテムの数
	const int size() const;
	
	// アイテムを追加
	const bool add(const SKShortcutItem& item);
	const bool add(const SKShortcutItem& item, int index);
	
	// アイテムを削除
	const bool remove(int index);

	// 全アイテム削除
	const bool clear();

	// XMLから取得
	const bool getFromXML(const string& data);

	// XMLに変換
	const string convXML(int level);

	// アイテムの位置を整頓
	const void orderIcon(const bool autoR, int minR, int maxR, int iconInterval);

	// アイテムの位置を動かす
	const void moveIcon(const float r, const float theta);
	// アイテムの位置を動かす＆描画
	const void moveDrawIcon(HWND hWnd, HRGN invalidRgn, LPPOINT center, bool bNewArea, float r, float theta);

	// 全アイテムのアイコン描画
	const void drawIcon(const HDC hDC, const LPPOINT center) const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コンストラクタ
//
inline SKGroupItem::SKGroupItem()
{
	// string型は全て空文字列, vectorは空配列で初期化される

	m_index = 0;					// 選択しているアイテムインデックス
	m_keyChar = 0;					// ジャンプキー
	m_keyShift = 0;					// ジャンプキーシフト
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// デストラクタ
//
inline SKGroupItem::~SKGroupItem()
{
	m_item.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コピーコンストラクタ
//
inline SKGroupItem::SKGroupItem(const SKGroupItem& other)
{
	string caption(other.getCaption());
	m_caption.assign(caption);

	string comment(other.getComment());
	m_comment.assign(comment);

	m_index = 0;
	m_keyChar = other.getKeyChar();
	m_keyShift = other.getKeyShift();

	// アイテム
	vector<SKShortcutItem> item(other.getItem());
	m_item.assign(item.begin(), item.end());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 代入演算子
//
inline SKGroupItem& SKGroupItem::operator=(const SKGroupItem& other)
{
	string caption(other.getCaption());
	m_caption.assign(caption);

	string comment(other.getComment());
	m_comment.assign(comment);

	m_index = 0;
	m_keyChar = other.getKeyChar();
	m_keyShift = other.getKeyShift();

	// アイテム
	vector<SKShortcutItem> item(other.getItem());
	m_item.assign(item.begin(), item.end());
	return(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 選択しているアイテムインデックス
inline const UINT SKGroupItem::getItemIndex() const
{
	return m_index;
}

// ジャンプキー
inline const int SKGroupItem::getKeyChar() const
{
	return m_keyChar;
}

// ジャンプキーシフト
inline const char SKGroupItem::getKeyShift() const
{
	return m_keyShift;
}
// アイテム
inline const vector<SKShortcutItem>& SKGroupItem::getItem() const
{
	return m_item;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 選択しているアイテムインデックス
inline void SKGroupItem::setItemIndex(const UINT arg)
{
	// 範囲内かどうか判定
	if(!m_item.empty() && arg >= 0 && arg < m_item.size())
		m_index = arg;
}

// ジャンプキー
inline void SKGroupItem::setKeyChar(const int arg)
{
	m_keyChar = arg;
}

// ジャンプキーシフト
inline void SKGroupItem::setKeyShift(const char arg)
{
	m_keyShift = arg;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテム取得
//
inline const SKShortcutItem& SKGroupItem::item(const int i) const
{
	return m_item.at(i);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 選択しているアイテム取得
//
inline const SKShortcutItem& SKGroupItem::selectedItem() const
{
	return m_item.at(m_index);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムの数
//
inline const int SKGroupItem::size() const
{
	return m_item.size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムを追加
//
inline const bool SKGroupItem::add(const SKShortcutItem& item)
{
	m_item.push_back(item);
	return true;
}

inline const bool SKGroupItem::add(const SKShortcutItem& item, int index)
{
	// indexを判定
	if(index > 0 && index < m_item.size()){
		// indexの位置に挿入
		m_item.insert(m_item.begin() + index + 1, item);
		return true;

	}else if(index == m_item.size()){
		// 末尾に挿入
		m_item.push_back(item);
		return true;

	}else{
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムを削除
//
inline const bool SKGroupItem::remove(int index)
{
	// indexを判定
	if(index > 0 && index < m_item.size()-1){
		// indexの位置の要素を削除
		m_item.erase(m_item.begin() + index);
		return true;

	}else if(index == m_item.size()-1){
		// 最後の要素を削除
		m_item.pop_back();
		return true;

	}else{
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全アイテム削除
//
inline const bool SKGroupItem::clear()
{
	m_item.clear();
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムの位置を動かす
//
inline const void SKGroupItem::moveIcon(float r, float theta)
{
	int size = m_item.size();

	for(int i=0; i<size; i++){
		m_item.at(i).addLoacation(r, theta);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムの位置を動かす＆描画
//
inline const void SKGroupItem::moveDrawIcon(HWND hWnd, HRGN invalidRgn, LPPOINT center, bool bNewArea, float r, float theta)
{
	int size = m_item.size();
	int x, y, cx, cy, ix, iy;
	HRGN addRgn;

	cx = (int)center->x - ICON_OFFSET;
	cy = (int)center->y - ICON_OFFSET;

	for(int i=0; i<size; i++){
		// アイテムの中心座標
		x = center->x + m_item.at(i).getX();
		y = center->y + m_item.at(i).getY();
		// 追加する範囲
		addRgn = CreateRectRgn(x - 16, y - 16, x + 16, y + 16);
		// 再描画範囲にプラス
		CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
		// 不要なリージョン削除
		DeleteObject(addRgn);

		m_item.at(i).addLoacation(r, theta);	// 移動

		ix = m_item.at(i).getX();				// 移動量
		iy = m_item.at(i).getY();

		if(bNewArea){
			// 移動後の座標
			x = center->x + ix;
			y = center->y + iy;
			// 追加する範囲
			addRgn = CreateRectRgn(x - 16, y - 16, x + 16, y + 16);
			// 再描画範囲にプラス
			CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
			// 不要なリージョン削除
			DeleteObject(addRgn);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// アイテムの位置を整頓
//
inline const void SKGroupItem::orderIcon(const bool autoR, int minR, int maxR, int iconInterval)
{
	int size = m_item.size();
	float r;

	// (r, 実際の値)=(0, 50)～(100, 550)
	minR = minR*5+50;
	maxR = maxR*5+50;

	if(autoR){
		// (iconInterval, 実際の値)=(0, 10)～(100, 310)
		iconInterval = iconInterval*3+10;
		r = iconInterval / sin(PI / size);
		if(r < minR) r = minR;
		if(r > maxR) r = maxR;
	}else{
		r = minR;
	}

	for(int i=0; i<size; i++){
		float theta = 2*PI * i/size + PI*3/2;
		m_item.at(i).setLoacation(r, theta);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XMLから取得
//
inline const bool SKGroupItem::getFromXML(const string& data)
{
	UINT posStt, posEnd;
	string buff;

	// posStt, posEndの設定
	posStt = data.find("<GroupItem>", 0);
	if(posStt == string::npos) return false;	// ない場合失敗
	buff = "</GroupItem>";
	posEnd = data.find(buff, 0);
	if(posEnd == string::npos) return false;	// ない場合失敗
	posEnd += buff.length();

	// キャプション
	buff = subStrXML("Caption", data, posStt, posEnd);
	if(buff != "")	m_caption = buff;
	else			return false;		// 必須項目
	// コメント
	buff = subStrXML("Comment", data, posStt, posEnd);
	if(buff != "")	m_comment = buff;
	else			m_comment = "";
	// ジャンプキー
	buff = subStrXML("KeyChar", data, posStt, posEnd);
	if(buff != "")	m_keyChar = atoi(buff.c_str());
	else			m_keyChar = 0;
	// ジャンプキーシフト
	buff = subStrXML("KeyShift", data, posStt, posEnd);
	if(buff != "")	m_keyShift = atoi(buff.c_str());
	else			m_keyShift = 0;

	// ショートカットアイテムの取得
	int pos1=posStt, pos2=posStt;

	while(1){
		// pos1, pos2の設定
		pos1 = data.find("<ShortcutItem>", pos2);
		if(pos1 == string::npos) break;			// ない場合終了
		buff = "</ShortcutItem>";
		pos2 = data.find(buff, pos1);
		if(pos2 == string::npos) return false;	// ない場合失敗
		pos2 += buff.length();

		SKShortcutItem item;
		if( !item.getFromXML(data.substr(pos1, pos2-pos1)) ){
			m_item.clear();
			return false;
		}

		m_item.push_back(item);
	}

	// アイテムがあるなら成功
	if(!m_item.empty())	return true;
	else				return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// getFromXMLのサブルーチン
//
inline const string SKGroupItem::subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const
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
// XMLに変換
//
inline const string SKGroupItem::convXML(int level)
{
	string ret;
	int level1 = level+1, level2 = level+2;

	// <GroupItem>
	ret.append(string(level*LEVEL_SPACE_NUM,' ') + "<GroupItem>\n");

	// グループのプロパティ
	ret.append(convertTag("Caption",	m_caption, level1));
	ret.append(convertTag("Comment",	m_comment, level1));
	ret.append(convertTag("KeyChar",	itoa(m_keyChar, NULL, 10), level1));
	ret.append(convertTag("KeyShift",	itoa(m_keyShift, NULL, 10), level1));

	// アイテム
	int size=m_item.size();
	for(int i=0; i<size; i++){
		SKShortcutItem& item = m_item.at(i);

		// <ShortcutItem>
		ret.append(string(level1*LEVEL_SPACE_NUM,' ') + "<ShortcutItem>\n");

		// アイテムのプロパティ
		ret.append(convertTag("Caption",	item.getCaption(), level2));
		ret.append(convertTag("Comment",	item.getComment(), level2));
		ret.append(convertTag("Path",		item.getPath(), level2));
		ret.append(convertTag("DirPath",	item.getDirPath(), level2));
		ret.append(convertTag("Param",		item.getParameter(), level2));
		ret.append(convertTag("ShowCmd",	itoa(item.getShowCmd(), NULL, 10), level2));
		ret.append(convertTag("IconPath",	item.getIconPath(), level2));
		ret.append(convertTag("IconIndex",	itoa(item.getIconIndex(), NULL, 10), level2));
		ret.append(convertTag("KeyChar",	itoa(item.getKeyChar(), NULL, 10), level2));
		ret.append(convertTag("KeyShift",	itoa(item.getKeyShift(), NULL, 10), level2));

		// </ShortcutItem>
		ret.append(string(level1*LEVEL_SPACE_NUM,' ') + "</ShortcutItem>\n");
	}

	// </GroupItem>
	ret.append(string(level*LEVEL_SPACE_NUM,' ') + "</GroupItem>\n");

	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XML用のタグを取得
//
string convertTag(string key, string value, int level)
{
	string ret(level * LEVEL_SPACE_NUM, ' ');

	if( value.empty() )	ret.append("<" + key + " />");
	else				ret.append("<" + key + ">" + value + "</" + key + ">\n");

	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 全アイテムのアイコン描画
//
inline const void SKGroupItem::drawIcon(const HDC hDC, const LPPOINT center) const
{
	int x, y, cx, cy;
	int size = m_item.size();

	cx = (int)center->x - ICON_OFFSET;
	cy = (int)center->y - ICON_OFFSET;

	for(int i=0; i<size; i++){
		// 座標を設定
		x = cx + m_item.at(i).getX();
		y = cy + m_item.at(i).getY();

		// 描画
		DrawIcon( hDC, x, y, m_item.at(i).getHIcon() );
	}
}

