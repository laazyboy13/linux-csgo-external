#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <X11/extensions/XTest.h>

#include <unistd.h>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <chrono>
#include <thread>

#include "remote.h"
#include "hack.h"

using namespace std;

std::string
Endi(bool bl)
{
    return bl ? "Enabled" : "Disabled";
}

int main() {
    if (getuid() != 0) {
        cout << "You should run this as root." << endl;
        return 0;
    }

    std::cout << "\n---------------[linux-csgo-external]---------------\n\n"                 << 
            "Original author: s0beit\n"                                       <<
            "Maintainers:\n\tlaazyboy13,\n\towerosu,\n\tMcSwaggens\n\tcommunity\n"    <<
            "github: https://github.com/McSwaggens/linux-csgo-external\n"   <<
            "\n---------------[linux-csgo-external]---------------\n"                   << endl;

    
    Display* display = XOpenDisplay(0);
	Window root = DefaultRootWindow(display);

    int keycodeGlow = XKeysymToKeycode(display, XK_F7);
    int keycodeFlash = XKeysymToKeycode(display, XK_F8);
    int keycodeBHopEnable = XKeysymToKeycode(display, XK_F9);
    int keycodeBHop = XKeysymToKeycode(display, XK_space);

    remote::Handle csgo;

    while (true) {
        if (remote::FindProcessByName("csgo_linux64", &csgo)) {
            break;
        }

        usleep(500);
    }

    cout << "CSGO Process Located [" << csgo.GetPath() << "][" << csgo.GetPid() << "]" << endl << endl;

    remote::MapModuleMemoryRegion client;

    client.start = 0;

    while (client.start == 0) {
        if (!csgo.IsRunning()) {
            cout << "Exited game before client could be located, terminating" << endl;
            return 0;
        }

        csgo.ParseMaps();

        for (auto region : csgo.regions) {
            if (region.filename.compare("client_client.so") == 0 && region.executable) {
                cout << "client_client.so: [" << std::hex << region.start << "][" << std::hex << region.end << "][" <<
                region.pathname << "]" << endl;
                client = region;
                break;
            }
        }

        usleep(500);
    }

    cout << "GlowObject Size: " << std::hex << sizeof(hack::GlowObjectDefinition_t) << endl;

    cout << "Found client_client.so [" << std::hex << client.start << "]" << endl;
    client.client_start = client.start;
    
    unsigned long pEngine = remote::getModule("engine_client.so", csgo.GetPid());
    
    if (pEngine == 0)
    {
        cout << "Could not find engine module..." << endl;
        return 0;
    }
    
    csgo.a_engine_client = pEngine;
    
    cout << "Found engine_client.so: [" << pEngine << "]" << endl;

    void* foundGlowPointerCall = client.find(csgo,
                                             "\xE8\x00\x00\x00\x00\x48\x8b\x10\x48\xc1\xe3\x06\x44",
                                             "x????xxxxxxxx");

    cout << "Glow Pointer Call Reference: " << std::hex << foundGlowPointerCall << " | Offset: " << (unsigned long) foundGlowPointerCall - client.start << endl;

    unsigned long call = csgo.GetCallAddress(foundGlowPointerCall);

    cout << "Glow function address: " << std::hex << call << endl;
    cout << "Glow function address offset: " << std::hex << call - client.start << endl;


    csgo.m_addressOfGlowPointer = csgo.GetCallAddress((void*)(call+0xF));
    cout << "Glow Array pointer " << std::hex << csgo.m_addressOfGlowPointer << endl << endl;
    
    
    long ptrLocalPlayer = (client.client_start + 0x5A9B1A0); // 27/06/16
    unsigned long foundLocalPlayerLea = (long)client.find(csgo,
                                             "\x48\x89\xe5\x74\x0e\x48\x8d\x05\x00\x00\x00\x00", //27/06/16
                                             "xxxxxxxx????");
                                             
    csgo.m_addressOfLocalPlayer = csgo.GetCallAddress((void*)(foundLocalPlayerLea+0x7));

    unsigned long foundAttackMov = (long)client.find(csgo,
                                             "\x44\x89\xe8\x83\xe0\x01\xf7\xd8\x83\xe8\x03\x45\x84\xe4\x74\x00\x21\xd0", //10/07/16
                                             "xxxxxxxxxxxxxxx?xx");
    csgo.m_addressOfForceAttack = csgo.GetCallAddress((void*)(foundAttackMov+19));

    unsigned long foundAlt1Mov = (long)client.find(csgo,
                                             "\x44\x89\xe8\xc1\xe0\x11\xc1\xf8\x1f\x83\xe8\x03\x45\x84\xe4\x74\x00\x21\xd0", //10/07/16
                                             "xxxxxxxxxxxxxxxx?xx");
    
    csgo.m_addressOfAlt1 = csgo.GetCallAddress((void*)(foundAlt1Mov+20));
    cout << "Address of local player " << csgo.m_addressOfLocalPlayer << endl;
    
    csgo.m_oAddressOfForceJump = client.client_start + 0x62F3500;

    csgo.m_bShouldGlow = true;
    csgo.m_bShouldNoFlash = true;
    csgo.m_bBhopEnabled = true;
    csgo.m_bShouldBHop = false;
    
    XEvent ev;
    
    char keys[32];
    char lastkeys[32];

    while (csgo.IsRunning()) {
        XQueryKeymap(display, keys);

        for (unsigned i = 0; i < sizeof(keys); ++i) {
            if (keys[i] != lastkeys[i]) {
                // check which key got changed
                for (unsigned j = 0, test = 1; j < 8; ++j, test *= 2) {
                    // if the key was pressed, and it wasn't before, print this
                    if ((keys[i] & test) &&
                            ((keys[i] & test) != (lastkeys[i] & test))) {
                        const int code = i * 8 + j;

                        if (code == keycodeGlow) {
                            csgo.m_bShouldGlow = !csgo.m_bShouldGlow;
                            cout << "Glow [" << Endi(csgo.m_bShouldGlow) << "]" << endl;
                        } else if (code == keycodeFlash) {
                            csgo.m_bShouldNoFlash = !csgo.m_bShouldNoFlash;
                            cout << "NoFlash [" << Endi(csgo.m_bShouldNoFlash) << "]" << endl;
                        } else if (code == keycodeBHopEnable) {
                            csgo.m_bBhopEnabled = !csgo.m_bBhopEnabled;
                            cout << "BHop [" << Endi(csgo.m_bBhopEnabled) << "]" << endl;
                        } else if (code == keycodeBHop) {
                            if (csgo.m_bBhopEnabled) {
                                csgo.m_bShouldBHop = !csgo.m_bShouldBHop;
                                cout << "BHop triggered [" << Endi(csgo.m_bShouldBHop) << "]" << endl;
                            }
                        }
                    }
                }
            }

            lastkeys[i] = keys[i];
        }

        hack::Glow(&csgo, &client);
        hack::Bhop(&csgo, &client, display);
    }

    return 0;
}
