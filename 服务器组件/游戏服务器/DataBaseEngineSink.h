#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink, public IGameDataBaseEngine
{
	//友元定义
	friend class CServiceUnits;

	//辅助变量
protected:
	DBO_GR_LogonFailure				m_LogonFailure;						//登录失败
	DBO_GR_LogonSuccess				m_LogonSuccess;						//登录成功

	//游戏数据库
protected:
	CDataBaseAide					m_GameDBAide;						//游戏数据
	CDataBaseHelper					m_GameDBModule;						//游戏数据

	//游戏币数据库
protected:
	CDataBaseAide					m_TreasureDBAide;					//游戏币数据库
	CDataBaseHelper					m_TreasureDBModule;					//游戏币数据库

	//平台数据库
protected:
	CDataBaseAide					m_PlatformDBAide;					//平台数据库
	CDataBaseHelper					m_PlatformDBModule;					//平台数据库

	//配置变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagGameParameter *				m_pGameParameter;					//配置参数
	tagDataBaseParameter *			m_pDataBaseParameter;				//连接信息
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//服务属性
	tagGameServiceOption *			m_pGameServiceOption;				//服务配置

	//组件变量
protected:
	IDataBaseEngine *				m_pIDataBaseEngine;					//引擎接口
	IGameServiceManager *			m_pIGameServiceManager;				//服务管理
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//查询接口
protected:
	IGameDataBaseEngineSink *		m_pIGameDataBaseEngineSink;			//数据接口

	//组件变量
public:
	IDBCorrespondManager *          m_pIDBCorrespondManager;            //数据协调

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//配置参数
public:
	//自定配置
	virtual VOID * GetCustomRule() { return m_pGameServiceOption->cbCustomRule; };
	//服务属性
	virtual tagGameServiceAttrib * GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//服务配置
	virtual tagGameServiceOption * GetGameServiceOption() { return m_pGameServiceOption; }

	//获取对象
public:
	//获取对象
	virtual VOID * GetDataBase(REFGUID Guid, DWORD dwQueryVer);
	//获取对象
	virtual VOID * GetDataBaseEngine(REFGUID Guid, DWORD dwQueryVer);

	//功能接口
public:
	//投递结果
	virtual bool PostGameDataBaseResult(WORD wRequestID, VOID * pData, WORD wDataSize);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//登录函数
protected:
	//I D 登录
	bool OnRequestLogonUserID(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//I D 登录
	bool OnRequestLogonMobile(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//帐号登录
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//系统处理
protected:
	//游戏写分
	bool OnRequestWriteGameScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//新手活动
	bool OnRequestBeginnerGameTime(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//离开房间
	bool OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//游戏记录
	bool OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//私人房游戏记录
	bool OnRequestPrivateGameRecord(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载参数
	bool OnRequestLoadParameter(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载列表
	bool OnRequestLoadGameColumn(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载机器
	bool OnRequestLoadAndroidUser(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载道具
	bool OnRequestLoadGameProperty(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//礼物请求
	bool OnRequestPropertyRequest(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//用户权限
	bool OnRequestManageUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//系统消息
	bool OnRequestLoadSystemMessage(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//加载敏感词
	bool OnRequestLoadSensitiveWords(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);


	//比赛服务
protected:
	//比赛报名
	bool OnRequestMatchSignup(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//退出比赛
	bool OnRequestMatchUnSignup(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛开始
	bool OnRequestMatchStart(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛结束
	bool OnRequestMatchOver(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛奖励
	bool OnRequestMatchReward(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//比赛淘汰
	bool OnRequestMatchEliminate(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//私人场服务
protected:
	bool OnRequestPrivateInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestCreatePrivate(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	bool OnRequestCreatePrivateCost(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	
	//银行服务
protected:
	//存入游戏币
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//提取游戏币
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//转账游戏币
	bool OnRequestUserTransferScore(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//查询银行
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);
	//查询用户
	bool OnRequestQueryTransferUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD &dwUserID);

	//辅助函数
private:
	//登录结果
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbDeviceType=DEVICE_TYPE_PC,WORD wBehaviorFlags=0,WORD wPageTableCount=0);
	//银行结果
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, SCORE lFrozenedScore, LPCTSTR pszErrorString, bool bMobileClient,BYTE cbActivityGame=FALSE);
};

//////////////////////////////////////////////////////////////////////////////////

#endif