#include <nds.h>
#include <stdio.h>
// #include "../include/nflib/nflib/include/nf_lib.h"
#include <nf_lib.h>
#include <stdlib.h>
#include <string.h>
#include <nds/ndstypes.h>

#include "entity.c"
#include "linked_list.h"

int main(void)
{
	int entityCount = 1;
	int frameCount = 0;
	JamieLinkedList *entities = initialise_linked_list();

	Sprite *mainCharacter = malloc(sizeof(Sprite));

	mainCharacter->screen = 0;
	mainCharacter->id = 0;
	mainCharacter->gfx = 0;
	mainCharacter->pal = 0;
	mainCharacter->x = 0;
	mainCharacter->y = SCREEN_HEIGHT - 16 - 8;
	mainCharacter->isFlipped = 0;
	mainCharacter->velocityX = 0;
	mainCharacter->velocityY = 0;
	mainCharacter->shouldFall = 1;

	// Turn on MODE 0 on the Top Screen
	NF_Set2D(0, 0);

	// Enable debugging mode on the bottom screen
	consoleDemoInit();

	// Set the Root Folder
	NF_SetRootFolder("NITROFS");

	// Initialize the Tiled Backgrounds System on the top screen
	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);

	// Initialize the Tiled Sprites System on the top screen
	NF_InitSpriteBuffers();
	NF_InitSpriteSys(0);

	// Load and Create the Tiled Background
	NF_LoadTiledBg("Background-With-Map", "Background-With-Map", 256, 256);
	NF_CreateTiledBg(0, 3, "Background-With-Map");

	// Init collision map stuff
	NF_InitCmapBuffers();
	NF_LoadColisionMap("Background-With-Map-Cmap", 0, 256, 256);

	// Load our Tiled Sprite
	NF_LoadSpriteGfx("Main-Character", 0, 16, 16);
	NF_LoadSpritePal("Main-Character", 0);
	NF_LoadSpriteGfx("Bullet", 1, 8, 8);
	NF_LoadSpritePal("Bullet", 1);
	NF_LoadSpriteGfx("Enemy", 2, 16, 16);
	NF_LoadSpritePal("Enemy", 2);
	// Transfer our sprite to VRAM
	NF_VramSpriteGfx(0, 0, 0, false);
	NF_VramSpritePal(0, 0, 0);
	NF_VramSpriteGfx(0, 1, 1, false);
	NF_VramSpritePal(0, 1, 1);
	NF_VramSpriteGfx(0, 2, 2, false);
	NF_VramSpritePal(0, 2, 2);

	// Create the Sprite!
	NF_CreateSprite(0, 0, 0, 0, mainCharacter->x, mainCharacter->y);

	while (1)
	{
		frameCount++;
		// iprintf("%d", NF_GetTile(0, mainCharacter->x, mainCharacter->y + 8));
		scanKeys();
		int keys = keysHeld();

		if (keys & KEY_LEFT)
		{
			int newSpeed = mainCharacter->velocityX - 1;

			if (newSpeed >= -5)
			{
				mainCharacter->velocityX = newSpeed;
			}
		}
		else
		{
			int newSpeed = mainCharacter->velocityX + 1;

			if (newSpeed <= 0)
			{
				mainCharacter->velocityX = newSpeed;
			}
		}
		if (keys & KEY_RIGHT)
		{
			int newSpeed = mainCharacter->velocityX + 1;

			if (newSpeed <= 5)
			{
				mainCharacter->velocityX = newSpeed;
			}
		}
		else
		{
			int newSpeed = mainCharacter->velocityX - 1;

			if (newSpeed >= 0)
			{
				mainCharacter->velocityX = newSpeed;
			}
		}
		if (keys & KEY_B)
		{
			if (NF_GetTile(0, mainCharacter->x, mainCharacter->y + 8) != 1)
			{
				mainCharacter->velocityY = 15;
			}
		}
		else if (NF_GetTile(0, mainCharacter->x, mainCharacter->y + 8) != 1)
			mainCharacter->velocityY = 0;
		else if (mainCharacter->velocityY > -8)
			mainCharacter->velocityY--;

		mainCharacter->x = mainCharacter->x + mainCharacter->velocityX;
		int newY = mainCharacter->y - mainCharacter->velocityY;
		if (newY >= 0)
		{
			mainCharacter->y = newY;
		}

		if (mainCharacter->velocityY > 0)
			mainCharacter->velocityY--;

		if (mainCharacter->isFlipped && mainCharacter->velocityX > 0)
		{
			NF_HflipSprite(mainCharacter->screen, mainCharacter->id, 0);
			mainCharacter->isFlipped = 0;
		}
		else if (!mainCharacter->isFlipped && mainCharacter->velocityX < 0)
		{
			NF_HflipSprite(mainCharacter->screen, mainCharacter->id, 1);
			mainCharacter->isFlipped = 1;
		}

		NF_MoveSprite(mainCharacter->screen, mainCharacter->id, mainCharacter->x, mainCharacter->y);

		// Bullet spawning
		keys = keysDown();
		if (keys & KEY_X)
		{
			Sprite *newBullet = malloc(sizeof(Sprite));

			newBullet->id = entityCount;
			newBullet->gfx = 1;
			newBullet->pal = 1;
			newBullet->x = mainCharacter->x + 8;
			newBullet->y = mainCharacter->y;
			newBullet->isFlipped = 0;
			newBullet->velocityX = mainCharacter->isFlipped ? -8 : 8;
			newBullet->velocityY = 0;
			newBullet->shouldFall = 0;

			NF_CreateSprite(0, newBullet->id, newBullet->gfx, newBullet->pal, newBullet->x, newBullet->y);

			append_linked_list(entities, newBullet);
			entityCount++;
		}

		if (frameCount % 120 == 0)
		{
			Sprite *newEnemy = malloc(sizeof(Sprite));

			newEnemy->id = entityCount;
			newEnemy->gfx = 2;
			newEnemy->pal = 2;
			newEnemy->x = SCREEN_WIDTH / 2;
			newEnemy->y = 10;
			newEnemy->isFlipped = 10;
			newEnemy->velocityX = 10;
			newEnemy->velocityY = 0;
			newEnemy->shouldFall = 1;

			NF_CreateSprite(0, newEnemy->id, newEnemy->gfx, newEnemy->pal, newEnemy->x, newEnemy->y);

			append_linked_list(entities, newEnemy);
			entityCount++;
		}

		// int count = 0;
		// for (JamieNode *node = entities->head; node != NULL; node = node->next)
		// 	count++;

		// iprintf("%d", count);

		for (JamieNode *node = entities->head; node != NULL; node = node->next)
		{
			Sprite *entity = node->data;

			entity->x = entity->x + entity->velocityX;

			if (entity->shouldFall)
			{
				iprintf("%d", NF_GetTile(0, entity->x, entity->y + 8));
				if (NF_GetTile(0, entity->x, entity->y + 8) != 1)
					entity->velocityY = 0;
				else if (entity->velocityY > -8)
					entity->velocityY--;

				int newY = entity->y - entity->velocityY;
				if (newY >= 0)
				{
					entity->y = newY;
				}
			}

			if (entity->x < 0 || entity->x > SCREEN_WIDTH)
			{
				if (entity->shouldFall)
				{
					entity->isFlipped = !entity->isFlipped;
					NF_HflipSprite(0, entity->id, entity->isFlipped);
					entity->velocityX = -entity->velocityX;
					NF_MoveSprite(0, entity->id, entity->x, entity->y);
				}
				else
				{
					NF_DeleteSprite(0, entity->id);
					free(entity);

					node->prev->next = node->next;
					node->next->prev = node->prev;

					if (node == entities->head)
						entities->head = node->next;
					if (node == entities->tail)
						entities->tail = node->prev;
					free_node(node);
				}
			}
			else
			{
				NF_MoveSprite(0, entity->id, entity->x, entity->y);
			}

			if (entity->y > SCREEN_HEIGHT)
			{
				NF_DeleteSprite(0, entity->id);
				free(entity);

				node->prev->next = node->next;
				node->next->prev = node->prev;

				if (node == entities->head)
					entities->head = node->next;
				if (node == entities->tail)
					entities->tail = node->prev;
				free_node(node);
			}
		}

		// Update NF OAM Settings
		NF_SpriteOamSet(0);
		swiWaitForVBlank();
		// Update OAM!
		oamUpdate(&oamMain);
		swiWaitForVBlank();
	}

	return 0;
}
