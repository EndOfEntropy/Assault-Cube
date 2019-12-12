#pragma once
#ifndef OFFSETS_H
#define OFFSETS_H
#include <Windows.h>


struct AcPlayer {
	
	uintptr_t	playerBaseAddress = { 0x509B74 };	// Currently the playerBaseAddress
//	uintptr_t   unknown1[0x4] { 0x0 };				// +0x0		BYTE
	uintptr_t	headPosition { 0x4 };				// +0x4		POINT3D?
//	uintptr_t	unknown2[0x24] { 0x10 };			// +0x10	BYTE
	uintptr_t	position { 0x34 };					// +0x34	POINT3D?
	uintptr_t	view { 0x40 };						// +0x40	POINT3D?
//	uintptr_t   unknown3[0x8] { 0x58 };				// +0x58	BYTE
	uintptr_t   jumpFallSpeed { 0x54 };				// +0x54	INT32
	uintptr_t   noClip { 0x58 };					// +0x58	FLOAT
//	uintptr_t   unknown4[0x14] { 0x5C };			// +0x5C	BYTE
	uintptr_t   isImmobile { 0x70 };				// +0x70	INT32
//	uintptr_t   unknown5[0xE] { 0x74 };				// +0x74	BYTE
	uintptr_t   state { 0x82 };						// +0x82	INT8
//	uintptr_t   unknown6[0x75] { 0x83 };			// +0x83	BYTE
	uintptr_t   hp { 0xF8 };						// +0xF8	INT32
	uintptr_t   armor { 0xFC };						// +0xFC	INT32
//	uintptr_t   unknown7[0xC] { 0x100 };			// +0x100	BYTE
	uintptr_t	dualPistolEnabled { 0x10C };		// +0x10C	INT8
//	uintptr_t   unknown8[0x7] { 0x10D };            // +0x10D	BYTE
	uintptr_t   pistolReserveAmmos { 0x114 };       // +0x114	INT32
	uintptr_t   carabineReserveAmmos { 0x118 };     // +0x118	INT32
	uintptr_t   shotgunReserveAmmos { 0x11C };      // +0x11C	INT32
	uintptr_t   smgReserveAmmos { 0x120 };          // +0x120	INT32
	uintptr_t   sniperRifleReserveAmmos { 0x124 };  // +0x124	INT32
	uintptr_t   assaultRifleReserveAmmos { 0x128 }; // +0x128	INT32
//	uintptr_t   unknown9[0x8] { 0x12C };            // +0x12C	BYTE
	uintptr_t   doublePistolReserveAmmos { 0x134 }; // +0x134	INT32
//	uintptr_t   unknown10[0x4] { 0x138 };           // +0x138	BYTE
	uintptr_t   pistolLoadedAmmos { 0x13C };        // +0x13C	INT32
	uintptr_t   carabineLoadedAmmos { 0x140 };      // +0x140	INT32
	uintptr_t   shotgunLoadedAmmos { 0x144 };       // +0x144	INT32
	uintptr_t   smgLoadedAmmos { 0x148 };           // +0x148	INT32
	uintptr_t   sniperRifleLoadedAmmos { 0x14C };   // +0x14C	INT32
	uintptr_t   assaultRifleLoadedAmmos { 0x150 };  // +0x150	INT32
//	uintptr_t   unknown11[0x4] { 0x154 };           // +0x154	BYTE
	uintptr_t   grenades { 0x158 };                 // +0x158	INT32
	uintptr_t   doublePistolLoadedAmmos { 0x15C };  // +0x15C	INT32
	uintptr_t   knifeSlashDelay { 0x160 };          // +0x160	INT32
	uintptr_t   pistolShootDelay { 0x164 };         // +0x164	INT32
	uintptr_t   carabineShootDelay{ 0x168 };		// +0x168	INT32
	uintptr_t   shotgunShootDelay { 0x16C };        // +0x16C	INT32
	uintptr_t   smgShootDelay { 0x170 };            // +0x170	INT32
	uintptr_t   sniperRifleShootDelay { 0x174 };    // +0x174	INT32
	uintptr_t   assaultRifleShootDelay { 0x178 };   // +0x178	INT32
//	uintptr_t   unknown12[0x8] { 0x17C };           // +0x17C	BYTE
	uintptr_t   doublePistolShootDelay { 0x184 };   // +0x184	INT32
//	uintptr_t   unknown13[0x7C] { 0x188 };          // +0x188	BYTE
	uintptr_t   numberOfDeaths { 0x204 };           // +0x204	INT32
//	uintptr_t   unknown14[0x1D] { 0x208 };          // +0x208	BYTE
	uintptr_t   nickname[16] { 0x225 };             // +0x225	CHAR
//	uintptr_t   unknown15[0xF7] { 0x235 };          // +0x235	BYTE
	uintptr_t   team { 0x32C };                     // +0x32C	INT8
};

/*
	BYTE    unknown1[0x4];            // +0x0
	POINT3D headPosition;             // +0x4
	BYTE    unknown2[0x24];           // +0x10
	POINT3D position;                 // +0x34
	POINT3D view;                     // +0x40
	BYTE    unknown3[0x8];            // +0x58
	INT32   jumpFallSpeed;            // +0x54
	FLOAT   noClip;                   // +0x58
	BYTE    unknown4[0x14];           // +0x5C
	INT32   isImmobile;               // +0x70
	BYTE    unknown5[0xE];            // +0x74
	INT8    state;                    // +0x82
	BYTE    unknown6[0x75];           // +0x83
	INT32   hp;                       // +0xF8
	INT32   armor;                    // +0xFC
	BYTE    unknown7[0xC];            // +0x100
	INT8    dualPistolEnabled;        // +0x10C
	BYTE    unknown8[0x7];            // +0x10D
	INT32   pistolReserveAmmos;       // +0x114
	INT32   carabineReserveAmmos;     // +0x118
	INT32   shotgunReserveAmmos;      // +0x11C
	INT32   smgReserveAmmos;          // +0x120
	INT32   sniperRifleReserveAmmos;  // +0x124
	INT32   assaultRifleReserveAmmos; // +0x128
	BYTE    unknown9[0x8];            // +0x12C
	INT32   doublePistolReserveAmmos; // +0x134
	BYTE    unknown10[0x4];           // +0x138
	INT32   pistolLoadedAmmos;        // +0x13C
	INT32   carabineLoadedAmmos;      // +0x140
	INT32   shotgunLoadedAmmos;       // +0x144
	INT32   smgLoadedAmmos;           // +0x148
	INT32   sniperRifleLoadedAmmos;   // +0x14C
	INT32   assaultRifleLoadedAmmos;  // +0x150
	BYTE    unknown11[0x4];           // +0x154
	INT32   grenades;                 // +0x158
	INT32   doublePistolLoadedAmmos;  // +0x15C
	INT32   knifeSlashDelay;          // +0x160
	INT32   pistolShootDelay;         // +0x164
	INT32   carabineShootDelay;       // +0x168
	INT32   shotgunShootDelay;        // +0x16C
	INT32   smgShootDelay;            // +0x170
	INT32   sniperRifleShootDelay;    // +0x174
	INT32   assaultRifleShootDelay;   // +0x178
	BYTE    unknown12[0x8];           // +0x17C
	INT32   doublePistolShootDelay;   // +0x184
	BYTE    unknown13[0x7C];          // +0x188
	INT32   numberOfDeaths;           // +0x204
	BYTE    unknown14[0x1D];          // +0x208
	CHAR    nickname[16];             // +0x225
	BYTE    unknown15[0xF7];          // +0x235
	INT8    team;                     // +0x32C
*/

#endif