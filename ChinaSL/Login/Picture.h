// Picture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__8E3260D8_473C_4670_99DA_9BE74B6BAB76__INCLUDED_)
#define AFX_PICTURE_H__8E3260D8_473C_4670_99DA_9BE74B6BAB76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include <atlbase.h>
#endif // _MSC_VER > 1000

class CPicture
{
public:
	CPicture();
	virtual ~CPicture();
	// Load frm various sosurces
	BOOL Load(UINT nIDRes);
	BOOL Load(LPCTSTR pszPathName);
	BOOL Load(CFile& file);
	BOOL Load(CArchive& ar);
	BOOL Load(IStream* pstm);

	// render to device context
	BOOL Render(CDC* pDC, CRect rc = CRect(0, 0, 0, 0),
		LPCRECT prcMFBounds = NULL) const;

	CSize GetImageSize(CDC* pDC = NULL) const;

	operator IPicture*() {
		return m_spIPicture;
	}

	void GetHIMETRICSize(OLE_XSIZE_HIMETRIC& cx, OLE_YSIZE_HIMETRIC& cy) const {
		cx = cy = 0;
		if (m_spIPicture)
		{
			const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Width(&cx);
			ASSERT(SUCCEEDED(m_hr));
			const_cast<CPicture*>(this)->m_hr = m_spIPicture->get_Height(&cy);
			ASSERT(SUCCEEDED(m_hr));
		}
	}

	void Free() {
		if (m_spIPicture) {
			m_spIPicture.Release();
		}
	}

protected:
	CComQIPtr<IPicture>m_spIPicture;   // ATL smart pointer to IPicture
	HRESULT m_hr;         // last error code


};

#endif // !defined(AFX_PICTURE_H__8E3260D8_473C_4670_99DA_9BE74B6BAB76__INCLUDED_)
