#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

// Link the library required for Windows Service functions
#pragma comment(lib, "advapi32.lib")

int main() 
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);

    if (hSCM == NULL) 
    {
        cout << "Error: Unable to access Service Manager." << endl;
        return 1;
    }

    DWORD bytesNeeded = 0;
    DWORD servicesReturned = 0;
    DWORD resumeHandle = 0;

    //get required buffer size
    EnumServicesStatusEx
    (
        hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
        NULL, 0, &bytesNeeded, &servicesReturned, &resumeHandle, NULL
    );

    //allocate memory
    vector<BYTE> buffer(bytesNeeded);
    LPENUM_SERVICE_STATUS_PROCESS pServices = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESS>(buffer.data());

    //get the information now
    BOOL success = EnumServicesStatusEx
    (
        hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
        buffer.data(), bytesNeeded, &bytesNeeded, &servicesReturned, &resumeHandle, NULL
    );

    if (success) 
    {
        cout << "List of Running Services:" << endl;

        for (DWORD i = 0; i < servicesReturned; i++) 
        {
            if (pServices[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING) 
            {
                wcout << L"Running: " << pServices[i].lpDisplayName << endl;
            }
        }
    }

    CloseServiceHandle(hSCM);

    return 0;
}