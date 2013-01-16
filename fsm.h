#ifndef _FSM_H_
#define _FSM_H_
#include "general.h"
#include "chess.h"

enum STATE{
	INTRO,
	COLOR_SELECT,
	GAME,
	END
};

Piece *curPiece;

class FSM {
	private:
		int state, curTurn;
	public:
		FSM() {
			state = INTRO;
		}
		~FSM() {}

		void turnStuff(bool turn) {
			struct coord *pos = new coord(-1, -1);
			bool done = false;
			bool pieceSelected = false;
			bool playerTurn = false;
			int minPiece = PAWN;
			int maxPiece = KNIGHT;
			vector<coord> moves;
			while (!done)
			{
				clearBox(220, 25, 360, 65);
				if (turn) {
					makeText("White's move!", font, textC, 220, 25);
					if (w.isPlayer) {
						playerTurn = true;
					}
				}
				else {
					makeText("Black's move!", font, textC, 220, 25);
					minPiece = BPAWN;
					maxPiece = BKNIGHT;
				}
				refresh(screen);
				while (((pos->x < 0 || pos->y < 0) ||
					(pos->x > 7 || pos->y > 7)))
				{
					click = getClick();
					*pos = getClickOnBoard(click);
				}
				std::cout << pos->x << "\t" << pos->y << endl;
				if (board[pos->x][pos->y] >= minPiece && board[pos->x][pos->y] <= maxPiece)
				{
					pieceSelected = true;
					draw_board();
					drawPieces();
					vector<Piece> wPieces = w.getPieceList();
					for (vector<Piece>::iterator it = wPieces.begin();
							it < wPieces.end();
							it++)
					{
						if (pos->x == it->x && pos->y == it->y)
						{
							curPiece = &(*it);
							std::cout << "rawr" << endl;
							it->findValidMoves();
							int moveListSize = it->validMoves.size();
							moves = it->validMoves;
							std::cout << "piece: " << it->type << endl;
							std::cout << "size: " << moveListSize << endl;
							if (moveListSize > 0)
							{
								std::cout << "\tlen: " << moveListSize << endl;
								//std::cin.get();
								int n = 0;
								for (vector<coord>::iterator it2 = moves.begin();
										it2 < moves.end();
										it2++)
								{
									makeText("X",pieces_font,pieceC3,  xDefault + (it2->x) * 40, yDefault + (it2->y) * 40);
									std::cout << "\t\tdrawing move " << n << "\t";
									it2->showCoord();
									n++;
									std::cout << " out of " << moveListSize << " possible moves " << endl;
									if (n >= moveListSize) {
										it2 = moves.end();
									}
								}
							}
							//std::cin.get();
							//std::cout << "end of moves" << endl;
							break;
						}
					}
				//std::cout << "end of piece list" << endl;
				}
				else {
					int xp = pos->x, yp = pos->y;
					//rad formatting
					std::cout << "move = [";
					curPiece->dispPieceCoord();
					std::cout << "->" << "(" << xp << "," << yp << ")\n";
					for (vector<coord>::iterator it = moves.begin(); it < moves.end(); it++)
					{
						if (it->x == xp && it->y == yp)
						{
							std::cout << "VALID MOVE" << endl;
							placePieceAt(curPiece->x, curPiece->y, EMPTY);
							std::cout << "HELLO" << endl;
							(&(*curPiece))->setPos(xp, yp);
							board[xp][yp] = (&(*curPiece))->type;
							/*vector<Piece> w_pieces = w.getPieceList();
							for (vector<Piece>::iterator iz = w_pieces.begin(); iz < w_pieces.end(); iz++) {
								if (&(*curPiece) == &(*iz)) {
									std::cout << "BIZZAM HELLO I AM HERE" << endl;
									break;
								}
							}*/
							std::cout << "HELLO" << endl;
							done = true;
							turn = !turn;
							(&(*curPiece))->dispPieceCoord();
							std::cout << endl;
							showBoard();
							pieceSelected = false;
							break;
						}
					}
				}
				/*else {
					//HOW I AI
				}*/
				refresh(screen);
				delete pos;
				pos = new coord(-1, -1);
			}
		}

		void gameLoop() {
			bool quit = false;
			while (quit != true) {
				switch(state) {
					case INTRO:
						//introAnimation();
						defaultBoard();
						state = COLOR_SELECT;
						break;
					case COLOR_SELECT:
						makeText("WHITE    BLACK", font, textC, 100, 25);
						boxArea(90, 20, 190, 80);
						boxArea(340, 20, 190, 80);
						refresh(screen);
						click = getClick();
						while (!click.isInBox(90, 20, 190, 80) && !click.isInBox(340, 20, 190, 80)) {
							std::string x = to_string(click.x), y = to_string(click.y);
							std::string temp = "click: ";
							temp += x;
							temp += ",";
							temp += y;
							if (click.x != 0 && click.y != 0) {
								clearBox(0, SCRN_HEIGHT/2, 4000, 80);
								makeText(temp, font, textC, 0, SCRN_HEIGHT/2);
								refresh(screen);
							}
							click = getClick();
						}
						if (click.isInBox(90, 20, 190, 80)) {
							w.isPlayer = true;
							b.isPlayer = false;
						}
						else {
							w.isPlayer = false;
							b.isPlayer = true;
						}
						b.initPlayer();
						w.initPlayer();
						state = GAME;
						curTurn = WHITE;
						clearScreen();
						break;
					case GAME:
					{
						/*w.unitTestWhite(BISHOP);
						w.unitTestWhite(BPAWN, 3, 3);
						w.unitTestWhite(BPAWN, 2, 2);
						w.unitTestWhite(PAWN, 3, 5);
						w.unitTestWhite(BPAWN, 2, 6);*/
						getCurrentBoardState();
						draw_board();
						drawPieces();
						showBoard();
						printBoardState();
						turnStuff(curTurn);
						std::cout << "WHABAM" << endl;
						showBoard();
						//put turn stuff here
					}
					break;
				}
			}
		}
};

FSM fsm;

#endif
