/********************************************************************
created:	2012-07-02
author:		Fish (于国平)
summary:	日志
*********************************************************************/
#pragma once
#include "FmConfig.h"

NS_FM_BEGIN

#define FmLog( ... ) printf( __VA_ARGS__ );printf("\n")

NS_FM_END