// AFK Assistant.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>




/*
auto start = std::chrono::high_resolution_clock::now();


auto end = std::chrono::high_resolution_clock::now();


double x = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
*/


using namespace std;

string windowName;
char windowCheck;


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



int main()
{

	double minutes;
	int timeCheck = 1;
	int oldTimeCheck = 0;
	cout << "Welcome to Jamboni's AFK assistant." << endl << endl;
	cout << "Used to alert you when you have been inactive in the selected window for the specified time." << endl << endl;
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
			//cout << "Click" << endl;
		}

		auto end = std::chrono::high_resolution_clock::now();
		double timeSince = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		if (timeSince >= minutes) {
			Beep(325, 100);
		}
		timeCheck = (int)timeSince;
		if (timeCheck > oldTimeCheck) {
			cout << timeSince << ": Seconds since last click." << endl;
			oldTimeCheck = timeSince;
		}
		Sleep(10);
	}
	cin.get();
	return 0;
}
