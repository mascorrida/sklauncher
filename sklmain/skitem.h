

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �p�u���b�N�ϐ�
//


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �v���g�^�C�v�錾
//

// �A�C�e���̕ҏW
LRESULT CALLBACK DlgProcItem(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// �A�C�e���̕ҏW
//
LRESULT CALLBACK DlgProcItem(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
		// �쐬��
        case WM_INITDIALOG:
			break;

		// �R���g���[��
        case WM_COMMAND:
			break;

		// �I����
		case WM_CLOSE:
			EndDialog(hWnd, IDOK);
			break;

        default:
            return FALSE;
    }
    return TRUE;
}