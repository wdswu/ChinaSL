
#pragma once



//------------------------------------------------------------------
// 根据本机硬件ID生成MD5_BASE64编码的机器码
CString CalculateMachineCode();

//------------------------------------------------------------------
// 根据本机硬件ID的MD5_BASE64编码，生成MD5授权码
CString CalculateLicenseKey(CString strTemp);