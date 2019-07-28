#ifndef _FLASHSTORE_H_
#define _FLASHSTORE_H_

#include "stdbool.h"
#include "stm32f10x.h"
#include "settings.h"

bool readStoredPara(void);
void storePara(void);

#endif
