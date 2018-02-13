//================================================================
// �ļ���:GetMachineCode.cpp
// ˵��:��ȡ����Ӳ��Key
// ����:�����Ե� wdswu@163.com
// ����ʱ�䣺2016.04.05
// ������ʱ�䣺2017.03.09
//================================================================

#include "stdafx.h"
#include "../stdafx.h"

#include <iostream>
#include <windows.h>
#include <WinIoCtl.h>
#include "GetMachineCode.h"

//��ȡ������Ϣ
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

//��һ������MD5��Base64����
#include "HMACMD5/HMAC_MD5_API.h"
//�ڶ���MD5����32λ�ַ�
#include "MD5.h"


#define SMART_GET_VERSION               CTL_CODE(IOCTL_DISK_BASE, 0x0020, METHOD_BUFFERED, FILE_READ_ACCESS)
#define SMART_RCV_DRIVE_DATA            CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define DFP_SEND_DRIVE_COMMAND			SMART_SEND_DRIVE_COMMAND
#define GETVERSIONOUTPARAMS				GETVERSIONINPARAMS
#define DFP_RCV_DRIVE_DATA				SMART_RCV_DRIVE_DATA
#define DFP_GET_VERSION					SMART_GET_VERSION
#define IDENTIFY_BUFFER_SIZE			512


using namespace std;

const WORD IDE_ATAPI_IDENTIFY = 0xA1;		//��ȡATAPI�豸������
const WORD IDE_ATA_IDENTIFY = 0xEC;			//��ȡATA�豸������
const int MAX_IDE_DRIVES = 1;				//���IDE�豸��

string Trim(string str)				// cut off leading and tailing blanks
{
	wstring::size_type pos = str.find_last_not_of(' ');
	if (pos != wstring::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if (pos != wstring::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());

	return str;
}

// fetch characters what we need from DiskData[FirstIndex ... LastIndex]
string __fastcall ConvertToString(DWORD DiskData[256], int FirstIndex, int LastIndex)
{
	int i = 0;
	TCHAR ResBuf[1024] = { 0 };

	for (char *p = (char*)(DiskData + FirstIndex); p <= (char*)(DiskData + LastIndex); p += sizeof(DiskData))
	{
		ResBuf[i++] = *(p + 1);
		ResBuf[i++] = *p;
	}

	return Trim(ResBuf);			// cut off leading and tailing blanks
}

BOOL __fastcall DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, PSENDCMDOUTPARAMS pSCOP, byte bIDCmd, byte bDriveNum, PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	//Compute the drive number.(���̺ʹ�������Ӧ��ֵ�ǲ�һ����)
	pSCIP->irDriveRegs.bDriveHeadReg = (bDriveNum & 1) ? 0xB0 : 0xA0;

	// The command can either be IDE identify or ATAPI identify.
	pSCIP->irDriveRegs.bCommandReg = bIDCmd;
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return DeviceIoControl(hPhysicalDriveIOCTL, DFP_RCV_DRIVE_DATA, (LPVOID)pSCIP, sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID)pSCOP, sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1, lpcbBytesReturned, NULL);
}

// ��ȡӲ���ͺš����кš��޶��汾��
void __fastcall ReadPhysicalDriveOnNT(TCHAR *serialNo, TCHAR *modelNo, TCHAR *revisionNo)
{
	//IDENTIFY_BUFFER_SIZE = 512
	byte IdOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
	//char serialNo[21], modelNo[21], revisionNo[21];

	for (int Drive = 0; Drive < 4; Drive++)
	{
		HANDLE hPhysicalDriveIOCTL;
		TCHAR DriveName[32];
		wsprintf(DriveName, TEXT("\\\\.\\PhysicalDrive%d"), Drive);//����Ӳ�̺�0,1,2,3
		hPhysicalDriveIOCTL = CreateFile(DriveName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			DWORD cbBytesReturned = 0;
			GETVERSIONOUTPARAMS VersionParams;

			// Get the version, etc of PhysicalDrive IOCTL
			ZeroMemory(&VersionParams, sizeof(GETVERSIONOUTPARAMS));

			if (!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION, NULL, 0, &VersionParams, sizeof(VersionParams), &cbBytesReturned, NULL))
			{
				continue;
			}

			if (VersionParams.bIDEDeviceMap > 0)
			{
				// IDE or ATAPI IDENTIFY cmd
				byte bIDCmd = 0;
				SENDCMDINPARAMS InParams;

				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				// �������ý����ο������˵��
				bIDCmd = (VersionParams.bIDEDeviceMap >> Drive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				ZeroMemory(&InParams, sizeof(SENDCMDINPARAMS));
				ZeroMemory(IdOutCmd, sizeof(IdOutCmd));

				if (DoIdentify(hPhysicalDriveIOCTL, &InParams, (PSENDCMDOUTPARAMS)IdOutCmd, (BYTE)bIDCmd, (BYTE)Drive, &cbBytesReturned))
				{
					DWORD DiskData[256];
					USHORT *pIDSector;												// ��Ӧ�ṹIDSECTOR����ͷ�ļ�
					pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)IdOutCmd)->bBuffer;

					for (int i = 0; i < 256; i++)
						DiskData[i] = pIDSector[i];

					//FILE *fp = NULL;
					//if(0 != _wfopen_s(&fp, L"a.log", L"w") )
					//{
					//	fwrite(DiskData, sizeof(DiskData), 1, fp);
					//	//_fwrite_nolock(DiskData, sizeof(DiskData), 1, fp);
					//	//DeleteFile(L"a.log");
					//	fclose(fp);
					//}

					_tcscpy_s(serialNo, 100, ConvertToString(DiskData, 10, 19).c_str());	// get serial number
					_tcscpy_s(modelNo, 100, ConvertToString(DiskData, 27, 46).c_str());		// get model type
					_tcscpy_s(revisionNo, 100, ConvertToString(DiskData, 23, 26).c_str());	// ��ȡ�޶��汾��
				}
			}
			CloseHandle(hPhysicalDriveIOCTL);
		}
	}
}

TCHAR g_szSystemInfo[1024] = { 0 }; // �ڳ���ִ����Ϻ󣬴˴��洢ȡ�õ�ϵͳ������
UINT g_uSystemInfoLen = 0; // �ڳ���ִ����Ϻ󣬴˴��洢ȡ�õ�ϵͳ������ĳ��� 

// ���� MAC ��ַ��ע��: MAC ��ַ�ǿ�����ע������޸ĵ�
BOOL GetMac()
{
	UINT uErrorCode = 0;
	IP_ADAPTER_INFO iai;
	ULONG uSize = 0;
	DWORD dwResult = GetAdaptersInfo(&iai, &uSize);
	if (dwResult == ERROR_BUFFER_OVERFLOW)
	{
		IP_ADAPTER_INFO* piai = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, uSize);
		if (piai != NULL)
		{
			dwResult = GetAdaptersInfo(piai, &uSize);
			if (ERROR_SUCCESS == dwResult)
			{
				IP_ADAPTER_INFO* piai2 = piai;
				while (piai2 != NULL && (g_uSystemInfoLen + piai2->AddressLength) < 4096U)
				{
					//CopyMemory(szSystemInfo + uSystemInfoLen, piai2->Address, piai2->AddressLength);
					TCHAR buf[18] = { 0 };
					wsprintf(buf, "%02X-%02X-%02X-%02X-%02X-%02X",
						piai2->Address[0], piai2->Address[1], piai2->Address[2],
						piai2->Address[3], piai2->Address[4], piai2->Address[5]);
					_tcscat_s(g_szSystemInfo + g_uSystemInfoLen, sizeof(g_szSystemInfo) - g_uSystemInfoLen, buf);
					g_uSystemInfoLen += _tcslen(buf);
					piai2 = piai2->Next;
				}
			}
			else
			{
				uErrorCode = 0xF0000000U + dwResult;
			}
			VERIFY(HeapFree(GetProcessHeap(), 0, piai));
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		uErrorCode = 0xE0000000U + dwResult;
	}
	if (uErrorCode != 0U)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL GetCpuID()
{
	//BOOL bException = FALSE;
	BYTE szCpu[16] = { 0 };
	UINT uCpuID = 0U;

	__try
	{
		_asm
		{
			mov eax, 0
			cpuid
				mov dword ptr szCpu[0], ebx
				mov dword ptr szCpu[4], edx
				mov dword ptr szCpu[8], ecx
				mov eax, 1
				cpuid
				mov uCpuID, edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//bException = TRUE;
		return FALSE;
	}

	//if (!bException)
	{
		//CopyMemory(szSystemInfo + uSystemInfoLen, &uCpuID, sizeof(UINT));
		TCHAR buf[11] = { 0 };
		wsprintf(buf, "%u", uCpuID);
		_tcscat_s(g_szSystemInfo + g_uSystemInfoLen, sizeof(g_szSystemInfo) - g_uSystemInfoLen, buf);
		g_uSystemInfoLen += _tcslen(buf);

		//uCpuID = strlen((char*)szCpu);
		//CopyMemory(g_szSystemInfo + g_uSystemInfoLen, szCpu, uCpuID);
		//g_uSystemInfoLen += uCpuID;
	}

	return TRUE;
}

//------------------------------------------------------------------
// ��ȡӲ�����к�
void GetHardDiskID()
{
	// ��ȡӲ���ͺš����кš��޶��汾��
	TCHAR serialNo[100] = { 0 }, modelNo[100] = { 0 }, revisionNo[100] = { 0 };
	ReadPhysicalDriveOnNT(serialNo, modelNo, revisionNo);

	_tcscat_s(g_szSystemInfo + g_uSystemInfoLen, sizeof(g_szSystemInfo) - g_uSystemInfoLen, serialNo);
	g_uSystemInfoLen += _tcslen(serialNo);

}

// ���ݱ�����Ӳ������ȡӲ��ID���
CString GetHardwareID()
{
	GetCpuID();
	GetHardDiskID();
	GetMac();

	CString str;

	VMProtectBeginUltra("WDS_VMBEGIN OF GetHardwareID()");
	str.Format("wdswu%s", g_szSystemInfo);

	//�����ȡ��ID�������ۼӻ�ȡ
	ZeroMemory(g_szSystemInfo, 1024);
	g_uSystemInfoLen = 0;

	VMProtectEnd();

	return str;
}


//------------------------------------------------------------------
// ���ݱ���Ӳ��ID����MD5_BASE64����Ļ�����--���ڵ���ע��
CString CalculateMachineCode()
{

	VMProtectBeginUltra("WDS_VMBEGIN OF CalculateMachineCode");

	CString strTemp;

	//��ȡӲ��ID���
	strTemp = GetHardwareID();

	//1����һ�ν�SNת��ΪMD5��BASE64����
	TCHAR outBuffer[1024] = { 0 };
	MD5_BASE64((const BYTE*)strTemp.GetBuffer(), strTemp.GetLength()*sizeof(TCHAR), outBuffer);
	strTemp.ReleaseBuffer();

	strTemp.Format(_T("%s"), outBuffer);

	VMProtectEnd();

	return strTemp;

}

//------------------------------------------------------------------
// ���ݱ���Ӳ��ID��MD5_BASE64���룬����MD5Ӳ����--���ڵ���ע��
CString CalculateLicenseKey(CString strMachineCode)
{

	VMProtectBeginUltra("WDS_VMBEGIN OF CalculateLicenseKey");
	
	CString strTemp = strMachineCode + _T("wdswxj");

	//2���ڶ��ν�BASE64������תһ��MD5����(32��Сд�ַ�)
	//ֱ��ʹ���๹�캯��
	string strMD5;
	strMD5 = MD5(strTemp.GetBuffer()).toString();
	strTemp.ReleaseBuffer();
	strTemp = strMD5.c_str();

	VMProtectEnd();

	return strTemp;
}

//------------------------------------------------------------------
// ���ݱ���Ӳ��SN���ɻ�����---Ҳ��ע�����ĺ���--����������֤�󶨻�����
CString GetMachineCode()
{

	VMProtectBeginUltra("WDS_VMBEGIN OF GetMachineCode");

	CString strTemp;

	//��ȡӲ��SN
	strTemp = GetHardwareID();

	//1����һ�ν�SNת��ΪMD5��BASE64����
	TCHAR outBuffer[1024] = { 0 };
	MD5_BASE64((const BYTE*)strTemp.GetBuffer(), strTemp.GetLength() * sizeof(TCHAR), outBuffer);
	strTemp.ReleaseBuffer();

	//2���ڶ��ν�BASE64������תһ��MD5����(32��Сд�ַ�)
	//ֱ��ʹ���๹�캯��
	string strMD5;
	strMD5 = MD5(outBuffer).toString();
	strTemp = strMD5.c_str();

	VMProtectEnd();

	return strTemp;
}