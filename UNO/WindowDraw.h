#pragma once
#include "GameField.h"

class WindowDraw
{
	HWND hBack, hLogo, hCards;
	RECT rWindow; //размер окна
	RECT rMenu1[4]/*главное меню*/ /*, rMenu2настройки*/, rMenu3[2]/*меню перед игрой*/;
	HWND hMenu1[4], hMenu2[3], hMenu3[2], hMenu4[2];
	HWND playVS, playCount;
	size_t playersNum = 0; //кол-во игроков
	size_t backImg = 0; //фон
	GameField gf;
	HWND hGamePause[3];
	vector<HWND> plscores;
	vector<HWND> hRecords;
public:
	vector<HWND> plnames;
	HWND ch1bot, ch2pl, spinplcount, editspinplcount, hMessages;

	HWND hDialog;
	HINSTANCE hInst;

	void WriteOptions();
	void ReadOptions();

	void SetBackground();
	void SetHeadMenu();
	void DestroyHeadMenu();
	void SetPlayMenu(BOOL vsBot);
	void RedrawPlayMenu(BOOL vsBot);
	void DestroyPlayMenu();
	void SetPauseMenu();
	void DestroyPauseMenu();
	void SetOptMenu(vector<vector<TCHAR*>> players, vector<vector<int>> scores);
	void ClearOptMenu();
	void DestroyOptMenu();
	void SetHelpMenu();
	void DestroyHelpMenu();

	void DrawGameField(vector<Player*> &pl, BOOL vsBot, short need);
	void DestroyGameField();

	void UpdateDetails(vector<Player*> &pl, BOOL dir, size_t nowmove, Player & cards, BOOL need, int cnum);
	void SetNeedColor(BOOL need);
	void ChangeCurrentColor(int nowcolor);

	void SetSpinPlCount(INT num);

	void DrawPlPause(TCHAR* name1, TCHAR* name2, TCHAR* message, size_t plcount, BOOL isItStart);
	void DestroyPlPause(BOOL isItStart);

	void DrawRoundEndPause(vector<Player*> &pl, short Games, vector<vector<int>> &roundTable, vector<TCHAR*> &plNames);
	void DestroyRoundEndPause();

	TCHAR* getPauseMessage(TCHAR* name1, TCHAR* name2, TCHAR* message);

	void MessageAboutPlayers(TCHAR* name1, TCHAR* name2, TCHAR* message);
	void DestroyMessageAboutPlayers();

	void EndGameWindow(vector<Player*> &pl);
};
