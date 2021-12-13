#pragma once
#include "Header.h"

class Player
{
	size_t score = 0; 
	size_t scoreAll = 0; 
	int uno = -1; //-1 - не использовалось, 0 - забыл сказать, 1 - сказал
	BOOL skipStroke = FALSE; //пропуск хода
public:
	TCHAR* name = NULL;
	vector<int> color; //цвет, +4 или поменять цвет //0, 1, 2, 3
	vector<int> type; //номер или тип
	size_t size = 0; //кол-во карт
	bool noBot = false;

	void PlayerCopy(Player & pl)
	{
		pl.score = score;
		pl.scoreAll = scoreAll;
		pl.uno = uno;
		pl.skipStroke = skipStroke;
		pl.SetName(name);
		pl.color = color;
		pl.type = type;
		pl.size = size;
		pl.noBot = noBot;
	}

	void SetName(TCHAR * n)
	{
		name = new TCHAR[_tcslen(n) + 1];
		_tcscpy(name, n);
	}

	TCHAR* GetName() const
	{
		return name;
	}

	void DelName()
	{
		delete[] name;
		name = NULL;
	}

	void AddCard(int newcolor, int nums)
	{
	color.push_back(newcolor);
		type.push_back(nums);
		size++;
	}

	void SwapCard(size_t num1, size_t num2)
	{
		int i = *(color.begin() + num2);
		int j = *(type.begin() + num2);
		*(color.begin() + num2) = *(color.begin() + num1);
		*(type.begin() + num2) = *(type.begin() + num1);
		*(color.begin() + num1) = i;
		*(type.begin() + num1) = j;
	}

	void DelCard(size_t num)
	{
		color.erase(color.begin() + num);
		type.erase(type.begin() + num);
		size--;
	}

	void DelCard(int newcolor, int nums)
	{
		for (size_t i = 0; i < size; i++)
		{
			if ((*(color.begin() + i)) == newcolor && (*(type.begin() + i)) == nums)
			{
				color.erase(color.begin() + i);
				type.erase(type.begin() + i);
				size--;
			}
		}
	}

	void CheckScore()
	{
		score = 0;
		for (size_t i = 0; i < color.size(); i++)
		{
			score += GetOneScore(color[i], type[i]);
		}
	}

	int GetOneScore(int c1, int n1) //очки карты
	{
		if ((c1 >= 0 && c1 <= 3) && (n1 >= 0 && n1 <= 9))
			return n1;
		else if (c1 == 4 || c1 == 5)
			return 50;
		else return 20;
	}

	void AddAllScore()
	{
		scoreAll += score;
	}

	void SetScore(size_t s){
		score = s;
	}
	int GetScore() const
	{
		return score;
	}
	int GetAllScore() const{
		return scoreAll;
	}

	int GetColor(size_t num)
	{
		return *(color.begin() + num);
	}
	int GetType(size_t num)
	{
		return *(type.begin() + num);
	}

	void SayUno()
	{
		uno = 1;
	}
	void DidNotSayUno()
	{
		uno = 0;
	}
	void UnoNormal()
	{
		uno = -1;
	}

	BOOL ForgotUno()
	{
		if (uno == 0) return TRUE;
		return FALSE;
	}

	int GetUno() const {
		return uno;
	}

	void ClearAll()
	{
		score = 0;
		scoreAll = 0;
		uno = -1;
		skipStroke = FALSE;
		if (color.size() > 0)
		{
			color.clear();
			type.clear();
		}
		size = 0;
		delete[] name;
		name = NULL;
		noBot = false;
	}
	void ClearForNextRound(){
		AddAllScore();
		score = 0;
		uno = -1;
		skipStroke = FALSE;
		if (color.size() > 0)
		{
			color.clear();
			type.clear();
		}
		size = 0;
	}
};