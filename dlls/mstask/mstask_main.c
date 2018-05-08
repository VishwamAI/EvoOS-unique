/*
 * Copyright (C) 2008 Google (Roy Shea)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>

#define COBJMACROS

#include "windef.h"
#include "winbase.h"
#include "objbase.h"
#include "rpcproxy.h"
#include "taskschd.h"
#include "mstask.h"
#include "mstask_private.h"
#include "atsvc.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(mstask);

static HINSTANCE hInst;
LONG dll_ref = 0;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    TRACE("(%p, %d, %p)\n", hinstDLL, fdwReason, lpvReserved);

    switch (fdwReason)
    {
        case DLL_WINE_PREATTACH:
            return FALSE;
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            hInst = hinstDLL;
            break;
    }

    return TRUE;
}

HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID iid, LPVOID *ppv)
{
    TRACE("(%s %s %p)\n", debugstr_guid(rclsid), debugstr_guid(iid), ppv);

    if (IsEqualGUID(rclsid, &CLSID_CTaskScheduler)) {
        return IClassFactory_QueryInterface((LPCLASSFACTORY)&MSTASK_ClassFactory, iid, ppv);
    }

    FIXME("Not supported class: %s\n", debugstr_guid(rclsid));
    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllCanUnloadNow(void)
{
    return dll_ref != 0 ? S_FALSE : S_OK;
}

HRESULT WINAPI DllRegisterServer(void)
{
    return __wine_register_resources( hInst );
}

HRESULT WINAPI DllUnregisterServer(void)
{
    return __wine_unregister_resources( hInst );
}

DECLSPEC_HIDDEN void __RPC_FAR *__RPC_USER MIDL_user_allocate(SIZE_T n)
{
    return HeapAlloc(GetProcessHeap(), 0, n);
}

DECLSPEC_HIDDEN void __RPC_USER MIDL_user_free(void __RPC_FAR *p)
{
    HeapFree(GetProcessHeap(), 0, p);
}

DECLSPEC_HIDDEN handle_t __RPC_USER ATSVC_HANDLE_bind(ATSVC_HANDLE str)
{
    static unsigned char ncalrpc[] = "ncalrpc";
    unsigned char *binding_str;
    handle_t rpc_handle = 0;

    if (RpcStringBindingComposeA(NULL, ncalrpc, NULL, NULL, NULL, &binding_str) == RPC_S_OK)
    {
        RpcBindingFromStringBindingA(binding_str, &rpc_handle);
        RpcStringFreeA(&binding_str);
    }
    return rpc_handle;
}

DECLSPEC_HIDDEN void __RPC_USER ATSVC_HANDLE_unbind(ATSVC_HANDLE ServerName, handle_t rpc_handle)
{
    RpcBindingFree(&rpc_handle);
}
