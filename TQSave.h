#ifndef __TQ_SAVE__
#define __TQ_SAVE__

#define OBJECT_NUMBER	10
#define	NUMBER_MONEY	0
#define NUMBER_LEVEL	1
#define NUMBER_EXP		2
#define NUMBER_DEATH	3

#ifdef __cplusplus
extern "C"{
#else
#endif

extern int RetrieveObjects(char* databuffer, unsigned long datalen, int* object_pos);


#ifdef __cplusplus
}
#endif

#endif