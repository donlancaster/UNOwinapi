#pragma once
#include "GameProcess.h"

class UNODlg
{
public:
	UNODlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static UNODlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
	void Cls_OnTimer(HWND hwnd, UINT id);
	void Cls_OnClose(HWND hwnd);
	void AddToRecordsTable(vector<TCHAR*> players, vector<int> scores);
	void SaveRecordsTable();
	void ReadRecordsTable();
	void ClearRecords();
	void SortPlayersInRecordsTable();
	WindowDraw wd;
	GameProcess gp;
	HWND hDialog;

	vector<vector<TCHAR*>> players;
	vector<vector<int>> scores;
};
