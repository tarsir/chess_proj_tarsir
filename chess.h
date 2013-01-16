#ifndef _CHESS_H_
#define _CHESS_H_
#include "general.h"
#include <vector>

using std::vector;

int board[8][8] = {{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				{0,0,0,0, 0,0,0,0},
				};

enum COLOR {
	BLACK,
	WHITE
};

enum Pieces {
	PAWN,
	KING,
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT,
	BPAWN,
	BKING,
	BQUEEN,
	BROOK,
	BBISHOP,
	BKNIGHT,
	EMPTY
};

bool verifyMove(struct coord* &move) {
	if (move->x > 7 || move->x < 0 || move->y > 7 || move->y < 0) {
		return false;
	}
	else if (board[move->x][move->y] != EMPTY) {
		return false;
	}
	else return true;
}

int getColor(int ptype) {
	if (ptype >= PAWN && ptype < BPAWN) {
		return WHITE;
	}
	else if (ptype >= BPAWN && ptype < EMPTY) {
		return BLACK;
	}
	else return -1;
}

struct Piece {
	Pieces type;
	vector<coord> validMoves;
	int x, y; //the position on the board

	Piece(Pieces _type) {
		type = _type;
	}

	void setPos(int _x, int _y) {
		x = _x;
		y = _y;
	}

	struct coord* getPos() {
		struct coord *res = new coord(this->x, this->y);
		return res;
	}

	void findValidMoves() {
		this->validMoves.clear();
		switch(this->type) {
			case PAWN:case BPAWN:
			{
				int add = -1;
				if (this->type == BPAWN) {
					add = 1;
				}
				struct coord *p = new coord(this->x, this->y+add);
				if (verifyMove(p))
					this->validMoves.push_back(*p);
				if ((this->y == 6 && this->type == PAWN) || (this->y == 1 && this->type == BPAWN)) {
					struct coord *p2 = new coord(this->x, this->y+(add * 2));
					if (verifyMove(p2))
						this->validMoves.push_back(*p2);
					delete p2;
				}

				delete p;
				//will also eventually need logic for en passant moves, but maybe not here
				break;
			}
			case KING:case BKING:
			{
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i != 0 || j != 0) {
							struct coord *p = new coord(i + this->x, j + this->y);
							if (verifyMove(p))
								if (getColor(this->type) != getColor(board[p->x][p->y]) &&
												getColor(board[p->x][p->y] != -1))
									this->validMoves.push_back(*p);
							delete p;
						}
					}
				}
				break;
			}
			case QUEEN:case BQUEEN:
			{
				bool b[] = {true, true, true, true, true, true, true, true};
				for (int i = 1; i < 8; i++) {
					struct coord *p1, *p2,*p3,*p4,*p5,*p6,*p7,*p8;
					p1 = new coord(i + this->x, this->y);
					p2 = new coord(abs(i - this->x), this->y);
					p3 = new coord(i + this->x, i + this->y);
					p4 = new coord(i + this->x, abs(i - this->y));
					p5 = new coord(abs(i - this->x), i + this->y);
					p6 = new coord(abs(i - this->x), abs(i - this->y));
					p7 = new coord(this->x, i + this->y);
					p8 = new coord(this->x, abs(i - this->y));
					vector<coord> possMoves;
					possMoves.push_back(*p1);
					possMoves.push_back(*p2);
					possMoves.push_back(*p3);
					possMoves.push_back(*p4);
					possMoves.push_back(*p5);
					possMoves.push_back(*p6);
					possMoves.push_back(*p7);
					possMoves.push_back(*p8);

					int count = 0;
					for (vector<coord>::iterator it = possMoves.begin(); it < possMoves.end() ; it++)
					{
						struct coord* move;
						move = &(*it);
						if (verifyMove(move) && b[count]) {
							this->validMoves.push_back(*move);
							move->showCoord();
						}
						else if (b[count]) {
							b[count] = false;
							if (board[move->x][move->y] != EMPTY && move->x <=  7 && move->y <= 7)
							{
								if (getColor(this->type) != getColor(board[move->x][move->y]) &&
											getColor(board[move->x][move->y] != -1)) {
									std::cout << "piece1 color: " << getColor(this->type) << "\t";
									std::cout << "piece2 color: " << getColor(board[move->x][move->y]) << endl;
									this->validMoves.push_back(*move);
									move->showCoord();
								}
							}
						}
						count++;
					}

					delete p1;
					delete p2;
					delete p3;
					delete p4;
					delete p5;
					delete p6;
					delete p7;
					delete p8;
					//std::cout << this->validMoves.size() << endl;
				}
				break;
			}
			case ROOK: case BROOK:
			{
				bool b[] = {true, true, true, true};
				for (int i = 1; i < 8; i++) {

					std::cout << "ROOK IN" << endl;
					struct coord*p1,*p2,*p3,*p4;
					p1 = new coord(i + this->x, this->y);
					p2 = new coord(abs(i - this->x), this->y);
					p3 = new coord(this->x, i + this->y);
					p4 = new coord(this->x, abs(i - this->y));
					vector<coord> possMoves;
					possMoves.push_back(*p1);
					possMoves.push_back(*p2);
					possMoves.push_back(*p3);
					possMoves.push_back(*p4);

					int count = 0;
					for (vector<coord>::iterator it = possMoves.begin(); it < possMoves.end() ; it++)
					{
						struct coord* move;
						move = &(*it);
						if (verifyMove(move) && b[count]) {
							this->validMoves.push_back(*move);
							move->showCoord();
						}
						else if (b[count]) {
							b[count] = false;
							if (board[move->x][move->y] != EMPTY && move->x <=  7 && move->y <= 7)
							{
								if (getColor(this->type) != getColor(board[move->x][move->y]) &&
											getColor(board[move->x][move->y] != -1)) {
									std::cout << "piece1 color: " << getColor(this->type) << "\t";
									std::cout << "piece2 color: " << getColor(board[move->x][move->y]) << endl;
									this->validMoves.push_back(*move);
									move->showCoord();
								}
							}
						}
						count++;
					}

					delete p1;
					delete p2;
					delete p3;
					delete p4;
				}
				break;
			}
			case BISHOP: case BBISHOP:
			{
				bool b[] = {true, true, true, true};
				std::cout << "b1 = " << b[0] << endl;
				for (int i = 1; i < 8; i++) {
					struct coord*p1,*p2,*p3,*p4;
					p1 = new coord(i + this->x, i + this->y);
					p2 = new coord(i + this->x, abs(i - this->y));
					p3 = new coord(abs(i - this->x), i + this->y);
					p4 = new coord(abs(i - this->x), abs(i - this->y));
					vector<coord> possMoves;
					possMoves.push_back(*p1);
					possMoves.push_back(*p2);
					possMoves.push_back(*p3);
					possMoves.push_back(*p4);

					std::cout << "b1x = " << b[0] << endl;
					int count = 0;
					for (vector<coord>::iterator it = possMoves.begin(); it < possMoves.end() ; it++)
					{
						struct coord* move;
						move = &(*it);
						if (verifyMove(move) && b[count]) {
							this->validMoves.push_back(*move);
							move->showCoord();
						}
						else if (b[count]) {
							b[count] = false;
							if (board[move->x][move->y] != EMPTY && move->x <=  7 && move->y <= 7)
							{
								if (getColor(this->type) != getColor(board[move->x][move->y]) &&
											getColor(board[move->x][move->y] != -1)) {
									std::cout << "piece1 color: " << getColor(this->type) << "\t";
									std::cout << "piece2 color: " << getColor(board[move->x][move->y]) << endl;
									this->validMoves.push_back(*move);
									move->showCoord();
								}
							}
						}
						count++;
					}

					delete p1;
					delete p2;
					delete p3;
					delete p4;
				}
				break;
			}
			case KNIGHT: case BKNIGHT:
			{
				for (int i = -2; i <= 2; i++) {
					for (int j = -2; j <= 2; j++) {
						if (abs(i) != abs(j) && (j != 0 && i != 0) && (i % 2 == 0 || j % 2 == 0)) {
							struct coord *p1;
							p1 = new coord(i + this->x, j + this->y);
							if (verifyMove(p1))
								this->validMoves.push_back(*p1);
							delete p1;
						}
					}
				}
				break;
			}
		}
	}

	void dispPieceCoord()
	{
		std::cout << "(" << this->x << ", " << this->y << ")";
	}
};


class Player {
	private:
		bool color; //true = white, false = black
		vector<Piece> pieces;
	public:
		bool isPlayer;
		Player(bool isWhite) {
			color = isWhite;
			isPlayer = false;
		}
		void initPlayer() {
			struct Piece k = Piece(KING);
			struct Piece q = Piece(QUEEN);
			struct Piece b1 = Piece(BISHOP);
			struct Piece b2 = Piece(BISHOP);
			struct Piece n1 = Piece(KNIGHT);
			struct Piece n2 = Piece(KNIGHT);
			struct Piece r1 = Piece(ROOK);
			struct Piece r2 = Piece(ROOK);
			struct Piece p1 = Piece(PAWN);
			struct Piece p2 = Piece(PAWN);
			struct Piece p3 = Piece(PAWN);
			struct Piece p4 = Piece(PAWN);
			struct Piece p5 = Piece(PAWN);
			struct Piece p6 = Piece(PAWN);
			struct Piece p7 = Piece(PAWN);
			struct Piece p8 = Piece(PAWN);
			if (color) {
				k.setPos(4, 7);
				q.setPos(3, 7);
				b1.setPos(2, 7);
				n1.setPos(1, 7);
				r1.setPos(0, 7);
				r2.setPos(7, 7);
				n2.setPos(6, 7);
				b2.setPos(5, 7);
				p1.setPos(0, 6);
				p2.setPos(1, 6);
				p3.setPos(2, 6);
				p4.setPos(3, 6);
				p5.setPos(4, 6);
				p6.setPos(5, 6);
				p7.setPos(6, 6);
				p8.setPos(7, 6);
			}
			else {
				k.setPos(4, 0);
				q.setPos(3, 0);
				b1.setPos(2, 0);
				n1.setPos(1, 0);
				r1.setPos(0, 0);
				r2.setPos(7, 0);
				n2.setPos(6, 0);
				b2.setPos(5, 0);
				p1.setPos(0, 1);
				p2.setPos(1, 1);
				p3.setPos(2, 1);
				p4.setPos(3, 1);
				p5.setPos(4, 1);
				p6.setPos(5, 1);
				p7.setPos(6, 1);
				p8.setPos(7, 1);
			}
			pieces.push_back(k);
			pieces.push_back(q);
			pieces.push_back(r1);
			pieces.push_back(r2);
			pieces.push_back(b1);
			pieces.push_back(b2);
			pieces.push_back(n1);
			pieces.push_back(n2);
			pieces.push_back(p1);
			pieces.push_back(p2);
			pieces.push_back(p3);
			pieces.push_back(p4);
			pieces.push_back(p5);
			pieces.push_back(p6);
			pieces.push_back(p7);
			pieces.push_back(p8);
		}
		vector<Piece> getPieceList() {
			return this->pieces;
		}

		void unitTestWhite(Pieces x, int x_ = 4, int y = 4) {
			if (x_ > 7 || y > 7) {
				std::cout << "CAN'T DO THAT" << endl;
				return;
			}
			struct Piece *unittest = new Piece(x);
			unittest->setPos(x_, y);
			pieces.push_back(*unittest);
			//std::cout << "test type: " << unittest->type << endl;
			//std::cout << "test2: " << pieces.begin()->type << endl;
			delete unittest;
			//std::cout << "test3: " << pieces.begin()->type << endl;
			//std::cout << pieces.size() << endl;
		}
};

Player w = Player(WHITE);
Player b = Player(BLACK);

int xDefault = SCRN_WIDTH/2 - 154;
int yDefault = SCRN_HEIGHT/2-167;

void placePieceAt(int x, int y, int piece) {
	board[x][y] = piece;
}

void draw_board(int x0 = SCRN_WIDTH/2-160, int y0=SCRN_HEIGHT/2-160) {
	int x = x0, y = y0;
	bool iswhite = true;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (iswhite) {
				apply_surface(x, y, white, screen);
			}
			else {
				apply_surface(x, y, black, screen);
			}
			iswhite = !iswhite;
			x += 40;
		}
		iswhite = !iswhite;
		y += 40;
		x = x0;
	}

	x = x0-5, y = y0-5;
	for (int xf = 0; xf < 326; xf+= 5) {
		for (int yf = 0; yf < 326; yf+= 5) {
			if (xf % 325 == 0 || yf % 325 == 0)
				apply_surface(x + xf, y + yf, tile, screen);
		}
	}
}

void showBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			switch (board[j][i]) {
				case KING:case BKING:
					std::cout << "K";
					break;
				case QUEEN:case BQUEEN:
					std::cout << "Q";
					break;
				case ROOK:case BROOK:
					std::cout << "R";
					break;
				case BISHOP:case BBISHOP:
					std::cout << "B";
					break;
				case KNIGHT:case BKNIGHT:
					std::cout << "N";
					break;
				case PAWN:case BPAWN:
					std::cout << "P";
					break;
				case EMPTY:
					std::cout << "X";
					break;
			}
			std::cout << "\t";
		}
		std::cout << endl;
	}
}

void drawPieces() {
	SDL_Color thispiece;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i] >= BPAWN) {
				thispiece = pieceC2;
			}
			else { thispiece = pieceC1; }
			switch(board[j][i]) {
				case KING:case BKING:
					makeText("K",pieces_font,thispiece, xDefault + j * 40, yDefault + i * 40);
					break;
				case QUEEN:case BQUEEN:
					makeText("Q",pieces_font,thispiece, xDefault + j * 40, yDefault + i * 40);
					break;
				case BISHOP:case BBISHOP:
					makeText("B",pieces_font,thispiece, xDefault + j * 40, yDefault + i * 40);
					break;
				case KNIGHT:case BKNIGHT:
					makeText("N",pieces_font,thispiece, xDefault + j * 40, yDefault + i * 40);
					break;
				case PAWN:case BPAWN:
					makeText("P",pieces_font,thispiece, xDefault + j * 40, yDefault + i * 40);
					break;
				case ROOK:case BROOK:
					makeText("R",pieces_font,thispiece, xDefault + j * 40, yDefault + i * 40);
					break;
			}
		}
	}
}

void randomizeBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[j][i] = rand()%14;
		}
	}
}

void defaultBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[j][i] = EMPTY;
		}
	}
}

void initializeBoard() {
	defaultBoard();
	board[0][2] = board[7][5] = ROOK;
	board[1][2] = board[6][5] = BISHOP;
	board[2][2] = board[5][5] = KNIGHT;
	board[3][2] = QUEEN;
	board[4][2] = KING;
}


struct coord getClickOnBoard(struct coord inp) {
	struct coord result;
	if ((inp.x < SCRN_WIDTH/2 - 160 && inp.x > SCRN_WIDTH/2+160) ||
		(inp.y < SCRN_HEIGHT/2 - 160 && inp.y > SCRN_WIDTH/2 + 160)) {
			result.x = result.y = -1;
	}
	else {
		result.x = (inp.x - (SCRN_WIDTH/2 - 160))/40;
		result.y = (inp.y - (SCRN_HEIGHT/2 - 160))/40;
	}
	return result;
}

void getCurrentBoardState() {
	defaultBoard();
	vector<Piece> w_piece = w.getPieceList();
	std::cout << w_piece.size();
	vector<Piece> b_piece = b.getPieceList();
	for (vector<Piece>::iterator it = w_piece.begin(); it < w_piece.end(); it++) {
		int tempx = it->x, tempy = it->y;
		board[tempx][tempy] = it->type;
	}
	for (vector<Piece>::iterator it = b_piece.begin(); it < b_piece.end(); it++) {
		int tempx = it->x, tempy = it->y;
		board[tempx][tempy] = it->type + 6;
	}
}

void printBoardState() {
	vector<Piece> w_piece = w.getPieceList();
	std::cout << w_piece.size();
	vector<Piece> b_piece = b.getPieceList();
	for (vector<Piece>::iterator it = w_piece.begin(); it < w_piece.end(); it++) {
		std::cout << it->type << "\t";
		it->dispPieceCoord();
		std::cout << endl;
	}
	for (vector<Piece>::iterator it = b_piece.begin(); it < b_piece.end(); it++) {
		std::cout << it->type+6 << "\t";
		it->dispPieceCoord();
		std::cout << endl;
	}
}

#endif
