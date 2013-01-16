#include "general.h"
#include "strings.h"
#include "chess.h"
#include "fsm.h"

int main(int argc, char** argv) {
	srand(time(NULL));
    startup();
    refresh(screen);

	fsm.gameLoop();
    /*while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
            	if (event.button.button == SDL_BUTTON_LEFT) {
					struct coord click;
					click.x = event.button.x;
					click.y = event.button.y;

					quit = true;
            	}
            }
        }
        refresh(screen);
        SDL_Delay(100);
    }
    //makeText("HELLO WORLD", font, pieceC);
    draw_board();
    randomizeBoard();
    drawPieces();
    refresh(screen);
    SDL_Delay(2000);*/
    initializeBoard();
    draw_board();
    drawPieces();
    refresh(screen);
    SDL_Delay(2000);
    refresh(screen);
    showBoard();
    SDL_Delay(2000);
    get_out();
    return 0;
}
