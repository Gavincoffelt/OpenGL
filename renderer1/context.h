#pragma once
class context
{

	struct GLFWwindow * window;


public:
	// initialize screen width, height.
	bool init(int width, int height, const char * title);
	// Update screen.
	void tick();
	// Cleans up unused objects. 
	void term();
	// Clears the screen.
	void clear();


	bool shouldClose() const;


};