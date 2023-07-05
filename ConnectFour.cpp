#include <iostream>
#include <GLFW/glfw3.h>

class ConnectFour {
    int board[6][7] = {};
    int turn = 1;
    int count = 0;

public:

    int getTurn() {
        return turn;
    }

    int getCount() {
        return count;
    }

    int get(int row, int column) {
        return board[row][column];
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
        int sum = 0;
        int color = 0;

        //Check horizontal connections
        for (int row = 0; row < 6; row++) {
            for (int column = 0; column < 2; column++) {
                if (board[row][column]) {
                    color = board[row][column];
                    sum = board[row][column];
                    for (int i = 1; i < 4; i++) {
                        sum += board[row][column + i];
                    }
                    if (sum == color * 4) {
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
                    sum = board[row][column];
                    for (int i = 1; i < 4; i++) {
                        sum += board[row + i][column];
                    }
                    if (sum == color * 4) {
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
                    sum = board[row][column];
                    for (int i = 1; i < 4; i++) {
                        sum += board[row + i][column + i];
                    }
                    if (sum == color * 4) {
                        return color;
                    }
                }
            }
        }
        //Check for upward diagonal connections
        for (int row = 0; row < 3; row++) {
            for (int column = 3; column < 7; column--) {
                if (board[row][column]) {
                    color = board[row][column];
                    sum = board[row][column];
                    for (int i = 1; i < 4; i++) {
                        sum += board[row + i][column - i];
                    }
                    if (sum == color * 4) {
                        return color;
                    }
                }
            }
        }

        return 0;
    }

    int drop(int column) {
        int row = 5;
        while (board[row][column]) {
            row--;
            if (row < 0) {
                return 0;
            }
        }
        board[row][column] = turn;
        turn = 3 - turn;
        return won();
    }
};

int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
