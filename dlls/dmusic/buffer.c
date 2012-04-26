/* IDirectMusicBuffer Implementation
 *
 * Copyright (C) 2003-2004 Rok Mandeljc
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "dmusic_private.h"
#include "initguid.h"
#include "dmksctrl.h"

WINE_DEFAULT_DEBUG_CHANNEL(dmusic);

static inline IDirectMusicBufferImpl *impl_from_IDirectMusicBuffer(IDirectMusicBuffer *iface)
{
    return CONTAINING_RECORD(iface, IDirectMusicBufferImpl, IDirectMusicBuffer_iface);
}

/* IDirectMusicBufferImpl IUnknown part: */
static HRESULT WINAPI IDirectMusicBufferImpl_QueryInterface(LPDIRECTMUSICBUFFER iface, REFIID riid, LPVOID *ret_iface)
{
    TRACE("(%p)->(%s, %p)\n", iface, debugstr_dmguid(riid), ret_iface);

    if (IsEqualIID(riid, &IID_IUnknown) ||
        IsEqualIID(riid, &IID_IDirectMusicBuffer))
    {
        IDirectMusicBuffer_AddRef(iface);
        *ret_iface = iface;
        return S_OK;
    }

    *ret_iface = NULL;

    WARN("(%p)->(%s, %p): not found\n", iface, debugstr_dmguid(riid), ret_iface);

    return E_NOINTERFACE;
}

static ULONG WINAPI IDirectMusicBufferImpl_AddRef(LPDIRECTMUSICBUFFER iface)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);
    ULONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p)->(): new ref = %u\n", iface, ref);

    DMUSIC_LockModule();

    return ref;
}

static ULONG WINAPI IDirectMusicBufferImpl_Release(LPDIRECTMUSICBUFFER iface)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p)->(): new ref = %u\n", iface, ref);

    if (!ref) {
        HeapFree(GetProcessHeap(), 0, This->data);
        HeapFree(GetProcessHeap(), 0, This);
    }

    DMUSIC_UnlockModule();

    return ref;
}

/* IDirectMusicBufferImpl IDirectMusicBuffer part: */
static HRESULT WINAPI IDirectMusicBufferImpl_Flush(LPDIRECTMUSICBUFFER iface)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p): stub\n", This);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_TotalTime(LPDIRECTMUSICBUFFER iface, LPREFERENCE_TIME prtTime)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, %p): stub\n", This, prtTime);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_PackStructured(LPDIRECTMUSICBUFFER iface, REFERENCE_TIME rt, DWORD dwChannelGroup, DWORD dwChannelMessage)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, 0x%s, %d, %d): stub\n", This, wine_dbgstr_longlong(rt), dwChannelGroup, dwChannelMessage);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_PackUnstructured(LPDIRECTMUSICBUFFER iface, REFERENCE_TIME rt, DWORD dwChannelGroup, DWORD cb, LPBYTE lpb)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, 0x%s, %d, %d, %p): stub\n", This, wine_dbgstr_longlong(rt), dwChannelGroup, cb, lpb);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_ResetReadPtr(LPDIRECTMUSICBUFFER iface)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p): stub\n", This);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_GetNextEvent(LPDIRECTMUSICBUFFER iface, LPREFERENCE_TIME prt, LPDWORD pdwChannelGroup, LPDWORD pdwLength, LPBYTE* ppData)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, %p, %p, %p, %p): stub\n", This, prt, pdwChannelGroup, pdwLength, ppData);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_GetRawBufferPtr(LPDIRECTMUSICBUFFER iface, LPBYTE* data)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    TRACE("(%p)->(%p)\n", iface, data);

    if (!data)
        return E_POINTER;

    *data = This->data;

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_GetStartTime(LPDIRECTMUSICBUFFER iface, LPREFERENCE_TIME prt)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, %p): stub\n", This, prt);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_GetUsedBytes(LPDIRECTMUSICBUFFER iface, LPDWORD pcb)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, %p): stub\n", This, pcb);

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_GetMaxBytes(LPDIRECTMUSICBUFFER iface, LPDWORD max_bytes)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    TRACE("(%p)->(%p)\n", iface, max_bytes);

    if (!max_bytes)
        return E_POINTER;

    *max_bytes = This->size;

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_GetBufferFormat(LPDIRECTMUSICBUFFER iface, LPGUID format)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    TRACE("(%p)->(%p)\n", iface, format);

    if (!format)
        return E_POINTER;

    *format = This->format;
    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_SetStartTime(LPDIRECTMUSICBUFFER iface, REFERENCE_TIME rt)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, 0x%s): stub\n", This, wine_dbgstr_longlong(rt));

    return S_OK;
}

static HRESULT WINAPI IDirectMusicBufferImpl_SetUsedBytes (LPDIRECTMUSICBUFFER iface, DWORD cb)
{
    IDirectMusicBufferImpl *This = impl_from_IDirectMusicBuffer(iface);

    FIXME("(%p, %d): stub\n", This, cb);

    return S_OK;
}

static const IDirectMusicBufferVtbl DirectMusicBuffer_Vtbl = {
	IDirectMusicBufferImpl_QueryInterface,
	IDirectMusicBufferImpl_AddRef,
	IDirectMusicBufferImpl_Release,
	IDirectMusicBufferImpl_Flush,
	IDirectMusicBufferImpl_TotalTime,
	IDirectMusicBufferImpl_PackStructured,
	IDirectMusicBufferImpl_PackUnstructured,
	IDirectMusicBufferImpl_ResetReadPtr,
	IDirectMusicBufferImpl_GetNextEvent,
	IDirectMusicBufferImpl_GetRawBufferPtr,
	IDirectMusicBufferImpl_GetStartTime,
	IDirectMusicBufferImpl_GetUsedBytes,
	IDirectMusicBufferImpl_GetMaxBytes,
	IDirectMusicBufferImpl_GetBufferFormat,
	IDirectMusicBufferImpl_SetStartTime,
	IDirectMusicBufferImpl_SetUsedBytes
};

HRESULT DMUSIC_CreateDirectMusicBufferImpl(LPDMUS_BUFFERDESC desc, LPVOID* ret_iface)
{
    IDirectMusicBufferImpl* dmbuffer;
    HRESULT hr;

    TRACE("(%p, %p)\n", desc, ret_iface);

    *ret_iface = NULL;

    dmbuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IDirectMusicBufferImpl));
    if (!dmbuffer)
        return E_OUTOFMEMORY;

    dmbuffer->IDirectMusicBuffer_iface.lpVtbl = &DirectMusicBuffer_Vtbl;
    dmbuffer->ref = 0; /* Will be inited by QueryInterface */

    if (IsEqualGUID(&desc->guidBufferFormat, &GUID_NULL))
        dmbuffer->format = KSDATAFORMAT_SUBTYPE_MIDI;
    else
        dmbuffer->format = desc->guidBufferFormat;
    dmbuffer->size = (desc->cbBuffer + 3) & ~3; /* Buffer size must be multiple of 4 bytes */

    dmbuffer->data = HeapAlloc(GetProcessHeap(), 0, dmbuffer->size);
    if (!dmbuffer->data) {
        HeapFree(GetProcessHeap(), 0, dmbuffer);
        return E_OUTOFMEMORY;
    }

    hr = IDirectMusicBufferImpl_QueryInterface((LPDIRECTMUSICBUFFER)dmbuffer, &IID_IDirectMusicBuffer, ret_iface);
    if (FAILED(hr))
    {
        HeapFree(GetProcessHeap(), 0, dmbuffer->data);
        HeapFree(GetProcessHeap(), 0, dmbuffer);
    }

    return hr;
}
