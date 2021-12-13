#include "WindowDraw.h"

//сохранение настроек игры
void WindowDraw::ReadOptions()
{
	//считывание из файла настроек игры
	//установка размера окна, фона
	long Style = GetWindowLong(hDialog, GWL_STYLE);
	Style = Style ^ WS_MAXIMIZEBOX;
	SetWindowLong(hDialog, GWL_STYLE, Style);
	int xScreen = 1024, yScreen = 768;
	backImg = 0;
	SetWindowPos(hDialog, HWND_TOP, 0, 0, xScreen + 16, yScreen + 6, SWP_SHOWWINDOW); //временный размер окна
	GetClientRect(hDialog, &rWindow); //размер клиентской области окна
}
void WindowDraw::WriteOptions()
{
	//запись в файл настроек игры
}

//установка фона
void WindowDraw::SetBackground()
{//CreateWindow - создает дочернее окно. Она определяет класс, 
//заголовок, стиль окна и (необязательно) начальную позицию и размер окна.
//Функция также определяет и окно родителя или владельца, если таковые имеются и меню окна.
	GetClientRect(hDialog, &rWindow);
	hBack = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP,
		0, 0, rWindow.right, rWindow.bottom, hDialog,
		(HMENU)ID_BACK0, hInst, NULL);
	SendMessage(hBack, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP55 + backImg)));
}

//отрисовка меню
void WindowDraw::SetHeadMenu()
{
	GetClientRect(hDialog, &rWindow);
	for (size_t i = 0; i < 4; i++)
	{
		rMenu1[i].right = 240;
		rMenu1[i].bottom = 70;
		rMenu1[i].left = rWindow.right - rMenu1[i].right - 50;
		rMenu1[i].top = 300 + i * (rMenu1[i].bottom + 20);
		hMenu1[i] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rMenu1[i].left, rMenu1[i].top, rMenu1[i].right, rMenu1[i].bottom, hDialog, (HMENU)(ID_MENU0_PLAY + i), hInst, NULL);
		if (backImg == 0)
			SendMessage(hMenu1[i], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP57 + i))); //загрузка картинок на кнопки
		//else if (backImg == 1)
		//	SendMessage(hMenu1[i], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP + )));
	}
	hLogo = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 260) / 2, 20, 260, 140, hDialog, NULL, hInst, NULL);
	SendMessage(hLogo, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP64))); //загрузка лого
	ShowWindow(hLogo, SW_HIDE);
	ShowWindow(hLogo, SW_SHOW);
	hCards = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 80, rWindow.bottom - 245 - 100, 370, 245, hDialog, NULL, hInst, NULL);
	SendMessage(hCards, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP65)));
	ShowWindow(hCards, SW_HIDE);
	ShowWindow(hCards, SW_SHOW);
}
void WindowDraw::DestroyHeadMenu()
{
	for (size_t i = 0; i < 4; i++)
		DestroyWindow(hMenu1[i]);
	DestroyWindow(hLogo);
	DestroyWindow(hCards);
}

//меню перед игрой
void WindowDraw::SetPlayMenu(BOOL vsBot)
{
	//настройка игры: против бота или людей
	//кол-во игроков

	ch1bot = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, (rWindow.right - 200) / 2 + 150, 180, 140, 40, hDialog, (HMENU)(ID_MENU3_CHECK1), hInst, NULL);
	SendMessage(ch1bot, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP87)));
	ch2pl = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, (rWindow.right - 200) / 2 + 150, 240, 140, 40, hDialog, (HMENU)(ID_MENU3_CHECK2), hInst, NULL);
	SendMessage(ch2pl, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP86)));

	editspinplcount = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NUMBER, (rWindow.right - 200) / 2 + 190, 360, 50, 40, hDialog, NULL, hInst, NULL);
	SetWindowText(editspinplcount, TEXT("2"));

	HFONT hfont = CreateFont(40, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Calibri");
	SetBkMode((HDC)hfont, TRANSPARENT);
	SetTextColor((HDC)hfont, RGB(255, 255, 255));
	SelectObject((HDC)hfont, hfont);
	SendMessage(editspinplcount, WM_SETFONT, (LPARAM)hfont, TRUE);

	UDACCEL pAcceleration[3] = { { 1, 1 } };
	spinplcount = CreateWindow(UPDOWN_CLASS, NULL, WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS, (rWindow.right - 200) / 2 + 240, 360, 20, 40, hDialog, NULL, hInst, NULL);
	SendMessage(spinplcount, UDM_SETRANGE32, 2, 4);
	SendMessage(spinplcount, UDM_SETACCEL, 1, LPARAM(pAcceleration)); //Sets the acceleration for an up-down control.
	SendMessage(spinplcount, UDM_SETBUDDY, WPARAM(editspinplcount), 0);

	playVS = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 200) / 2 - 150, 200, 220, 60, hDialog, NULL, hInst, NULL);
	SendMessage(playVS, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP82)));
	ShowWindow(playVS, SW_HIDE);
	ShowWindow(playVS, SW_SHOW);
	playCount = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 200) / 2 - 150, 350, 220, 60, hDialog, NULL, hInst, NULL);
	SendMessage(playCount, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP83)));
	ShowWindow(playCount, SW_HIDE);
	ShowWindow(playCount, SW_SHOW);


	for (size_t i = 0; i < 2; i++) //"назад" и "начать игру"
	{
		rMenu3[i].right = 240;
		rMenu3[i].bottom = 70;
		if (i == 0)
			rMenu3[i].left = 20;
		else rMenu3[i].left = rWindow.right - 20 - rMenu3[i].right;
		rMenu3[i].top = rWindow.bottom - 40 - rMenu3[i].bottom;
		hMenu3[i] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rMenu3[i].left, rMenu3[i].top, rMenu3[i].right, rMenu3[i].bottom, hDialog, (HMENU)(ID_MENU3_BACK + i), hInst, NULL);
		if (backImg == 0)
		{
			if (i == 0)
				SendMessage(hMenu3[i], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP61))); //загрузка картинок на кнопки
			else if (i == 1)
				SendMessage(hMenu3[i], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP63)));
		}
	}
}

void WindowDraw::RedrawPlayMenu(BOOL vsBot)
{
	if (vsBot)
	{
		SendMessage(ch1bot, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP87)));
		SendMessage(ch2pl, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP86)));
	}
	else
	{
		SendMessage(ch1bot, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP85)));
		SendMessage(ch2pl, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP88)));
	}

}
void WindowDraw::DestroyPlayMenu()
{
	for (size_t i = 0; i < 2; i++)
		DestroyWindow(hMenu3[i]);
	DestroyWindow(playVS);
	DestroyWindow(playCount);
	DestroyWindow(ch1bot);
	DestroyWindow(ch2pl);
	DestroyWindow(spinplcount);
	DestroyWindow(editspinplcount);
}

//пауза в игре
void WindowDraw::SetPauseMenu()
{
	hGamePause[0] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 320) / 2, 150, 320, 180, hDialog, NULL, hInst, NULL);
	SendMessage(hGamePause[0], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP64)));
	ShowWindow(hGamePause[0], SW_HIDE);
	ShowWindow(hGamePause[0], SW_SHOW);
	hGamePause[1] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 40, rWindow.bottom - 110, 240, 70, hDialog, (HMENU)(ID_PAUSEEXIT), hInst, NULL);
	SendMessage(hGamePause[1], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP101)));
	hGamePause[2] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rWindow.right - 240 - 40, rWindow.bottom - 110, 240, 70, hDialog, (HMENU)(ID_CONTINUE), hInst, NULL);
	SendMessage(hGamePause[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP100)));
}
void WindowDraw::DestroyPauseMenu()
{
	for (auto it : hGamePause)
		DestroyWindow(it);
}

//рекорды
void WindowDraw::SetOptMenu(vector<vector<TCHAR*>> players, vector<vector<int>> scores)
{
	GetClientRect(hDialog, &rWindow);
	int x = (rWindow.right - 800) / 2, y = 40;
	hMenu2[0] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 20, rWindow.bottom - 40 - 70, 240, 70, hDialog, (HMENU)(ID_MENU1_BACK), hInst, NULL);
	SendMessage(hMenu2[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP61))); //загрузка картинок на кнопки
	hMenu2[2] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rWindow.right - 300, rWindow.bottom - 165, 190, 40, hDialog, (HMENU)(ID_CLEAR_RECORDS), hInst, NULL);
	SendMessage(hMenu2[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP102))); //сброс статистики рекордов

	hMenu2[1] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 800) / 2, 40, 800, 560, hDialog, NULL, hInst, NULL);
	SendMessage(hMenu2[1], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP93)));
	ShowWindow(hMenu2[1], SW_HIDE);
	ShowWindow(hMenu2[1], SW_SHOW);
	x += 50;
	y += 65;
	HFONT hfont = CreateFont(25, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Calibri");
	SetBkMode((HDC)hfont, TRANSPARENT);
	SetTextColor((HDC)hfont, RGB(255, 255, 255));
	SelectObject((HDC)hfont, hfont);
	for (size_t i = 0; i < players.size(); i++) {
		HWND num = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, x, y, 35, 23, hDialog, NULL, hInst, NULL);
		SendMessage(num, WM_SETFONT, (LPARAM)hfont, TRUE);
		ShowWindow(num, SW_HIDE);
		ShowWindow(num, SW_SHOW);
		hRecords.push_back(num);
		TCHAR t[10] = { 0 };
		_itot_s(i + 1, t, 10);
		SetWindowText(hRecords[hRecords.size() - 1], t);
		int x1 = x + 50;
		for (size_t j = 0; j < players[i].size(); j++) {
			HWND n = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, x1, y, 120, 23, hDialog, NULL, hInst, NULL);
			SendMessage(n, WM_SETFONT, (LPARAM)hfont, TRUE);
			ShowWindow(n, SW_HIDE);
			ShowWindow(n, SW_SHOW);
			hRecords.push_back(n);
			SetWindowText(hRecords[hRecords.size() - 1], players[i][j]);
			x1 += 130;
		}
		x1 = x + 50;
		y += 30;
		for (size_t j = 0; j < scores[i].size(); j++) {
			HWND s = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, x1, y, 60, 23, hDialog, NULL, hInst, NULL);
			SendMessage(s, WM_SETFONT, (LPARAM)hfont, TRUE);
			ShowWindow(s, SW_HIDE);
			ShowWindow(s, SW_SHOW);
			hRecords.push_back(s);
			TCHAR t[15] = { 0 };
			_itot_s(scores[i][j], t, 10);
			SetWindowText(hRecords[hRecords.size() - 1], t);
			x1 += 130;
		}
		y += 38;
	}
}
void WindowDraw::ClearOptMenu() {
	for (auto it : hRecords)
		DestroyWindow(it);
}
void WindowDraw::DestroyOptMenu()
{
	for (auto it : hMenu2)
		DestroyWindow(it);
	for (auto it : hRecords)
		DestroyWindow(it);
}

//помошь в игре
void WindowDraw::SetHelpMenu()
{
	GetClientRect(hDialog, &rWindow);
	hMenu4[0] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, 20, (rWindow.bottom - 40 - 70), 240, 70, hDialog, (HMENU)(ID_MENU2_BACK), hInst, NULL);
	if (backImg == 0)
		SendMessage(hMenu4[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP61)));
	hMenu4[1] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 900) / 2, 40, 900, 560, hDialog, NULL, hInst, NULL);
	SendMessage(hMenu4[1], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP92)));
	ShowWindow(hMenu4[1], SW_HIDE);
	ShowWindow(hMenu4[1], SW_SHOW);
}
void WindowDraw::DestroyHelpMenu()
{
	//очистка экрана при возвращении в меню
	for (auto it : hMenu4)
		DestroyWindow(it);
}

//начальное игровое поле
void WindowDraw::DrawGameField(vector<Player*>& pl, BOOL vsBot, short need)
{
	gf.SetEmptyField(pl, hDialog, hInst, backImg, vsBot, need);
}
//очистить экран после игры
void WindowDraw::DestroyGameField()
{
	gf.DestroyEmptyField();
	gf.DestroyDetailsField();
}

//отображение данных игроков
void WindowDraw::UpdateDetails(vector<Player*>& pl, BOOL dir, size_t nowmove, Player& cards, BOOL need, int cnum)
{
	gf.SetDetailsField(hDialog, hInst, pl, dir, nowmove, cards, need, cnum);
}
void WindowDraw::SetNeedColor(BOOL need) {
	gf.SetNeedColor(hDialog, need);
}
void WindowDraw::ChangeCurrentColor(int nowcolor) {
	gf.ChangeNowColor(hInst, nowcolor);
}
void WindowDraw::SetSpinPlCount(INT num)
{
	if (num < 2)
		num = 2;
	TCHAR t[4] = { 0 };
	_itot_s(num, t, 10);
	SetWindowText(editspinplcount, t);
}

void WindowDraw::DrawPlPause(TCHAR* name1, TCHAR* name2, TCHAR* message, size_t plcount = 0, BOOL isItStart = FALSE) //имя текущего и следующего игрока
{
	DestroyGameField();
	if (isItStart) //если это только начало игры, создать окна со вводом имен
	{
		//поля для ввода имен
		int yNameStatic;
		for (size_t i = 0; i < plcount; i++)
		{
			if (i == 0)
				yNameStatic = ((rWindow.bottom - 100) / plcount) - 90;
			HWND plN = CreateWindow(TEXT("EDIT"), TEXT("Игрок "), WS_CHILD | WS_VISIBLE | WS_TABSTOP, (rWindow.right - 200) / 2, ((rWindow.bottom - 100) / plcount) + i * 50, 200, 30, hDialog, NULL, hInst, NULL);

			TCHAR number[5] = { 0 };
			_itot((i + 1), number, 10);
			int len = GetWindowTextLength(plN);
			SendMessage(plN, EM_SETSEL, len, len);
			SendMessage(plN, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(number));

			HFONT hfont = CreateFont(30, 15, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));
			SetTextColor((HDC)hfont, RGB(255, 255, 255));
			SelectObject((HDC)hfont, hfont);
			SendMessage(plN, WM_SETFONT, (LPARAM)hfont, TRUE);
			plnames.push_back(plN);
			ShowWindow(plN, SW_HIDE);
			ShowWindow(plN, SW_SHOW);
		
		}
	
		//выбор имен игроков
		hGamePause[2] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 280) / 2, yNameStatic, 280, 70, hDialog, NULL, hInst, NULL);
		SendMessage(hGamePause[2], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP96)));
		ShowWindow(hGamePause[2], SW_HIDE);
		ShowWindow(hGamePause[2], SW_SHOW);
	}
	else {
		hGamePause[2] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 420) / 2, (rWindow.bottom - 100) / 2 - 20, 420, 95, hDialog, NULL, hInst, NULL);
		SendMessage(hGamePause[2], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP98)));
		ShowWindow(hGamePause[2], SW_HIDE);
		ShowWindow(hGamePause[2], SW_SHOW);
		hGamePause[1] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, (rWindow.right - 400) / 2, (rWindow.bottom - 75) / 2 - 20, 400, 70, hDialog, NULL, hInst, NULL);
		HFONT hfont = CreateFont(23, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));
		SetWindowText(hGamePause[1], getPauseMessage(name1, name2, message));
		SetTextColor((HDC)hfont, RGB(255, 255, 255));
		SetTextAlign((HDC)hfont, TA_CENTER);
		SelectObject((HDC)hfont, hfont);
		SendMessage(hGamePause[1], WM_SETFONT, (LPARAM)hfont, TRUE);
		ShowWindow(hGamePause[1], SW_HIDE);
		ShowWindow(hGamePause[1], SW_SHOW);
	}
	hGamePause[0] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, (rWindow.right - 240) / 2, rWindow.bottom - 250, 240, 70, hDialog, (HMENU)(ID_NEXT_PLAYER), hInst, NULL);
	SendMessage(hGamePause[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP89)));
}
void WindowDraw::DestroyPlPause(BOOL isItStart = FALSE)
{
	DestroyWindow(hGamePause[2]);
	if (isItStart)
	{
		for (auto it : plnames)
			DestroyWindow(it);
		if (plnames.size() > 0)
			plnames.erase(plnames.begin(), plnames.begin() + plnames.size());
	}
	else {
		DestroyWindow(hGamePause[1]);
	}
	DestroyWindow(hGamePause[0]);
}

void WindowDraw::DrawRoundEndPause(vector<Player*>& pl, short Games, vector<vector<int>>& roundTable, vector<TCHAR*>& plNames) {
	DestroyGameField();
	//таблица игроков
	int x = (rWindow.right - 800) / 2, y = 60;
	if (roundTable.size() != 0 && plNames.size() != 0) {
		hGamePause[1] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, x, y, 800, 282, hDialog, NULL, hInst, NULL);
		SendMessage(hGamePause[1], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP97)));
		ShowWindow(hGamePause[1], SW_HIDE);
		ShowWindow(hGamePause[1], SW_SHOW);
	}
	else {
		hGamePause[1] = nullptr;
	}
	//шрифт
	HFONT hfont = CreateFont(24, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Calibri");
	SetBkMode((HDC)hfont, TRANSPARENT);
	SetTextColor((HDC)hfont, RGB(255, 255, 255));
	SelectObject((HDC)hfont, hfont);

	x += 42;
	int z = x;
	y += 115;
	for (size_t i = 0; i < plNames.size(); i++) {
		//имя
		HWND pln = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, x, y, 120, 23, hDialog, NULL, hInst, NULL);
		SendMessage(pln, WM_SETFONT, (LPARAM)hfont, TRUE);
		ShowWindow(pln, SW_HIDE);
		ShowWindow(pln, SW_SHOW);
		plnames.push_back(pln);
		SetWindowText(plnames[i], plNames[i]);

		x += 170;
		for (size_t j = 0; j < (roundTable[i]).size(); j++) {
			if (roundTable[i][j] == -1)
				break;
			HWND pls = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, x, y, 60, 23, hDialog, NULL, hInst, NULL);
			SendMessage(pls, WM_SETFONT, (LPARAM)hfont, TRUE);
			ShowWindow(pls, SW_HIDE);
			ShowWindow(pls, SW_SHOW);
			plscores.push_back(pls);
			TCHAR t[15] = { 0 };
			_itot_s(roundTable[i][j], t, 10);
			SetWindowText(plscores[plscores.size() - 1], t);
			x += 84;
			if (j == 4)
				x += 26;
		}
		x = z;
		y += 30;
	}
	if (Games == 5) {
		hGamePause[0] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, (rWindow.right - 240) / 2, rWindow.bottom - 220, 240, 70, hDialog, (HMENU)(ID_END_GAME), hInst, NULL);
		SendMessage(hGamePause[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP60)));
	}
	else {
		hGamePause[0] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, (rWindow.right - 380) / 2, rWindow.bottom - 220, 380, 70, hDialog, (HMENU)(ID_NEXT_ROUND), hInst, NULL);
		SendMessage(hGamePause[0], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP94)));

		//кнопка перехода по уровням (magic)
		hGamePause[2] = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, rWindow.right - 60, rWindow.bottom - 50, 40, 26, hDialog, (HMENU)(ID_MAGIC), hInst, NULL);
		SendMessage(hGamePause[2], BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP99)));
	}


}
void WindowDraw::DestroyRoundEndPause()
{
	for (auto it : plnames)
		DestroyWindow(it);
	if (plnames.size() > 0)
		plnames.erase(plnames.begin(), plnames.begin() + plnames.size());
	for (auto it : plscores)
		DestroyWindow(it);
	if (plscores.size() > 0)
		plscores.erase(plscores.begin(), plscores.begin() + plscores.size());
	if (hGamePause[1] != nullptr)
		DestroyWindow(hGamePause[1]);
	DestroyWindow(hGamePause[0]);
	DestroyWindow(hGamePause[2]);
}

TCHAR* WindowDraw::getPauseMessage(TCHAR* name1, TCHAR* name2, TCHAR* message) {
	TCHAR* buf = NULL;
	if (name1 == NULL && message != NULL)
		buf = new TCHAR[_tcslen(name2) + _tcslen(message) + 1 + 13];
	else if (message == NULL)
		buf = new TCHAR[_tcslen(name2) + 1 + 13];
	else buf = new TCHAR[_tcslen(name1) + _tcslen(name2) + _tcslen(message) + 1 + 13];
	buf[0] = '\0';
	if (message != NULL && name1 != NULL) {
		_tcscat(buf, name1);
		_tcscat(buf, message);
	}
	else if (message != NULL && name1 == NULL)
		_tcscat(buf, message);

	_tcscat(buf, TEXT("Сейчас ходит "));
	_tcscat(buf, name2);
	return buf;
}

void WindowDraw::MessageAboutPlayers(TCHAR* name1, TCHAR* name2, TCHAR* message) {
	DestroyMessageAboutPlayers();
	if (message != NULL) {
		hGamePause[2] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, (rWindow.right - 420) / 2, rWindow.bottom - 290, 420, 95, hDialog, NULL, hInst, NULL);
		SendMessage(hGamePause[2], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP98)));
		ShowWindow(hGamePause[2], SW_HIDE);
		ShowWindow(hGamePause[2], SW_SHOW);
		hGamePause[1] = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, (rWindow.right - 400) / 2, rWindow.bottom - 280, 400, 70, hDialog, NULL, hInst, NULL);
		HFONT hfont = CreateFont(23, 10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Calibri"));
		SetWindowText(hGamePause[1], getPauseMessage(name1, name2, message));
		SetTextColor((HDC)hfont, RGB(255, 255, 255));
		SetTextAlign((HDC)hfont, TA_CENTER);
		SelectObject((HDC)hfont, hfont);
		SendMessage(hGamePause[1], WM_SETFONT, (LPARAM)hfont, TRUE);
		ShowWindow(hGamePause[1], SW_HIDE);
		ShowWindow(hGamePause[1], SW_SHOW);
	}
}
void WindowDraw::DestroyMessageAboutPlayers() {
	DestroyWindow(hGamePause[1]);
	DestroyWindow(hGamePause[2]);
}

void WindowDraw::EndGameWindow(vector<Player*>& pl) {
	DestroyGameField();
	for (size_t i = 0; i < pl.size(); i++) 
	{

	}

}