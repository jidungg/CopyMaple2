#include "..\Public\Base.h"

Engine::CBase::CBase()
{
	
}

/* _uint  : ������ ������� �����Ѵ�. */
_uint CBase::AddRef()
{
	return ++m_iRefCnt;	
}

/* _uint  : �����ϱ� ���� ������� �����Ѵ�. */
_uint CBase::Release()
{
	/* �����Ѵ�. */	
	if (0 == m_iRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	/* �����Ѵ�. */
	else
		return m_iRefCnt--;	
}

void CBase::Free()
{
}

