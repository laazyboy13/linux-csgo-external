#ifndef __TYPES_H_
#define __TYPES_H_

struct Vector {
	float x;
	float y;
	float z;
};

struct Color {
	unsigned char _color[4];
};

struct Vector2D {
	float x, y;
};

struct QAngle {
	float x; // Pitch
	float y; // Yaw
	float z; // Roll
};

struct Entity {
	char __buf_0x00[0x88]; // 0x0
	int m_bIsAutoaimTarget; // 0x88
	unsigned int m_iEntityId; // 0x8C
	char __buf_0x8C[0x10]; // 0x90
	int m_clrRender; // 0xA0
	int m_cellbits; // 0xA4
	char __buf_0xA8[0x4]; // 0xA8
	int m_cellX; // 0xAC
	int m_cellY; // 0xB0
	int m_cellZ; // 0xB4
	char __buf_0xB8[0xC]; // 0xB8
	Vector m_vecAbsVelocity; // 0xC4
	Vector m_vecAbsOrigin; // 0xD0
	Vector m_vecOrigin; // 0xDC
	Vector m_vecAngVelocity; // 0xE8
	QAngle m_angAbsRotation; // 0xF4
	QAngle m_angRotation; // 0x100
	float m_flGravity; // 0x10C
	float m_flProxyRandomValue; // 0x110
	int m_iEFlags; // 0x114
	char __buf_0x11A[0x4];
	unsigned char m_nWaterType; // 0x118
	unsigned char m_bDormant; // 0x119
	char __buf_0x11ssA[0xA]; // 0x11A
	float m_fEffects; // 0x11C
	int m_iTeamNum; // 0x120
	int m_iPendingTeamNum; // 0x124
	int m_iHealth; // 0x12C
	unsigned long m_fFlags; // 0x130
	//char __buf_0x130[0x8]; // 0x134
	Vector m_vecViewOffset; // 0x134
	Vector m_vecVelocity; // 0x140
	Vector m_vecBaseVelocity; // 0x14C
	QAngle m_angNetworkAngles; // 0x158
	Vector m_vecNetworkOrigin; // 0x164
};

#endif
