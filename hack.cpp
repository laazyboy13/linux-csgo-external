#include "hack.h"

struct iovec g_remote[1024], g_local[1024];
struct hack::GlowObjectDefinition_t g_glow[1024];

int count = 0;
unsigned char spotted = 1;

void NoFlash(remote::Handle* csgo, remote::MapModuleMemoryRegion* client, unsigned long localPlayer) {
	float fFlashAlpha = 70.0f;
	float fFlashAlphaFull = 255.0f;
	float fFlashAlphaFromGame = 0.0f;

	csgo->Read((void*) (localPlayer+0xABE4), &fFlashAlphaFromGame, sizeof(float));
	if ((fFlashAlphaFromGame > fFlashAlpha) && (csgo->m_bShouldNoFlash))
		csgo->Write((void*) (localPlayer+0xABE4), &fFlashAlpha, sizeof(float));
	else if ((fFlashAlphaFromGame < fFlashAlphaFull) && (!csgo->m_bShouldNoFlash))
		csgo->Write((void*) (localPlayer+0xABE4), &fFlashAlphaFull, sizeof(float));
}

void hack::Bhop(remote::Handle* csgo, remote::MapModuleMemoryRegion* client, Display* display) {
	if (!csgo || !client)
		return;

	unsigned long localPlayer = 0;

	csgo->Read((void*) csgo->m_addressOfLocalPlayer, &localPlayer, sizeof(long));

	unsigned int onGround = 0;
	csgo->Read((void*) (localPlayer+0x134+0x4), &onGround, sizeof(int));

	onGround = onGround & (1 << 0);

	if (onGround == 1 && csgo->m_bShouldBHop && csgo->m_bBhopEnabled) {
		unsigned int jump = 5;
		csgo->Write((void*) (csgo->m_oAddressOfForceJump), &jump, sizeof(int));
		this_thread::sleep_for(chrono::milliseconds(2));
		jump = 4;
		csgo->Write((void*) (csgo->m_oAddressOfForceJump), &jump, sizeof(int));
	}
}

void hack::Glow(remote::Handle* csgo, remote::MapModuleMemoryRegion* client) {
	if (!csgo || !client)
		return;

	// Reset
	bzero(g_remote, sizeof(g_remote));
	bzero(g_local, sizeof(g_local));
	bzero(g_glow, sizeof(g_glow));

	hack::CGlowObjectManager manager;

	if (!csgo->Read((void*) csgo->m_addressOfGlowPointer, &manager, sizeof(hack::CGlowObjectManager))) {
		Logger::error ("Failed to read glowClassAddress");
		throw 1;
		return;
	}

	size_t count = manager.m_GlowObjectDefinitions.Count;

	void* data_ptr = (void*) manager.m_GlowObjectDefinitions.DataPtr;

	if (!csgo->Read(data_ptr, g_glow, sizeof(hack::GlowObjectDefinition_t) * count)) {
		Logger::error ("Failed to read m_GlowObjectDefinitions");
		throw 1;
		return;
	}

	size_t writeCount = 0;
	unsigned long localPlayer = 0;
	unsigned int teamNumber = 0;

	csgo->Read((void*) csgo->m_addressOfLocalPlayer, &localPlayer, sizeof(long));

	if (localPlayer != 0) {
		csgo->Read((void*) (localPlayer+0x128), &teamNumber, sizeof(int));
		NoFlash(csgo, client, localPlayer);
	}

	for (unsigned int i = 0; i < count; i++) {
		if (g_glow[i].m_pEntity != NULL) {
			Entity ent;

			if (csgo->Read(g_glow[i].m_pEntity, &ent, sizeof(Entity))) {
				if (ent.m_iTeamNum != 2 && ent.m_iTeamNum != 3) {
					g_glow[i].m_bRenderWhenOccluded = 0;
					g_glow[i].m_bRenderWhenUnoccluded = 0;
					continue;
				}

				unsigned int iAlt1Status = 0 ;
				csgo->Read((void*) (csgo->m_addressOfAlt1), &iAlt1Status, sizeof(int));

				if (localPlayer != 0 && (iAlt1Status == 0x5)) {
					if (ent.m_iTeamNum != teamNumber) {
						unsigned int crossHairId = 0;
						unsigned int entityId = 0;
						unsigned int attack = 0x5;
						unsigned int release = 0x4;
						csgo->Read((void*) (localPlayer+0xB380), &crossHairId, sizeof(int));
						csgo->Read((void*) (g_glow[i].m_pEntity + 0x94), &entityId, sizeof(int));

						if (crossHairId == entityId) {
							usleep(100);
							csgo->Write((void*) (csgo->m_addressOfForceAttack), &attack, sizeof(int));
							usleep(100);
							csgo->Write((void*) (csgo->m_addressOfForceAttack), &release, sizeof(int));
						}
					}
				}

				csgo->Write((void*) ((unsigned long) g_glow[i].m_pEntity + 0xEC5), &spotted, sizeof(unsigned char));

				if (g_glow[i].m_bRenderWhenOccluded == 1)
					continue;

				g_glow[i].m_bRenderWhenOccluded = 1;
				g_glow[i].m_bRenderWhenUnoccluded = 0;

				if (ent.m_iTeamNum == 2 || ent.m_iTeamNum == 3) {
					g_glow[i].m_flGlowRed = (teamNumber != ent.m_iTeamNum ? 1.0f : 0.0f);
					g_glow[i].m_flGlowGreen = 0.0f;
					g_glow[i].m_flGlowBlue = (teamNumber == ent.m_iTeamNum ? 1.0f : 0.0f);
					g_glow[i].m_flGlowAlpha = 0.6f;
				}
			}
		}

		if (csgo->m_bShouldGlow) {
			size_t bytesToCutOffEnd = sizeof(hack::GlowObjectDefinition_t) - g_glow[i].writeEnd();
			size_t bytesToCutOffBegin = (size_t) g_glow[i].writeStart();
			size_t totalWriteSize = (sizeof(hack::GlowObjectDefinition_t) - (bytesToCutOffBegin + bytesToCutOffEnd));

			g_remote[writeCount].iov_base =
				((uint8_t*) data_ptr + (sizeof(hack::GlowObjectDefinition_t) * i)) + bytesToCutOffBegin;
			g_local[writeCount].iov_base = ((uint8_t*) &g_glow[i]) + bytesToCutOffBegin;
			g_remote[writeCount].iov_len = g_local[writeCount].iov_len = totalWriteSize;

			writeCount++;
		}
	}

	process_vm_writev(csgo->GetPid(), g_local, writeCount, g_remote, writeCount, 0);
}
