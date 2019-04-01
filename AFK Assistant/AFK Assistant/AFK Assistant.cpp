// AFK Assistant.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "resource.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#pragma comment(lib,"Winmm.lib")


/*
auto start = std::chrono::high_resolution_clock::now();


auto end = std::chrono::high_resolution_clock::now();


double x = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
*/


using namespace std;

string windowName;
char windowCheck;
bool willPlay = false;
int audioDelaySeconds = 0;



string GetActiveWindowTitle()
{
	char wnd_title[256];
	HWND hwnd = GetForegroundWindow(); // get handle of currently active window
	GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
	return wnd_title;
}

void GetWindowCheck() {
	cout << "Is this the window you want to AFK? - (Y/N)" << endl << endl;
	cin >> windowCheck;
	cout << endl;
	windowCheck = toupper(windowCheck);
}

void GetWindow() {
	cout << "Click into the window you want to focus." << endl << endl;
	windowName = "";
	Sleep(1000);

	while (true) {
		if (GetKeyState(VK_LBUTTON) & 0x800) {
			windowName = GetActiveWindowTitle();

			if (windowName != "") {
				cout << "You selected " << windowName << endl << endl;
				GetWindowCheck();
				if (windowCheck == 'Y' || windowCheck == 'N') {
					break;
				}
				else {
					cout << "You're supposed to enter Y or N. Dumbass." << endl << endl;
					GetWindowCheck();
					break;
				}
			}
		}
		Sleep(10);
	}
}

void AfkAlert() {
	//Beep(325, 100);
	int audioWaitTime = 9;
	while (true) {
		if (willPlay == true && audioDelaySeconds > audioWaitTime) {
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE);
			audioDelaySeconds = 0;
		}
		audioDelaySeconds++;
		Sleep(1000);
	}
}

void WatchForClicks() {

	double minutes;
	int timeCheck = 1;
	int oldTimeCheck = 0;
	cout << "Welcome to Jamboni's AFK assistant." << endl << endl;
	cout << "Used to alert you when you have been inactive in the selected window \nfor the specified time." << endl << endl;
	GetWindow();
	if (windowCheck == 'N') {
		GetWindow();
	}

	cout << endl << "Enter AFK time in minutes. eg 4.3" << endl << endl;
	cin >> minutes;
	cout << endl;
	if (cin.fail()) {
		cout << "You think you're funny? Fuck off I'm not checking for proper number input." << endl << endl;
	}
	minutes = minutes * 60;


	auto start = std::chrono::high_resolution_clock::now();
	while (true) {

		if (GetActiveWindowTitle() == windowName && (GetKeyState(VK_LBUTTON) & 0x800)) {
			start = std::chrono::high_resolution_clock::now();
			oldTimeCheck = 0;
			willPlay = false;
		}

		auto end = std::chrono::high_resolution_clock::now();
		double timeSince = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		if (timeSince >= minutes) { // If no click do something here
			willPlay = true;
		}
		timeCheck = (int)timeSince;
		if (timeCheck > oldTimeCheck) {
			cout << timeSince << ": Seconds since last click." << endl;
			oldTimeCheck = timeSince;
		}
		Sleep(2);
	}
	cin.get();
}

int main()
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 600, 250, TRUE); // 800 width, 100 height


	thread watchClicks(WatchForClicks);
	thread playAudio(AfkAlert);
	watchClicks.join();
	playAudio.join();
	return 0;
}
