#include <windows.h>
#include <stdio.h>

void GetSubkeys(HKEY rootKey, LPCSTR path)
{
	HKEY key;
	DWORD index = 0;
	CHAR subkeyName[255];
	DWORD subkeyNameSize = sizeof(subkeyName);
	FILETIME lastWriteTime;

	if (RegOpenKeyExA(rootKey, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		printf("Subkeys of %s:\n", path);
		while (RegEnumKeyExA(key, index, subkeyName, &subkeyNameSize, NULL, NULL, NULL, &lastWriteTime) == ERROR_SUCCESS)
		{
			printf("index = %d, %s\n", index, subkeyName);
			subkeyNameSize = sizeof(subkeyName);
			index++;
		}
		RegCloseKey(key);
	}
	else
	{
		printf("Failure opening key\n");
	}

}

int main()
{
	GetSubkeys(HKEY_CURRENT_USER, "Software");
	return 0;
}