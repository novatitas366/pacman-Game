#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <chrono>
#include <utility>
#include <map>

using namespace std;
ofstream out("out.txt");

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

class playerNPCobject {
protected:
    pair <int, int> coords;
    bool isDead;
    char direction = 'n';
    playerNPCobject(int xx, int yy) : coords(make_pair(xx, yy)), isDead(0) {};
public:
    virtual void movePlayer(vector<vector<char>>& Pacman_map) {}
    virtual void chooseDirection(vector<vector<char>>& Pacman_map) {}
    char getDirection() {
        return direction;
    }
    pair <int, int> getcoords() {
        return coords;
    }

};

class player : public playerNPCobject {
private:
    bool isPoweredUp{};
    pair <int, int> startingCoords = (coords);
    int lives = 3;

public:
    player(int xx, int yy) : playerNPCobject(xx, yy) {};
    ~player() {};
    void godMode() {
        isPoweredUp = 1;
    }
    int getLives() {
        return lives;
    }
    void chooseDirection(vector<vector<char>>& Pacman_map) override {
        if (GetAsyncKeyState(VK_TAB)) {
            godMode();
        }

        if (GetAsyncKeyState(VK_DOWN) && Pacman_map[coords.first + 1][coords.second] != '#') {
            direction = 'd';
        }
        if (GetAsyncKeyState(VK_LEFT) && Pacman_map[coords.first][coords.second - 1] != '#') {
            direction = 'l';
        }
        if (GetAsyncKeyState(VK_RIGHT) && Pacman_map[coords.first][coords.second + 1] != '#') {
            direction = 'r';
        }
        if (GetAsyncKeyState(VK_UP) && Pacman_map[coords.first - 1][coords.second] != '#') {
            direction = 'u';
        }
    }

    void movePlayer(vector<vector<char>>& Pacman_map) {
        
        if (direction == 'r' && Pacman_map[coords.first][coords.second + 1] != '#') {
            Pacman_map[coords.first][coords.second] = ' ';
            Pacman_map[coords.first][coords.second + 1] = 'C';
            coords.second++;
        }
        else if (direction == 'l' && Pacman_map[coords.first][coords.second - 1] != '#') {
            Pacman_map[coords.first][coords.second] = ' ';
            Pacman_map[coords.first][coords.second - 1] = 'C';
            coords.second--;
        }
        else if (direction == 'd' && Pacman_map[coords.first + 1][coords.second] != '#') {
            Pacman_map[coords.first][coords.second] = ' ';
            Pacman_map[coords.first + 1][coords.second] = 'C';
            coords.first++;
        }
        else if (direction == 'u' && Pacman_map[coords.first - 1][coords.second] != '#') {
            Pacman_map[coords.first][coords.second] = ' ';
            Pacman_map[coords.first - 1][coords.second] = 'C';
            coords.first--;
        }
        if (coords.second == 39) {
            coords.second = 0;
            Pacman_map[15][39] = ' ';
        }
        else if (coords.second == 0) {
            coords.second = 39;
            Pacman_map[15][0] = ' ';
        }
    }
    void revive() {
        isDead = 0;
    }
    void powerUp() {
        isPoweredUp = 1;
    }
    void powerDown() {
        isPoweredUp = 0;
    }
    void die() {
        if (lives != 0) {
            isDead = 0;
            coords = startingCoords;
            powerUp();
            lives--;
            return;
        }
        system("cls");
        isDead = 1;
    }
    bool getPowerStatus() {
        return isPoweredUp;
    }
    bool getIsDead() {
        return isDead;
    }
    char getDirection() {
        return direction;
    }

};
class Ghost : public playerNPCobject {
private:
    char identifier;
    pair <int, int> sCoords;
    bool isDisabled;
    bool attackmode;
    
    pair <int, int> prevCoords;
    char inFront = ' ';
public:
    Ghost(int xx, int yy, char id) : playerNPCobject(xx, yy), sCoords(xx, yy), identifier(id), isDisabled(0), attackmode(1), prevCoords (0, 0) {};
    ~Ghost() {};
    void turnOffAttackMode() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        attackmode = 1;
    }
    void turnBackonCount() { 
        isDisabled = 1;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        isDisabled = 0;
    }
    void chooseDirection(vector<vector<char>>& Pacman_map, pair<int, int> playerCoords) {
        char randomDirections[] = { 'u', 'd', 'l', 'r' };
        if (attackmode) {
            int xAtstumas = playerCoords.second - coords.second;
            int yAtstumas = playerCoords.first - coords.first;
            
            


            if (abs(xAtstumas) > abs(yAtstumas)) {
                
                if (xAtstumas > 0 && Pacman_map[coords.first][coords.second + 1] != '#') {
                    direction = 'r';
                }
                else if (xAtstumas < 0 && Pacman_map[coords.first][coords.second - 1] != '#') {
                    direction = 'l';
                }
                else if (yAtstumas > 0 && Pacman_map[coords.first + 1][coords.second] != '#') {
                    direction = 'd';
                }
                else if (yAtstumas < 0 && Pacman_map[coords.first - 1][coords.second] != '#') {
                    direction = 'u';
                }
            }
            else {
                
                if (yAtstumas > 0 && Pacman_map[coords.first + 1][coords.second] != '#') {
                    direction = 'd';
                }
                else if (yAtstumas < 0 && Pacman_map[coords.first - 1][coords.second] != '#') {
                    direction = 'u';
                }
                else if (xAtstumas > 0 && Pacman_map[coords.first][coords.second + 1] != '#') {
                    direction = 'r';
                }
                else if (xAtstumas > 0 && Pacman_map[coords.first][coords.second + 1] != '#') {
                    direction = 'l';
                }
                else if (xAtstumas < 0 && Pacman_map[coords.first][coords.second - 1] != '#') {
                    direction = 'l';
                }
            }
            
            if (prevCoords == coords) {
                attackmode = 0;
                thread turnOffAttackModeThread(&Ghost::turnOffAttackMode, this);
                turnOffAttackModeThread.detach();
            }
            
            prevCoords = coords;
        }
        else if (!attackmode) {
            srand(time(NULL));
            int randomIndex = rand() % 4;
            direction = randomDirections[randomIndex];
        }
    }
    void movePlayer(vector<vector<char>>& Pacman_map, map <pair<int, int>, char> pellets, player& pacman) {
        auto it = pellets.find(make_pair(coords.first, coords.second));
        char replace;
        bool check = 1;


        if (!isDisabled) {
            if (it == pellets.end()) {
                replace = ' ';
            }
            else {
                replace = it->second;

            }
            if (direction == 'r' && Pacman_map[coords.first][coords.second + 1] != '#' && Pacman_map[coords.first][coords.second + 1] != 'I' &&
                Pacman_map[coords.first][coords.second + 1] != 'B' && Pacman_map[coords.first][coords.second + 1] != 'K' && Pacman_map[coords.first][coords.second + 1] != 'V') {

                Pacman_map[coords.first][coords.second] = replace;
                inFront = Pacman_map[coords.first][coords.second + 1];
                Pacman_map[coords.first][coords.second + 1] = identifier;

                coords.second++;
            }
            else if (direction == 'l' && Pacman_map[coords.first][coords.second - 1] != '#' && Pacman_map[coords.first][coords.second - 1] != 'I' &&
                Pacman_map[coords.first][coords.second - 1] != 'B' && Pacman_map[coords.first][coords.second - 1] != 'K' && Pacman_map[coords.first][coords.second - 1] != 'V') {

                Pacman_map[coords.first][coords.second] = replace;


                inFront = Pacman_map[coords.first][coords.second - 1];
                Pacman_map[coords.first][coords.second - 1] = identifier;
                coords.second--;
            }
            else if (direction == 'd' && Pacman_map[coords.first + 1][coords.second] != '#' && Pacman_map[coords.first + 1][coords.second] != 'I' &&
                Pacman_map[coords.first + 1][coords.second] != 'B' && Pacman_map[coords.first + 1][coords.second] != 'K' && Pacman_map[coords.first + 1][coords.second] != 'V') {
                Pacman_map[coords.first][coords.second] = replace;
                inFront = Pacman_map[coords.first + 1][coords.second];
                Pacman_map[coords.first + 1][coords.second] = identifier;
                coords.first++;
            }
            else if (direction == 'u' && Pacman_map[coords.first - 1][coords.second] != '#' && Pacman_map[coords.first - 1][coords.second] != 'I' &&
                Pacman_map[coords.first - 1][coords.second] != 'B' && Pacman_map[coords.first - 1][coords.second] != 'K' && Pacman_map[coords.first - 1][coords.second] != 'V') {
                    Pacman_map[coords.first][coords.second] = replace;
                    inFront = Pacman_map[coords.first - 1][coords.second];
                    Pacman_map[coords.first - 1][coords.second] = identifier;
                    coords.first--;
                }
        }
        else {
            Pacman_map[sCoords.first][sCoords.second] = identifier;
            coords.first = sCoords.first;
            coords.second = sCoords.second;
        }

    }
    
    char getIdentifier() {
        return identifier;
    }
    void disable() {
        isDisabled = 1;
        thread countdownToEnableGhost(&Ghost::turnBackonCount, this);
        countdownToEnableGhost.detach();
    }
    bool getDisabledStatus() {
        return isDisabled;
    }
};
class Score {
private:
    unsigned int score;
public:
    Score() : score(0) {};
    void operator ++(int) {
        score += 100;
    }
    unsigned int getScore() {
        return score;
    }
    void resetScore() {
        score = 0;
    }
};

class game {
private:
    vector<vector<char>> Pacman_map;
    Score score;
    player* ptrToPlayer;
    thread* countdownThread;
    vector <Ghost*> Ghosts;
    bool isCountingDown;
    int choice = 0;
    bool IsGameOn{};
    
    char ch{};
    map <pair<int, int>, char> pellets;
    unsigned int pelletNum;
    unsigned int PowerPelletNum;
    map <pair<int, int>, bool> Power_pellets;
    string PlayGame = "                              Play Game                                ";
    string QuitGame = "                              Quit Game                                ";
public:
    game() :ptrToPlayer(nullptr), choice(0), IsGameOn(0), ch(), pelletNum(0), PowerPelletNum(0), isCountingDown(0) {};
    void countdownPowerUp() {
        for (int i = 5; i > 0; --i) {
            
            ptrToPlayer->powerUp();
            std::this_thread::sleep_for(std::chrono::seconds(2));

        }
        ptrToPlayer->powerDown();
    }
    void loadMapAndObjects() {
        try {
            ifstream pacmanMap("pacmanMap.txt");
            if (pacmanMap.fail()) {
                throw "file does not exist";
            }
            int k = 0;
            while (!pacmanMap.eof()) {
                Pacman_map.push_back(vector <char>());
                for (int i = 0; i < 40; i++) {
                    char temp;
                    pacmanMap >> temp;
                    if (temp == '*') {
                        pellets[make_pair(k, i)] = '*';
                        out << k << " " << i << " | " << pellets[make_pair(k, i)] << endl;
                        pelletNum++;
                    }
                    if (temp == 'O') {
                        pellets[make_pair(k, i)] = 'O';
                        PowerPelletNum++;
                    }
                    if (temp == 'C') {
                        ptrToPlayer = new player(k, i);
                    }
                    if (temp == 'I') {
                        Ghosts.push_back(new Ghost(k, i, temp));
                    }
                    if (temp == 'B') {
                        Ghosts.push_back(new Ghost(k, i, temp));
                    }
                    if (temp == 'K') {
                        Ghosts.push_back(new Ghost(k, i, temp));
                    }
                    if (temp == 'V') {
                        Ghosts.push_back(new Ghost(k, i, temp));
                    }
                    Pacman_map[k].push_back(temp);
                }
                k++;

            }
            pacmanMap.close();
        }
        catch (const char* errtext) {
            std::cout << errtext << endl;
            cout << "Press any key to exit.";
            _getch();
            exit(EXIT_FAILURE);
        }


    }
    void youWin() {
        while (true) {
            setCursorPosition(0, 0);
            string PlayAgain = "                              Play Again                              ";
            if (choice == 1 && GetAsyncKeyState(VK_RETURN)) {
                system("cls");
                exit(EXIT_SUCCESS);
            }
            else if (choice == 0 && GetAsyncKeyState(VK_RETURN)) {
                eraseEverythingAndRestart();
                playGame();
            }
            if (GetAsyncKeyState(VK_DOWN)) {
                PlayAgain[28] = ' ';
                PlayAgain[29] = ' ';
                QuitGame[28] = '>';
                QuitGame[29] = '>';
                choice = 1;
            }

            if (GetAsyncKeyState(VK_UP)) {
                PlayAgain[28] = '>';
                PlayAgain[29] = '>';
                QuitGame[28] = ' ';
                QuitGame[29] = ' ';
                choice = 0;
            }

            cout << "\033[0m#######################################################################" << endl;
            cout << "#        \033[93m __   __  _______  __   __   \033[34m _     _ \033[95m ___  \033[31m __    _ \033[97m        #" << endl;
            cout << "#        \033[93m|  | |  ||       ||  | |  |  \033[34m| | _ | |\033[95m|   | \033[31m|  |  | |\033[97m        #" << endl;
            cout << "#        \033[93m|  |_|  ||   _   ||  | |  |  \033[34m| || || |\033[95m|   | \033[31m|   |_| |\033[97m        #" << endl;
            cout << "#        \033[93m|       ||  | |  ||  |_|  |  \033[34m|       |\033[95m|   | \033[31m|       |\033[97m        #" << endl;
            cout << "#        \033[93m|_     _||  |_|  ||       |  \033[34m|       |\033[95m|   | \033[31m|  _    |\033[97m        #" << endl;
            cout << "#        \033[93m  |   |  |       ||       |  \033[34m|   _   |\033[95m|   | \033[31m| | |   |\033[97m        #" << endl;
            cout << "#        \033[93m  |___|  |_______||_______|  \033[34m|__| |__|\033[95m|___| \033[31m|_|  |__|\033[97m        #" << endl;
            cout << "#######################################################################" << endl;
            cout << PlayAgain << endl;
            cout << QuitGame << endl;
            _getch();
        }
    }
    void youLose() {
        system("cls");
        while (true) {
            setCursorPosition(0, 0);
            string PlayAgain = "                              Play Again                              ";
            if (choice == 1 && GetAsyncKeyState(VK_RETURN)) {
                system("cls");
                exit(EXIT_SUCCESS);
            }
            else if (choice == 0 && GetAsyncKeyState(VK_RETURN)) {
                eraseEverythingAndRestart();
                playGame();
            }
            if (GetAsyncKeyState(VK_DOWN)) {
                PlayAgain[28] = ' ';
                PlayAgain[29] = ' ';
                QuitGame[28] = '>';
                QuitGame[29] = '>';
                choice = 1;
            }

            if (GetAsyncKeyState(VK_UP)) {
                PlayAgain[28] = '>';
                PlayAgain[29] = '>';
                QuitGame[28] = ' ';
                QuitGame[29] = ' ';
                choice = 0;
            }
            cout << "\033[0m#######################################################################" << endl;
            cout << "#  \033[93m__   __  _______  __   __    \033[31m___     \033[95m _______  \033[33m_______  \033[34m_______\033[97m    #" << endl;
            cout << "# \033[93m|  | |  ||       ||  | |  |  \033[31m|   |    \033[95m|       |\033[33m|       |\033[34m|       |\033[97m   #" << endl;
            cout << "# \033[93m|  |_|  ||   _   ||  | |  |  \033[31m|   |    \033[95m|   _   |\033[33m|  _____|\033[34m|    ___|\033[97m   #" << endl;
            cout << "# \033[93m|       ||  | |  ||  |_|  |  \033[31m|   |    \033[95m|  | |  |\033[33m| |_____ \033[34m|   |___ \033[97m   #" << endl;
            cout << "# \033[93m|_     _||  |_|  ||       |  \033[31m|   |___ \033[95m|  |_|  |\033[33m|_____  |\033[34m|    ___|\033[97m   #" << endl;
            cout << "#   \033[93m|   |  |       ||       |  \033[31m|       |\033[95m|       |\033[33m _____| |\033[34m|   |___ \033[97m   #" << endl;
            cout << "#   \033[93m|___|  |_______||_______|  \033[31m|_______|\033[95m|_______|\033[33m|_______|\033[34m|_______| \033[97m  #" << endl;
            cout << "#######################################################################" << endl;
            cout << PlayAgain << endl;
            cout << QuitGame << endl;
            _getch();
        }
    }
    void playGame() {
        int pelletAmount = pellets.size();
        while (!(ptrToPlayer->getIsDead())) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (pelletAmount == 0) {
                youWin();
                break;
            }
            if (GetAsyncKeyState(VK_ESCAPE)) {
                system("cls");
                exit(EXIT_SUCCESS);
            }
            if (GetAsyncKeyState(VK_F1)) {
                system("cls");
                youWin();
                break;
            }
            ShowConsoleCursor(false);
            setCursorPosition(0, 0);

            ptrToPlayer->chooseDirection(Pacman_map);
            Ghosts[0]->chooseDirection(Pacman_map, ptrToPlayer->getcoords());
            Ghosts[1]->chooseDirection(Pacman_map, ptrToPlayer->getcoords());
            Ghosts[2]->chooseDirection(Pacman_map, ptrToPlayer->getcoords());
            Ghosts[3]->chooseDirection(Pacman_map, ptrToPlayer->getcoords());
            
            
            if (pellets.find(ptrToPlayer->getcoords()) != pellets.end()) {
                if (pellets.find(ptrToPlayer->getcoords())->second == 'O') {
                    pellets.find(ptrToPlayer->getcoords())->second = ' ';
                    pelletAmount--;
                    thread countdown(&game::countdownPowerUp, this);
                    countdown.detach();
                }

            }
           
            
            
            if (pellets.find(ptrToPlayer->getcoords()) != pellets.end()) {
                if (pellets.find(ptrToPlayer->getcoords())->second == '*') {
                    pellets[ptrToPlayer->getcoords()] = ' ';
                    pelletAmount--;
                    score++;
                }
            }
            ptrToPlayer->movePlayer(Pacman_map);
            for (int i = 0; i < 4; i++) {
                if (ptrToPlayer->getcoords() == Ghosts[i]->getcoords()) {
                    if (!ptrToPlayer->getPowerStatus()) {
                        ptrToPlayer->die();
                        thread countdown(&game::countdownPowerUp, this);
                        countdown.detach();
                        //system("cls");
                    }
                    else {
                        Ghosts[i]->disable();
                    }
                }
            }
            Ghosts[0]->movePlayer(Pacman_map, pellets, *ptrToPlayer);
            Ghosts[1]->movePlayer(Pacman_map, pellets, *ptrToPlayer);
            Ghosts[2]->movePlayer(Pacman_map, pellets, *ptrToPlayer);
            Ghosts[3]->movePlayer(Pacman_map, pellets, *ptrToPlayer);
            for (int i = 0; i < 4; i++) {
                if (ptrToPlayer->getcoords() == Ghosts[i]->getcoords()) {
                    if (!ptrToPlayer->getPowerStatus()) {
                        ptrToPlayer->die();
                        thread countdown(&game::countdownPowerUp, this);
                        countdown.detach();
                        //system("cls");
                        
                    }
                    else {
                        Ghosts[i]->disable();
                        
                    }
                }
            }
            printMap();
        }
        if (ptrToPlayer->getIsDead()) {
            youLose();
        }



    }

    void eraseEverythingAndRestart() {
        Pacman_map.erase(Pacman_map.begin(), Pacman_map.end());
        delete Ghosts[0];
        delete Ghosts[1];
        delete Ghosts[2];
        delete Ghosts[3];
        Ghosts.erase(Ghosts.begin(), Ghosts.end());
        delete ptrToPlayer;
        score.resetScore();
        for (auto it = pellets.begin(); it != pellets.end(); ) {
            it = pellets.erase(it);
        }
        for (auto it = Power_pellets.begin(); it != Power_pellets.end(); ) {
            it = Power_pellets.erase(it);
        }
        loadMapAndObjects();
        system("cls");
    }

    void Menu() {
        
        while (true) {
            setCursorPosition(0, 0);
            //ShowConsoleCursor(false);
            if (GetAsyncKeyState(VK_RETURN) && choice == 0) {
                system("cls");
                ptrToPlayer->revive();
                playGame();
            }
            else if (GetAsyncKeyState(VK_RETURN) && choice == 1) {
                system("cls");
                break;
            }
            if (GetAsyncKeyState(VK_ESCAPE)) {
                break;
            }

            if (GetAsyncKeyState(VK_DOWN)) {
                PlayGame[28] = ' ';
                PlayGame[29] = ' ';
                QuitGame[28] = '>';
                QuitGame[29] = '>';
                choice = 1;
            }

            if (GetAsyncKeyState(VK_UP)) {
                PlayGame[28] = '>';
                PlayGame[29] = '>';
                QuitGame[28] = ' ';
                QuitGame[29] = ' ';
                choice = 0;
            }

            /*cout << "##########################################################" << endl;
            cout << "#                                                        #" << endl;
            cout << "# \033[0;33m _______  _______  _______  __   __  _______  __    _ \033[0m #" << endl;
            cout << "# \033[0;33m|       ||   _   ||       ||  |_|  ||   _   ||  |  | |\033[0m #" << endl;
            cout << "# \033[0;33m|    _  ||  |_|  ||       ||       ||  |_|  ||   |_| |\033[0m #" << endl;
            cout << "# \033[0;33m|   |_| ||       ||       ||       ||       ||       |\033[0m #" << endl;
            cout << "# \033[0;33m|    ___||       ||      _||       ||       ||  _    |\033[0m #" << endl;
            cout << "# \033[0;33m|   |    |   _   ||     |_ | ||_|| ||   _   || | |   |\033[0m #" << endl;
            cout << "# \033[0;33m|___|    |__| |__||_______||_|   |_||__| |__||_|  |__|\033[0m #" << endl;
            cout << "#                                                        #" << endl;
            cout << "##########################################################" << endl;*/
            cout << "#######################################################################" << endl;
            cout << "#                                                                     #" << endl;
            cout << "# \033[0;33m#######       #       ######     #       #         #     #       #\033[0m  #" << endl;
            cout << "# \033[0;33m#     #      # #      #         # #     # #       # #    # #     #\033[0m  #" << endl;
            cout << "# \033[0;33m#######     #####     #        #   #   #   #     #####   #   #   #\033[0m  #" << endl;
            cout << "# \033[0;33m#          #     #    #       #     # #     #   #     #  #     # #\033[0m  #" << endl;
            cout << "# \033[0;33m#         #       #   ###### #       #       # #       # #       #\033[0m  #" << endl;
            cout << "#                                                                     #" << endl;
            cout << "#######################################################################" << endl;
            cout << PlayGame << endl;
            cout << QuitGame << endl;
            cout << choice << endl;
            cout << "VK_DOWN State: " << GetAsyncKeyState(VK_DOWN) << std::endl;
            cout << "VK_UP State: " << GetAsyncKeyState(VK_UP) << std::endl;
            _getch();

        }
    }
    void printMap() {
        int line = 0;
        int widht = 0;
        bool check = 0;
        bool value = 0;
        cout << endl << endl;
        for (auto i : Pacman_map) {
            widht = 0;
            for (auto j : i) {
                auto it = pellets.find(make_pair(line, widht));
                if (it != pellets.end()) {
                    value = it->second;
                    check = 1;
                }
                else if (it == pellets.end()) {
                    check = 0;
                }
                if (j == 'O') {
                    cout << "\033[0;31m" << j;
                    
                }
                else if (j == 'C') {
                    cout << "\033[0;33m" << j;
                    
                }
                else if (j == '#') {

                    cout << "\033[34m" << j;
                    
                }
                else if (j == 'K') {
                    cout << "\033[38;5;208m" << j;
                    
                }
                else if (j == 'I') {
                    cout << "\033[94m" << j;
                    
                }
                else if (j == 'B') {
                    cout << "\033[95m" << j;
                    
                }
                else if (j == 'V') {
                    cout << "\033[31m" << j;
                    
                }
                else cout << "\033[0m" << j;

                widht++;
                /*out << line<< " "<< widht << " | "  << value << endl;*/
            }
            line++;
            cout << endl;

        }
        cout << "\n Score: " << score.getScore() << endl;
        //cout << "\npellets: " << pellets.size() << "    " << "Power pellets: " << Power_pellets.size() << endl;
        //cout << "\npacman direction: " << ptrToPlayer->getDirection() << endl;
        cout << "pacman powerStatus: " << ptrToPlayer->getPowerStatus() << endl;
        cout << "LIVES: " << ptrToPlayer->getLives()+1 << endl;
        cout << "pacman coords: " << ptrToPlayer->getcoords().first << " " << ptrToPlayer->getcoords().second << endl;
        cout << "Ghost: " << Ghosts[0]->getIdentifier() << " coords: " << Ghosts[0]->getcoords().first << " " << Ghosts[0]->getcoords().second << " is disabled: " << Ghosts[0]->getDisabledStatus() << " Direction: " << Ghosts[0]->getDirection() << endl;
        cout << "Ghost: " << Ghosts[1]->getIdentifier() << " coords: " << Ghosts[1]->getcoords().first << " " << Ghosts[1]->getcoords().second << " is disabled: " << Ghosts[1]->getDisabledStatus() << " Direction: " << Ghosts[1]->getDirection() << endl;
        cout << "Ghost: " << Ghosts[2]->getIdentifier() << " coords: " << Ghosts[2]->getcoords().first << " " << Ghosts[2]->getcoords().second << " is disabled: " << Ghosts[2]->getDisabledStatus() << " Direction: " << Ghosts[2]->getDirection() << endl;
        cout << "Ghost: " << Ghosts[3]->getIdentifier() << " coords: " << Ghosts[3]->getcoords().first << " " << Ghosts[3]->getcoords().second << " is disabled: " << Ghosts[3]->getDisabledStatus() << " Direction: " << Ghosts[3]->getDirection() << endl;

    }



};



int main() {
    game mapac;
    mapac.loadMapAndObjects();
    system("cls");

    mapac.Menu();
}