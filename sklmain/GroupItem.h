/**
 *
 * GroupItem.h �V���[�g�J�b�g�̃O���[�v�́i�����ہj�N���X
 *
 */

#include <string>
#include <vector>
#include "ShortcutItem.h"

using namespace std;

/**
 * �e�N���X GroupItem
 */
class GroupItem
{
protected:
	// ���������o /////////////////////////

	string m_caption;		// �L���v�V����
	string m_comment;		// �R�����g

public:
	// Getter /////////////////////////

	const string getCaption() const;	// �L���v�V����
	const string getComment() const;	// �R�����g

	// Setter /////////////////////////

	void setCaption(const string& arg);		// �L���v�V����
	void setComment(const string& arg);		// �R�����g
	
	// ���J���\�b�h /////////////////////////
	
	// �R���X�g���N�^
	GroupItem(){}
	// �f�X�g���N�^
	virtual ~GroupItem(){}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Getter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �L���v�V����
inline const string GroupItem::getCaption() const
{
	return m_caption;
}

// �R�����g
inline const string GroupItem::getComment() const
{
	return m_comment;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setter
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// �L���v�V����
inline void GroupItem::setCaption(const string& arg)
{
	string s(arg);
	m_caption.assign(s);
}

// �R�����g
inline void GroupItem::setComment(const string& arg)
{
	string s(arg);
	m_comment.assign(s);
}
