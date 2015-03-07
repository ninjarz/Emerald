#pragma once
#ifndef _EE_MEMORYLEAK_H_
#define _EE_MEMORYLEAK_H_

#ifdef _EE_MEMORYLEAK_ON_

int before_main();
int after_main();

#pragma data_seg(".CRT$XIU")
static int(*before[])() = { before_main };
#pragma data_seg(".CRT$XPU")
static int(*after[])() = { after_main };
#pragma data_seg()

#endif

#endif