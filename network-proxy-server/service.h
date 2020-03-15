/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#pragma once



#define SVC_OK						(DWORD)0
#define SVC_NOT_CREATE				(DWORD)1
#define SVC_NOT_START				(DWORD)2
#define SVC_NOT_INIT				(DWORD)3
#define SVC_ERROR_SCMANAGER			(DWORD)4
#define SVC_ERROR_CREATE			(DWORD)5
#define SVC_ERROR_START				(DWORD)6
#define SVC_ERROR_OPEN				(DWORD) 7
#define SVC_ERROR_STOP				(DWORD)8
#define SVC_ERROR_UNLOAD			(DWORD) 9


class CService
{
public:
	CService(void);
	CService(LPTSTR _lpFilePath, LPTSTR _lpServiceName,LPTSTR _lpDisplayName, DWORD _dwStartType);
	~CService(void);


	DWORD init_service(LPTSTR _lpFilePath, LPTSTR _lpServiceName,LPTSTR _lpDisplayName, DWORD _dwStartType);
	DWORD create_service(void);  
	DWORD unload_service(void);  
	DWORD start_service(void);  
	DWORD stop_service(void);   


	BOOL   is_init(void);	
	BOOL   is_loaded(void);	
	BOOL   is_started(void); 

private:

	BOOL init;
	BOOL loaded;
	BOOL started;

	LPTSTR file_path;		//driver file path
	LPTSTR service_name;	//service name
	LPTSTR display_name;	//service's dos name

	DWORD start_type;		//start type

	SC_HANDLE hservice;		//service's handle

	

};