#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 10
#define MONSTER_COUNT 2

char map[HEIGHT][WIDTH + 1] = {
    "####################",
    "#........#.........#",
    "#.####...#...####..#",
    "#.................##",
    "#.###.#######.###..#",
    "#...#...C.....#....#",
    "#...###.#.###.###..#",
    "#.......#.........##",
    "#..#####...####....#",
    "####################"
};

int score = 0;
int pacmanX = 9, pacmanY = 5;
int monsterX[MONSTER_COUNT] = {3, 16};
int monsterY[MONSTER_COUNT] = {1, 7};
char prevMonsterChar[MONSTER_COUNT] = {'.', '.'};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawMap() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char ch = map[i][j];
            if (ch == '#') setColor(8);          // Grey
            else if (ch == '.') setColor(7);     // White
            else if (ch == 'C') setColor(14);    // Yellow
            else if (ch == 'M') setColor(12);    // Red (monster)
            else setColor(0);                    // Black or default
            printf("%c", ch);
        }
        printf("\n");
    }
    setColor(15);
    printf("\nScore: %d\n", score);
}

void movePacman(char direction) {
    int newX = pacmanX;
    int newY = pacmanY;

    switch (direction) {
        case 'w': newY--; break;
        case 's': newY++; break;
        case 'a': newX--; break;
        case 'd': newX++; break;
        default: return;
    }

    if (map[newY][newX] != '#') {
        if (map[newY][newX] == '.') {
            score++;
        } else if (map[newY][newX] == 'M') {
            setColor(12);
            printf("\nGame Over! You were caught by a monster!\n");
            exit(0);
        }
        map[pacmanY][pacmanX] = ' ';
        pacmanX = newX;
        pacmanY = newY;
        map[pacmanY][pacmanX] = 'C';
    }
}

void moveMonsters() {
    for (int i = 0; i < MONSTER_COUNT; i++) {
        int dx = (pacmanX > monsterX[i]) ? 1 : (pacmanX < monsterX[i]) ? -1 : 0;
        int dy = (pacmanY > monsterY[i]) ? 1 : (pacmanY < monsterY[i]) ? -1 : 0;

        int newX = monsterX[i] + dx;
        int newY = monsterY[i] + dy;

        if (map[newY][newX] == ' ' || map[newY][newX] == '.' || map[newY][newX] == 'C') {
            if (map[newY][newX] == 'C') {
                setColor(12);
                printf("\nGame Over! You were caught by a monster!\n");
                exit(0);
            }

            map[monsterY[i]][monsterX[i]] = prevMonsterChar[i];
            prevMonsterChar[i] = map[newY][newX];
            monsterX[i] = newX;
            monsterY[i] = newY;
            map[monsterY[i]][monsterX[i]] = 'M';
        }
    }
}

int main() {
    for (int i = 0; i < MONSTER_COUNT; i++) {
        prevMonsterChar[i] = map[monsterY[i]][monsterX[i]];
        map[monsterY[i]][monsterX[i]] = 'M';
    }
    drawMap();
    while (1) {
        if (_kbhit()) {
            char ch = _getch();
            movePacman(ch);
        }
        moveMonsters();
        drawMap();
        Sleep(200);
    }
    return 0;
}
