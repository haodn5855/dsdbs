/******************************************************************************
**************************��ʯ����*********************************************
*******************************************************************************
	*��Ȩ��2013-2021 �¶��� ��Ȩ���� 
	*�汾��1.8
	*���ߣ��¶���
	*΢�ţ�19843355855
	*���䣺han_dn@sohu.com
	*���𼰰�Ȩ������
	1��ʹ�ñ���Դ��Ʒʱ�������������˰�Ȩ���������������ܶԴ˰�Ȩ��������������
	�κ����ӡ��޸ġ�ɾ���Ȳ�����
	2��ʹ�ñ���Ʒ��������ؼ���֧�ַ�����������й���ط��ɼ����ڹ����ɡ�������
	 Ʒ��������ڷǷ�Ŀ���������޹ء����߲����������Σ�����������ʹ�������ге���
	3������Ʒ�޷�������Bug���ʲ��е��򱾲�ƷBug��ȱ�ݵ��µ��κκ�������,��ʹ
	 �ñ���Ʒǰ�����������ز��ԡ�
	4������Դ��Ʒ���ڻ������ṩ������Դ��Ʒ��My Sql,protobuf,redis�ȵĽ���ʾ
	����������ʾ�����ڶ���Ӧ��Դ��Ʒ��Ȩ�뼰ʱ��֪����������Ӧ�޸Ļ�ɾ��;ʹ����
	��ʹ�ñ���Ʒʱ�õ���Ӧ�Ŀ�Դ��Ʒ�������л����ز�Ʒ��ʹ����ɡ�
	5����˾����֯����100���ҷ���������50̨�Լ�����ʹ�����ڷ����й���ط��ɼ���
	 �ڹ����ɣ�����������Ʒ�������𼰰�Ȩ�����������⸴�ơ��޸ġ�ʹ�á�ѧϰ���
	 ���롣
	6����˾����֯����100�˻����������50̨��ʹ�ñ���Ʒ������ʹ����ɣ�������
	ֱ�ӻ���ʹ�û���������ӵ�������ú��б���Ʒ��Դ����򾭹����뷢���Ĳ�Ʒ
	������ʹ����ɡ�
*******************************************************************************
**************************��ʯ����*********************************************
******************************************************************************/


#ifndef GX_LIST_2011_01_01
#define GX_LIST_2011_01_01

#pragma warning(disable:4127)
#pragma warning(disable:4150)

#include <windows.h>
namespace zsGame
{
typedef int (__cdecl *LISTCOMPFUNC)(const void *_pItem1,const void *_pItem2);
int zsDefaultCompListItemFunc(const void *_pItem1,const void *_pItem2);

template <class TYPE> class CgxList
{
public:
	CgxList(int _nCnIncr=16) 
	{
		m_cnIncr=_nCnIncr;
		m_pList=NULL;
		m_nTotal= 0 ;
		m_nCount=0;
	}
	virtual ~CgxList()
	{
		RemoveAll(TRUE);
	}
public:
	inline int GetCount()
	{
		return m_nCount;
	}
	
	inline void SetIncrCount(int _cnIncr=16)
	{
		m_cnIncr=_cnIncr;
	}

	inline BOOL IsEmpty()
	{
		return !m_nCount;
	}

	BOOL RemoveAll(BOOL _bFreeList=TRUE)
	{
		if (_bFreeList)
		{
			free(m_pList);
			m_pList=NULL;
			m_nTotal=0;
		}
		m_nCount=0;
		return TRUE;
	}
	inline TYPE *GetHead()
	{
		if (m_nCount<=0) 
			return NULL;
		return m_pList[0];
	}
	inline TYPE *GetAt(int _nPos)
	{
		if ((_nPos<0) ||( _nPos>=m_nCount)) 
			return NULL;
		return m_pList[_nPos];
	}
	inline TYPE *GetTail()
	{
		if (m_nCount<=0) 
			return NULL;
		return m_pList[m_nCount-1];
	}
	BOOL AddHead(TYPE *_pItem)
	{
		if (!ReallocMem()) 
			return FALSE;
		memmove(m_pList+1,m_pList,m_nCount*sizeof(TYPE *));
		m_pList[0]=_pItem;
		m_nCount++;
		return TRUE;
	}

	BOOL AddAt(int _nPos,TYPE *_pItem)
	{
		if (_nPos<=0) 
			return AddHead(_pItem);
		if (_nPos>=m_nCount) 
			return AddTail(_pItem);
		if (!ReallocMem()) 
			return FALSE;

		memmove(m_pList+_nPos+1,m_pList+_nPos,(m_nCount-_nPos)*sizeof(TYPE *));
		m_pList[_nPos]=_pItem;
		m_nCount++;
		return TRUE;
	}
	BOOL AddTail(TYPE *_pItem)
	{
		if (!ReallocMem()) 
			return FALSE;
		m_pList[m_nCount++]=_pItem;
		return TRUE;
	}
	BOOL RemoveHead()
	{
		if (m_nCount<=0) 
			return FALSE;
		memmove(m_pList,m_pList+1,(m_nCount-1)*sizeof(TYPE *));
		m_nCount--;
		return TRUE;
	}
	
	BOOL ReplaceAt(int _nPos,TYPE *_pItem)
	{
		if ((_nPos<0)  || (_nPos>=m_nCount))
			return FALSE;
		m_pList[_nPos]=_pItem;
		return TRUE;
	}

	BOOL RemoveTail()
	{
		if (m_nCount<=0) 
			return FALSE;
		m_nCount--;
		return TRUE;
	}
	
	BOOL RemoveAt(int _nPos)
	{
		if ((_nPos>=m_nCount) || (_nPos <0))
			return FALSE;
		memmove(m_pList+_nPos,m_pList+_nPos+1,(m_nCount-_nPos-1)*sizeof(TYPE *));
		m_nCount--;
		return TRUE;
	}

	int FindItemPos(TYPE *_pItem,LISTCOMPFUNC _pCompFunc=NULL)
	{
		if (m_nCount<=0) 
			return -1;
		for (int i=0;i<m_nCount;i++)
		{
			if (_pCompFunc==NULL)
			{
				if (m_pList[i]==_pItem) 
					return i;
			}
			else
			{
				if (_pCompFunc(&m_pList[i],&_pItem)==0) 
					return i;
			}
		}
		return -1;
	}
	
	TYPE *FindItem(TYPE *_pItem,LISTCOMPFUNC _pCompFunc=NULL)
	{
		int nPos=FindItemPos(_pItem,_pCompFunc);
		if (nPos<0) 
			return NULL;
		return m_pList[nPos];
	}
	
	BOOL DeleteItem(TYPE *_pItem,LISTCOMPFUNC _pCompFunc=NULL)
	{
		int nPos=FindItemPos(_pItem,_pCompFunc);
		if (nPos<0) 
			return FALSE;
		return RemoveAt(nPos);
	}

	void Sort(LISTCOMPFUNC _pCompFunc)
	{
		if (m_nCount>0)
		{
			qsort(m_pList,m_nCount,sizeof(TYPE *),_pCompFunc);
		}
	}

	TYPE *Search(TYPE *_pItem,LISTCOMPFUNC _pCompFunc)
	{
		if (m_pList!=NULL)
		{
			DWORD *pdw=(DWORD *)bsearch(&_pItem,m_pList,m_nCount,sizeof(TYPE *),_pCompFunc);
			if (pdw==NULL) 
				return NULL;
			return (TYPE *)(*pdw);
		}
		return NULL;
	}
	
	BOOL Insert(TYPE *_pItem,LISTCOMPFUNC _pCompFunc)
	{
		if (m_nCount==0) 
			return AddTail(_pItem);

		TYPE *pItemTmp=NULL;
		if (m_nCount==1)
		{
			pItemTmp=GetHead();
			if (_pCompFunc(&_pItem,&pItemTmp)<=0) 
				return AddHead(_pItem);
			return AddTail(_pItem);
		}
		int n1=0;
		int n2=m_nCount-1;
		
		pItemTmp=GetAt(n1);
		if (_pCompFunc(&_pItem,&pItemTmp)<=0) 
			return AddHead(_pItem);
		
		pItemTmp=GetAt(n2);
		if (_pCompFunc(&_pItem,&pItemTmp)>=0) 
			return AddTail(_pItem);

		while (TRUE)
		{
			if (n1+1==n2) 
				return AddAt(n2,_pItem);
			int n=n1+(n2-n1)/2;
			pItemTmp=GetAt(n);
			int nRet=_pCompFunc(&_pItem,&pItemTmp);
			if (nRet==0) 
				return AddAt(n,_pItem);
			else if (nRet<=0) 
				n2=n;
			else n1=n;
		}
		return FALSE;
	}
	int FindPos(TYPE *_pItem,LISTCOMPFUNC _pCompFunc)
	{
		if (m_nCount<=0) 
			return -1;

		LPVOID *pdw=(LPVOID *)bsearch(&_pItem,m_pList,m_nCount,sizeof(TYPE *),_pCompFunc);
		if (pdw==NULL) 
			return -1;
		return int(pdw-(LPVOID *)m_pList);
	}
	
	TYPE *Find(TYPE *_pItem,LISTCOMPFUNC _pCompFunc)
	{
		int nPos=FindPos(_pItem,_pCompFunc);
		if (nPos<0) 
			return NULL;
		return m_pList[nPos];
	}

	BOOL Delete(TYPE *_pItem,LISTCOMPFUNC _pCompFunc)
	{
		int nPos=FindPos(_pItem,_pCompFunc);
		if (nPos<0) 
			return FALSE;
		return RemoveAt(nPos);
	}
	void FreeAll(BOOL _bFreeList=FALSE)
	{
		for (int i=0;i<m_nCount;i++)
		{
			free(m_pList[i]);
		}
		RemoveAll(_bFreeList);
	}

	void DeleteAll(BOOL _bFreeList=FALSE)
	{
		for (int i=0;i<m_nCount;i++)
		{
			delete m_pList[i];
		}
		RemoveAll(_bFreeList);
	}
private:
	BOOL ReallocMem()
	{
		if (m_nTotal > m_nCount) 
			return TRUE;
		TYPE **pList=(TYPE **)realloc(m_pList,(m_nTotal+m_cnIncr)*sizeof(TYPE *));
		if (pList==NULL) 
			return FALSE;

		m_pList=pList;
		m_nTotal+=m_cnIncr;
		return TRUE;
	}
private:
	int m_cnIncr;				
	int m_nTotal;				
	int m_nCount;				
	TYPE **m_pList;				
};

}

#pragma warning(default:4127)
#pragma warning(default:4150)

#endif 
