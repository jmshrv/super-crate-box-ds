/*---------------------------------------------------------------------------------

	$Id: main.cpp,v 1.13 2008-12-02 20:21:20 dovoto Exp $

	Simple console print demo
	-- dovoto


---------------------------------------------------------------------------------*/
#include <nds.h>

#include <stdio.h>

volatile int frame = 0;

//---------------------------------------------------------------------------------
void Vblank()
{
	//---------------------------------------------------------------------------------
	frame++;
}

//---------------------------------------------------------------------------------
int main(void)
{
	//---------------------------------------------------------------------------------
	touchPosition touchXY;
	int x = 0, y = 0;

	irqSet(IRQ_VBLANK, Vblank);

	consoleDemoInit();

	iprintf("      Hello DS dev'rs\n");
	iprintf("     \x1b[32mwww.devkitpro.org\n");
	iprintf("   \x1b[32;1mwww.drunkencoders.com\x1b[39m");

	while (1)
	{

		swiWaitForVBlank();
		scanKeys();
		int keys = keysHeld();
		if (keys & KEY_START)
			break;

		if (keys & KEY_UP)
			if (y < SCREEN_HEIGHT)
				y++;
		if (keys & KEY_DOWN)
			if (y > 0)
				y--;
		if (keys & KEY_LEFT)
			if (x > 0)
				x--;
		if (keys & KEY_RIGHT)
			if (x < SCREEN_WIDTH)
				x++;

		touchRead(&touchXY);

		// print at using ansi escape sequence \x1b[line;columnH
		iprintf("\x1b[10;0HFrame = %d", frame);
		iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
		iprintf("X = %d\n", x);
		iprintf("Y = %d\n", y);
	}

	return 0;
}
