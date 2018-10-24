#include "stdafx.h"
#include "TQSave.h"

/*	function	:	get object's position in the savefile
	parameters	:
		databuffer[read]	:	all data in the file
		datalen[read]		:	bytes of data
		object_pos[write]	:	retrieved object's position in the file
	return		:   0 finish
*/

int RetrieveObjects(char* databuffer, unsigned long datalen, int* object_pos)
{
	char* object_text[OBJECT_NUMBER];
	int object_strlen[OBJECT_NUMBER];

	*object_text = (char*)malloc(10 * 100);

	object_text[NUMBER_MONEY] = "money";
	object_text[NUMBER_EXP] = "currentStats.experiencePoints";
	object_text[NUMBER_LEVEL] = "currentStats.charLevel";
	object_text[NUMBER_DEATH] = "numberOfDeath";

	memset(object_pos, 0, sizeof(object_pos));

	unsigned long loopcount = 864;	//skip header(about) of save
	int objectcount = 0;
	int objectnum = sizeof(*object_text);
	for (; objectcount < objectnum; objectcount++)
	{
		object_pos[objectcount] = 0;
		object_strlen[objectcount] = strnlen_s(object_text[objectcount], 35);
		for (; loopcount < datalen; loopcount++)
		{
			if (databuffer[loopcount] == object_text[objectcount][object_pos[objectcount]])
			{
				object_pos[objectcount]++;
				if (object_pos[objectcount] == object_strlen[objectcount])
				{
					object_pos[objectcount] = loopcount + 1;	// found
					break;
				}
			}
			else
				object_pos[objectcount] = 0;
		}
	}
	return 0;
}
