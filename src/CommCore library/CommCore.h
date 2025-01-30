//
//	UDP based Communication Core
//	GSC Game World
//	© 2001 Serguei I. Ivantsov aka ManOwaR Linux Lover
//
//	Jun 22, 2001		V0.1
//	Jul 09, 2001		V0.2
//	Aug	06, 2001		V0.4
// 
//
// Для корректной работы за NAT-ом, последний должен удовлетворять рекомендациям
// фирмы Microsoft на трансляцию UDP-пакетов через шлюз

#define _COOL_
//#define CC_DEBUG

#ifdef CC_DEBUG
#define _log_message(message) DebugMessage(message)
#else
#define _log_message(message) 
#endif //CC_DEBUG

#ifndef _COMM_CORE_H_INCLUDED_
#define _COMM_CORE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <assert.h>
#ifdef CC_DEBUG
#include <stdio.h>
#endif //CC_DEBUG

#pragma warning (disable : 4200)

#pragma pack(1)

// ---------------------------------------------------------------------------------------------

typedef in_addr PEER_ADDR;
typedef unsigned short	PEER_PORT;
typedef unsigned short	PEER_ID, *LPPEER_ID;

// ---------------------------------------------------------------------------------------------
// Константы
#define PROTO_ID				0x47534370	// Идентификатор протокола ('GSCp')
#define DATA_PORT				34000		// Порт данных (34000)
#define RETRY_COUNT				8			// Сколько раз нужно пытаться отослать пакет
											// в случае неподтверждения
											// после чего пакет удаляется из очереди

//in main exe's commcore.h default RETRY_TIME was 700
//in commlib's commcore.h default RETRY_TIME was 2800
#define RETRY_TIME				700			// Интервал повтора отправки пакет в миллисекундах
#define MAX_QUEUE_LEN			4096		// Максимальная длина очереди пакетов
											// ожидающих подтверждения
#define MAX_PEERS				7			// Максимальное число хостов 
#define CC_PROTO_VERSION		0x03		// Версия коммуникационного протокола
#define MAX_PACKET_STAMP		0xEE6B2800	// Максимальный номер пакета (используется для идентификации)
#define RECV_BUFFER_LENGTH		8192		// Размер приемного буфера (8 килобайт)
#define MAX_HOST_NAME			32			// Максимальная длина имени хоста

// ---------------------------------------------------------------------------------------------
// Описание хоста
typedef struct PEER_ENTRY
{						// Хост
	PEER_ADDR	m_ex_Addr;						// Внешний адрес хоста (или NAT-а)
	PEER_PORT	m_ex_Port;						// Внешний порт хоста (или NAT-а)
	PEER_ID		m_Id;							// Идентификатор хоста в списке сервера
	int		m_bAlive;						// Доступен ли хост на данный момент
	int		m_bOverNAT;						// Находится ли хост за NAT-ом по отношению к серверу
	unsigned short		m_uLatency;						// Время ответа хоста
	char		m_szUserName[MAX_HOST_NAME];	// Название хоста / пользователя
	unsigned short		m_uUserDataSize;				// Размер пользовательских данных
	unsigned char*		m_lpbUserData;					// Указатель на пользовательские данные
	char		m_szCCUID[23];					// Глобальный идентификатор хоста
} *LPPEER_ENTRY;

// ---------------------------------------------------------------------------------------------
// Типы пакетов (указываются в заголовке)
#define CC_PT_FRAME_CONFIRM		0x01	// Хост подтверждает прием пакета данных
#define CC_PT_HOST_ALIVE		0x02	// Хост подтверждает, что он живой; измерение Latency
#define CC_PT_SEND_DATA			0x03	// Пакет пользовательских данными без подтверждения
#define CC_PT_SEND_DATA_SECURE	0x04	// Пакет пользовательских данными с подтверждением
#define CC_PT_CHCK_HOST			0x05	// Пакет запроса доступности хоста; измерение Latency
#define CC_PT_TRY_CONNECT		0x06	// Попытка подключения к серверу
#define CC_PT_CONNECT_OK		0x07	// Разрешение на подключение к серверу
#define CC_PT_CONNECT_REJECT	0x08	// Отказ в подключении к серверу
#define CC_PT_SERVER_LIST		0x09	// Рассылка списка хостов (от сервера)
#define CC_PT_HOST_EXIT			0x0A	// Хост выходит из игры
#define CC_PT_HOST_DROP			0x0B	// Хост выброшен из игры (от сервера)
#define CC_PT_CONNECT_DIFF		0x0C	// Пакет приходит к хосту от другого хоста, если он знает кто сервер
#define CC_PT_HOST_DROP_OK		0x0D	// Сервер подтверждает отключение хоста

#define CC_PT_SEND_USER_NAME	0x0E	// Пользователь посылает свое имя
#define CC_PT_SEND_USER_DATA	0x0F	// Пользователь посылает свои данные

#define CC_PT_SEND_NEW_NAME		0x10	// Сервер отсылает данные об изменившимся имени пользователя
#define CC_PT_SEND_NEW_DATA		0x11	// Сервер отсылает данные об изменившихся данных пользователя

//
// ---------------------------------------------------------------------------------------------
enum ConnectState
{
	csNone, csWait, csRejected, csConnected, csTimedOut, csBadProto, csSynch
};

enum SessionState
{
	ssNone, ssOpen, ssClosed
};

// ---------------------------------------------------------------------------------------------
// Ошибки
#define	BAD_PEER_ID				255		// Неверный идентификатор хоста
#define CE_NOT_SERVER			0x01	// Попытка подключения не к серверу
#define CE_NO_ROOM				0x02	// Нет свободых слотов подключения
#define CE_NO_ERROR				0x03	// Нет ошибок
#define CE_BAD_VERSION			0x04	// Неверная версия протокола
#define CE_BAD_MSG_SIZE			0x05	// Недопустимая длина сообщения
#define CE_SESSION_CLOSED		0x06	// Невозможно подключить хост, т.к. сессия уже закрыта

// ---------------------------------------------------------------------------------------------
// Заголовки пакетов

// Общий вид кадра для протокола
typedef struct CC_PK_RAW_FRAME
{
	unsigned long	m_lProto;				// Тип протокола
	unsigned short	m_uType;				// Тип пакета
	unsigned long	m_lStamp;				// Уникальный номер пакета; 0, если не требуется подтверждение
	PEER_ID	m_PeerId;				// Идентификатор отправителя
// ----------------------------------
	unsigned char	m_bData[];				// Специфические для пакета данные переменной длины
} *LPCC_PK_RAW_FRAME;

// CC_PT_FRAME_CONFIRM
typedef struct CC_PK_FRAME_CONFIRM
{
	unsigned long	m_lConfirmStamp;		// Уникальный номер пакета, подтверждающий прием
} *LPCC_PK_FRAME_CONFIRM;

// CC_PT_SEND_DATA
typedef struct CC_PK_SEND_DATA
{
	unsigned short	m_uSize;				// Размер поля данных
	unsigned char	m_bData[];				// Поле данных переменной длины
} *LPCC_PK_SEND_DATA;

// CC_PT_TRY_CONNECT
typedef struct CC_PK_TRY_CONNECT
{
	char		m_cProtoVersion;	// Номер версии протокола / приложения
	char		m_szUserName[MAX_HOST_NAME];
	char		m_szCCUID[23];
	unsigned short		m_uAddrCount;
	unsigned long		m_dwAddrList[];
} *LPCC_PK_TRY_CONNECT;

// CC_PT_CONNECT_OK
typedef struct CC_PK_CONNECT_OK
{
	PEER_ID		m_Id;							// Номер хоста в списке сервера
	char		m_szSessionName[MAX_HOST_NAME];	// Название сессии
	unsigned long		m_dwOptions;
} *LPCC_PK_CONNECT_OK;

// CC_PT_CONNECT_REJECT
typedef struct CC_PK_CONNECT_REJECT
{
	unsigned short		m_uReason;			// Причина отказа
} *LPCC_PK_CONNECT_REJECT;

// CC_PT_SERVER_LIST
typedef struct CC_PK_SERVER_LIST
{
	char		m_szSessionName[MAX_HOST_NAME];	// Название сессии
	unsigned short		m_uCount;						// Количество хостов
	unsigned char		m_PeerList[];					// Список хостов переменной + динамические данные :)
} *LPCC_PK_SERVER_LIST;

// CC_PT_CHCK_HOST
typedef struct CC_PK_CHCK_HOST
{
	unsigned long		m_dwTickCount;		// Текущее число тиков
} *LPCC_PK_CHCK_HOST;

// CC_PT_HOST_ALIVE
typedef struct CC_PK_HOST_ALIVE
{
	unsigned long		m_dwTickCount;		// Число тиков в момент отправки пакета (оригинального)
} *LPCC_PK_HOST_ALIVE;

// CC_PT_HOST_EXIT
typedef struct CC_PK_HOST_EXIT
{
	unsigned long		m_dwReserved;		//
} *LPCC_PK_HOST_EXIT;

// CC_PT_HOST_DROP
typedef struct CC_PK_HOST_DROP
{
	unsigned long		m_dwReserved;		//
} *LPCC_PK_HOST_DROP;

// CC_PT_HOST_DROP_OK
typedef struct CC_PK_HOST_DROP_OK
{
	unsigned long		m_dwReserved;		//
} *LPCC_PK_HOST_DROP_OK;

// CC_PT_SEND_USER_NAME
typedef struct CC_PK_SEND_USER_NAME
{
	char		m_szUserName[MAX_HOST_NAME];
} *LPCC_PK_SEND_USER_NAME;

// CC_PT_SEND_USER_DATA
typedef struct CC_PK_SEND_USER_DATA
{
	unsigned short		m_uUserDataSize;
	unsigned char		m_UserData[];
} *LPCC_PK_SEND_USER_DATA;

// CC_PT_SEND_NEW_NAME
typedef struct CC_PK_SEND_NEW_NAME
{
	PEER_ID		m_PeerId;
	char		m_szUserName[MAX_HOST_NAME];
} *LPCC_PK_SEND_NEW_NAME;

// CC_PT_SEND_NEW_DATA
typedef struct CC_PK_SEND_NEW_DATA
{
	PEER_ID		m_PeerId;
	unsigned short		m_uUserDataSize;
	unsigned char		m_UserData[];
} *LPCC_PK_SEND_NEW_DATA;

// ---------------------------------------------------------------------------------------------
// Описание пакета в очереди сообщений, ожидающих подтверждение
typedef struct FRAME_ENTRY
{						// Пакет
	LPCC_PK_RAW_FRAME			m_lpFrame;		// Указатель на пакет
	unsigned short						m_uSize;		// Размер пакета
	PEER_ADDR					m_PeerAddr;		// Адрес хоста
	PEER_PORT					m_PeerPort;		// Порт хоста
	unsigned long						m_dwSendTime;	// Время последней отправки пакета
												// в миллисекундах
	unsigned short						m_uRetrCount;	// Количество попыток отправки пакета
} *LPFRAME_ENTRY;

// Описание функций обратного вызова
typedef int( __stdcall* LP_CC_IDLE_PROC )( );
typedef int( __stdcall* LP_CC_ENUM_PROC )( const PEER_ID PeerID, const char* lpcszPeerName );

// ---------------------------------------------------------------------------------------------
// Класс ядра связи
class CCommCore
{
	// ---------------------------------------------------------------------------------------------
public:
	// ---------------------------------------------------------------------------------------------
	LP_CC_IDLE_PROC	lpIdleProc;				// Функция обратного вызова, вызывается ядром во время
											// работы блокирующих функций
	LP_CC_ENUM_PROC	lpEnumProc;				// Функция обратного вызова для перечисления хостов
// ---------------------------------------------------------------------------------------------
											// Послать сообщение выбранному хосту
	int SendToPeer( PEER_ID piNumber, unsigned char* lpbBuffer, unsigned short u_Size, int bSecure = 0 );

	// Послать сообщение всем хостам
	int SendToAll( unsigned char* lpbBuffer, unsigned short u_Size, int bSecure = 0 );

	unsigned short ReceiveData( unsigned char* lpbBuffer, LPPEER_ID lpPeerId = NULL );	// Извлекает один пакет из очереди
											// должна часто вызываться, во избежание переполнения буфера
	int SendDropClient( PEER_ID PeerID );	// Сервер просит клиента выйти

	// IP адрес в виде строки с точками aaa.bbb.ccc.ddd
	int InitClient( const char* lpcszServerIP, const char* lpcszUserName, unsigned short port );

	int DoneClient();						// Клиент отсылает запрос на удаление серверу 
	int DoneServer();						// Завершает работу сервера удаляя все хосты
	int DeletePeer( PEER_ID piNumber );		// Удаляет хост из списка хостов
	int InitServer( const char* lpcszSessionName, const char* lpcszUserName );	// Инициализирует сервер
	int QueueProcess();					// Обрабатывает очередь отправки пакетов
											// Должна часто вызываться

	int SendServerList();					// Рассылает список хостов


	int	IsOverNAT( PEER_ID PeerId );


	const char*	GetUserName( PEER_ID PeerId );
	int	SetUserName( const char* lpcszUserName );
	int	SendUserName();

	int	SetSessionName( const char* lpcszSessionName );

	int	GetUserData( PEER_ID PeerId, unsigned char* lpbUserData, unsigned short * puUserDataSize );
	int	SetUserData( const unsigned char* lpcbUserData, unsigned short uUserDataSize );
	int	SendUserData();

	int	SendUdpHolePunch( sockaddr *server, char *content, const int content_len );

	void	GetServerAddress( char* lpszServerAddress );

	int	EnumPeers();

	int	InitNetwork();					// Инициализация сетевого интерфейса
	int	CloseNetwork();					// Закрытие сетевого интерфейса

	CCommCore();
	virtual ~CCommCore();

	PEER_ID	GetPeerID()
	{
		return m_piNumber;
	}

	unsigned short GetPeersCount()
	{
		return m_uPeerCount;
	}
	const char*	GetSessionName()
	{
		return m_szSessionName;
	}

	unsigned short	GetMaxPeers()
	{
		return m_uMaxPeers;
	}
	void	SetMaxPeers( unsigned short uMaxPeers )
	{
		m_uMaxPeers = uMaxPeers;
	}

	void	CloseSession()
	{
		m_ssState = ssClosed;
	}

	int	IsClient()
	{
		return !m_bServer;
	}
	int	IsServer()
	{
		return m_bServer;
	}

	void	SetOptions( unsigned long dwOptions )
	{
		m_dwOptions = dwOptions;
	}
	unsigned long	GetOptions()
	{
		return m_dwOptions;
	}

	// Сколько получено байт
	unsigned long	GetRxBytes()
	{
		return m_dwRxBytes;
	}
	// Сколько послано байт
	unsigned long	GetTxBytes()
	{
		return m_dwTxBytes;
	}
	// Сколько перепослано байт
	unsigned long	GetNxBytes()
	{
		return m_dwNxBytes;
	}
	// Какое число миллисекунд назад был принят последний пакет
	unsigned long	GetRecvTimeOut()
	{
		return ( GetTickCount() - m_dwLastPacketTime );
	}

	// ---------------------------------------------------------------------------------------------
protected:
	// ---------------------------------------------------------------------------------------------
#ifdef CC_DEBUG
	FILE	*	m_DebugStream;
	void		DebugMessage( const char* lpcszMessage );
#endif //CC_DEBUG

	char		m_szUserName[MAX_HOST_NAME];		//
//	PEER_ADDR	m_paHostAddr;						//
//	char		m_szDotAddr[18];					//
	PEER_ADDR	m_paServAddr;						//
	PEER_PORT	m_paServPort;						// True port of game host (recieved from server)

	unsigned long		m_dwAddrList[8];					// Список адресов локальной машины в network order формате
	unsigned short		m_uAddrCount;						// Количество адресов локальной машины

	unsigned long		m_dwLastPacketTime;

	unsigned short		m_uMaxPeers;
	int		m_bOverNAT;

	unsigned long		m_dwRxBytes;
	unsigned long		m_dwTxBytes;
	unsigned long		m_dwNxBytes;

	unsigned long		m_dwOptions;

	char		m_szSessionName[MAX_HOST_NAME];		//

	char		m_szCCUID[23];

	unsigned char*		m_lpbRecvBuffer;					//

	SessionState	m_ssState;						//
	ConnectState	m_csState;						//
	int			m_bBlockingCall;				// В режиме блокировки обрабатываются только
													// пакеты-подтверждения

	unsigned short		m_uRejectReason;					//

	unsigned short		m_uMaxMsgSize;						//

	SOCKET		m_DataSocket;						// Сокет, обслуживающий пользовательские сообщения

	unsigned long		m_lStamp;							// Автоинкрементный счетчик пакетов

	int		m_bServer;							// Является ли хост сервером
	PEER_ID		m_piNumber;							// Идентификатор хоста в списке сервера
													// для сервера всегда равен 0x01
	PEER_ID		m_piAutoInc;						// Автоинкрементный счетчик хостов

	unsigned char*		m_lpbUserData;						// Динамические данные о пользователе
	unsigned short		m_uUserDataSize;					// Размер динамических данных о пользователе

	PEER_ENTRY	m_PeerList[MAX_PEERS];				// Список доступных хостов
	unsigned short		m_uPeerCount;						// Количество доступных хостов

	FRAME_ENTRY m_FrameList[MAX_QUEUE_LEN];			// Очередь сообщений, ожидающих подтверждения
	unsigned short		m_uFrameCount;						// Количество сообщений в очереди

// ---------------------------------------------------------------------------------------------

//	PEER_ID		GetIdBySender();					// Возвращает логический номер хоста
													// По его адресу/порту
	unsigned short GetPeerById( PEER_ID PeerId );			//
	unsigned short GetPeerByCCUID( const char* lpcszCCUID );
	int	DropPeer( unsigned short uPeer );					//

	int InitSocket();
	int CloseSocket();
	int InitHost();

	int QueueAddPacket( PEER_ADDR			PeerAddr,
		PEER_PORT			PeerPort,
		LPCC_PK_RAW_FRAME	lpRawFrame,
		unsigned short				uSize );

	int SendRawPacket( PEER_ADDR			PeerAddr,				// Адрес хоста
		PEER_PORT			PeerPort,				// Порт хоста
		unsigned short				uType,					// Тип пакета
		unsigned char*				lpbBuffer,				// Буфер с данными
		unsigned short				uSize,					// Размер данных в байтах
		int				bSecureMessage,			// TRUE, если нужна псевдо-гарантированная доставка данных
		int				bWaitForCompletion );	// TRUE, если блокирующий вызов,
													// т.е. ждать подтверждения или таймаута


	int ProcessServerList( LPCC_PK_SERVER_LIST lpServerList );

	int ReSendFrame( unsigned short uFrameNum );
	void Cleanup();

	int QueueDropPacket( int iFrameNum );
	int QueueDropConfirmedPacket( unsigned long lStamp );
	int QueueClearAll();
	int QueuePacketExists( unsigned long lStamp );

	int SendConfirmDataPacket( sockaddr_in * lpSender, unsigned long lStamp );
	int SendConnectReject( sockaddr_in *lpSender, unsigned short uReason );
	int SendConnectOk( sockaddr_in *lpSender, PEER_ID PeerId );
	int SendDropOk( sockaddr_in *lpSender );

	int SendNewName( PEER_ID PeerId );		// Отсылает информацию о имени	(сервер)
	int SendNewData( PEER_ID PeerId );		// Отсылает информацию о дате	(сервер)

	void SetCommCoreUID( const char* lpcszCCUID );
	void NewCommCoreUID( char* lpszCCUID );
	void GetCommCoreUID( char* lpszCCUID );

	// ---------------------------------------------------------------------------------------------
};

#pragma warning (default : 4200)

#endif // _COMM_CORE_H_INCLUDED_
