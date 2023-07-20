#include <iostream>
#include <climits>
#include "raylib.h"

class ConnectFour {
    int board[6][7];
    int turn;
    int count = 0;
    bool done = false;
    int recent = 0;

    //Give a numerical evaluation for game state, negative for player 1, positive for player 2 (AI)
    int eval() {
        int state = won();
        switch (state) {
        case -1:
            return 0;
        case 1:
            return INT_MAX;
        case 2:
            return INT_MIN;
        case 0:
            int score = 0;
            int scores[6][7] = { {3, 4, 5, 7, 5, 4, 3},{4, 6, 8, 10, 8, 6, 4}, {5, 8, 11, 13, 11, 8, 5}, {5, 8, 11, 13, 11, 8, 5}, {4, 6, 8, 10, 8, 6, 4}, {3, 4, 5, 7, 5, 4, 3} };
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 7; j++) {
                    switch (board[i][j]) {
                    case 1:
                        score -= scores[i][j];
                        break;
                    case 2:
                        score += scores[i][j];
                    }
                }
            }
            return score;
        }
    }

    //Undo the most recent move
    void undo() {
        if (recent == 0 || done) {
            return;
        }
        int row = 0;
        while (board[row][recent - 1] == 0) {
            row++;
        }
        board[row][recent - 1] = 0;
        recent = 0;
        count--;
        turn = 3 - turn;
    }

    //Recursive minimax algorithm
    int minimax(ConnectFour state, int depth, bool maximizing) {
        if(depth <= 0 || won() != 0){
            return eval();
        }

        if (maximizing) {
            ConnectFour copy = state; //Copy state to test
            int max = INT_MIN;
            for (int i = 1; i <= 7; i++) {
                if (copy.drop(i)) {
                    max = std::max(max, minimax(copy, depth - 1, false));
                    undo();
                }
            }
            return max;
        }
        else {
            ConnectFour copy = state; //Copy state to test
            int min = INT_MAX;
            for (int i = 1; i <= 7; i++) {
                if (copy.drop(i)) {
                    min = std::min(min, minimax(copy, depth - 1, true));
                    undo();
                }
            }
            return min;
        }
    }

public:
    ConnectFour(int startTurn) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                board[i][j] = 0;
            }
        }

        turn = startTurn;
    }

    ConnectFour(const ConnectFour& old) {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                board[i][j] = old.board[i][j];
            }
        }
        turn = old.turn;
        count = old.count;
    }
    ConnectFour& operator=(const ConnectFour& old);

    int getTurn() {
        return turn;
    }

    int getCount() {
        return count;
    }

    int get(int row, int column) {
        return board[row][column];
    }

    bool isDone() {
        return done;
    }

    //Place a token in a column, return a boolean based off if the move went through
    bool drop(int column) {
        if (done) {
            return false;
        }
        int row = 5;
        while (board[row][column - 1]) {
            row--;
            if (row <= 0) {
                recent = 0;
                return false;
            }
        }
        board[row][column - 1] = turn;
        count++;
        turn = 3 - turn;
        done = (won() != 0);
        recent = column;
        return true;
    }

    //Find best move
    int AiMove(ConnectFour self, int depth) {
        if (depth <= 0) {
            return 0;
        }
        int bestMove = 0;
        int bestScore = INT_MIN;
        ConnectFour copy = self;
        for (int i = 1; i <= 7; i++) {
            if (copy.drop(i)) {
                int temp = minimax(copy, depth - 1, false);
                if (temp >= bestScore) {
                    bestScore = temp;
                    bestMove = i;
                }
            }
        }
        return bestMove;
    }

    //Returns 1 or 2 if a player won, a 0 if game is still going, and a -1 for a stalemate
    int won() {

        //Check for enough tokens
        if (count < 8) {
            return 0;
        }
        //Check for stalemate
        if (count == 42) {
            return -1;
        }

        //Variables for continuity checking
        bool continuity = true;
        int color = 0;

        //Check horizontal connections
        for (int row = 0; row < 6; row++) {
            for (int column = 0; column < 2; column++) {
                if (board[row][column]) {
                    color = board[row][column];
                    continuity = true;
                    for (int i = 1; i < 4; i++) {
                        continuity = continuity && (board[row][column + i] == color);
                    }
                    if (continuity) {
                        return color;
                    }
                }
            }
        }
        //Check vertical connections
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 7; column++) {
                if (board[row][column]) {
                    color = board[row][column];
                    continuity = true;
                    for (int i = 1; i < 4; i++) {
                        continuity = continuity && (board[row + i][column] == color);
                    }
                    if (continuity) {
                        return color;
                    }
                }
            }
        }
        //Check for downward diagonal connections
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 4; column++) {
                if (board[row][column]) {
                    color = board[row][column];
                    continuity = true;
                    for (int i = 1; i < 4; i++) {
                        continuity = continuity && (board[row + i][column + i] == color);
                    }
                    if (continuity) {
                        return color;
                    }
                }
            }
        }
        //Check for upward diagonal connections
        for (int row = 0; row < 3; row++) {
            for (int column = 3; column < 7; column++) {
                if (board[row][column]) {
                    color = board[row][column];
                    continuity = true;
                    for (int i = 1; i < 4; i++) {
                        continuity = continuity && (board[row + i][column - i] == color);
                    }
                    if (continuity) {
                        return color;
                    }
                }
            }
        }

        return 0;
    }
};

int main() {
    //Graphics constants
    const int WIDTH = 800; //Width of screen
    const int HEIGHT = 500; //Height of screen
    const int RADIUS = 30; //Radius of token
    const int VOFFSET = 15; //Vertical offset
    const int HOFFSET = 25; //Horrizontal offset

    //Game constants
    const int MAX_DEPTH = 5;
    
    //Graphics setup
    InitWindow(WIDTH, HEIGHT, "Connect Four");
    SetTargetFPS(60);

    //Game setup
    ConnectFour game = ConnectFour(1);
    bool running = true;
    bool AiTurn = false;

    //Event loop
    while (!WindowShouldClose()) {
        BeginDrawing();

        //Draw components
        ClearBackground(BLUE);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                int color = game.get(i, j);
                DrawCircle(((WIDTH / 7) * j) + RADIUS + HOFFSET, ((HEIGHT / 6) * i) + RADIUS + VOFFSET, RADIUS, (color == 1)?RED:((color == 2)?YELLOW:WHITE));
            }
        }
        
        //Action listeners
        if (running && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !AiTurn) {
            int column = GetMouseX() / (WIDTH / 7);
            if (column != 7) {
                column++;
            }
            if (game.drop(column)) {
                //AiTurn = true;
                running = !game.isDone();
            }
        }
        /*else if (AiTurn) {
            game.drop(game.AiMove(game, std::min(MAX_DEPTH, 42 - game.getCount())));
            AiTurn = false;
            running = (game.won() == 0);
        }*/

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
