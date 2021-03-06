#ifndef __IO_PORT_H
#define __IO_PORT_H

#include "cmodel.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define IS_VALID_IO(io) (IOSTUS_DONE == IO_GetIOFlg(io))

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
		IO_ADTYPE_A, // 模拟量
		IO_ADTYPE_D, // 数字量
	} IO_ADTYPE_e;

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

	typedef struct IO_t *IO; // IO模块指针

	/// IO引脚结构体
	typedef struct
	{
		a_value **ppA;		//< 指向模拟量指针的指针
		d_value **ppD;		//< 指向数字量指针的指针
		unsigned char ANum; //< 模拟量数量
		unsigned char DNum; //< 数字量数量
	} InPort_t;

	typedef struct
	{
		a_value *pA;		//< 指向模拟量指针的指针
		d_value *pD;		//< 指向数字量指针的指针
		unsigned char ANum; //< 模拟量数量
		unsigned char DNum; //< 数字量数量
	} OutPort_t;

	/// IO模块结构体
	struct IO_t
	{
		CMODEL_STATUS_e Flag; //< IO模块状态
		InPort_t I;	   //< IO模块输入引脚
		OutPort_t O;   //< IO模块输出引脚
	};
	CMODEL_STATUS_e IO_Create(IO *io, unsigned char num[4]);
	CMODEL_STATUS_e IO_Deleate(IO io);
	CMODEL_STATUS_e IO_setLink(IO io, const IOTYP_e type, const IOPIN_e pin, void *pValue);
	a_value *IO_GetAOPoint(IO io, IOPIN_e pin);
	a_value IO_GetAValue(IO io, IOPIN_e num, IOTYP_e Type);
	d_value *IO_GetDOPoint(IO io, IOPIN_e pin);
	d_value IO_GetDValue(IO io, IOPIN_e num, IOTYP_e type);
	CMODEL_STATUS_e IO_GetIOFlg(IO io);
	CMODEL_STATUS_e IO_SetAOValue(IO io, IOPIN_e pin, float fVal);
	CMODEL_STATUS_e IO_SetDOValue(IO io, IOPIN_e pin, unsigned char ucVal);
	void IO_ShowALL(IO io);
	void IO_ShowPin(IO io, IOTYP_e Type, IOPIN_e pin);

#ifdef __cplusplus
}
#endif

#endif
