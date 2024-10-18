#include <iostream>
using namespace std;
#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 40;

float fplayerX = 8.0f;
float fplayerY = 8.0f; 
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFov = 3.13159/ 4.0; 
float fDepth = 16; 

int main(){

    // Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

    // map
    wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    while(1){

        if(GetAsyncKeyState((unsigned short)'A') & 0x8000)
            fPlayerA -= (0.1f);

        if(GetAsyncKeyState((unsigned short)'D') & 0x8000)
            fPlayerA += (0.1f);

        // wall mechanics
        for(int x = 0; x < nScreenWidth; x++){
            float fRayAngle = (fPlayerA - fFov / 2.0f) + ((float)x / (float)nScreenWidth) * fFov;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < fDepth){
                fDistanceToWall += 0.1f;

                int nTestX = (int)(fplayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fplayerY + fEyeY * fDistanceToWall);

                // test if ray is out of bounds
                if(nTestX < 0|| nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight){
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                } else {
                    if(map[nTestY * nMapWidth + nTestX] == '#'){
                        bHitWall = true;
                    }
                }
            }

            // adjsuting the distance to ceiling and floor.
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            for(int y = 0; y < nScreenHeight; y++){
                if(y < nCeiling){
                    screen[y*nScreenWidth + x] = ' ';
                } else if(y > nCeiling && y <= nFloor) {
                     screen[y*nScreenWidth + x] = '#';
                } else {
                    screen[y*nScreenWidth + x] = ' ';
                }
            }
        }

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    return 0;
}