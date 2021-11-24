/******************************************************************************
**************************鼎石数据*********************************************
*******************************************************************************
	*版权：2013-2021 郝多年 版权所有 
	*版本：1.8
	*作者：郝多年
	*微信：19843355855
	*邮箱：han_dn@sohu.com
	*免责及版权声明：
	1、使用本开源产品时必须完整保留此版权及免责声明，不能对此版权及免责声明进行
	任何增加、修改、删除等操作。
	2、使用本产品或享受相关技术支持服务等需遵守中国相关法律及所在国法律。若将产
	 品或服务用于非法目地与作者无关。作者不负连带责任，所有责任由使用者自行承担。
	3、本产品无法做到零Bug。故不承担因本产品Bug或缺陷导致的任何后果。因此,在使
	 用本产品前请务必做好相关测试。
	4、本开源产品现在或将来会提供与多个开源产品如My Sql,protobuf,redis等的接入示
	例，若接入示例存在对相应开源产品侵权请及时告知，将进行相应修改或删除;使用者
	在使用本产品时用到相应的开源产品，请自行获得相关产品的使用许可。
	5、公司或组织少于100人且服务器少于50台以及个人使用者在符合中国相关法律及所
	 在国法律，并保留本产品完整免责及版权声明。可任意复制、修改、使用、学习相关
	 代码。
	6、公司或组织大于100人或服务器大于50台的使用本产品均需获得使用许可，不论是
	直接或间接使用或何种渠道从第三方获得含有本产品的源代码或经过编译发布的产品
	均需获得使用许可。
*******************************************************************************
**************************鼎石数据*********************************************
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
