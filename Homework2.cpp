#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

//link the library required for Windows Service functions
#pragma comment(lib, "advapi32.lib")

//helper function to fetch the DLL path from the registry
wstring GetServiceDll(const wstring& serviceName)
{
    wstring subkey = L"SYSTEM\\CurrentControlSet\\Services\\" + serviceName + L"\\Parameters";
    HKEY hKey;
    wchar_t dllPath[MAX_PATH] = L"";
    DWORD bufferSize = sizeof(dllPath);

    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExW(hKey, L"ServiceDll", NULL, NULL, (LPBYTE)dllPath, &bufferSize) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return wstring(dllPath);
        }
        RegCloseKey(hKey);
    }
    return L"N/A (standalone executable)";
}

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
                wcout << L"DLL: " << GetServiceDll(pServices[i].lpServiceName);
            }
        }
    }

    CloseServiceHandle(hSCM);

    return 0;
}