#pragma once
#include "Player.h"
#include "WindowDraw.h"

class GameProcess
{
	Player deckBot; //колода, управляемая ботом - не вытащенные карты
	Player cards; //колода, в которую кладут карты
	vector<Player*> pl; //колоды игроков
	BOOL direction = TRUE;
	size_t cureentTurn = 0;

	short Games = 0;
	BOOL specCard = 0; // взял доп карту ?
	BOOL needUpdate = TRUE;

	vector<TCHAR*> playersNames;
	vector<vector<int>> roundTable;
public:
	HWND hwnd;
	BOOL VsBot = TRUE;
	BOOL checkColor = FALSE;
	BOOL playerGo = FALSE;
	BOOL needUno = FALSE;
	int currentColor = 0; //0 - 3
	BOOL startGame = TRUE;
	TCHAR* mesageAboutPlayers = NULL;
	BOOL botGo = FALSE;
	time_t StartTime;
	BOOL nowTimer = false;

	void Start(size_t plNum, BOOL vsWho)
	{
		VsBot = vsWho;
		for (size_t i = 0; i < plNum; i++)  //создание колод игроков(без карт)
		{
			Player* p = new Player;
			pl.push_back(p);
		}
		pl[0]->noBot = true;
		FirstDeck();
		SwapCards(deckBot);
		GiveOutCards();
		CheckScores();
	}
	void StartRound(){
		FirstDeck();
		SwapCards(deckBot);
		GiveOutCards();
		CheckScores();
	}
	void StartGame(WindowDraw & wd) //начало процесса игры
	{
		if (!startGame)
			wd.DrawGameField(pl, VsBot, currentColor);
		GiveOutFirstCard();
		currentColor = *(cards.color.begin());
		if (!startGame)
			wd.UpdateDetails(pl, direction, cureentTurn, cards, checkColor, currentColor);
	}

	void End(WindowDraw & wd, bool end = false)
	{
		wd.DestroyGameField();
		deckBot.ClearAll();
		cards.ClearAll();
		while (pl.size() != 0)
		{
			delete pl[0];
			pl.erase(pl.begin());
		}
		if (nowTimer)
			KillTimer(hwnd, 1);
	/*	if (mesageAboutPlayers != NULL)
		{
			delete[] mesageAboutPlayers;
			mesageAboutPlayers = NULL;
		}*/
		direction = TRUE;
		cureentTurn = 0;
		Games = 0;
		specCard = 0;
		needUpdate = TRUE;
		DeleteRoudTable();
		DeletePlayersNames(end);
		VsBot = TRUE;
		checkColor = FALSE;
		playerGo = FALSE;
		needUno = FALSE;
		currentColor = 0;
		startGame = TRUE;
		botGo = FALSE;
		StartTime = time(NULL);
		nowTimer = false;
	}
	void EndRound(WindowDraw & wd)
	{
		wd.DestroyGameField();
		deckBot.ClearAll();
		cards.ClearAll();
		CheckScores();
		SetEndRoundScores();
		AddRoundTable();
		if (nowTimer)
			KillTimer(hwnd, 1);
		nowTimer = false;
		for (size_t i = 0; i < pl.size(); i++)
		{
			pl[i]->ClearForNextRound();
		}
		direction = TRUE;
		if (VsBot)
			cureentTurn = 0;
		//if (mesageAboutPlayers != NULL)
		//{
		//	delete[] mesageAboutPlayers;
		//	mesageAboutPlayers = NULL;
		//}
		specCard = 0;
		needUpdate = TRUE;
		playerGo = TRUE;              //?
		botGo = FALSE;
		checkColor = FALSE;
		needUno = FALSE;
		currentColor = 0;
		specCard = FALSE;
		cureentTurn = 0;
		startGame = TRUE;
		StartTime = time(NULL);
	}

	void ResetDeck()
	{
		if (deckBot.size < 5)
		{
			for (size_t i = 0; i < cards.size - 5; i++)
			{
				deckBot.AddCard(cards.color[i], cards.type[i]);
				cards.DelCard(i);
			}
			SwapCards(deckBot);
		}
	}

	void FirstDeck() //первая колода карт
	{
		for (size_t c = 0; c <= 5; c++) //цвет / поменять цвет и + 4
		{
			if (c <= 3)
			{
				short f = 0;
				for (size_t n = 0; n <= 12; n++)
				{
					deckBot.AddCard(c, n);
					if (n == 12 && f == 0)
					{
						n = 0;
						f++;
					}
				}
			}
			else if (c == 4 || c == 5)
				for (size_t i = 0; i < 4; i++)
					deckBot.AddCard(c, 0);
		}
	}
	void SwapCards(Player & plcards) //перемешивание карт в колоде
	{
		int r;
		for (size_t i = 0; i < plcards.size; i++) //0 - 107
		{
			r = rand() % plcards.size;
			plcards.SwapCard(i, r);
		}
	}
	void GiveOutFirstCard()
	{
		for (size_t i = 0; i < deckBot.size; i++)
		{
			if (!IsSpecCard(deckBot.color[i], deckBot.type[i]))
			{
				cards.AddCard(deckBot.color[i], deckBot.type[i]);
				deckBot.DelCard(0);
				break;
			}
		}
	}
	void GiveOutCards() //начальная раздача карт игрокам
	{
		for (size_t i = 0; i < 8; i++) //по 8 карт
			for (size_t j = 0; j < pl.size(); j++) //каждому игроку
				GiveOutCard((*(pl.begin() + j))); //номер игрока
	}
	void GiveOutCard(Player * player)
	{
		player->AddCard((*(deckBot.color.begin())), (*(deckBot.type.begin())));
		deckBot.DelCard(0);
	}
	
	void PutCard(size_t plNum, size_t num, BOOL interrupt) //положить карту игрока в колоду игры
	{
		int c = (*(pl.begin() + plNum))->GetColor(num); //цвет карты, которую кладут
		int n = (*(pl.begin() + plNum))->GetType(num); //номер
		if (IsCorrectCard(*(pl.begin() + plNum), c, n, interrupt)) //проверка, что такую карту можно положить на предыдущую
		{
			cards.AddCard(c, n);
			(*(pl.begin() + plNum))->DelCard(num);
		}
	}

	BOOL HaveNeedColorCard(Player* pl, int color)
	{
		for (auto it : pl->color)
			if (it == color && it <= 3)
				return TRUE;
		return FALSE;
	}
	BOOL IsCorrectCard(Player* pl, int c, int n, BOOL spcard)
	{
		int c1 = cards.GetColor(cards.size - 1);
		int n1 = cards.GetType(cards.size - 1);
		if (!spcard) {
			if (c1 <= 3 && n1 <= 9)
			{
				if (c1 == c)
					if (n <= 12)
						return TRUE;
				if (n == n1) return TRUE;
				if (c == 5 || (c == 4 && !HaveNeedColorCard(pl, c1)))
					return TRUE;
			}
			if (c1 <= 3 && (n1 == 10 || n1 == 11 || n1 == 12))
			{
				if (c1 == c)
					if (n <= 12)
						return TRUE;
				if (n == n1) return TRUE;
				if (c == 5 || (c == 4 && !HaveNeedColorCard(pl, c1)))
					return TRUE;
			}
			if (c1 == 5)
			{
				if (currentColor == c)
					if (n <= 12)
						return TRUE;
				if (c == 5 || (c == 4 && !HaveNeedColorCard(pl, c1)))
					return TRUE;
			}
			if (c1 == 4)
			{
				if (currentColor == c)
					if (n <= 12)
						return TRUE;
				if (c == 5 || (c == 4 && !HaveNeedColorCard(pl, c1)))
					return TRUE;
			}
		}
		else if (spcard){
			if (c1 <= 3 && n1 == 10) {
				if (c <= 3 && n == 10)
					return TRUE;
			}
			if (c1 <= 3 && n1 == 12) {
				if ((c <= 3 && n == 12) || c == 4)
					return TRUE; //иногда карту можно перебить +4(в каких-то правилах)
			}
			if (c1 == 4 && c == 4)
				return TRUE;
		}
		return FALSE;
	}
	BOOL IsSpecCard(int c, int n)
	{
		if (c <= 3 && n <= 9)
			return FALSE;
		return TRUE;
	}

	void ChangeDirection() //смена направления игры
	{
		if (direction)
			direction = FALSE;
		else direction = TRUE;
	}
	void CheckToSpecCardEffect(WindowDraw & wd)
	{
		int c1 = *(cards.color.begin() + (cards.color.size() - 1));
		int n1 = *(cards.type.begin() + (cards.type.size() - 1));
		if (specCard == 1 && !IsPlayerHasNeededCard(pl[cureentTurn], specCard))
		{
			if (c1 <= 3 && n1 == 10) {
				ResetMessage(TEXT(" не может отбить и пропускает ход.\n"), pl[cureentTurn]->name);
				UpdateAfterMove(wd);
			}
			else if (c1 <= 3 && n1 == 12)
			{
				GiveOutCard(pl[cureentTurn]);
				GiveOutCard(pl[cureentTurn]);
				ResetMessage(TEXT(" берет 2 карты и пропускает ход.\n"), pl[cureentTurn]->name);
				UpdateAfterMove(wd);
			}
			else if (c1 == 4)
			{
				GiveOutCard(pl[cureentTurn]);
				GiveOutCard(pl[cureentTurn]);
				GiveOutCard(pl[cureentTurn]);
				GiveOutCard(pl[cureentTurn]);
				ResetMessage(TEXT(" берет 4 карты и пропускает ход.\n"), pl[cureentTurn]->name);
				UpdateAfterMove(wd);
			}
			specCard = 0;
			if (VsBot)
				wd.MessageAboutPlayers(NULL, GetCurrentPlayerName(), mesageAboutPlayers);
		}
	}

	BOOL Move(INT id, WindowDraw & wd)
	{
		id -= ID_СARDS;
		int c = pl[cureentTurn]->color[id];
		int n = pl[cureentTurn]->type[id];
		int c1 = *(cards.color.begin() + (cards.color.size() - 1));
		int n1 = *(cards.type.begin() + (cards.type.size() - 1));

		BOOL y = IsCorrectCard(pl[cureentTurn], c, n, specCard);
		if(y){
			//wd.DestroyMessageAboutPlayers();
			for (size_t i = 0; i < pl.size(); i++)
			{
				if (pl[i]->ForgotUno()){
					GiveOutCard(pl[cureentTurn]);
					SayUno();
					Update(wd);
					ResetMessage(TEXT(" не заметил, что у предыдущего игрока последняя карта. Берет 1 карту.\n"), pl[cureentTurn]->name);
				}
			}
			PutCard(cureentTurn, id, 0);
			if (pl[cureentTurn]->size == 1) {
				needUno = TRUE;
				StartTime = time(NULL);
				SetTimer(hwnd, 1, 100, NULL);
				nowTimer = true;
			}
			if (specCard == 0 && !IsSpecCard(c, n))
				specCard = 0;
			else if (specCard == 0 && (c <= 3 && n == 10))
				specCard = 1;
			else if (specCard == 0 && (c <= 3 && n == 11))
			{
				ChangeDirection();
				if (!needUno)
					needUpdate = TRUE;
				ResetMessage(TEXT(" меняет направление.\n"), pl[cureentTurn]->name);
			}
			else if (specCard == 0 && (c <= 3 && n == 12))
				specCard = 1;
			else if (specCard == 0 && (c == 5))
			{
				checkColor = TRUE;
				needUpdate = FALSE;
				wd.SetNeedColor(1);
			}
			else if (specCard == 1 && (c1 <= 3 && (n1 == 10 || n1 == 12)))
					specCard = 1;
			else if (c == 4)
			{
				checkColor = TRUE;
				needUpdate = FALSE;
				wd.SetNeedColor(1);
				specCard = 1;
			}
			
			if (needUpdate && !needUno)
				UpdateAfterMove(wd);
			else {
				if (!checkColor)
					if (!needUno)
						NextPlayer();
				CheckScores();
			}
			ResetDeck();
			if (currentColor != c && c <= 3)
			{
				currentColor = c;
				wd.ChangeCurrentColor(currentColor);
			}
			return TRUE;
		}
		return FALSE;
	}
	void BotMove(WindowDraw & wd) {
		wd.DestroyMessageAboutPlayers();
		BotPutCards(wd);
		ResetDeck();
		UpdateAfterMove(wd);
		CheckToSpecCardEffect(wd);
		Sleep(5);
	}
	void BotPutCards(WindowDraw & wd){
		if (IsPlayerHasNeededCard(pl[cureentTurn], specCard)) {
			for (size_t i = 0; i < pl.size(); i++)
			{
				if (pl[i]->ForgotUno()){
					SayUno();
					ResetMessage(TEXT(" заметил, что у игрока последняя карта.\n"), pl[cureentTurn]->name);
					Update(wd);
					break;
				}
			}
			int c = 0, n = 0;
			for (size_t i = 0; i < pl[cureentTurn]->color.size(); i++) {
				if (IsCorrectCard(pl[cureentTurn], pl[cureentTurn]->color[i], pl[cureentTurn]->type[i], specCard)){
					c = pl[cureentTurn]->GetColor(i);
					n = pl[cureentTurn]->GetType(i);

					PutCard(cureentTurn, i, specCard);
					Sleep(2);
				//	ResetMessage(TEXT(" положил карту\n"), pl[cureentTurn]->name);
					//Update(wd);
					
					break;
				}
			}
			if (pl[cureentTurn]->size == 1) {
				BotSaidUno(wd); //бот говорит уно
				Update(wd);
			}
			if (specCard == 0 && !IsSpecCard(c, n))
				specCard = 0;
			else if (specCard == 0 && (c <= 3 && n == 10))
				specCard = 1;
			else if (specCard == 0 && (c <= 3 && n == 11))
			{
				ChangeDirection();
				ResetMessage(TEXT(" меняет направление.\n"), pl[cureentTurn]->name);
				wd.MessageAboutPlayers(NULL, GetNextPlayer()->name, mesageAboutPlayers);
				ResetMessage();
			}
			else if (specCard == 0 && (c <= 3 && n == 12))
				specCard = 1;
			else if (specCard == 0 && (c == 5)) //выбор следующего цвета по присутствующим в колоде бота
			{
				currentColor = GetColorForBot((*pl[cureentTurn]));
			}
			else if (specCard == 0 && c == 4)
			{
				specCard = 1;
			}
			else{
				int c1 = *(cards.color.begin() + (cards.color.size() - 1));
				int n1 = *(cards.type.begin() + (cards.type.size() - 1));
				if (specCard == 1 && ((c1 <= 3 && (n1 == 10 || n1 == 12)) || c1 == 4))
					specCard = 1;
			}
			if (currentColor != c){  //если бот изменил цвет
				if (c <= 3) {
					currentColor = c;
					wd.ChangeCurrentColor(currentColor);
				}
				else {
					ResetMessage(TEXT(" изменил цвет.\n"), GetCurrentPlayerName());
					wd.MessageAboutPlayers(NULL, GetNextPlayer()->name, mesageAboutPlayers);
					ResetMessage();
				}
			}
		}
		else {
			if (pl[cureentTurn]->size == 1){
				BotSaidUno(wd);
				Update(wd);
			}
			int dCheck = DeckChecked(wd);
			if (dCheck != 1){ //если после взятия карты из колоды есть чем ходить
				BotPutCards(wd);
			}
			else {
				wd.MessageAboutPlayers(GetCurrentPlayerName(), GetNextPlayer()->name, TEXT(" нечем ходить.\n"));
				ResetMessage();
			}
		}
	}

	int GetColorForBot(Player & bot) //поиск цвета, который будет полезен для бота при следующем ходе
	{
		int s = 0;
		for (size_t i = 0; i < bot.color.size(); i++){
			if (bot.color[i] <= 3){
				return bot.color[i];
			}
		}
		return rand() % 4;
	}

	void BotPutCard() {
		for (size_t i = 0; i < pl[cureentTurn]->color.size(); i++) {
			if (pl[cureentTurn]->color[i] == cards.color[(cards.color.size()) - 1]) {
				PutCard(cureentTurn, i, specCard);
				break;
			}
		}
		
	}

	void UpdateAfterMove(WindowDraw & wd)
	{
		NextPlayer();
		CheckScores();
	}
	void Update(WindowDraw & wd){
		wd.UpdateDetails(pl, direction, cureentTurn, cards, checkColor, currentColor);
	}

	Player* GetNextPlayer(){
		if (direction) {
			if (VsBot)
			{
				if (cureentTurn < pl.size() - 1)
					return pl[cureentTurn + 1];
				else return pl[0];
			}
			else return pl[1];
		}
		else
		{
			if (VsBot)
			{
				if (cureentTurn > 0)
					return pl[cureentTurn - 1];
				else return pl[pl.size() - 1];
			}
			else return pl[pl.size() - 1];
		}
	}
	Player* GetPrevPlayer() {
		if (!direction){
			if (VsBot)
			{
				if (cureentTurn < pl.size() - 1)
					return pl[cureentTurn + 1];
				else return pl[0];
			}
			else return pl[1];
		}
		else {
			if (VsBot)
			{
				if (VsBot)
				{
					if (cureentTurn > 0)
						return pl[cureentTurn - 1];
					else return pl[pl.size() - 1];
				}
			}
			return pl[pl.size() - 1];
		}
	}
	TCHAR* GetCurrentPlayerName() {
		return (*(pl.begin() + cureentTurn))->name;
	}
	TCHAR* GetPrevPlayerName(){
		if (!direction)
			return (*(pl.begin() + 1))->name;
		else return (*(pl.begin() + (pl.size() - 1)))->name;
	}

	void NextPlayer()
	{
		if (!VsBot) {
			if (direction) {
				Player* p = new Player;
				pl.push_back(p);
				(*(pl.begin()))->PlayerCopy((*p));
				pl.erase(pl.begin());
			}
			else {
				Player* p = new Player;
				pl.insert(pl.begin(), p);
				(*(pl.begin() + (pl.size() - 1)))->PlayerCopy((*p));
				pl.erase(pl.begin() + (pl.size() - 1));
			}
		}
		else {
			if (direction) {
				if (cureentTurn < pl.size() - 1)
					cureentTurn++;
				else cureentTurn = 0;
			}
			else {
				if (cureentTurn > 0)
					cureentTurn--;
				else cureentTurn = pl.size() - 1;
			}
		}
		if (pl[cureentTurn]->size == 1) pl[cureentTurn]->SayUno();
	}

	void CheckScores()
	{
		for (size_t i = 0; i < pl.size(); i++)
			(*(pl.begin() + i))->CheckScore();
	}
	void SetEndRoundScores(){
		size_t scoreWin = 0;
		int n = 0;
		for (size_t i = 0; i < pl.size(); i++){
			if (pl[i]->size == 0)
				n = i;
			else {
				scoreWin += pl[i]->GetScore();
				pl[i]->SetScore(0);
			}
		}
		pl[n]->SetScore(scoreWin);
	}

	int DeckChecked(WindowDraw & wd)
	{
		if (!IsPlayerHasNeededCard((pl[cureentTurn]), specCard))
		{
			if (pl[cureentTurn]->size == 1)
				pl[cureentTurn]->UnoNormal();
			GiveOutCard(pl[cureentTurn]);
			CheckScores();
			if (pl[cureentTurn]->noBot)
				wd.UpdateDetails(pl, direction, cureentTurn, cards, checkColor, currentColor);
			if (!IsPlayerHasNeededCard((*(pl.begin())), specCard)){
				return 1;
			}
			else return 0;
		}
		return -1;
	}

	BOOL IsPlayerHasNeededCard(Player * pl, BOOL sCard)
	{
		for (size_t i = 0; i < pl->size; i++)
		{
			if (IsCorrectCard(pl, pl->color[i], pl->type[i], sCard))
				return TRUE;
		}
		return FALSE;
	}

	void OutOfMenuPause(WindowDraw & wd){
		wd.DestroyPauseMenu();
		OutOfPause(wd);
	}
	void OutOfPause(WindowDraw & wd){
		wd.DrawGameField(pl, VsBot, currentColor);
		wd.UpdateDetails(pl, direction, cureentTurn, cards, checkColor, currentColor);
	}
	void OutOfPlPause(WindowDraw & wd, BOOL isItStart)
	{
		wd.DestroyPlPause(isItStart);
		OutOfPause(wd);
	}

	void SetPlayersNames(vector<HWND> &plnames) {
		DeletePlayersNames();

		for (size_t i = 0; i < pl.size() && i < plnames.size(); i++)
		{
			TCHAR * t = new TCHAR[13];
			GetWindowText(plnames[i], t, 13);
			(*(pl.begin() + i))->SetName(t);
			int l = _tcslen((*(pl.begin() + i))->GetName()) + 1;
			TCHAR* tmp = new TCHAR[l];
			for (int j = 0; j < l + 1; j++)
				tmp[j] = t[j];
			playersNames.push_back(t);
		}
	}
	void DeletePlayersNames(bool end = false)
	{
		if (!end){
			for (auto it : playersNames)
				delete[] it;
		}
		if (playersNames.size() > 0)
			playersNames.erase(playersNames.begin(), playersNames.begin() + playersNames.size());
	}
	void ClearPlayersNames()
	{
		for (auto it : pl)
			it->DelName();
	}
	BOOL IsCorrectNames(vector<HWND> &plnames) {
		TCHAR** tmp = new TCHAR*[plnames.size()];
		for (size_t i = 0; i < pl.size() && i < plnames.size(); i++)
		{
			size_t l = GetWindowTextLength(plnames[i]);
			if (l < 2){
				delete[] tmp;
				return FALSE;
			}
			TCHAR * t = new TCHAR[l + 1];
			GetWindowText(plnames[i], t, 12);
			if (i != 0){
				for (size_t j = 0; j < i; j++){
					if (_tcscmp(t, tmp[j]) == 0)
					{
						delete[] tmp;
						delete[] t;
						return FALSE;
					}
				}
			}
			tmp[i] = t;
		}
		delete[] tmp;
		return TRUE;
	}

	int WinnerScores(){
		int wS = 0;
		for (size_t i = 0; i < pl.size(); i++) {
			wS += pl[i]->GetScore();
		}
		return wS;
	}
	Player& Winner(){
		for (size_t i = 0; i < pl.size(); i++)
			if (pl[i]->GetScore() == 0)
				return (*pl[i]);
	}
	void AddRoundTable(){
		if (roundTable.size() == 0) {
			for (size_t i = 0; i < pl.size(); i++){
				vector<int> tmp(6);
				for (size_t j = 0; j < tmp.size(); j++){
					tmp[j] = -1;
				}
				roundTable.push_back(tmp);
			}
		}
		for (size_t i = 0; i < pl.size(); i++){
			for (size_t j = 0; j < pl.size(); j++){
				if (_tcscmp(pl[i]->GetName(), playersNames[j]) == 0)
				{
					roundTable[j][Games - 1] = pl[i]->GetScore();
					if (Games == 5){
						pl[i]->ClearForNextRound();
						roundTable[j][Games] = pl[i]->GetAllScore();
					}
					break;
				}
			}
		}
	}
	void DeleteRoudTable(){
		if (roundTable.size() > 0)
			roundTable.erase(roundTable.begin(), roundTable.begin() + roundTable.size());
	}

	void ResetMessage(TCHAR* mes = NULL, TCHAR* name = NULL){
		if (mes == NULL) {
			delete[] mesageAboutPlayers;
			mesageAboutPlayers = NULL;
		}
		else {
			delete[] mesageAboutPlayers;
			mesageAboutPlayers = new TCHAR[_tcslen(mes) + _tcslen(name) + 1];
			mesageAboutPlayers[0] = '\0';
			_tcscat(mesageAboutPlayers, name);
			_tcscat(mesageAboutPlayers, mes);
		}
	}
	TCHAR* AddToPauseMessage(TCHAR* pMes){
		if (mesageAboutPlayers != NULL) {
			TCHAR* buf = new TCHAR[_tcslen(mesageAboutPlayers) + _tcslen(pMes) + 1];
			buf[0] = '\0';
			_tcscat(buf, mesageAboutPlayers);
			_tcscat(buf, pMes);
			delete[] mesageAboutPlayers;
			mesageAboutPlayers = NULL;
			return buf;
		}
		return pMes;
	}

	void SetUno(WindowDraw & wd, bool say) //сказал уно или не успел
	{
		if (!playerGo){
			for (size_t i = 0; i < pl.size(); i++)
			{
				if (i != cureentTurn && pl[i]->ForgotUno()){
					SayUno();
					Update(wd);
					return;
				}
			}
		}
		else {
			if (say)
				SayUno();
			else pl[cureentTurn]->DidNotSayUno();
			needUno = FALSE;

			TCHAR * tmpn = new TCHAR[_tcslen(GetCurrentPlayerName()) + 1];
			_tcscpy(tmpn, GetCurrentPlayerName());
			UpdateAfterMove(wd);
			CheckToSpecCardEffect(wd);
			TCHAR* buf = NULL;
			if (mesageAboutPlayers == NULL) {
				buf = new TCHAR[_tcslen(tmpn) + 37 + 1];
				buf[0] = '\0';
			}
			else {
				if (say)
					buf = new TCHAR[_tcslen(mesageAboutPlayers) + _tcslen(tmpn) + 14 + 1];
				else buf = new TCHAR[_tcslen(mesageAboutPlayers) + _tcslen(tmpn) + 21 + 1];
				buf[0] = '\0';
				_tcscat(buf, mesageAboutPlayers);
			}
			if (say)
				ResetMessage(TEXT(" нажал \"UNO\".\n"), tmpn);
			else ResetMessage(TEXT(" забыл нажать \"UNO\".\n"), tmpn);
			_tcscat(buf, mesageAboutPlayers);
			ResetMessage(TEXT(""), buf);
			if (!needUno) {
				playerGo = FALSE;
				if (!CheckToEnd(wd)) {
					if (!VsBot){
						wd.DrawPlPause(NULL, GetCurrentPlayerName(), mesageAboutPlayers, 0, FALSE); //пауза после нажатия уно игроком
						ResetMessage();
					}
					else {
						if (IsNowBot()){
							botGo = TRUE;
							nowTimer = true;
							SetTimer(hwnd, 1, 1000, NULL);
						}
						else {
							botGo = FALSE;
							Update(wd);
						}
					}
				}
			}
		}
	}
	void SayUno(){
		if (pl[cureentTurn]->size == 1)
			pl[cureentTurn]->SayUno();
		for (size_t i = 0; i < pl.size(); i++){
			if (i != cureentTurn){
				if (pl[i]->GetUno() == 0 || (pl[i]->GetUno() == 1 && pl[i]->size != 1)) {
					if (pl[i]->GetUno() == 0){ //добавить 2 карты игроку
						GiveOutCard(pl[i]);
						GiveOutCard(pl[i]);
						ResetMessage(TEXT(" берет 2 карты.\n"), pl[cureentTurn]->name);
					}
					pl[i]->UnoNormal(); //вернуть как обычно
				}
			}
		}
	}
	void BotSaidUno(WindowDraw & wd){
		int u = rand() % 2;
		if (u == 1){
			pl[cureentTurn]->SayUno();
			ResetMessage(TEXT(" нажал \"UNO\".\n"), pl[cureentTurn]->name);
		}
		else {
			pl[cureentTurn]->DidNotSayUno();
			ResetMessage(TEXT(" забыл нажать \"UNO\".\n"), pl[cureentTurn]->name);
		}
		wd.MessageAboutPlayers(NULL, GetCurrentPlayerName(), mesageAboutPlayers);
		//ResetMessage();
	}

	BOOL IsEndRound() {
		for (auto it : pl)
			if (it->size == 0) return TRUE;
		return FALSE;
	}
	BOOL CheckToEnd(WindowDraw & wd) { //проверка количества карт на завершение игры
		if (IsEndRound()){
			if (VsBot)
				wd.DestroyMessageAboutPlayers();
			Games++;
			EndRound(wd);
			wd.DrawRoundEndPause(pl, Games, roundTable, playersNames);
			return TRUE;
		}
		return FALSE;
	}

	INT GetCardsCount(size_t nowpl = 0) const
	{
		return pl[nowpl]->size;
	}
	BOOL IsNowBot(){
		return !pl[cureentTurn]->noBot;
	}
	size_t GetPlayerCount(){
		return pl.size();
	}
	vector<TCHAR*> GetPlayerNames() const
	{
		return playersNames;
	}
	vector<int> GetPlayerScores() const
	{
		vector<int> tmp;
		for (auto it : roundTable)
			tmp.push_back(it[5]);
		return tmp;
	}

	void PlayersToTable(){
		for (size_t i = 0; i < pl.size() - 1; i++)
		{
			for (size_t j = i + 1; j < pl.size(); j++)
			{
				if (roundTable[i][5] < roundTable[j][5])
				{
					TCHAR* tmp = new TCHAR[_tcslen(playersNames[i]) + 1];
					for (size_t x = 0; x < _tcslen(playersNames[i]) + 1; x++)
						tmp[x] = playersNames[i][x];
					delete[] playersNames[i];
					playersNames[i] = new TCHAR[_tcslen(playersNames[j]) + 1];
					for (size_t x = 0; x < _tcslen(playersNames[j]) + 1; x++)
						playersNames[i][x] = playersNames[j][x];
					delete[] playersNames[j];
					playersNames[j] = new TCHAR[_tcslen(tmp) + 1];
					for (size_t x = 0; x < _tcslen(tmp) + 1; x++)
						playersNames[j][x] = tmp[x];

					vector<int> t = roundTable[i];
					roundTable[i] = roundTable[j];
					roundTable[j] = t;
				}
			}
		}
	}
	void NextRandomRound(WindowDraw & wd){
		Games++;
		int n = rand() % pl.size();
		pl[n]->SetScore(rand() % 190 + 10);
		for (size_t j = 0; j < pl.size(); j++){
			if (j != n)
				pl[j]->SetScore(0);
		}
		AddRoundTable();
		for (auto it : pl)
			it->ClearForNextRound();
		wd.DestroyRoundEndPause();
		wd.DrawRoundEndPause(pl, Games, roundTable, playersNames);
	}
	void EndNowRound(WindowDraw & wd){
		int r = rand() % pl.size();
		while (pl[r]->color.size() != 0)
			pl[r]->DelCard(0);
		CheckToEnd(wd);
	}
};

