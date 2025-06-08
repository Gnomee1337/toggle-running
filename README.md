# Toggle-running
A Windows C++ utility that simulates keyboard input (specifically toggling the "W" key for movement) in a target game window, with an on-screen overlay and sound feedback. Designed for games like "Stalker2-Win64-Shipping.exe".

# Features
* Keyboard Simulation: Press F3 to toggle automatic "W" key holding (start/stop movement).
* Overlay: Displays the current running state ("Running On"/"Running Off") on top of the game window.
* Sound Feedback: Plays a sound when toggling the running state (using hello.wav and bye.wav).
* Process Detection: Automatically finds the target game window by executable name.
* Admin Check: Verifies the application is running with administrator privileges.

# How It Works
1. The app hides its console window and checks for admin rights.
2. It locates the target game process and window.
3. An overlay window is created and positioned over the game.
4. Pressing F3 toggles the simulated "W" key press:
5. Overlay updates to show the current state.
6. Plays a sound for on/off.
7. Sends input to the game window.

# Usage
1. Start the target game (e.g., "Stalker2-Win64-Shipping.exe").
2. Run this application as administrator.
3. Press F3 to toggle auto-move ("W" key) on/off.
4. The overlay will show the current state, and a sound will play on each toggle.
