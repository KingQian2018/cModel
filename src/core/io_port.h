#ifndef __IO_PORT_H
#define __IO_PORT_H

#include "cmodel.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef enum
	{
		IOTYP_AI, // 模拟量输入
		IOTYP_DI, // 数字量输入
		IOTYP_AO, // 模拟量输出
		IOTYP_DO, // 数字量输出
		IOTYP_RESE = 0xFF
	} IOTYP_e; // IO类型枚举量

	typedef enum
	{
		IOPIN_1, // 1# 引脚
		IOPIN_2, // 2# 引脚
		IOPIN_3, // 3# 引脚
		IOPIN_4, // 4# 引脚
		IOPIN_5, // 5# 引脚
		IOPIN_6, // 6# 引脚
		IOPIN_7, // 7# 引脚
		IOPIN_8, // 8# 引脚
		IOPIN_RESE = 0xFF
	} IOPIN_e; // IO引脚枚举量

#define IOSTUS_ERROR \
	T(NONE)          \
	T(DONE)          \
	T(OK)            \
	T(ERR)           \
	T(FILE)          \
	T(RESE)

#define T(t) IOSTUS_##t,
	typedef enum
	{
		IOSTUS_ERROR
	} IOSTUS_e; // IO状态枚举量
#undef T

	typedef struct IO_t *IO; // IO模块指针

	IOSTUS_e IO_Create(IO *io, const char *pName, unsigned char num[4]);
	IOSTUS_e IO_SetLink(IO io, const IOTYP_e type, const IOPIN_e pin, void *pValue);
	a_value IO_GetAValue(IO io, IOPIN_e num, IOTYP_e Type);
	d_value IO_GetDValue(IO io, IOPIN_e num, IOTYP_e type);
	IOSTUS_e IO_GetIOFlg(IO io);
	IOSTUS_e IO_SetAOValue(IO io, IOPIN_e pin, float fVal);
	IOSTUS_e IO_SetDOValue(IO io, IOPIN_e pin, unsigned char ucVal);
	void IO_ShowALL(IO io);
	void IO_ShowPin(IO io, IOTYP_e Type, IOPIN_e pin);

#ifdef __cplusplus
}
#endif

#endif
