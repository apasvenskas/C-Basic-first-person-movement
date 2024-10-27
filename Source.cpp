#include <iostream>
using namespace std;
#include <Windows.h>
#include <chrono>

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
    map += L"#..........#...#";
    map += L"#..........#...#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......########";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = chrono::system_clock::now(); // for timing movement
    auto tp2 = chrono::system_clock::now();

    while(1){

        tp2 = chrono::system_clock::now(); // used for to get fElapsed time & time the palyer movement. 
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();
        
        // movement controls
        if(GetAsyncKeyState((unsigned short)'A') & 0x8000)
            fPlayerA -= (0.5f) * fElapsedTime;

        if(GetAsyncKeyState((unsigned short)'D') & 0x8000)
            fPlayerA += (0.5f) * fElapsedTime;

        if(GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
             fplayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
             fplayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
        }

        if(GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
             fplayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
             fplayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
        }
           

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

            short nShade = ' '; 

            if (fDistanceToWall <= fDepth / 4.0f)			nShade = 0x2588;	// Very close	
			else if (fDistanceToWall < fDepth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < fDepth)				nShade = 0x2591;
			else											nShade = ' ';		// Too far away


            for(int y = 0; y < nScreenHeight; y++){
                if(y < nCeiling){
                    screen[y*nScreenWidth + x] = ' ';
                } else if(y > nCeiling && y <= nFloor) {
                     screen[y*nScreenWidth + x] = nShade;
                } else {
                  // Shade floor based on distance
					float b = 1.0f - (((float)y -nScreenHeight/2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y*nScreenWidth + x] = nShade;
                }
            }
        }

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    return 0;
}