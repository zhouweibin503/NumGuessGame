// NumGuessGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "NumGuessGame.h"


// 此代码模块中包含的函数的前向声明: 
BOOL CALLBACK	MainDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	TopDlgProc(HWND, UINT, WPARAM, LPARAM);

//定义相关全局变量
HINSTANCE hInst;
TCHAR szNum[3], szNum1[3], szNum2[3], szNum3[3], szNum4[3];
HANDLE hFile;
TCHAR szName[20];
HWND hListBox;
HWND hCheckBoxNum1, hCheckBoxNum2, hCheckBoxNum3, hCheckBoxNum4,
hCheckBoxPos1, hCheckBoxPos2, hCheckBoxPos3, hCheckBoxPos4;
int iNum1, iNum2, iNum3, iNum4;
int iCount = 1;
int iFlag = 0, iFlag1 = 0;
FILE * fp;
int i = 0;
int n = 0;
int iMax;
WORD a = 0xfeff;
int count[10] = { 0 };
double value[10] = { 0 };
char szBuffer[10][10] = { 0 };

//定义用于储存玩家信息的结构体
typedef struct LPlayer{
	TCHAR Name[20];
	TCHAR Times[10];
	struct LPlayer *next;
}LPlayer, *LinkPlayer;

//主函数入口
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	
	hInst = hInstance;//初始化实例句柄

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);//弹出第一个对话框

	return 0;
}


//窗口消息处理函数，其中除排行榜对话框窗口外，其他窗口相应消息均由此函数负责
BOOL CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//产生随机数，iCount控制游戏过程中不改变随机数
	if (iCount == 1){
		do
		{
			iNum1 = rand() % 10;
			iNum2 = rand() % 10;
			iNum3 = rand() % 10;
			iNum4 = rand() % 10;
		} while (iNum1 == iNum2 || iNum1 == iNum3 || iNum1 == iNum4 ||
			iNum2 == iNum3 || iNum2 == iNum4 || iNum3 == iNum4);
	}


	//接收并相应消息队列中的窗口消息
	switch (message)
	{
	case WM_INITDIALOG://窗口初始化要执行的操作
		SetDlgItemText(hWnd, IDC_EDIT_COUNT, TEXT("0"));
		SendMessage(hCheckBoxNum1, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxPos1, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxNum2, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxPos2, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxNum3, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxPos3, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxNum4, BM_SETCHECK, 0, 0);
		SendMessage(hCheckBoxPos4, BM_SETCHECK, 0, 0);
		SetDlgItemText(hWnd, IDC_EDIT_GUESSED1, TEXT(""));
		SetDlgItemText(hWnd, IDC_EDIT_GUESSED2, TEXT(""));
		SetDlgItemText(hWnd, IDC_EDIT_GUESSED3, TEXT(""));
		SetDlgItemText(hWnd, IDC_EDIT_GUESSED4, TEXT(""));

		break;


	case WM_COMMAND:
		
		switch (LOWORD(wParam))//wParam中储存了对话框控件的信息
		{
		
		case IDC_BUTTON_START:
			EndDialog(hWnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_NAME), NULL, MainDlgProc);
			break;

		case IDC_BUTTON_RULE:
			EndDialog(hWnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_RULE), NULL, MainDlgProc);
			break;

		case IDC_BUTTON_RULE1:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_RULE1), hWnd, MainDlgProc);
			break;

		case IDC_BUTTON_TOP:
			EndDialog(hWnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_TOP), hWnd, TopDlgProc);
			break;

		case IDC_BUTTON_TOP1:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_TOP1), 0, TopDlgProc);
			break;

		case IDC_BUTTON_EXIT:
			EndDialog(hWnd, 0);
			break;

		case IDC_BUTTON_CANCEL:
			EndDialog(hWnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
			break;

		case IDC_BUTTON_RETRY:
			EndDialog(hWnd, 0);
			break;

		case IDOK_NAME:
			iFlag = 0;

			//防止玩家输入空姓名
			if (GetDlgItemText(hWnd, IDC_EDIT_NAME, szName, sizeof(szName)) == NULL) {
				MessageBox(hWnd, TEXT("请输入您的名字"), TEXT("Warning"), MB_OK | MB_ICONEXCLAMATION); 
			}

			//将玩家的姓名储存在Name.txt文件中
			else
			{
				hFile = CreateFile(TEXT("Name.txt"), GENERIC_ALL, FILE_SHARE_WRITE,
					NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				SetFilePointer(hFile, NULL, NULL, FILE_END);
				
				WriteFile(hFile, &a, 2, NULL, NULL);//向文件写入a = 0xfeff，将新建的ASCII码的文件转换为Unicode码的文件
				wsprintf(szName, TEXT("%s\r\n"), szName);//在字符串末尾写入换行符
				WriteFile(hFile, szName, lstrlen(szName) * 2, NULL, NULL);
				/*WriteFile(hFile, TEXT("\r\n"), lstrlen(TEXT("\r\n"))*2, NULL, NULL);*/   //在字符串末尾写入换行符

				CloseHandle(hFile);
				EndDialog(hWnd, 0);
				DialogBox(hInst, MAKEINTRESOURCE(IDD_GAME), NULL, MainDlgProc);
				//在游戏区播放背景音乐
				//PlaySound(TEXT("xxx.wav"), NULL, SND_FILENAME | SND_ASYNC);
				
			}
			break;
		
		case IDOK_RULE:
			EndDialog(hWnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
			break;

		case IDOK_RULE1:
			EndDialog(hWnd, 0);
			break;

		case IDC_BUTTON_MENU_BACK:
			EndDialog(hWnd, 0);
			iFlag = 1;
			break;

		//确保编辑框不为空后输出玩家选择的数字
		case IDC_BUTTON_0:

			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("0"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("0"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("0"));	
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("0"));
			}
			break; 

		case IDC_BUTTON_1:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("1"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("1"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("1"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("1"));
			}
			break;

		case IDC_BUTTON_2:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("2"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("2"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("2"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("2"));
			}
			break;

		case IDC_BUTTON_3:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("3"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("3"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("3"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("3"));
			}
			break;

		case IDC_BUTTON_4:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("4"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("4"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("4"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("4"));
			}
			break;

		case IDC_BUTTON_5:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("5"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("5"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("5"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("5"));
			}
			break;

		case IDC_BUTTON_6:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("6"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("6"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("6"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("6"));
			}
			break;

		case IDC_BUTTON_7:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("7"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("7"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("7"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("7"));
			}
			break;

		case IDC_BUTTON_8:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("8"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("8"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("8"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("8"));
			}
			break; 

		case IDC_BUTTON_9:
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT("9"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT("9"));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT("9"));
			}
			else if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) == NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT("9"));
			}
			break;

		//实现撤销输入的功能
		case IDC_BUTTON_BACK:
			if (GetDlgItemText(hWnd, IDC_EDIT4, szNum, sizeof(szNum)) != NULL){
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT(""));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT3, szNum, sizeof(szNum)) != NULL){
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT(""));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT2, szNum, sizeof(szNum)) != NULL){
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT(""));
			}

			else if (GetDlgItemText(hWnd, IDC_EDIT1, szNum, sizeof(szNum)) != NULL){
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT(""));
			}
			break;
		
		//提交所猜的四个数字后执行相关操作
		case IDC_BUTTON_SUBMIT:

			//获取check控件句柄
			hCheckBoxNum1 = GetDlgItem(hWnd, IDC_CHECK_NUM1);
			hCheckBoxNum2 = GetDlgItem(hWnd, IDC_CHECK_NUM2);
			hCheckBoxNum3 = GetDlgItem(hWnd, IDC_CHECK_NUM3);
			hCheckBoxNum4 = GetDlgItem(hWnd, IDC_CHECK_NUM4);
			hCheckBoxPos1 = GetDlgItem(hWnd, IDC_CHECK_POS1);
			hCheckBoxPos2 = GetDlgItem(hWnd, IDC_CHECK_POS2);
			hCheckBoxPos3 = GetDlgItem(hWnd, IDC_CHECK_POS3);
			hCheckBoxPos4 = GetDlgItem(hWnd, IDC_CHECK_POS4);

			//确保玩家输入四个数
			if (GetDlgItemText(hWnd, IDC_EDIT1, szNum1, sizeof(szNum1)) == NULL || 
				GetDlgItemText(hWnd, IDC_EDIT2, szNum2, sizeof(szNum2)) == NULL || 
				GetDlgItemText(hWnd, IDC_EDIT3, szNum3, sizeof(szNum3)) == NULL || 
				GetDlgItemText(hWnd, IDC_EDIT4, szNum4, sizeof(szNum4)) == NULL)
			{
				MessageBox(hWnd, TEXT("请输入四个数字！"), TEXT("Warning"), MB_OK | MB_ICONEXCLAMATION);
				SetDlgItemText(hWnd, IDC_EDIT1, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT2, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT3, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT4, TEXT(""));
			}

			else
			{
				//确保玩家输入四个互不相同的数（游戏规定要输入四个互不相同的数）
				if (_tcscmp(szNum1, szNum2) == 0 || _tcscmp(szNum1, szNum3) == 0 ||
					_tcscmp(szNum1, szNum4) == 0 || _tcscmp(szNum2, szNum3) == 0 ||
					_tcscmp(szNum2, szNum4) == 0 || _tcscmp(szNum3, szNum4) == 0)
				{
					MessageBox(hWnd, TEXT("请输入四个互不相同的数！"), TEXT("Warning"), MB_OK | MB_ICONEXCLAMATION);
					SetDlgItemText(hWnd, IDC_EDIT1, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT2, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT3, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT4, TEXT(""));
				}

				else{
					TCHAR itoaCount[3];
					_itot(iCount, itoaCount, 10);
					SetDlgItemText(hWnd, IDC_EDIT_COUNT, itoaCount);
					iCount++;
					SetDlgItemText(hWnd, IDC_EDIT1, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT2, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT3, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT4, TEXT(""));
					SetDlgItemText(hWnd, IDC_EDIT_GUESSED1, szNum1);
					SetDlgItemText(hWnd, IDC_EDIT_GUESSED2, szNum2);
					SetDlgItemText(hWnd, IDC_EDIT_GUESSED3, szNum3);
					SetDlgItemText(hWnd, IDC_EDIT_GUESSED4, szNum4);

					int atoiNum1, atoiNum2, atoiNum3, atoiNum4;

					hFile = CreateFile(TEXT("Num.txt"), GENERIC_ALL, FILE_SHARE_WRITE,
						NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					SetFilePointer(hFile, NULL, NULL, FILE_END);

					if (iCount == 2){
						
						WriteFile(hFile, &a, 2, NULL, NULL);
						WriteFile(hFile, TEXT("\r\n"), lstrlen(TEXT("\r\n")) * 2, NULL, NULL);
					}//玩家完成游戏或重新开始游戏时将Num.txt文件储存的随机数换行

					TCHAR itoaNum1[3], itoaNum2[3], itoaNum3[3], itoaNum4[3];
					_itot(iNum1, itoaNum1, 10);
					_itot(iNum2, itoaNum2, 10);
					_itot(iNum3, itoaNum3, 10);
					_itot(iNum4, itoaNum4, 10);
					
					WriteFile(hFile, itoaNum1, lstrlen(itoaNum1)*2, NULL, NULL);
					WriteFile(hFile, itoaNum2, lstrlen(itoaNum2)*2, NULL, NULL);
					WriteFile(hFile, itoaNum3, lstrlen(itoaNum3)*2, NULL, NULL);
					WriteFile(hFile, itoaNum4, lstrlen(itoaNum4)*2, NULL, NULL);
					WriteFile(hFile, TEXT("  "), lstrlen(TEXT("  "))*2, NULL, NULL);
					
					CloseHandle(hFile); //关闭句柄， 以便更改输入。
					atoiNum1 = _ttoi(szNum1);
					atoiNum2 = _ttoi(szNum2);
					atoiNum3 = _ttoi(szNum3);
					atoiNum4 = _ttoi(szNum4);
				
					//向check控件发送打勾的消息
					if (atoiNum1 == iNum1){
						SendMessage(hCheckBoxNum1, BM_SETCHECK, 1, 0);
						SendMessage(hCheckBoxPos1, BM_SETCHECK, 1, 0);
					}

					if (atoiNum2 == iNum2){
						SendMessage(hCheckBoxNum2, BM_SETCHECK, 1, 0);
						SendMessage(hCheckBoxPos2, BM_SETCHECK, 1, 0);
					}

					if (atoiNum3 == iNum3){
						SendMessage(hCheckBoxNum3, BM_SETCHECK, 1, 0);
						SendMessage(hCheckBoxPos3, BM_SETCHECK, 1, 0);
					}

					if (atoiNum4 == iNum4){
						SendMessage(hCheckBoxNum4, BM_SETCHECK, 1, 0);
						SendMessage(hCheckBoxPos4, BM_SETCHECK, 1, 0);
					}

					if (atoiNum1 != iNum1){
						SendMessage(hCheckBoxNum1, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos1, BM_SETCHECK, 0, 0);
					}

					if (atoiNum2 != iNum2){
						SendMessage(hCheckBoxNum2, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos2, BM_SETCHECK, 0, 0);
					}

					if (atoiNum3 != iNum3){
						SendMessage(hCheckBoxNum3, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos3, BM_SETCHECK, 0, 0);
					}

					if (atoiNum4 != iNum4){
						SendMessage(hCheckBoxNum4, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos4, BM_SETCHECK, 0, 0);
					}

					if (atoiNum1 == iNum2 || atoiNum1 == iNum3 || atoiNum1 == iNum4){
						SendMessage(hCheckBoxNum1, BM_SETCHECK, 1, 0);
					}

					if (atoiNum2 == iNum1 || atoiNum2 == iNum3 || atoiNum2 == iNum4){
						SendMessage(hCheckBoxNum2, BM_SETCHECK, 1, 0);
					}

					if (atoiNum3 == iNum1 || atoiNum3 == iNum2 || atoiNum3 == iNum4){
						SendMessage(hCheckBoxNum3, BM_SETCHECK, 1, 0);
					}

					if (atoiNum4 == iNum1 || atoiNum4 == iNum2 || atoiNum4 == iNum3){
						SendMessage(hCheckBoxNum4, BM_SETCHECK, 1, 0);
					}

					//当玩家答对时的操作
					if (atoiNum1 == iNum1 && atoiNum2 == iNum2 && atoiNum3 == iNum3 && atoiNum4 == iNum4){
						DialogBox(hInst, MAKEINTRESOURCE(IDD_WIN), hWnd, MainDlgProc);
						if (iFlag == 1){
							EndDialog(hWnd, 0);
							DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
						}

						iCount = 1;
						SetDlgItemText(hWnd, IDC_EDIT_COUNT, TEXT("0"));

						SendMessage(hCheckBoxNum1, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos1, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxNum2, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos2, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxNum3, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos3, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxNum4, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos4, BM_SETCHECK, 0, 0);

						SetDlgItemText(hWnd, IDC_EDIT_GUESSED1, TEXT(""));
						SetDlgItemText(hWnd, IDC_EDIT_GUESSED2, TEXT(""));
						SetDlgItemText(hWnd, IDC_EDIT_GUESSED3, TEXT(""));
						SetDlgItemText(hWnd, IDC_EDIT_GUESSED4, TEXT(""));

						hFile = CreateFile(TEXT("Top.txt"), GENERIC_ALL, FILE_SHARE_WRITE,
							NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

						SetFilePointer(hFile, NULL, NULL, FILE_END);
						
						WriteFile(hFile, &a, 2, NULL, NULL);
						WriteFile(hFile, &itoaCount, lstrlen(itoaCount)*2, NULL, NULL);
						WriteFile(hFile, TEXT("\r\n"), lstrlen(TEXT("\r\n"))*2, NULL, NULL);
						WriteFile(hFile, szName, lstrlen(szName) * 2, NULL, NULL);

						WriteFile(hFile, TEXT("\r\n"), lstrlen(TEXT("\r\n")), NULL, NULL);

						CloseHandle(hFile);
					}

					//当玩家答错时的操作
					else if (iCount > 10){
						
						DialogBox(hInst, MAKEINTRESOURCE(IDD_GAMEOVER), hWnd, MainDlgProc);

						if (iFlag == 1){
							EndDialog(hWnd, 0);
							DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
						}

						iCount = 1;
						SetDlgItemText(hWnd, IDC_EDIT_COUNT, TEXT("0"));

						SendMessage(hCheckBoxNum1, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos1, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxNum2, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos2, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxNum3, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos3, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxNum4, BM_SETCHECK, 0, 0);
						SendMessage(hCheckBoxPos4, BM_SETCHECK, 0, 0);

						SetDlgItemText(hWnd, IDC_EDIT_GUESSED1, TEXT(""));
						SetDlgItemText(hWnd, IDC_EDIT_GUESSED2, TEXT(""));
						SetDlgItemText(hWnd, IDC_EDIT_GUESSED3, TEXT(""));
						SetDlgItemText(hWnd, IDC_EDIT_GUESSED4, TEXT(""));
					}
				}
			}

			break;
		default:
			break;
		}

		break;

	//点击右上角X号时关闭对话框
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		break;
	default:
		return 0;
	}

	return 0;
}

//定义自己的atoi函数
int myatoi(TCHAR *b)
{
	int g;
	n = 0;
	
	while (*(b + n) != '\0'){
		if (*(b + n) >= '0' && *(b + n) <= '9'){
			g = _ttoi(b + n);
			return g;
		}
		n++;
	}

}


//排行榜对话框的窗口消息处理函数
BOOL CALLBACK	TopDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		//初始化对话框时要执行的操作
		memset(count, 0, sizeof(count));
		memset(szBuffer, 0, sizeof(szBuffer));
		iFlag1 = 0;
		LinkPlayer L, p, q, h, Max, o;
		L = (LinkPlayer)malloc(sizeof(LPlayer));
		L->next = NULL;
		for (int j = 0; j < 20; j++)
		{
			p = (LinkPlayer)malloc(sizeof(LPlayer));
			p->next = L->next;
			L->next = p;
		}

		//如果Top.txt不存在则创建该文件，并写入unicode码识别标志
		hFile = CreateFile(TEXT("Top.txt"), GENERIC_ALL, FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		WriteFile(hFile, &a, 2, NULL, NULL);
		CloseHandle(hFile);

		fp = _tfopen(TEXT("Top.txt"), TEXT("rb"));//以二进制的方式读取文件，防止乱码

		i = 0;
		p = L;
		while (fgetws((p->next)->Times, 10, fp) != NULL){
			fgetws((p->next)->Name, 20, fp);
			p = p->next;
			i++;
		}
		p->next = NULL;//将链表末尾指向空

		p = L->next;
		o = q = h = p;
		L->next = NULL;

		//将玩家成绩进行排序
		while (o){
			Max = p = h;
			iMax = myatoi(h->Times);


			while (p->next)
			{

				if (iMax < myatoi((p->next)->Times)){
					iMax = myatoi((p->next)->Times);
					q = p;
					Max = p->next;
				}
				p = p->next;
			}

			if (Max == h){
				h = h->next;

				Max->next = L->next;
				L->next = Max;

			}

			else{
				q->next = Max->next;


				Max->next = L->next;
				L->next = Max;

			}

			o = h;
		}
	
		//去除写入a = 0xfeff产生的空白节点
		if ((L->next)){
			if (!(_ttoi((L->next)->Times) > 0 && _ttoi((L->next)->Times) <= 10))
				L = L->next;
		}

		//向排行榜输出玩家姓名和成绩
		p = L->next;
		if (p){ 
			SetDlgItemText(hWnd, IDC_EDIT_TIMES1, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP1, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES2, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP2, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES3, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP3, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES4, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP4, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES5, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP5, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES6, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP6, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES7, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP7, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES8, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP8, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES9, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP9, p->Name);
			p = p->next;
		}
		
		if (p){
			
			SetDlgItemText(hWnd, IDC_EDIT_TIMES10, p->Times);
			SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP10, p->Name);
			p = p->next;
		}

		//计算平均成功率
		p = L->next;
		while (p)
		{
			for (int j = 0; j < 10; j++){
				if (myatoi(p->Times) == j + 1)
					count[j]++;
			}
			p = p->next;
		}

		for (int j = 0; j < 10; j++){
			if (i != 1){
				value[j] = count[j] * 1.0 / (i - 1);
				sprintf(szBuffer[j], "%.3lf", value[j]);
			}
		}

		//向排行榜输出1-10各自的平均成功率
		SetDlgItemTextA(hWnd, IDC_EDIT_1, szBuffer[0]);
		SetDlgItemTextA(hWnd, IDC_EDIT_2, szBuffer[1]);
		SetDlgItemTextA(hWnd, IDC_EDIT_3, szBuffer[2]);
		SetDlgItemTextA(hWnd, IDC_EDIT_4, szBuffer[3]);
		SetDlgItemTextA(hWnd, IDC_EDIT_5, szBuffer[4]);
		SetDlgItemTextA(hWnd, IDC_EDIT_6, szBuffer[5]);
		SetDlgItemTextA(hWnd, IDC_EDIT_7, szBuffer[6]);
		SetDlgItemTextA(hWnd, IDC_EDIT_8, szBuffer[7]);
		SetDlgItemTextA(hWnd, IDC_EDIT_9, szBuffer[8]);
		SetDlgItemTextA(hWnd, IDC_EDIT_10, szBuffer[9]);
		fclose(fp);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_TOP_BACK:
			EndDialog(hWnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
			break;

		case IDC_BUTTON_TOP_BACK1:
			EndDialog(hWnd, 0);
			break;

		case IDC_BUTTON_CLEAR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CLEAR), hWnd, TopDlgProc);

			if (iFlag1 == 1){
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP1, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP2, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP3, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP4, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP5, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP6, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP7, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP8, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP9, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_NAME_TOP10, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES1, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES2, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES3, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES4, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES5, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES6, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES7, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES8, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES9, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_TIMES10, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_1, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_2, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_3, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_4, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_5, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_6, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_7, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_8, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_9, TEXT(""));
				SetDlgItemText(hWnd, IDC_EDIT_10, TEXT(""));
				fp = _tfopen(TEXT("Top.txt"), TEXT("w"));
				fclose(fp);
			}
			break;

		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;

		case IDOK_CLEAR:
			iFlag1 = 1;
			EndDialog(hWnd, 0);
			break;

		default:
			break;
		}

		break;

	//点击右上角X号时关闭对话框
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		break;

	default:
		break;
	}

	return 0;
}