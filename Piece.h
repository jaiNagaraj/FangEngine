#pragma once
#include <iostream>
using namespace std;
class Piece
{
	private:
		pair<int,int> pos;
		string type; // type of piece
		int side; // 0 for white, 1 for black
	public:
		virtual void move() = 0;
		Piece();
		~Piece();
};

