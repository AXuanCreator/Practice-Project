#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

class Solution
{
public:
    void countDown()
    {
        int time;
        cout << "Enter Waiting Time:";
        cin >> time;

        for (int i = time; i > 0; --i)
        {
            system("cls");
            cout << "[ " << i << " ]";
            Sleep(1000);
        }
    }
    void areaCal()
    {
        cout << "Enter Row and Col: ";
        cin >> row >> col;
        while (row <= 0 || col <= 0)
        {
            cout << "Error , please enter again" << endl;
            cin >> row >> col;
        }
        cout << "The Area : " << row * col << endl;
        Sleep(2000);
    }
    // 隐藏光标
    void hideCmdCursor()
    {
        // 删除光标
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(consoleHandle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);

        /* //重新显示光标
        cursorInfo.bVisible = true;
        SetConsoleCursorInfo(consoleHandle, &cursorInfo);
        */
    }

    // 输入光标移动
    void cursorMove(short x, short y)
    {
        COORD pos = {x, y}; // 将光标移动回到输出位置
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }
    ~Solution()
    {
        cout << "Delete Complete!" << endl;
    }

private:
    int time;
    int row;
    int col;
    int area;
};

class GameCreat
{
public:
    // 地图初始化-构造
    GameCreat(int x, int y, int times)
    {

        mapInfo.resize(x, vector<pair<int, int>>(y, make_pair(0, 0)));
        map.resize(x, vector<char>(y, '.'));

        // 随机数范围限定
        dis = uniform_int_distribution<>(1, max(x, y)); // 补给金钱随机
        CRow = uniform_int_distribution<>(0, x - 1);    // 行坐标随机
        CCol = uniform_int_distribution<>(0, y - 1);    // 列坐标随机

        row = x;
        col = y;

        food = dis(gen);
        money = 0;
        swMove = 1;

        ruleShow();
        mapRandomCreat(times);
    }

    // 析构
    ~GameCreat()
    {
        cout << "\rGameOver!              ";
        Sleep(2000);
    }

private:
    // 普通变量
    int row;
    int col;
    int money;
    int food;
    bool swMove;      // 控制movePos第一次
    int xMove, yMove; // 光标位置
    // 随机数变量
    random_device rd;                // 随机数种子
    mt19937 gen{rd()};               // 随机数引擎mt19937
    uniform_int_distribution<> dis;  // 食物和钱的随机数
    uniform_int_distribution<> CRow; // 坐标x的随机数
    uniform_int_distribution<> CCol; // 坐标y的随机数

    // 地图变量
    vector<vector<char>> map;
    vector<vector<pair<int, int>>> mapInfo; // money-food
    constexpr static int dx[4] = {-1, 0, 0, 1};
    constexpr static int dy[4] = {0, -1, 1, 0};

    // 规则
    void ruleShow()
    {
        cout << "This Is A Interesting Game" << endl;
        cout << "You Can Enter 'W-A-S-D' To Move The '#'" << endl;
        cout << "You Need Get More Money" << endl;
        cout << "You Can Enter 'c' and 'v' To Using Tools:" << endl;
        cout << "[c] ==> Creat A New Supply Point | USING FOOD : 15" << endl
             << "[v] ==> Randomly Move To A Position | USING FOOD : 5" << endl;
        cout << "The Game Will Start In [ 5 ] Second! Good Luck!";
        for (int i = 5; i > 0; --i)
        {
            cout << "\033[26G"; // ANSI控制码
            cout << i;
            Sleep(1000);
        }
        system("cls");
    }

    // 位置随机
    void posRandomCreat()
    {
        int x = CRow(gen);
        int y = CCol(gen);
        swap(map[xMove][yMove], map[x][y]);
        xMove = x;
        yMove = y;
    }
    // 地图随机点创建
    void mapRandomCreat(int times = 1)
    {
        for (int i = 0; i < times; ++i)
            randomFoodAndMoney();

        showMap();
        Sleep(500);
        movePos();
    }

    // 补给随机化
    void randomFoodAndMoney()
    {
        // 随机变量创建
        int x = CRow(gen);
        int y = CCol(gen);
        int money = dis(gen);
        int food = dis(gen);

        mapInfo[x][y] = make_pair(money, food);
        map[x][y] = '*';
    }

    // 位置移动
    void movePos()
    {
        if (swMove)
        {
            xMove = CRow(gen);
            yMove = CCol(gen);
            map[xMove][yMove] = '#';
            swMove = 0;
            showMap();
        }
        while (food > 0)
        {
            int pos;
            char ch = _getch();

            if (ch == 'w')
                pos = 0;
            else if (ch == 'a')
                pos = 1;
            else if (ch == 'd')
                pos = 2;
            else if (ch == 's')
                pos = 3;
            else if (ch == 'c')
            {
                if (food <= 15)
                {
                    cout << "\rNot Enough Food                    ";
                    continue;
                }
                randomFoodAndMoney();
                food -= 15;
                showMap();
                continue;
            }
            else if (ch == 'v')
            {
                if (food <= 5)
                {

                    cout << "\rNot Enough Food                     ";
                    continue;
                }
                posRandomCreat();
                food -= 5;
                showMap();
                continue;
            }
            else
            {
                cout << "\rERROR , plear Enter again!               ";
                continue;
            }

            // 移动
            if (judgeXY(xMove + dx[pos], yMove + dy[pos]))
            {
                --food;
                cout << "\rAvaliable Move                            ";
                if (judgeFood(xMove + dx[pos], yMove + dy[pos]))
                {
                    money += mapInfo[xMove + dx[pos]][yMove + dy[pos]].first;
                    food += mapInfo[xMove + dx[pos]][yMove + dy[pos]].second;
                    map[xMove][yMove] = '.';
                    xMove += dx[pos];
                    yMove += dy[pos];
                    map[xMove][yMove] = '#';
                    //[x]随机分配位置，可以放在所有if后
                    randomFoodAndMoney(); // 重新创建1个随机点
                    showMap();
                }
                else
                {
                    swap(map[xMove][yMove], map[xMove + dx[pos]][yMove + dy[pos]]);
                    xMove += dx[pos];
                    yMove += dy[pos];
                    showMap();
                }
            }
        }

        delete this;
    }

    // 地图显示
    void showMap()
    {
        COORD pos = {0, 0}; // 将光标移动回到输出位置
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

        for (auto row : map)
        {
            for (auto val : row)
                cout << val << "   ";
            cout << endl
                 << endl;
        }
        infoShow();
    }

    // 信息显示
    void infoShow()
    {
        cout << "-------------------" << endl;
        cout << "Food: " << setfill('0') << setw(4) << food << " Money: " << setfill('0') << setw(4) << money << endl;
        cout << "Skill Fee: [c]->15 [v]->5" << endl;
        cout << "-------------------" << endl;
    }

    // 判断坐标合法性
    bool judgeXY(int x, int y)
    {
        return x >= 0 && x < row && y >= 0 && y < col;
    }

    // 判断是否为补给点
    bool judgeFood(int x, int y)
    {
        return map[x][y] == '*';
    }
};

int main()
{
    Solution *sol = new Solution();
    sol->hideCmdCursor(); // 隐藏光标
    while (true)
    {
        system("cls"); // 清屏
        cout << "--------------------------------------" << endl;
        cout << "----------Choose Your Choice----------" << endl;
        cout << "1.Timer" << endl;
        cout << "2.Area Calculation" << endl;
        cout << "3.Game" << endl;
        cout << "0.Exit" << endl;
        cout << "Your Choose Is : " << endl;
        cout << "--------------------------------------" << endl;

        char c;
        c = _getch();
        sol->cursorMove(0,9);
        cout << "LOADING..." << endl;
        Sleep(1000);
        system("cls");
        if (c == '1')
            sol->countDown();
        else if (c == '2')
            sol->areaCal();
        else if (c == '3')
        {
            int row, col, rd;
            cout << "Set You: " << endl;
            cout << "Row : ";
            cin >> row;
            cout << "Col : ";
            cin >> col;
            cout << "Random Quantity : ";
            cin >> rd;
            cout << "LOADING..." << endl;
            Sleep(1000);
            system("cls");
            GameCreat *gc = new GameCreat(row, col, rd);

        }
        else if (c == '0')
        {
            break;
        }
    }

    system("pause");
    return 0;
}