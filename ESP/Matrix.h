#pragma once
#include "Vector.h"
#include "Offsets.h"


bool WorldToScreen(Vec3 In, Vec3& Out, ViewMatrix vm, int ScreenX, int ScreenY)
{
	Out.m_xPos = In.getxPos() * vm.m_Matrix[0] + In.getyPos() * vm.m_Matrix[4] + In.getzPos() * vm.m_Matrix[8] + vm.m_Matrix[12];
	Out.m_yPos = In.getxPos() * vm.m_Matrix[1] + In.getyPos() * vm.m_Matrix[5] + In.getzPos() * vm.m_Matrix[9] + vm.m_Matrix[13];
	Out.m_zPos = In.getxPos() * vm.m_Matrix[2] + In.getyPos() * vm.m_Matrix[6] + In.getzPos() * vm.m_Matrix[10] + vm.m_Matrix[14];
	float w = In.getxPos() * vm.m_Matrix[3] + In.getyPos() * vm.m_Matrix[7] + In.getzPos() * vm.m_Matrix[11] + vm.m_Matrix[15];

	if (w < 0.0)
		return false;

	Out.m_xPos /= w;
	Out.m_yPos /= w;
	Out.m_zPos /= w;

	Out.m_xPos *= ScreenX / 2.0f;
	Out.m_xPos += ScreenX / 2.0f;

	Out.m_yPos *= -ScreenY / 2.0f;
	Out.m_yPos += ScreenY / 2.0f;

	return true;
}
