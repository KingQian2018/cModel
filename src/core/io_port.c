/**
 * @file io_port.c
 * @author KisWang (KingQian2018)
 * @brief 引脚核心
 * @version 0.1
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdlib.h> //< MDK
#include <stdio.h>
#include "io_port.h"

#if CM_USE_LOG
#define LOG_TAG "IO PORT"
#endif
#include "cm_log.h"

/// 宏定义，判断是否为有效引脚
#define IS_VALID_AI_PIN(io, p) ((unsigned char)p < (io->I.ANum) && p <= IOPIN_8)
#define IS_VALID_AO_PIN(io, p) ((unsigned char)p < (io->O.ANum) && p <= IOPIN_8)
#define IS_VALID_DI_PIN(io, p) ((unsigned char)p < (io->I.DNum) && p <= IOPIN_8)
#define IS_VALID_DO_PIN(io, p) ((unsigned char)p < (io->O.DNum) && p <= IOPIN_8)

/**
 * @brief IO模块 实例化
 *
 * @param io 指向IO模块的指针
 * @param num IO引脚数量，分别对应AI, DI, AO, DO
 * @return CMODEL_STATUS_OK	IO模块实例化成功
 * @return CMODEL_STATUS_IO_ERR	IO模块实例化失败。由于需要实例化的IO模块非空
 * @return CMODEL_STATUS_IO_FILE
 * 			1. IO模块动态分配空间失败
 * 			2. IO模块动态分配指向模拟量输入引脚空间失败
 * 			3. IO模块动态分配指向数字量输入引脚空间失败
 * 			4. IO模块动态分配指向模拟量输出引脚空间失败
 * 			5. IO模块动态分配模拟量输出引脚空间失败
 * 			6. IO模块动态分配指向数字量输出引脚空间失败
 * 			7. IO模块动态分配数字量输出引脚空间失败
 * @details
 * 			1. 首先判断指向IO模块的指针是否为空
 * 			2. 开辟新的IO模块
 * 			3. 为新开辟的IO模块分配IO引脚个数以及名称
 * 			4. 循环开辟指向输入引脚指针的指针
 * 			5. 循环开辟指向输出引脚指针的指针以及输出引脚空间
 */
CMODEL_STATUS_e IO_Create(IO *io, unsigned char num[4])
{
	if (io[0] != 0)
	{
		LOG_E("IO io is not Null!");
		return CMODEL_STATUS_IO_ERR;
	}
	io[0] = (IO)calloc(1, sizeof(struct IO_t));
	if (io[0] == 0)
	{
		LOG_E("Create IO Filed!");
		return CMODEL_STATUS_IO_FILE;
	}
	io[0]->Flag = CMODEL_STATUS_IO_DONE;

	io[0]->I.ANum = num[0];
	io[0]->I.DNum = num[1];
	io[0]->O.ANum = num[2];
	io[0]->O.DNum = num[3];

	io[0]->I.ppA = (io[0]->I.ANum != 0) ? (float **)calloc(io[0]->I.ANum, sizeof(float *)) : 0;
	if ((io[0]->I.ppA == 0x0) && (io[0]->I.ANum != 0))
	{
		LOG_E("Init I.ppA Error!");
		free(io[0]);
		io[0] = 0;
		return CMODEL_STATUS_IO_FILE;
	}

	io[0]->I.ppD = (io[0]->I.DNum != 0) ? (d_value **)calloc(io[0]->I.DNum, sizeof(d_value *)) : 0;
	if ((io[0]->I.ppD == 0x0) && (io[0]->I.DNum != 0))
	{
		LOG_E("Init I.ppD Error!");
		free(io[0]->I.ppA);
		free(io[0]);
		io[0] = 0;
		return CMODEL_STATUS_IO_FILE;
	}

	io[0]->O.pA = (io[0]->O.ANum != 0) ? (a_value *)calloc(io[0]->O.ANum, sizeof(a_value)) : 0;
	if ((io[0]->O.pA == 0x0) && (io[0]->O.ANum != 0))
	{
		LOG_E("Init O.pA Error!");
		free(io[0]->I.ppA);
		free(io[0]->I.ppD);
		free(io[0]);
		io[0] = 0;
		return CMODEL_STATUS_IO_FILE;
	}

	io[0]->O.pD = (io[0]->O.DNum != 0) ? (d_value *)calloc(io[0]->O.DNum, sizeof(d_value)) : 0;
	if ((io[0]->O.pD == 0x0) && (io[0]->O.DNum != 0))
	{
		LOG_E("Init O.pD Error!");
		free(io[0]->I.ppA);
		free(io[0]->I.ppD);
		free(io[0]->O.pA);
		free(io[0]);
		io[0] = 0;
		return CMODEL_STATUS_IO_FILE;
	}
	return CMODEL_STATUS_OK;
}

/**
 * @brief 删除非空IO模块
 *
 * @param io
 * @return CMODEL_STATUS_e
 */
CMODEL_STATUS_e IO_Deleate(IO io)
{
	if (io == NULL)
	{
		return CMODEL_STATUS_OK;
	}
	free(io->I.ppA);
	free(io->I.ppD);
	free(io->O.pA);
	free(io->O.pD);
	free(io);
	io = NULL;
	return CMODEL_STATUS_OK;
}

/**
 * @brief IO模块输入引脚建立连接
 *
 * @param io IO模块指针
 * @param type IO模块类型枚举量
 * @param pin IO模块引脚类型枚举量
 * @param pValue 指向变量的指针，注意此处其变量类型为空，在程序内部对其进行了处理
 * @return CMODEL_STATUS_OK	引脚建立成功
 * @return CMODEL_STATUS_IO_ERR
 * 		1. 需要建立的引脚非 AI、DI 类型
 * 		2. 需要建立的引脚数量越界
 */
CMODEL_STATUS_e IO_setLink(IO io, const IOTYP_e type, const IOPIN_e pin, void *pValue)
{
	unsigned char myLoca = (unsigned char)pin;
	if (type == IOTYP_DI)
	{
		unsigned char *pD = (unsigned char *)pValue;
		if (myLoca > io->I.DNum - 1)
		{
			LOG_E("DI max is %d, %d is out of range.", io->I.DNum - 1, myLoca);
			return CMODEL_STATUS_IO_ERR;
		}
		io->I.ppD[myLoca] = pD;
	}
	else if (type == IOTYP_AI)
	{
		float *pA = (float *)pValue;
		if (myLoca > io->I.ANum - 1)
		{
			LOG_E("AI max is %d, %d is out of range.", io->I.ANum - 1, myLoca);
			return CMODEL_STATUS_IO_ERR;
		}
		io->I.ppA[myLoca] = pA;
	}
	else
	{
		LOG_E("IO Set Link type Error.");
		return CMODEL_STATUS_IO_ERR;
	}
	return CMODEL_STATUS_OK;
}

/**
 * @brief 获取模块模拟量数值
 *
 * @param io IO模块指针
 * @param num IO模块引脚类型枚举量
 * @param type IO模块类型枚举量
 * @return a_value 对应模拟量引脚数值
 * @note 当引脚越界或者非模拟量类型引脚时，返回为0
 */
a_value IO_GetAValue(IO io, IOPIN_e num, IOTYP_e type)
{
	a_value val = 0.0f;
	if (type == IOTYP_AI)
	{
		if (!IS_VALID_AI_PIN(io, num))
		{
			LOG_W("Get AI[%d] is out of range [%d], and return 0.", num, io->I.ANum - 1);
			return val;
		}
		else
		{
			return (io->I.ppA[num] == 0x00) ? val : io->I.ppA[num][0];
		}
	}
	else if (type == IOTYP_AO)
	{
		if (!IS_VALID_AO_PIN(io, num))
		{
			LOG_W("Get AO[%d] is out of range [%d], and return 0.", num, io->O.ANum - 1);
			return val;
		}
		else
		{
			return (io->O.pA[num] == 0x00) ? val : io->O.pA[num];
		}
	}
	else
	{
		LOG_W("Get type error, and return 0.", num, io->O.ANum - 1);
		return val;
	}
}

/**
 * @brief 获取模块数字量数值
 *
 * @param io IO模块指针
 * @param num IO模块引脚类型枚举量
 * @param type IO模块类型枚举量
 * @return d_value 对应数字量引脚数值
 * @note 当引脚越界或者非数字量类型引脚时，返回为0
 */
d_value IO_GetDValue(IO io, IOPIN_e num, IOTYP_e type)
{
	d_value val = 0;
	if (type == IOTYP_DI)
	{
		if (!IS_VALID_AI_PIN(io, num))
		{
			LOG_W("Get DI[%d] is out of range [%d], and return 0.", num, io->I.DNum - 1);
			return val;
		}
		else
		{
			return (io->I.ppD[num] == 0x00) ? val : io->I.ppD[num][0];
		}
	}
	else if (type == IOTYP_DO)
	{
		if (!IS_VALID_AO_PIN(io, num))
		{
			LOG_W("Get DO[%d] is out of range [%d], and return 0.", num, io->O.DNum - 1);
			return val;
		}
		else
		{
			return (io->O.pD[num] == 0x00) ? val : io->O.pD[num];
		}
	}
	else
	{
		LOG_W("Get type error, and return 0.", num, io->O.ANum - 1);
		return val;
	}
}

/**
 * @brief 获取模块模拟量输出指针
 *
 * @param io IO模块指针
 * @param pin IO模块引脚类型枚举量
 * @return a_value* 对应模块模拟量输出指针
 * @note 当引脚越界时，返回为0
 */
a_value *IO_GetAOPoint(IO io, IOPIN_e pin)
{
	if (!IS_VALID_AO_PIN(io, pin))
	{
		LOG_E("Model: %s Out of range.");
		return 0;
	}
	return &(io->O.pA[pin]);
}

/**
 * @brief 获取模块数字量输出指针
 *
 * @param io IO模块指针
 * @param pin IO模块引脚类型枚举量
 * @return d_value* 对应模块数字量输出指针
 * @note 当引脚越界时，返回为0
 */
d_value *IO_GetDOPoint(IO io, IOPIN_e pin)
{
	if (!IS_VALID_DO_PIN(io, pin))
	{
		LOG_E("Model: %s Out of range.");
		return 0;
	}
	return &io->O.pD[pin];
}

/**
 * @brief 描述：获取模块状态
 *
 * @param io IO模块指针
 * @return CMODEL_STATUS_e 模块状态
 */
CMODEL_STATUS_e IO_GetIOFlg(IO io)
{
	return io->Flag;
}

/**
 * @brief 设置模块模拟量输出值
 *
 * @param io IO模块指针
 * @param pin IO模块引脚类型枚举量
 * @param fVal 输出值
 * @return CMODEL_STATUS_OK	设置成功
 * @return CMODEL_STATUS_IO_ERR	设置出错。模拟量输出引脚越界
 */
CMODEL_STATUS_e IO_SetAOValue(IO io, IOPIN_e pin, a_value fVal)
{
	if (!IS_VALID_AO_PIN(io, pin))
	{
		LOG_E("Model: %s Out of range.");
		return CMODEL_STATUS_IO_ERR;
	}
	io->O.pA[pin] = fVal;
	return CMODEL_STATUS_OK;
}

/**
 * @brief 设置模块数字量输出值
 *
 * @param io IO模块指针
 * @param pin IO模块引脚类型枚举量
 * @param ucVal 输出值
 * @return CMODEL_STATUS_OK	设置成功
 * @return CMODEL_STATUS_IO_ERR	设置出错。数字量输出引脚越界
 */
CMODEL_STATUS_e IO_SetDOValue(IO io, IOPIN_e pin, d_value ucVal)
{
	if (!IS_VALID_DO_PIN(io, pin))
	{
		LOG_E("Model: %s Out of range.");
		return CMODEL_STATUS_IO_ERR;
	}
	io->O.pD[pin] = ucVal;
	return CMODEL_STATUS_OK;
}

/**
 * @brief 显示模块引脚数值
 *
 * @param io 引脚
 */
void IO_ShowALL(IO io)
{
	printf("\n\t======Model AI-%d DI-%d AO-%d DO-%d======\n", io->I.ANum, io->I.DNum, io->O.ANum, io->O.DNum);
	unsigned char _pin_max = io->I.ANum > io->O.ANum ? io->I.ANum : io->O.ANum;
	unsigned char flag = 0;
	for (unsigned char pin = 0; pin < _pin_max; pin++)
	{
		flag = 0;
		printf("\t");
		if (io->I.ppA != NULL)
		{
			if (io->I.ppA[pin] != 0)
			{
				printf("%.3f\t==>A%d\t\t", io->I.ppA[pin][0], pin + 1);
			}
			else
			{
				printf("None\t==>A%d\t\t", pin + 1);
			}
			flag = 1;
		}
		if (pin < io->O.ANum)
		{
			if (flag == 0)
			{
				printf("\t\t\t");
			}
			printf("A%d==>\t%.3f", pin + 1, io->O.pA[pin]);
		}
		printf("\n");
	}
	_pin_max = io->I.DNum > io->O.DNum ? io->I.DNum : io->O.DNum;
	for (unsigned char pin = 0; pin < _pin_max; pin++)
	{
		flag = 0;
		printf("\t");
		if (io->I.ppD != NULL)
		{
			if (io->I.ppD[pin] != 0)
			{
				printf("%d\t-->D%d\t\t", io->I.ppD[pin][0], pin + 1);
			}
			else
			{
				printf("None\t-->D%d\t\t", pin + 1);
			}
		}
		if (pin < io->O.DNum)
		{
			if (flag == 0)
			{
				printf("\t\t\t");
			}
			printf("D%d-->\t%d", pin + 1, io->O.pD[pin]);
		}
		printf("\n");
	}
}

/**
 * @brief 显示模块指定引脚数值
 *
 * @param io IO模块指针
 * @param type IO模块类型枚举量
 * @param pin IO模块引脚类型枚举量
 */
void IO_ShowPin(IO io, IOTYP_e type, IOPIN_e pin)
{
	unsigned char myPinMax = 0;
	unsigned char myPin = pin;
	myPinMax = (type == IOTYP_AI) ? io->I.ANum - 1 : (type == IOTYP_AO) ? io->O.ANum - 1
												 : (type == IOTYP_DI)	? io->I.DNum - 1
												 : (type == IOTYP_DO)	? io->O.DNum - 1
																		: IOTYP_RESE;

	if (myPinMax == IOTYP_RESE)
	{
		LOG_E("Show type %d Error.", myPinMax);
	}
	if (pin > IOPIN_8 || pin > (IOPIN_e)myPinMax)
	{
		LOG_E("Search %d IO is out of range.", (unsigned char)pin);
		return;
	}
	printf("\n\t======IO======\n\t");
	switch (type)
	{
	case IOTYP_AI:
		if (io->I.ppA[myPin] != 0)
		{
			printf("%.3f\t==>A%d\t\t", io->I.ppA[myPin][0], myPin + 1);
		}
		else
		{
			printf("None\t==>A%d\t\t", myPin + 1);
		}
		break;
	case IOTYP_DI:
		if (io->I.ppD[myPin] != 0)
		{
			printf("%d\t-->D%d\t\t", io->I.ppD[myPin][0], myPin + 1);
		}
		else
		{
			printf("None\t==>D%d\t\t", myPin + 1);
		}
		break;
	case IOTYP_AO:
		printf("A%d==>\t%.3f\n", myPin + 1, io->O.pA[myPin]);
		break;
	case IOTYP_DO:
		printf("D%d-->\t%d\n", myPin + 1, io->O.pD[myPin]);
		break;
	default:
		break;
	}
}
