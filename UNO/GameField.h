#pragma once
#include "Header.h"
#include "Player.h"

#define ID_BACK0 WM_USER //фон1
#define ID_BACK1 WM_USER + 1 //фон2
#define ID_BUTTON WM_USER + 2
#define ID_MENU0_PLAY WM_USER + 3 //главное меню
#define ID_MENU0_OPTIONS WM_USER + 4
#define ID_MENU0_HELP WM_USER + 5
#define ID_MENU0_EXIT WM_USER + 6
#define ID_LOGO WM_USER + 7
#define ID_MENU1_BACK WM_USER + 8 //настройки
#define ID_MENU1_SAVE WM_USER + 9 
#define ID_MENU2_BACK WM_USER + 10 //правила/помощь
#define ID_MENU3_BACK WM_USER + 11 //игра
#define ID_MENU3_PLAY WM_USER + 12

#define ID_MENU3_CHECK1 WM_USER + 13 
#define ID_MENU3_CHECK2 WM_USER + 14
#define ID_MENU3_COUNT WM_USER + 15 

#define ID_MENU_PAUSE WM_USER + 16 //пауза
#define ID_MENU_EXIT WM_USER + 17 //выход из игры
#define ID_COLORS_RED WM_USER + 18 //красный
#define ID_COLORS_BLUE WM_USER + 19 //синий
#define ID_COLORS_YELLOW WM_USER + 20 //желтый
#define ID_COLORS_GREEN WM_USER + 21 //зеленый
#define ID_UNO WM_USER + 22 //уно
#define ID_DECK WM_USER + 23 //колода
#define ID_NEXT_PLAYER + 24
#define ID_NEXT_ROUND + 25
#define ID_END_GAME + 26
#define ID_CONTINUE + 27
#define ID_PAUSEEXIT + 28
#define ID_CLEAR_RECORDS + 29
#define ID_MAGIC + 30
#define ID_END_NOW_ROUND + 31

#define ID_СARDS WM_USER + 32 //карты игрока


class GameField
{
	vector<HWND> plfields;
	vector<HWND> plcards;
	vector<HWND> plсardnum;
	vector<HWND> plscore;
	vector<HWND> pluno;
	vector<HWND> plnames;
	size_t plcount = 0;
	vector<HWND> Plfield;
	HWND hExit, hPause;
	size_t backImg = 0;
	HWND arrow;

	BOOL vsBot = FALSE;
	HWND unoBtn;
	vector<HWND> bcolors;
	vector<HWND> plHcards;
	vector<HWND> deck;
	HWND plDeck;
	HWND hMagic;
	HWND hNowColor, hNowMove;
public:
	void SetEmptyField(vector<Player*> &pl, HWND hDialog, HINSTANCE hInst, size_t backimg, BOOL VsBot, BOOL need)
	{
		vsBot = VsBot;
		backImg = backimg;
		SetPlayersZone(pl, hDialog, hInst);
		SetPlayerZone(hDialog, hInst);
		SetOtherZone(pl, hDialog, hInst);
		SetNeedColor(hDialog, need);
	}
	void DestroyEmptyField()
	{
		DestroyPlayersZone();
		DestroyPlayerZone();
		DestroyOtherZone();
	}
	
	void SetDetailsField(HWND hDialog, HINSTANCE hInst, vector<Player*> &pl, BOOL dir, size_t nowmove, Player & cards, BOOL need, int cnum = 0)
	{
		DestroyDetailsField(); //очистка старых данных с экрана
		DrawCards(hDialog, hInst, *(pl[0])); //отрисовка карт текущего игрока
		DrawDirection(hDialog, hInst, dir);
		DrawDeck(hDialog, hInst, cards);
		SetNeedColor(hDialog, need);
		SetNowColor(hDialog, hInst, cnum);
		//отображение очков, карт, имен игроков
		SetPlayersScore(pl);
		if (vsBot && nowmove != 0)
			ChangeNowPlayer(hDialog, hInst, nowmove - 1);
		//отображение уно у игроков
		ChangeUnoButtoms(pl, hInst);
		//кнопка перехода по уровням (magic)

		hMagic = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 1010, 725, 40, 26, hDialog, (HMENU)(ID_END_NOW_ROUND), hInst, NULL);
		SendMessage(hMagic, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP99)));
	}
	void DestroyDetailsField()
	{
		DestroyCards();
		DestroyWindow(arrow); //direction
		arrow = NULL;
		DestroyDeck();
		DestroyWindow(hNowColor);
		if (vsBot) DestroyWindow(hNowMove);
		DestroyWindow(hMagic);
	}
	
	void SetPlayersZone(vector<Player*> &pl, HWND hDialog, HINSTANCE hInst)
	{
		plcount = pl.size() - 1;
		RECT rw;
		GetClientRect(hDialog, &rw);
		for (size_t i = 0; i < plcount; i++)
		{
			//фон игрока сверху
			HWND h1 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (((rw.right / plcount) - 335) / 2) + (rw.right / plcount)*i, 0, 335, 150, hDialog, NULL, hInst, NULL);
			SendMessage(h1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP66)));
			plfields.push_back(h1);
			RECT r;
			GetClientRect(h1, &r);
			//карта
			HWND h2 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, ((((rw.right / plcount) - 340) / 2) + (rw.right / plcount)*i) + 35, 10, 90, 130, hDialog, NULL, hInst, NULL);
			SendMessage(h2, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP67)));
			plcards.push_back(h2);

			//отображение кол-ва карт
			HWND h4 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, ((((rw.right / plcount) - 340) / 2) + (rw.right / plcount)*i) + 135, 65, 100, 23, hDialog, NULL, hInst, NULL);
			//установить текст
			plscore.push_back(h4);
			HFONT hfont = CreateFont(19, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Calibri");
			SetBkMode((HDC)hfont, TRANSPARENT);
			SetTextColor((HDC)hfont, RGB(255, 255, 255));
			SelectObject((HDC)hfont, hfont);
			SendMessage(h4, WM_SETFONT, (LPARAM)hfont, TRUE);

			//кнопка уно
			HWND h5 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, ((((rw.right / plcount) - 340) / 2) + (rw.right / plcount)*i) + 240, 40, 70, 70, hDialog, NULL, hInst, NULL);
			if (pl[i + 1]->GetUno() == -1 || pl[i + 1]->GetUno() == 1)
				SendMessage(h5, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP68)));
			else SendMessage(h5, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP91)));
			pluno.push_back(h5);

			//отображение имени
			HWND h6 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, ((((rw.right / plcount) - 340) / 2) + (rw.right / plcount)*i) + 135, 40, 100, 23, hDialog, NULL, hInst, NULL);
			//установить шрифт
			plnames.push_back(h6);
			SendMessage(h6, WM_SETFONT, (LPARAM)hfont, TRUE);
			
		}
		for (size_t i = 0; i < plcount; i++)
		{
			ShowWindow(plfields[i], SW_HIDE);
			ShowWindow(plfields[i], SW_SHOW);
			ShowWindow(plcards[i], SW_HIDE);
			ShowWindow(plcards[i], SW_SHOW);
			ShowWindow(plscore[i], SW_HIDE);
			ShowWindow(plscore[i], SW_SHOW);
			ShowWindow(pluno[i], SW_HIDE);
			ShowWindow(pluno[i], SW_SHOW);
			ShowWindow(plnames[i], SW_HIDE);
			ShowWindow(plnames[i], SW_SHOW);
		}
	}
	void DestroyPlayersZone()
	{
		for (auto it : plfields)
			DestroyWindow(it);
		for (auto it : plcards)
			DestroyWindow(it);
		for (auto it : plсardnum)
			DestroyWindow(it);
		for (auto it : plscore)
			DestroyWindow(it);
		for (auto it : pluno)
			DestroyWindow(it);
		for (auto it : plnames)
			DestroyWindow(it);
		if (plfields.size() > 0)
			plfields.erase(plfields.begin(), plfields.begin() + plfields.size());
		if (plcards.size() > 0)
			plcards.erase(plcards.begin(), plcards.begin() + plcards.size());
		if (plсardnum.size() > 0)
			plсardnum.erase(plсardnum.begin(), plсardnum.begin() + plсardnum.size());
		if (plscore.size() > 0)
			plscore.erase(plscore.begin(), plscore.begin() + plscore.size());
		if (pluno.size() > 0)
			pluno.erase(pluno.begin(), pluno.begin() + pluno.size());
		if (plnames.size() > 0)
			plnames.erase(plnames.begin(), plnames.begin() + plnames.size());
	}

	void SetPlayerZone(HWND hDialog, HINSTANCE hInst)
	{
		RECT rw;
		GetClientRect(hDialog, &rw);
		//колода карт
		HWND h1 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, rw.right - 220, rw.bottom - 270, 220, 270, hDialog, NULL, hInst, NULL);
		SendMessage(h1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP70)));
		Plfield.push_back(h1);
		HWND h2 = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rw.right - 176, rw.bottom - 235, 130, 160, hDialog, (HMENU)(ID_DECK), hInst, NULL);
		SendMessage(h2, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP69)));
		Plfield.push_back(h2);
		ShowWindow(h1, SW_HIDE);
		ShowWindow(h1, SW_SHOW);
		ShowWindow(h2, SW_HIDE);
		ShowWindow(h2, SW_SHOW);

		//место для карт
		HWND h3 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, rw.bottom - 176, 790, 176, hDialog, NULL, hInst, NULL);
		SendMessage(h3, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP71)));
		Plfield.push_back(h3);
		ShowWindow(h3, SW_HIDE);
		ShowWindow(h3, SW_SHOW);

		//фон для выбора цвета
		HWND h4 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, (rw.bottom - 176 - 235), 138, 230, hDialog, NULL, hInst, NULL);
		SendMessage(h4, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP72)));
		Plfield.push_back(h4);
		ShowWindow(h4, SW_HIDE);
		ShowWindow(h4, SW_SHOW);

		//кол-во очков
		HWND h5 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 19, (rw.bottom - 176 - 42), 100, 23, hDialog, NULL, hInst, NULL);
		Plfield.push_back(h5);
		HFONT hfont = CreateFont(19, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Calibri");
		SetTextColor((HDC)hfont, RGB(255, 255, 255));
		SelectObject((HDC)hfont, hfont);
		SendMessage(h5, WM_SETFONT, (LPARAM)hfont, TRUE);
		ShowWindow(h5, SW_HIDE);
		ShowWindow(h5, SW_SHOW);

		//имя текущего игрока
		HWND h6 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 19, (rw.bottom - 176 - 67), 100, 23, hDialog, NULL, hInst, NULL);
		Plfield.push_back(h6);
		SendMessage(h6, WM_SETFONT, (LPARAM)hfont, TRUE);
		ShowWindow(h6, SW_HIDE);
		ShowWindow(h6, SW_SHOW);
	}
	void DestroyPlayerZone()
	{
		for (auto it : Plfield)
			DestroyWindow(it);
		if (Plfield.size() > 0)
			Plfield.erase(Plfield.begin(), Plfield.begin() + Plfield.size());
	}

	void SetOtherZone(vector<Player*> &pl, HWND hDialog, HINSTANCE hInst)
	{
		RECT rw;
		GetClientRect(hDialog, &rw);
		hPause = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rw.right - 160, 170, 120, 40, hDialog, (HMENU)(ID_MENU_PAUSE), hInst, NULL);
		hExit = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rw.right - 160, 220, 120, 40, hDialog, (HMENU)(ID_MENU_EXIT), hInst, NULL);
		if (backImg == 0) //пауза выход
		{
			SendMessage(hPause, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP73)));
			SendMessage(hExit, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP74)));
		}
		//выбор цвета
		HWND h1 = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 30, rw.bottom - 235 - 110, 40, 40, hDialog, (HMENU)(ID_COLORS_RED), hInst, NULL);
		HWND h2 = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 70, rw.bottom - 235 - 110, 40, 40, hDialog, (HMENU)(ID_COLORS_BLUE), hInst, NULL);
		HWND h3 = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 30, rw.bottom - 235 - 70, 40, 40, hDialog, (HMENU)(ID_COLORS_YELLOW), hInst, NULL);
		HWND h4 = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 70, rw.bottom - 235 - 70, 40, 40, hDialog, (HMENU)(ID_COLORS_GREEN), hInst, NULL);
		bcolors.push_back(h1);
		bcolors.push_back(h2);
		bcolors.push_back(h3);
		bcolors.push_back(h4);
		for (size_t i = 0; i < 4; i++)
			SendMessage(bcolors[i], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP75 + i)));
		//кнопка уно
		unoBtn = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 150, rw.bottom - 300, 110, 110, hDialog, (HMENU)(ID_UNO), hInst, NULL);
		BOOL sayuno = FALSE;
		for (auto it : pl) {
			if (it->GetUno() == 0) {
				sayuno = TRUE;
				break;
			}
		}
		if (sayuno) SendMessage(unoBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP90)));
		else SendMessage(unoBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP81)));
	}
	void DestroyOtherZone()
	{
		DestroyWindow(hPause);
		DestroyWindow(hExit);
		hPause = NULL;
		hExit = NULL;
		for (auto it : bcolors)
			DestroyWindow(it);
		if (bcolors.size() > 0)
			bcolors.erase(bcolors.begin(), bcolors.begin() + bcolors.size());
		DestroyWindow(unoBtn);
		unoBtn = NULL;
	}

	//отрисовка карт
	void DrawCards(HWND hDialog, HINSTANCE hInst, Player& cards)
	{
		RECT rw;
		GetClientRect(hDialog, &rw);
		for (size_t i = 0; i < cards.color.size(); i++)
		{
			HWND h = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, (700 / cards.size) * i + 20, (rw.bottom - 136 - 30), 90, 136, hDialog, (HMENU)(ID_СARDS + i), hInst, NULL);
			if (cards.color[i] <= 3)
			{
				INT id = cards.color[i] * 13 + cards.type[i];
				SendMessage(h, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + id)));
			}
			else if (cards.color[i] == 4)
				SendMessage(h, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP53)));
			else if (cards.color[i] == 5)
				SendMessage(h, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP54)));
			plHcards.push_back(h);
		}
	}
	void DestroyCards()
	{
		for (auto it : plHcards)
			DestroyWindow(it);
		if (plHcards.size() > 0)
			plHcards.erase(plHcards.begin(), plHcards.begin() + plHcards.size());
	}

	void DrawDirection(HWND hDialog, HINSTANCE hInst, BOOL d)
	{
		RECT rw;
		GetClientRect(hDialog, &rw);
		arrow = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, rw.right - 160, rw.bottom - 350, 100, 40, hDialog, NULL, hInst, NULL);
		if (d) SendMessage(arrow, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP79)));
		else SendMessage(arrow, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP80)));
		ShowWindow(arrow, SW_HIDE);
		ShowWindow(arrow, SW_SHOW);
	}
	void DrawDeck(HWND hDialog, HINSTANCE hInst, Player& cards)
	{
		if (cards.size == 0)
			return;
		RECT rw;
		GetClientRect(hDialog, &rw);
		int x = 0, x1 = 0;
		int i = cards.size - 5;
		if (i <= 0)
			x1 = ((rw.right / 2) + 45) - (5 + i) * 45; //45 - полкарты
		else {
			x1 = ((rw.right / 2) + 45) - 5*45;
		}
		if (i < 0)
			i = 0;
		for (size_t j = 0; j < 5; j++)
		{
			x = x1 + (90 / 2)*j;
			HWND h = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, x, 220, 90, 136, hDialog, NULL, hInst, NULL);
			RECT r;
			GetClientRect(h, &r);
			if (cards.color[i] <= 3)
			{
				INT id = cards.color[i] * 13 + cards.type[i];
				SendMessage(h, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + id)));
			}
			else if (cards.color[i] == 4)
				SendMessage(h, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP53)));
			else if (cards.color[i] == 5)
				SendMessage(h, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP54)));
			deck.push_back(h);
			if (i == cards.size - 1)
				break;
			i++;
		}
		for (size_t i = 0; i < deck.size(); i++)
		{
			ShowWindow(deck[i], SW_HIDE);
			ShowWindow(deck[i], SW_SHOW);
		}
		
	}
	void DestroyDeck()
	{
		for (auto it : deck)
			DestroyWindow(it);
		if (deck.size() > 0)
			deck.erase(deck.begin(), deck.begin() + deck.size());
	}

	//очки у игроков и кол-во карт, имена игроков
	void SetPlayersScore(vector<Player*> &pl)
	{
		for (size_t i = 1; i < pl.size(); i++)
		{
			TCHAR t[15] = { 0 };
			_itot_s((*(pl.begin() + i))->size, t, 10);
			if (plscore.size() != 0)
				SetWindowText((*(plscore.begin() + (i - 1))), t);
			if ((*(pl.begin() + i))->name != NULL && plnames.size() != 0)
				SetWindowText(plnames[i - 1], pl[i]->name);
		}
		TCHAR t[15] = { 0 };
		_itot_s(pl[0]->GetScore(), t, 10);
		SetWindowText((*(Plfield.begin() + 4)), t);
		SetWindowText((*(Plfield.begin() + 5)), pl[0]->GetName());
	}
	
	void SetNeedColor(HWND hDialog, BOOL need)
	{
		for (size_t i = 0; i < bcolors.size(); i++)
		{
			if (need) EnableWindow(bcolors[i], TRUE);
			else EnableWindow(bcolors[i], FALSE);
		}
		
	}

	void SetNowColor(HWND hDialog, HINSTANCE hInst, int num) {
		RECT rw;
		GetClientRect(hDialog, &rw);
		hNowColor = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 50, rw.bottom - 235 - 165, 40, 40, hDialog, NULL, hInst, NULL);
		SendMessage(hNowColor, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP75 + num)));
		ShowWindow(hNowColor, SW_HIDE);
		ShowWindow(hNowColor, SW_SHOW);
	}

	void ChangeNowPlayer(HWND hDialog, HINSTANCE hInst, int nowmove) {
		RECT rw;
		GetClientRect(hDialog, &rw);
		hNowMove = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, ((((rw.right / plcount) - 340) / 2) + (rw.right / plcount) * nowmove) + 35, 10, 20, 20, hDialog, NULL, hInst, NULL);
		SendMessage(hNowMove, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP95)));
		ShowWindow(hNowMove, SW_HIDE);
		ShowWindow(hNowMove, SW_SHOW);
	}
	void ChangeNowColor(HINSTANCE hInst, int num) {
		SendMessage(hNowColor, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP75 + num)));
		ShowWindow(hNowColor, SW_HIDE);
		ShowWindow(hNowColor, SW_SHOW);
	}
	void ChangeUnoButtoms(vector<Player*> &pl, HINSTANCE hInst){
		BOOL u = FALSE;
		for (size_t i = 1; i < pl.size(); i++){
			if (pl[i]->GetUno() == -1 || pl[i]->GetUno() == 1)
				SendMessage(pluno[i - 1], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP68)));
			else{
				SendMessage(pluno[i - 1], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP91)));
				u = TRUE;
			}
			ShowWindow(pluno[i - 1], SW_HIDE);
			ShowWindow(pluno[i - 1], SW_SHOW);

		}
		if (u) SendMessage(unoBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP90)));
		else if (pl[0]->size == 1 && pl[0]->GetUno() == -1) SendMessage(unoBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP90)));
		else SendMessage(unoBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP81)));
		
	}
};
