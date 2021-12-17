#pragma once

#include "resource.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windowsx.h>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")