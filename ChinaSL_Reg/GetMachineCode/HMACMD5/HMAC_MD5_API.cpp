
#include "stdafx.h"
//#include "../../stdafx.h"

#include "HMAC_MD5_API.h"
#include "HMAC_MD5_DATA.h"
#include "..\\BASE64\\BASE64_API.h"

// �������ݽṹ
typedef struct _MD5CTX {
	DWORD	aState[4];		// ��¼���ݵı仯״̬
	DWORD	aCount[2];		// ��¼���ݵ�ԭʼ����(��bitΪ��λ)
	BYTE	aBuffer[64];	// ԭʼ����
} MD5CTX;

// ��ʼ��
void MD5_Init( MD5CTX* pstruContext )
{
	const DWORD *MDOriginState;

	MDOriginState = MD5_ARGUMENTS.p;
	pstruContext->aState[0] = MDOriginState[0];
	pstruContext->aState[1] = MDOriginState[1];
	pstruContext->aState[2] = MDOriginState[2];
	pstruContext->aState[3] = MDOriginState[3];
	pstruContext->aCount[0] = pstruContext->aCount[1] = 0;
}

// MD5�����任����
void MD5_Transform( DWORD* pState, DWORD* px )
{
	const DWORD (*MDOffTable)[16];
	DWORD a,b,c,d;

	MDOffTable = MD5_ARGUMENTS.q;
	a = pState[0], b = pState[1], c = pState[2], d = pState[3];

	// ��һ�ֱ任
	FF(a, b, c, d, px[ 0], MDShiftTable[0][0], MDOffTable[0][ 0]);
	FF(d, a, b, c, px[ 1], MDShiftTable[0][1], MDOffTable[0][ 1]);
	FF(c, d, a, b, px[ 2], MDShiftTable[0][2], MDOffTable[0][ 2]);
	FF(b, c, d, a, px[ 3], MDShiftTable[0][3], MDOffTable[0][ 3]);
	FF(a, b, c, d, px[ 4], MDShiftTable[0][0], MDOffTable[0][ 4]);
	FF(d, a, b, c, px[ 5], MDShiftTable[0][1], MDOffTable[0][ 5]);
	FF(c, d, a, b, px[ 6], MDShiftTable[0][2], MDOffTable[0][ 6]);
	FF(b, c, d, a, px[ 7], MDShiftTable[0][3], MDOffTable[0][ 7]);
	FF(a, b, c, d, px[ 8], MDShiftTable[0][0], MDOffTable[0][ 8]);
	FF(d, a, b, c, px[ 9], MDShiftTable[0][1], MDOffTable[0][ 9]);
	FF(c, d, a, b, px[10], MDShiftTable[0][2], MDOffTable[0][10]);
	FF(b, c, d, a, px[11], MDShiftTable[0][3], MDOffTable[0][11]);
	FF(a, b, c, d, px[12], MDShiftTable[0][0], MDOffTable[0][12]);
	FF(d, a, b, c, px[13], MDShiftTable[0][1], MDOffTable[0][13]);
	FF(c, d, a, b, px[14], MDShiftTable[0][2], MDOffTable[0][14]);
	FF(b, c, d, a, px[15], MDShiftTable[0][3], MDOffTable[0][15]);

	// �ڶ��ֱ任
	GG(a, b, c, d, px[ 1], MDShiftTable[1][0], MDOffTable[1][ 0]);
	GG(d, a, b, c, px[ 6], MDShiftTable[1][1], MDOffTable[1][ 1]);
	GG(c, d, a, b, px[11], MDShiftTable[1][2], MDOffTable[1][ 2]);
	GG(b, c, d, a, px[ 0], MDShiftTable[1][3], MDOffTable[1][ 3]);
	GG(a, b, c, d, px[ 5], MDShiftTable[1][0], MDOffTable[1][ 4]);
	GG(d, a, b, c, px[10], MDShiftTable[1][1], MDOffTable[1][ 5]);
	GG(c, d, a, b, px[15], MDShiftTable[1][2], MDOffTable[1][ 6]);
	GG(b, c, d, a, px[ 4], MDShiftTable[1][3], MDOffTable[1][ 7]);
	GG(a, b, c, d, px[ 9], MDShiftTable[1][0], MDOffTable[1][ 8]);
	GG(d, a, b, c, px[14], MDShiftTable[1][1], MDOffTable[1][ 9]);
	GG(c, d, a, b, px[ 3], MDShiftTable[1][2], MDOffTable[1][10]);
	GG(b, c, d, a, px[ 8], MDShiftTable[1][3], MDOffTable[1][11]);
	GG(a, b, c, d, px[13], MDShiftTable[1][0], MDOffTable[1][12]);
	GG(d, a, b, c, px[ 2], MDShiftTable[1][1], MDOffTable[1][13]);
	GG(c, d, a, b, px[ 7], MDShiftTable[1][2], MDOffTable[1][14]);
	GG(b, c, d, a, px[12], MDShiftTable[1][3], MDOffTable[1][15]);
                        
	// �����ֱ任    
	HH(a, b, c, d, px[ 5], MDShiftTable[2][0], MDOffTable[2][ 0]);
	HH(d, a, b, c, px[ 8], MDShiftTable[2][1], MDOffTable[2][ 1]);
	HH(c, d, a, b, px[11], MDShiftTable[2][2], MDOffTable[2][ 2]);
	HH(b, c, d, a, px[14], MDShiftTable[2][3], MDOffTable[2][ 3]);
	HH(a, b, c, d, px[ 1], MDShiftTable[2][0], MDOffTable[2][ 4]);
	HH(d, a, b, c, px[ 4], MDShiftTable[2][1], MDOffTable[2][ 5]);
	HH(c, d, a, b, px[ 7], MDShiftTable[2][2], MDOffTable[2][ 6]);
	HH(b, c, d, a, px[10], MDShiftTable[2][3], MDOffTable[2][ 7]);
	HH(a, b, c, d, px[13], MDShiftTable[2][0], MDOffTable[2][ 8]);
	HH(d, a, b, c, px[ 0], MDShiftTable[2][1], MDOffTable[2][ 9]);
	HH(c, d, a, b, px[ 3], MDShiftTable[2][2], MDOffTable[2][10]);
	HH(b, c, d, a, px[ 6], MDShiftTable[2][3], MDOffTable[2][11]);
	HH(a, b, c, d, px[ 9], MDShiftTable[2][0], MDOffTable[2][12]);
	HH(d, a, b, c, px[12], MDShiftTable[2][1], MDOffTable[2][13]);
	HH(c, d, a, b, px[15], MDShiftTable[2][2], MDOffTable[2][14]);
	HH(b, c, d, a, px[ 2], MDShiftTable[2][3], MDOffTable[2][15]);
                        
	// �����ֱ任    
	II(a, b, c, d, px[ 0], MDShiftTable[3][0], MDOffTable[3][ 0]);
	II(d, a, b, c, px[ 7], MDShiftTable[3][1], MDOffTable[3][ 1]);
	II(c, d, a, b, px[14], MDShiftTable[3][2], MDOffTable[3][ 2]);
	II(b, c, d, a, px[ 5], MDShiftTable[3][3], MDOffTable[3][ 3]);
	II(a, b, c, d, px[12], MDShiftTable[3][0], MDOffTable[3][ 4]);
	II(d, a, b, c, px[ 3], MDShiftTable[3][1], MDOffTable[3][ 5]);
	II(c, d, a, b, px[10], MDShiftTable[3][2], MDOffTable[3][ 6]);
	II(b, c, d, a, px[ 1], MDShiftTable[3][3], MDOffTable[3][ 7]);
	II(a, b, c, d, px[ 8], MDShiftTable[3][0], MDOffTable[3][ 8]);
	II(d, a, b, c, px[15], MDShiftTable[3][1], MDOffTable[3][ 9]);
	II(c, d, a, b, px[ 6], MDShiftTable[3][2], MDOffTable[3][10]);
	II(b, c, d, a, px[13], MDShiftTable[3][3], MDOffTable[3][11]);
	II(a, b, c, d, px[ 4], MDShiftTable[3][0], MDOffTable[3][12]);
	II(d, a, b, c, px[11], MDShiftTable[3][1], MDOffTable[3][13]);
	II(c, d, a, b, px[ 2], MDShiftTable[3][2], MDOffTable[3][14]);
	II(b, c, d, a, px[ 9], MDShiftTable[3][3], MDOffTable[3][15]);	

	pState[0] += a;
	pState[1] += b;
	pState[2] += c;
	pState[3] += d;
}

// MD5����²���
void MD5_Update( MD5CTX* pstruContext, const BYTE* pInput, DWORD dwInputLen )
{
	DWORD i, dwIndex, dwPartLen, dwBitsNum;

	// ���� mod 64 ���ֽ���
	dwIndex = (pstruContext->aCount[0] >> 3) & 0x3F;

	// ��������λ��
	dwBitsNum = dwInputLen << 3;
	pstruContext->aCount[0] += dwBitsNum;
	
	if(pstruContext->aCount[0] < dwBitsNum)
	{
		pstruContext->aCount[1]++;
	}

	pstruContext->aCount[1] += dwInputLen >> 29;

	dwPartLen = 64 - dwIndex;
	if(dwInputLen >= dwPartLen)
	{
		memcpy( pstruContext->aBuffer+dwIndex, pInput, dwPartLen );
		MD5_Transform( pstruContext->aState, (DWORD*)pstruContext->aBuffer );

		for(i = dwPartLen; i + 63 < dwInputLen; i += 64 )
		{
			MD5_Transform( pstruContext->aState, (DWORD*)(pInput + i) );
		}

		dwIndex = 0;
	}
	else
	{
		i = 0;
	}

	memcpy( pstruContext->aBuffer + dwIndex, pInput + i, dwInputLen - i );
}

// �����������ݿ�
void MD5_Final( MD5CTX* pstruContext )
{
	DWORD dwIndex, dwPadLen;
	BYTE pBits[8];
	
	memcpy( pBits, pstruContext->aCount, 8 );
	
	// ���� mod 64 ���ֽ���
	dwIndex = (pstruContext->aCount[0] >> 3) & 0x3F;

	// ʹ��������K*64+56���ֽ�
	dwPadLen = (dwIndex < 56) ? (56-dwIndex) : (120-dwIndex);
	MD5_Update( pstruContext, MDPadding, dwPadLen );
	MD5_Update( pstruContext, pBits, 8 );
}

INT MD5_Hash( const BYTE* inputBuffer, UINT inputCount, BYTE* outputBuffer )
{
	MD5CTX struContext;
	
	if( inputBuffer == NULL )
	{
		inputCount = 0;
	}

	// ����MD5�任
	MD5_Init( &struContext );		// ��ʼ��
	MD5_Update( &struContext, inputBuffer, inputCount );	// MD5���ݿ���²���
	MD5_Final( &struContext );	// ������ս��

	// ��ȡ��ϣֵ
	if( outputBuffer != NULL )
	{
		memcpy( outputBuffer, struContext.aState, 16 );
	}	

	return 16;
}

INT HMAC_MD5_Hash( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, BYTE* outputBuffer )
{	
	BYTE hmacKey[64] = {0};
	BYTE k_ipad[64];
	BYTE k_opad[64];
	MD5CTX struContext;

	if( inputBuffer == NULL )
	{
		inputCount = 0;
	}

	if( userKey == NULL )
	{
		UserKeyLen = 0;
	}

	// ��֤��Կ���Ȳ�����64�ֽ�
	if( UserKeyLen > 64 )
	{
		 MD5_Hash( userKey, UserKeyLen, hmacKey );
	}
	else
	{
		memcpy( hmacKey, userKey, UserKeyLen );
	}

	for( UINT i = 0; i < 64; i++ )
	{
		k_ipad[i] = hmacKey[i] ^ 0x36;
		k_opad[i] = hmacKey[i] ^ 0x5C;
	}

	// ��ȦMD5����
	MD5_Init( &struContext );		// ��ʼ��
	MD5_Update( &struContext, k_ipad, 64 );	// MD5���ݿ���²���
	MD5_Update( &struContext, inputBuffer, inputCount );	// MD5���ݿ���²���
	MD5_Final( &struContext );	// ������ս��
	memcpy( hmacKey, struContext.aState, 16 );

	// ��ȦMD5����
	MD5_Init( &struContext );		// ��ʼ��
	MD5_Update( &struContext, k_opad, 64 );	// MD5���ݿ���²���
	MD5_Update( &struContext, hmacKey, 16 );	// MD5���ݿ���²���
	MD5_Final( &struContext );	// ������ս��

	// ��ȡ��ϣֵ
	if( outputBuffer != NULL )
	{
		memcpy( outputBuffer, struContext.aState, 16 );
	}	

	return 16;
}

INT MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, TCHAR* outputBuffer )
{
	BYTE hash[16];
	INT iByteNum;

	// �������봮MD5�Ĺ�ϣֵ
	iByteNum = MD5_Hash( inputBuffer, inputCount, hash );

	// ����ϣֵת����BASE64����
	return BASE64_Encode( hash, iByteNum, outputBuffer );
}

INT HMAC_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, TCHAR* outputBuffer )
{
	BYTE hash[16];
	INT iByteNum;

	// �������봮HMAC-MD5�Ĺ�ϣֵ
	iByteNum = HMAC_MD5_Hash( inputBuffer, inputCount, userKey, UserKeyLen, hash );

	// ����ϣֵת����BASE64����
	return BASE64_Encode( hash, iByteNum, outputBuffer );
}
