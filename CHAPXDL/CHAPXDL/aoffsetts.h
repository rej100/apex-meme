#pragma once

#include "pch.h"

namespace off
{

	//DWORD64 NUM_ENT_ENTRIES = 0x10000;
	DWORD64 NUM_ENT_ENTRIES = 128;

	DWORD64 cl_entitylist = 0x1a1e3b8;
	DWORD64 LocalPlayer = 0x1dcf5e8;

	DWORD64 ViewRender = 0x7544150;
	DWORD64 ViewMatrix = 0x11a210;

	DWORD64 m_ModelName = 0x0030;
	DWORD64 m_vecAbsOrigin = 0x014c; //[DataMap.C_BaseEntity]
	DWORD64 m_shieldHealth = 0x0170;
	DWORD64 m_shieldHealthMax = 0x0174;
	DWORD64 m_iHealth = 0x0438;
	DWORD64 m_iTeamNum = 0x0448;
	DWORD64 m_iName = 0x0589;
	DWORD64 m_bConstrainBetweenEndpoints = 0xF38;

	
}

//namespace off
//{
//
//	//DWORD64 NUM_ENT_ENTRIES = 0x10000;
//	DWORD64 NUM_ENT_ENTRIES = 128;
//
//	DWORD64 cl_entitylist = 0x19fbf98;
//	DWORD64 LocalPlayer = 0x1dacd48;
//
//	DWORD64 ViewRender = 0x7519c20;
//	DWORD64 ViewMatrix = 0x11a210;
//
//	DWORD64 m_ModelName = 0x0030;
//	DWORD64 m_vecAbsOrigin = 0x014c;
//	DWORD64 m_shieldHealth = 0x0170;
//	DWORD64 m_shieldHealthMax = 0x0174;
//	DWORD64 m_iHealth = 0x0438;
//	DWORD64 m_iTeamNum = 0x0448;
//	DWORD64 m_iName = 0x0589;
//	DWORD64 m_bConstrainBetweenEndpoints = 0x0f38;
//
//
//}