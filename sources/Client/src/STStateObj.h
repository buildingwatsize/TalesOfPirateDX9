#pragma once

#define  _STATE_DEBUG

#include "HMSynchroObj.h"

class CStateSynchro;
class CMoveList;
class CActor;
class CCharacter;

// ��������
class CActionState
{
	friend class CActor;
public:
    CActionState(CActor* p);

    void Start();								// ��ʼִ��

	virtual void PushPoint( int x, int y ){}					// ���ڿ������ƶ����������
	virtual void ServerEnd(int nState)      { _IsOver = true;	}	// ���÷�����ͨ��ִ�����
	virtual void MoveEnd(int x, int y, int nState)	{ _IsOver = true;	}	// �������ƶ���Action���������ƶ�
    virtual void SetServerID( int n );
    virtual void Cancel();
	virtual void MouseRightDown(){}

    virtual const char*     GetExplain()    { return "CActionState";}

    int     GetServerID()       { return _nServerID;}

	void	PopState()			{_isExecEnd = true;	}

	CActor* GetActor() 			{ return _pActor;	}

	bool	GetIsOver()			{ return _IsOver;	}			// ִ����ʣ�ද��,�����������
	bool	GetIsExecEnd()		{ return _isExecEnd;}			// �����Ѿ�ȫ��ִ�����,(��������ͨ��ִ�����)
	bool	GetIsCancel()		{ return _IsCancel;	}			// ��������Ѿ������ȡ����		
	bool	GetIsWait()			{ return _isWait;	}

	// �������Ϊ��������������,ע:��CActor�в�֪���������
    void			SetParent( CActionState* p ){ _pParent=p;		}	
	CActionState*	GetParent()				{ return _pParent;		}

	void			SetIsSend( bool v )		{ _IsSend = v;		    }
    bool            GetIsSend()             { return _IsSend;       }


    bool            GetIsInit()             { return _IsInit;       }

protected:
    virtual		~CActionState();

    virtual void End();												
    virtual void FrameMove();
    virtual void ActionBegin( DWORD pose_id ){}
    virtual void ActionFrame( DWORD pose_id, int key_frame ) {}	
    virtual void ActionEnd( DWORD pose_id )	{}
    virtual void BeforeNewState(){}
    virtual bool IsKeepPose()       { return false;     }       // ����ִ����ɺ�,�Ƿ�ظ�������״̬
    virtual bool IsAllowUse()       { return true;      }
	virtual void StartFailed()		{}

	virtual bool	_Start()		{ return true;		}
	virtual void	_End()			{}          // ִ�н���
    virtual bool    _IsAllowCancel();			// �Ƿ���Ա�Cancel����״̬

	bool			_AllowCancel()		{ return !_isWait && !_IsCancel && !_IsOver;	}

private:
	void			SetIsWait( bool v )		    { _isWait=v;		}
    void			_StartFailed();		// ��ʼʱʧ��ʱ����

protected:
	bool	_IsCancel;		
	bool	_IsSend;
	bool	_IsOver;
    int     _nServerID;

private:
	CActor			*_pActor;
	CActionState	*_pParent;

	bool	_isWait;			// Ϊtrue,����Actor�Ķ�����,������ִ��,��IsAllowStart()����false
	bool	_isExecEnd;
	bool	_IsInit;

};

// ActorҪ�ƶ����б�
class CMoveList
{
public:
	void	Clear(unsigned int count=0);
	void	GotoFront(CCharacter *pCha);
	void	PushPoint( int x, int y );

	bool	IsEmpty()						{ return _path.empty();	    }
	int		GetCount()						{ return (int)_path.size();	}

	void	PopFront()						{ _path.pop_front();		}
	POINT&	GetFront()						{ return _path.front();		}
	POINT&	GetBack()						{ return _path.back();		}

	int		GetListDistance(int x, int y);

private:
	typedef std::list<POINT> path;
	path	_path;

};

// ��������
inline void CActionState::SetServerID( int n )
{
    if( _nServerID==INT_MAX )
    {
        _nServerID = n;
    }
    else
    {
        LG( "state", RES_STRING(CMISS_000409), GetExplain(), n );
    }
}

