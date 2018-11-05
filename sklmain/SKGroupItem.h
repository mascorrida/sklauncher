/**
 *
 * SKGroupItem.h SK�����`���p�O���[�v�N���X
 *
 */

#include "GroupItem.h"
#include "SKShortcutItem.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �萔
//

#define PI				(3.141593)	// �~����
#define ICON_OFFSET		(16)		// �A�C�R���̕`��ʒu�I�t�Z�b�g
#define LEVEL_SPACE_NUM	(2)			// XML�ɏo�͂���ۂ̃^�u�̃X�y�[�X��


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���g�^�C�v�錾
//

// XML�p�̃^�O���擾
string convertTag(string key, string value, int level);


/**
 * �O���[�v�N���X SKGroupItem
 */
class SKGroupItem: public GroupItem {
private:
	// ���������o /////////////////////////

	// �L���v�V����
	// �R�����g
	// �A�C�e��
	
	vector<SKShortcutItem> m_item;	// �A�C�e��
	UINT m_index;					// �I�����Ă���A�C�e���C���f�b�N�X
	int m_keyChar;					// �W�����v�L�[
	char m_keyShift;				// �W�����v�L�[�V�t�g

	// �������\�b�h /////////////////////////
	
	// getFromXML�̃T�u���[�`��
	const string subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const;

public:
	// Getter /////////////////////////

	const UINT getItemIndex() const;					// �I�����Ă���A�C�e���C���f�b�N�X
	const int getKeyChar() const;						// �W�����v�L�[
	const char getKeyShift() const;						// �W�����v�L�[�V�t�g
	const vector<SKShortcutItem>& getItem() const;		// �A�C�e��

	// Setter /////////////////////////

	void setItemIndex(const UINT arg);	// �I�����Ă���A�C�e���C���f�b�N�X
	void setKeyChar(const int arg);		// �W�����v�L�[
	void setKeyShift(const char arg);	// �W�����v�L�[�V�t�g

	
	// ���J���\�b�h /////////////////////////

	// �R���X�g���N�^
	SKGroupItem();
	// �f�X�g���N�^
	~SKGroupItem();

	// �R�s�[�R���X�g���N�^
	SKGroupItem(const SKGroupItem& other);
	// ������Z�q
	SKGroupItem& operator=(const SKGroupItem& other);
	
	// �A�C�e���擾
	const SKShortcutItem& item(const int i) const;
	
	// �I�����Ă���A�C�e���擾
	const SKShortcutItem& selectedItem() const;
	
	// �A�C�e���̐�
	const int size() const;
	
	// �A�C�e����ǉ�
	const bool add(const SKShortcutItem& item);
	const bool add(const SKShortcutItem& item, int index);
	
	// �A�C�e�����폜
	const bool remove(int index);

	// �S�A�C�e���폜
	const bool clear();

	// XML����擾
	const bool getFromXML(const string& data);

	// XML�ɕϊ�
	const string convXML(int level);

	// �A�C�e���̈ʒu�𐮓�
	const void orderIcon(const bool autoR, int minR, int maxR, int iconInterval);

	// �A�C�e���̈ʒu�𓮂���
	const void moveIcon(const float r, const float theta);
	// �A�C�e���̈ʒu�𓮂������`��
	const void moveDrawIcon(HWND hWnd, HRGN invalidRgn, LPPOINT center, bool bNewArea, float r, float theta);

	// �S�A�C�e���̃A�C�R���`��
	const void drawIcon(const HDC hDC, const LPPOINT center) const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �R���X�g���N�^
//
inline SKGroupItem::SKGroupItem()
{
	// string�^�͑S�ċ󕶎���, vector�͋�z��ŏ����������

	m_index = 0;					// �I�����Ă���A�C�e���C���f�b�N�X
	m_keyChar = 0;					// �W�����v�L�[
	m_keyShift = 0;					// �W�����v�L�[�V�t�g
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �f�X�g���N�^
//
inline SKGroupItem::~SKGroupItem()
{
	m_item.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �R�s�[�R���X�g���N�^
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

	// �A�C�e��
	vector<SKShortcutItem> item(other.getItem());
	m_item.assign(item.begin(), item.end());
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ������Z�q
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

	// �A�C�e��
	vector<SKShortcutItem> item(other.getItem());
	m_item.assign(item.begin(), item.end());
	return(*this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �I�����Ă���A�C�e���C���f�b�N�X
inline const UINT SKGroupItem::getItemIndex() const
{
	return m_index;
}

// �W�����v�L�[
inline const int SKGroupItem::getKeyChar() const
{
	return m_keyChar;
}

// �W�����v�L�[�V�t�g
inline const char SKGroupItem::getKeyShift() const
{
	return m_keyShift;
}
// �A�C�e��
inline const vector<SKShortcutItem>& SKGroupItem::getItem() const
{
	return m_item;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �I�����Ă���A�C�e���C���f�b�N�X
inline void SKGroupItem::setItemIndex(const UINT arg)
{
	// �͈͓����ǂ�������
	if(!m_item.empty() && arg >= 0 && arg < m_item.size())
		m_index = arg;
}

// �W�����v�L�[
inline void SKGroupItem::setKeyChar(const int arg)
{
	m_keyChar = arg;
}

// �W�����v�L�[�V�t�g
inline void SKGroupItem::setKeyShift(const char arg)
{
	m_keyShift = arg;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���擾
//
inline const SKShortcutItem& SKGroupItem::item(const int i) const
{
	return m_item.at(i);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �I�����Ă���A�C�e���擾
//
inline const SKShortcutItem& SKGroupItem::selectedItem() const
{
	return m_item.at(m_index);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���̐�
//
inline const int SKGroupItem::size() const
{
	return m_item.size();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e����ǉ�
//
inline const bool SKGroupItem::add(const SKShortcutItem& item)
{
	m_item.push_back(item);
	return true;
}

inline const bool SKGroupItem::add(const SKShortcutItem& item, int index)
{
	// index�𔻒�
	if(index > 0 && index < m_item.size()){
		// index�̈ʒu�ɑ}��
		m_item.insert(m_item.begin() + index + 1, item);
		return true;

	}else if(index == m_item.size()){
		// �����ɑ}��
		m_item.push_back(item);
		return true;

	}else{
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e�����폜
//
inline const bool SKGroupItem::remove(int index)
{
	// index�𔻒�
	if(index > 0 && index < m_item.size()-1){
		// index�̈ʒu�̗v�f���폜
		m_item.erase(m_item.begin() + index);
		return true;

	}else if(index == m_item.size()-1){
		// �Ō�̗v�f���폜
		m_item.pop_back();
		return true;

	}else{
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �S�A�C�e���폜
//
inline const bool SKGroupItem::clear()
{
	m_item.clear();
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���̈ʒu�𓮂���
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
// �A�C�e���̈ʒu�𓮂������`��
//
inline const void SKGroupItem::moveDrawIcon(HWND hWnd, HRGN invalidRgn, LPPOINT center, bool bNewArea, float r, float theta)
{
	int size = m_item.size();
	int x, y, cx, cy, ix, iy;
	HRGN addRgn;

	cx = (int)center->x - ICON_OFFSET;
	cy = (int)center->y - ICON_OFFSET;

	for(int i=0; i<size; i++){
		// �A�C�e���̒��S���W
		x = center->x + m_item.at(i).getX();
		y = center->y + m_item.at(i).getY();
		// �ǉ�����͈�
		addRgn = CreateRectRgn(x - 16, y - 16, x + 16, y + 16);
		// �ĕ`��͈͂Ƀv���X
		CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
		// �s�v�ȃ��[�W�����폜
		DeleteObject(addRgn);

		m_item.at(i).addLoacation(r, theta);	// �ړ�

		ix = m_item.at(i).getX();				// �ړ���
		iy = m_item.at(i).getY();

		if(bNewArea){
			// �ړ���̍��W
			x = center->x + ix;
			y = center->y + iy;
			// �ǉ�����͈�
			addRgn = CreateRectRgn(x - 16, y - 16, x + 16, y + 16);
			// �ĕ`��͈͂Ƀv���X
			CombineRgn(invalidRgn, invalidRgn, addRgn, RGN_OR);
			// �s�v�ȃ��[�W�����폜
			DeleteObject(addRgn);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���̈ʒu�𐮓�
//
inline const void SKGroupItem::orderIcon(const bool autoR, int minR, int maxR, int iconInterval)
{
	int size = m_item.size();
	float r;

	// (r, ���ۂ̒l)=(0, 50)�`(100, 550)
	minR = minR*5+50;
	maxR = maxR*5+50;

	if(autoR){
		// (iconInterval, ���ۂ̒l)=(0, 10)�`(100, 310)
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
// XML����擾
//
inline const bool SKGroupItem::getFromXML(const string& data)
{
	UINT posStt, posEnd;
	string buff;

	// posStt, posEnd�̐ݒ�
	posStt = data.find("<GroupItem>", 0);
	if(posStt == string::npos) return false;	// �Ȃ��ꍇ���s
	buff = "</GroupItem>";
	posEnd = data.find(buff, 0);
	if(posEnd == string::npos) return false;	// �Ȃ��ꍇ���s
	posEnd += buff.length();

	// �L���v�V����
	buff = subStrXML("Caption", data, posStt, posEnd);
	if(buff != "")	m_caption = buff;
	else			return false;		// �K�{����
	// �R�����g
	buff = subStrXML("Comment", data, posStt, posEnd);
	if(buff != "")	m_comment = buff;
	else			m_comment = "";
	// �W�����v�L�[
	buff = subStrXML("KeyChar", data, posStt, posEnd);
	if(buff != "")	m_keyChar = atoi(buff.c_str());
	else			m_keyChar = 0;
	// �W�����v�L�[�V�t�g
	buff = subStrXML("KeyShift", data, posStt, posEnd);
	if(buff != "")	m_keyShift = atoi(buff.c_str());
	else			m_keyShift = 0;

	// �V���[�g�J�b�g�A�C�e���̎擾
	int pos1=posStt, pos2=posStt;

	while(1){
		// pos1, pos2�̐ݒ�
		pos1 = data.find("<ShortcutItem>", pos2);
		if(pos1 == string::npos) break;			// �Ȃ��ꍇ�I��
		buff = "</ShortcutItem>";
		pos2 = data.find(buff, pos1);
		if(pos2 == string::npos) return false;	// �Ȃ��ꍇ���s
		pos2 += buff.length();

		SKShortcutItem item;
		if( !item.getFromXML(data.substr(pos1, pos2-pos1)) ){
			m_item.clear();
			return false;
		}

		m_item.push_back(item);
	}

	// �A�C�e��������Ȃ琬��
	if(!m_item.empty())	return true;
	else				return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// getFromXML�̃T�u���[�`��
//
inline const string SKGroupItem::subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const
{
	UINT pos1, pos2;

	// {<word} �� {>} �̌���
	pos1 = data.find("<" + word, posStt);
	if(pos1 == string::npos) return "";	// �Ȃ��Ȃ玸�s
	pos2 = data.find(">", pos1);
	if(pos2 == string::npos) return "";	// �Ȃ��Ȃ玸�s

	// {<word>�`</word>} ���𔻒f
	if( data.substr(pos2-1, 1).compare("/") != 0 && pos2 < posEnd){
		pos1 = pos2 + 1;
		pos2 = data.find("</" + word + ">", pos1);
		
		if(pos1 == pos2) return "";	// �󕶎���Ȃ玸�s

		return data.substr(pos1, pos2 - pos1);

	}else return "";
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// XML�ɕϊ�
//
inline const string SKGroupItem::convXML(int level)
{
	string ret;
	int level1 = level+1, level2 = level+2;

	// <GroupItem>
	ret.append(string(level*LEVEL_SPACE_NUM,' ') + "<GroupItem>\n");

	// �O���[�v�̃v���p�e�B
	ret.append(convertTag("Caption",	m_caption, level1));
	ret.append(convertTag("Comment",	m_comment, level1));
	ret.append(convertTag("KeyChar",	itoa(m_keyChar, NULL, 10), level1));
	ret.append(convertTag("KeyShift",	itoa(m_keyShift, NULL, 10), level1));

	// �A�C�e��
	int size=m_item.size();
	for(int i=0; i<size; i++){
		SKShortcutItem& item = m_item.at(i);

		// <ShortcutItem>
		ret.append(string(level1*LEVEL_SPACE_NUM,' ') + "<ShortcutItem>\n");

		// �A�C�e���̃v���p�e�B
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
// XML�p�̃^�O���擾
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
// �S�A�C�e���̃A�C�R���`��
//
inline const void SKGroupItem::drawIcon(const HDC hDC, const LPPOINT center) const
{
	int x, y, cx, cy;
	int size = m_item.size();

	cx = (int)center->x - ICON_OFFSET;
	cy = (int)center->y - ICON_OFFSET;

	for(int i=0; i<size; i++){
		// ���W��ݒ�
		x = cx + m_item.at(i).getX();
		y = cy + m_item.at(i).getY();

		// �`��
		DrawIcon( hDC, x, y, m_item.at(i).getHIcon() );
	}
}

