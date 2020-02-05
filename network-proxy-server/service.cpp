/*
 * Copyright (c) 2020 Alexandr Shevchenko. All rights reserved.
 */

#include <windows.h>

#include "debug.h"
#include "service.h"

CService::CService(void)
{
	file_path = NULL;
	service_name = NULL;
	display_name = NULL;
	start_type = 0;
	hservice = NULL;

	init = false;
	loaded = false;
	started = false;
}

CService::CService(LPTSTR _lpFilePath, LPTSTR _lpServiceName, LPTSTR _lpDisplayName, DWORD _dwStartType)
{
	file_path = _lpFilePath;
	service_name = _lpServiceName;
	display_name = _lpDisplayName;

	start_type = _dwStartType;

	hservice = NULL;

	init = true;
	loaded = false;
	started = false;
}

CService::~CService(void)
{
	unload_service();

	file_path = NULL;
	service_name = NULL;
	display_name = NULL;

	start_type = 0;

	hservice = NULL;

	init = false;
	loaded = false;
	started = false;

}

DWORD CService::init_service(LPTSTR _lpFilePath, LPTSTR _lpServiceName, LPTSTR _lpDisplayName, DWORD _dwStartType)
{
	if (is_init())
		return SVC_OK;


	file_path = _lpFilePath;
	service_name = _lpServiceName;
	display_name = _lpDisplayName;

	start_type = _dwStartType;

	hservice = NULL;

	init = true;
	loaded = false;
	started = false;

	return SVC_OK;
}


DWORD CService::create_service(void)
{
	if (!is_init())
		return SVC_NOT_INIT;

	if (is_loaded())
		return SVC_OK;

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	DBG_ASSERT(hSCManager);
	if (hSCManager == NULL)
		return SVC_ERROR_SCMANAGER;

	hservice = CreateService(hSCManager, service_name, display_name,
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		start_type,
		SERVICE_ERROR_NORMAL,
		file_path,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	DBG_ASSERT(hservice);
	if (hservice == NULL) {

		hservice = OpenService(hSCManager, service_name, SERVICE_ALL_ACCESS);
		DBG_ASSERT(hservice);
		if (hservice == NULL) {
			CloseServiceHandle(hSCManager);
			return SVC_ERROR_CREATE;
		}
	}

	loaded = true;
	CloseServiceHandle(hSCManager);

	return SVC_OK;
}

DWORD CService::start_service(void)
{
	if (!is_loaded())
		return SVC_NOT_CREATE;

	if (is_started())
		return SVC_OK;

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	DBG_ASSERT(hSCManager);
	if (hSCManager == NULL)
		return SVC_ERROR_SCMANAGER;

	hservice = OpenService(hSCManager, service_name, SERVICE_ALL_ACCESS);
	DBG_ASSERT(hservice);
	if (hservice == NULL) {
		CloseServiceHandle(hSCManager);
		return SVC_ERROR_OPEN;
	}

	if (StartService(hservice, 0, NULL) == NULL) {
		CloseServiceHandle(hSCManager);
		CloseServiceHandle(hservice);
		return SVC_ERROR_START;
	}

	CloseServiceHandle(hSCManager);
	started = true;

	return SVC_OK;
}

DWORD CService::stop_service(void)
{
	SERVICE_STATUS ss;

	if (!is_started())
		return SVC_OK;

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	DBG_ASSERT(hSCManager);
	if (hSCManager == NULL)
		return SVC_ERROR_SCMANAGER;

	hservice = OpenService(hSCManager, service_name, SERVICE_ALL_ACCESS);
	DBG_ASSERT(hservice);
	if (hservice == NULL) {
		CloseServiceHandle(hSCManager);
		return SVC_ERROR_OPEN;
	}

	if (ControlService(hservice, SERVICE_CONTROL_STOP, &ss) == NULL) {
		CloseServiceHandle(hSCManager);
		CloseServiceHandle(hservice);
		return SVC_ERROR_STOP;

	}

	started = false;

	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hservice);
	return SVC_OK;
}

DWORD CService::unload_service(void)
{
	if (!is_loaded())
		return SVC_OK;

	if (is_started())
		if (stop_service() != SVC_OK)
			return SVC_ERROR_UNLOAD;

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (hSCManager == NULL)
		return SVC_ERROR_SCMANAGER;

	hservice = OpenService(hSCManager, service_name, SERVICE_ALL_ACCESS);
	if (hservice == NULL) {
		CloseServiceHandle(hSCManager);
		return SVC_ERROR_OPEN;
	}

	DeleteService(hservice);
	CloseServiceHandle(hSCManager);

	loaded = false;

	return SVC_OK;
}

BOOL CService::is_init(void)
{
	return init;
}

BOOL CService::is_loaded(void)
{
	return loaded;
}

BOOL CService::is_started(void)
{
	return started;
}