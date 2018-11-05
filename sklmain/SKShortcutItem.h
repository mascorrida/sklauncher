/**
 *
 * SKShortcutItem.h SK�����`���p�V���[�g�J�b�g�N���X
 *
 */

//#include "ShortcutItem.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �萔
//

// �~����
#define PI				(3.141593)
// �~����
#define ICON_NOTFOUND	"notfound.ico"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���g�^�C�v�錾
//

string subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd);

// skutil.h�̊֐��� ////////////////////////////////////////

// �J�����g�f�B���N�g���̎擾
string GetAppDir();


/**
 * �V���[�g�J�b�g�A�C�e���N���X SKShortcutItem
 */
class SKShortcutItem: public ShortcutItem
{
private:
	// ���������o /////////////////////////

	// �L���v�V����
	// �p�X
	// ��ƃt�H���_�̃p�X
	// �R�����g
	// �R�}���h���C������

	INT m_showCmd;			// �\�����@
	string m_iconPath;		// �A�C�R���̃p�X
	UINT m_iconIndex;		// �A�C�R���̈ʒu

	float m_r;				// ���a
	float m_theta;			// ���ʊp(0�`2PI)

	int m_keyChar;			// �W�����v�L�[
	char m_keyShift;		// �W�����v�L�[�V�t�g

	HICON m_hIcon;			// �A�C�R���̃n���h��

	// �������\�b�h /////////////////////////

	// �p�x�𒲐�
	const float trim0to2PI(float theta) const;
	const float trimMPItoPI(float theta) const;
	// getFromXML�̃T�u���[�`��
	const string subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const;
	// �A�C�R����ݒ�
	const bool setIcon();
	const HICON getIconFromFile(const string& path, UINT index, bool bIconFile) const;

public:
	// Getter /////////////////////////

	const INT getShowCmd() const;		// �\�����@
	const string& getIconPath() const;	// �A�C�R���̃p�X
	const UINT getIconIndex() const;	// �A�C�R���̈ʒu
	const HICON getHIcon() const;		// �A�C�R���̃n���h��
	const int getKeyChar() const;		// �W�����v�L�[
	const char getKeyShift() const;		// �W�����v�L�[�V�t�g
	const float getR() const;			// ���a
	const float getTheta() const;		// ���ʊp

	// Setter /////////////////////////

	void setShowCmd(const INT arg);			// �\�����@
	void setPath(const string& arg);		// �p�X
	void setIconPath(const string& arg);	// �A�C�R���̃p�X
	void setIconIndex(const UINT arg);		// �A�C�R���̈ʒu
	void setKeyChar(const int arg);			// �W�����v�L�[
	void setKeyShift(const char arg);		// �W�����v�L�[�V�t�g


	// ���J���\�b�h /////////////////////////

	// �R���X�g���N�^
	SKShortcutItem();
	// �f�X�g���N�^
	virtual ~SKShortcutItem();

    // �R�s�[�R���X�g���N�^
    SKShortcutItem(const SKShortcutItem& other);
    // ������Z�q
    SKShortcutItem& operator=(const SKShortcutItem& other);

	// �ʒu���擾
	const int getX() const;
	const int getY() const;

	// �ʒu��ݒ�
	const void setR(float r);
	const void addR(float r);
	const void setTheta(float theta);
	const void addTheta(float theta);
	const void setLoacation(float r, float theta);
	const void addLoacation(float r, float theta);

	// �t�@�C�����J��
	const int fileOpen() const;
	const int folderOpen() const;
	
	// �t�@�C�������݂��邩���ׂ�
	const bool fileExist() const;
	
	// �A�C�R���t�@�C�������݂��邩���ׂ�
	const bool iconExist() const;

	// XML����ǂݍ���
	const bool getFromXML(const string& data);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �R���X�g���N�^
//
inline SKShortcutItem::SKShortcutItem()
{
	// string�^�͑S�ċ󕶎���ŏ����������

	m_showCmd = SW_SHOWNORMAL;	// �\�����@
	m_iconIndex = 0;			// �A�C�R���̈ʒu

	m_r = 0;					// ���a
	m_theta = 0;				// ���ʊp(0�`2PI)

	m_keyChar = 0;				// �W�����v�L�[
	m_keyShift = 0;				// �W�����v�L�[�V�t�g

	m_hIcon = NULL;				// �A�C�R���̃n���h��
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �f�X�g���N�^
//
inline SKShortcutItem::~SKShortcutItem()
{
	// �A�C�R���̍폜�@
	if(iconExist()) DestroyIcon(m_hIcon);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �R�s�[�R���X�g���N�^
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
// ������Z�q
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

// �\�����@
inline const INT SKShortcutItem::getShowCmd() const
{
	return m_showCmd;
}

// �A�C�R���̃p�X
inline const string& SKShortcutItem::getIconPath() const
{
	return m_iconPath;
	m_r;
}

// �A�C�R���̈ʒu
inline const UINT SKShortcutItem::getIconIndex() const
{
	return m_iconIndex;
}

// �A�C�R���̃n���h��
inline const HICON SKShortcutItem::getHIcon() const
{
	return m_hIcon;
}

// �W�����v�L�[
inline const int SKShortcutItem::getKeyChar() const
{
	return m_keyChar;
}

// �W�����v�L�[�V�t�g
inline const char SKShortcutItem::getKeyShift() const
{
	return m_keyShift;
}

// ���a
inline const float SKShortcutItem::getR() const
{
	return m_r;
}

// ���ʊp
inline const float SKShortcutItem::getTheta() const
{
	return m_theta;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �\�����@
inline void SKShortcutItem::setShowCmd(const INT arg)
{
	m_showCmd = arg;
}

// �p�X
inline void SKShortcutItem::setPath(const string& arg)
{
	string s(arg);
	m_path.assign(s);
	setIcon();
}

// �A�C�R���̃p�X
inline void SKShortcutItem::setIconPath(const string& arg)
{
	string s(arg);
	m_iconPath.assign(s);
	setIcon();
}

// �A�C�R���̈ʒu
inline void SKShortcutItem::setIconIndex(const UINT arg)
{
	m_iconIndex = arg;
	setIcon();
}

// �W�����v�L�[
inline void SKShortcutItem::setKeyChar(const int arg)
{
	m_keyChar = arg;
}

// �W�����v�L�[�V�t�g
inline void SKShortcutItem::setKeyShift(const char arg)
{
	m_keyShift = arg;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �ʒu���擾
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
// �ʒu��ݒ�
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
// 0����2PI�͈̔͂ɂ���
//
inline const float SKShortcutItem::trim0to2PI(float theta) const
{
	float ret = theta;

	// �͈͊O�̊ԃ��[�v
	while(ret < 0 || ret >= 2 * PI){
		if(ret < 0)	ret += (float)(2 * PI);
		else		ret -= (float)(2 * PI);
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// -PI����PI�͈̔͂ɂ���
//
inline const float SKShortcutItem::trimMPItoPI(float theta) const
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
// �t�@�C�����J��(����:0, �G���[:0�ȊO�̃G���[�萔)
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
// �t�@�C�����J��(����:0, �G���[:0�ȊO�̃G���[�萔)
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
// �t�@�C�������݂��邩���ׂ�
//
inline const bool SKShortcutItem::fileExist() const
{
	return( GetFileAttributes(m_path.c_str()) != (DWORD)-1 );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�R���t�@�C�������݂��邩���ׂ�
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
// XML����ǂݍ���
//
inline const bool SKShortcutItem::getFromXML(const string& data)
{
	UINT posStt, posEnd;
	string buff;

	// posStt, posEnd�̐ݒ�
	posStt = data.find("<ShortcutItem>", 0);
	if(posStt == string::npos) return false;	// �Ȃ��ꍇ���s
	buff = "</ShortcutItem>";
	posEnd = data.find(buff, 0);
	if(posEnd == string::npos) return false;	// �Ȃ��ꍇ���s
	posEnd += buff.length();

	// �L���v�V����
	buff = subStrXML("Caption", data, posStt, posEnd);
	if(buff != "")	m_caption = buff;
	else			return false;		// �K�{����
	// �p�X
	buff = subStrXML("Path", data, posStt, posEnd);
	if(buff != "")	m_path = buff;
	else			return false;		// �K�{����
	// �R�����g
	buff = subStrXML("Comment", data, posStt, posEnd);
	if(buff != "")	m_comment = buff;
	else			m_comment = "";
	// ��ƃp�X
	buff = subStrXML("DirPath", data, posStt, posEnd);
	if(buff != "")	m_dirPath = buff;
	else			m_dirPath = "";
	// �R�}���h���C��
	buff = subStrXML("Param", data, posStt, posEnd);
	if(buff != "")	m_parameter = buff;
	else			m_parameter = "";
	// �\�����@
	buff = subStrXML("ShowCmd", data, posStt, posEnd);
	if(buff != "")	m_showCmd = atoi(buff.c_str());
	else			m_showCmd = 0;
	// �A�C�R���̃p�X
	buff = subStrXML("IconPath", data, posStt, posEnd);
	if(buff != "")	m_iconPath = buff;
	else			m_iconPath = "";
	// �A�C�R���̈ʒu
	buff = subStrXML("IconIndex", data, posStt, posEnd);
	if(buff != "")	m_iconIndex = atoi(buff.c_str());
	else			m_iconIndex = 0;
	// �W�����v�L�[
	buff = subStrXML("KeyChar", data, posStt, posEnd);
	if(buff != "")	m_keyChar = atoi(buff.c_str());
	else			m_keyChar = 0;
	// �W�����v�L�[�V�t�g
	buff = subStrXML("KeyShift", data, posStt, posEnd);
	if(buff != "")	m_keyShift = atoi(buff.c_str());
	else			m_keyShift = 0;

	return true;	// ����
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// getFromXML�̃T�u���[�`��
//
inline const string SKShortcutItem::subStrXML(const string& word, const string& data, UINT posStt, UINT posEnd) const
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
// �A�C�R����ݒ�
//
inline const bool SKShortcutItem::setIcon()
{
	if( iconExist() ){
		// �A�C�R���t�@�C��������Ȃ�
		DestroyIcon(m_hIcon);		// ���̃A�C�R���폜
		if(m_iconPath.compare("") == 0){
			// �A�C�R���t�@�C�����w�肳��Ă��Ȃ��Ȃ�
			m_hIcon = getIconFromFile(m_path.c_str(), NULL, false);
			
		}else{
			// �A�C�R���t�@�C�����w�肳��Ă���Ȃ�
			m_hIcon = getIconFromFile(m_iconPath.c_str(), m_iconIndex, true);
		}
		return true;

	}else{
		// �A�C�R���t�@�C�����Ȃ��Ȃ�NOT FOUND�A�C�R��
		string s;
		s = GetAppDir() + "\\" + ICON_NOTFOUND;
        m_hIcon = getIconFromFile(s, 0, true);
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �t�@�C������A�C�R�����擾
//
inline const HICON SKShortcutItem::getIconFromFile(const string& path, UINT index, bool bIconFile) const
{
	if(bIconFile){
		// �A�C�R���t�@�C��(exe,ico,dll,icl)�Ȃ�
		HICON hIcon;
		ExtractIconEx(path.c_str(), (int)index, &hIcon, NULL, 1);
		return hIcon;

	}else{
		// �A�C�R���t�@�C���łȂ��Ȃ�
		SHFILEINFOA fi;
		SHGetFileInfo(path.c_str(), FILE_ATTRIBUTE_ARCHIVE, &fi, sizeof(fi), SHGFI_ICON | SHGFI_LARGEICON);
		return fi.hIcon;
	}
}
