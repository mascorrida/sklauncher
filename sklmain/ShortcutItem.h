/**
 *
 * ShortcutItem.h �V���[�g�J�b�g�́i�����ہj�N���X
 *
 */

#include <string>

using namespace std;

/**
 * �e�N���X ShortcutItem
 */
class ShortcutItem
{
protected:
	// ���������o /////////////////////////

	string m_caption;		// �L���v�V����
	string m_path;			// �p�X
	string m_dirPath;		// ��ƃt�H���_�̃p�X
	string m_comment;		// �R�����g
	string m_parameter;		// �R�}���h���C������

public:
	// Getter /////////////////////////

	const string getCaption() const;	// �L���v�V����
	const string getPath() const;		// �p�X
	const string getDirPath() const;	// ��ƃt�H���_�̃p�X
	const string getComment() const;	// �R�����g
	const string getParameter() const;	// �R�}���h���C������

	// Setter /////////////////////////

	void setCaption(const string& arg);		// �L���v�V����
	void setPath(const string& arg);		// �p�X
	void setDirPath(const string& arg);		// ��ƃt�H���_�̃p�X
	void setComment(const string& arg);		// �R�����g
	void setParameter(const string& arg);	// �R�}���h���C������
	
	// ���J���\�b�h /////////////////////////
	
	// �R���X�g���N�^
	ShortcutItem(){}
	ShortcutItem::ShortcutItem
		(const string& caption, 
		 const string& path,
		 const string& dirPath,
		 const string& comment,
		 const string& parameter);
	// �f�X�g���N�^
	virtual ~ShortcutItem(){}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �R���X�g���N�^(�����w��)
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

// �L���v�V����
inline const string ShortcutItem::getCaption() const
{
	return m_caption;
}

// �p�X
inline const string ShortcutItem::getPath() const
{
	return m_path;
}

// ��ƃt�H���_�̃p�X
inline const string ShortcutItem::getDirPath() const
{
	return m_dirPath;
}

// �R�����g
inline const string ShortcutItem::getComment() const
{
	return m_comment;
}

// �R�}���h���C������
inline const string ShortcutItem::getParameter() const
{
	return m_parameter;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �L���v�V����
inline void ShortcutItem::setCaption(const string& arg)
{
	string s(arg);
	m_caption.assign(s);
}

// �p�X
inline void ShortcutItem::setPath(const string& arg)
{
	string s(arg);
	m_path.assign(s);
}

// ��ƃt�H���_�̃p�X
inline void ShortcutItem::setDirPath(const string& arg)
{
	string s(arg);
	m_dirPath.assign(s);
}

// �R�����g
inline void ShortcutItem::setComment(const string& arg)
{
	string s(arg);
	m_comment.assign(s);
}

// �R�}���h���C������
inline void ShortcutItem::setParameter(const string& arg)
{
	string s(arg);
	m_parameter.assign(s);
}
