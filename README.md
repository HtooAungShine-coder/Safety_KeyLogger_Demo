# Safety_KeyLogger_Demo
This is a "Safe" keylogger to demonstrate how the real-world malicious keylogger would function. Close to 60%. This is "Educational" purpose only and It depends on your ethical overview to use.


-- This is coded in C++ . 

You can see the mind map here !


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

