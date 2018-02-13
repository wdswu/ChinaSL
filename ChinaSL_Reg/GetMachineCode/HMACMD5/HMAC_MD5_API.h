/* ----------------------------------------------------------
�ļ����ƣ�HMAC_MD5_API.h

���ߣ��ؽ���

MSN��splashcn@msn.com

��ǰ�汾��V1.1

��ʷ�汾��
	V1.1	2010��05��08��
			�������BASE64�����ַ����ӿڡ�

	V1.0	2010��04��15��
			�����ʽ�汾��

����������
	MD5��HMAC-MD5����

�ӿں�����
	MD5_Hash
	HMAC_MD5_Hash
	MD5_BASE64
	HMAC_MD5_BASE64
 ------------------------------------------------------------ */
#pragma once

#include <windows.h>

//-------------------��������-------------
#ifdef __cplusplus
extern "C"{
#endif

/*
���ܣ������������ݵ�MD5��ϣֵ
��ڲ�����
	inputBuffer����������
	inputCount���������ݳ��ȣ��ֽ�����
	outputBuffer���������ݵĹ�ϣֵ
����ֵ��
	��ϣֵ����Ч���ȣ��ֽ�����
*/
INT MD5_Hash( const BYTE* inputBuffer, UINT inputCount, BYTE* outputBuffer );

/*
���ܣ������������ݵ�HMAC-MD5��ϣֵ
��ڲ�����
	inputBuffer����������
	inputCount���������ݳ��ȣ��ֽ�����
	userKey���û���Կ
	UserKeyLen���û���Կ����
	outputBuffer���������ݵĹ�ϣֵ
����ֵ��
	��ϣֵ����Ч���ȣ��ֽ�����
*/
INT HMAC_MD5_Hash( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, BYTE* outputBuffer );

/*
���ܣ������������ݵ�MD5��ϣֵ����ת��ΪBASE64�����ַ��������
��ڲ�����
	inputBuffer����������
	inputCount���������ݳ��ȣ��ֽ�����
	outputBuffer��MD5��ϣֵ��BASE64�����ַ���
����ֵ��
	BASE64�����ַ������ȣ��ַ�����,�������ַ���������
*/
INT MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, TCHAR* outputBuffer );

/*
���ܣ������������ݵ�HMAC-MD5��ϣֵ����ת��ΪBASE64�����ַ��������
��ڲ�����
	inputBuffer����������
	inputCount���������ݳ��ȣ��ֽ�����
	userKey���û���Կ
	UserKeyLen���û���Կ����
	outputBuffer��HMAC-MD5��ϣֵ��BASE64�����ַ���
����ֵ��
	BASE64�����ַ������ȣ��ַ�����,�������ַ���������
*/
INT HMAC_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, TCHAR* outputBuffer );

#ifdef __cplusplus
}
#endif
