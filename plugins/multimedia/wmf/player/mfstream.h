/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef MFSTREAM_H
#define MFSTREAM_H

#include <mfapi.h>
#include <mfidl.h>
#include <QtCore/qmutex.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qcoreevent.h>

QT_USE_NAMESPACE

class MFStream : public QObject, public IMFByteStream
{
    Q_OBJECT
public:
    MFStream(QIODevice *stream, bool ownStream);

    ~MFStream();

    //from IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppvObject);

    STDMETHODIMP_(ULONG) AddRef(void);

    STDMETHODIMP_(ULONG) Release(void);


    //from IMFByteStream
    STDMETHODIMP GetCapabilities(DWORD *pdwCapabilities);

    STDMETHODIMP GetLength(QWORD *pqwLength);

    STDMETHODIMP SetLength(QWORD);

    STDMETHODIMP GetCurrentPosition(QWORD *pqwPosition);

    STDMETHODIMP SetCurrentPosition(QWORD qwPosition);

    STDMETHODIMP IsEndOfStream(BOOL *pfEndOfStream);

    STDMETHODIMP Read(BYTE *pb, ULONG cb, ULONG *pcbRead);

    STDMETHODIMP BeginRead(BYTE *pb, ULONG cb, IMFAsyncCallback *pCallback,
                           IUnknown *punkState);

    STDMETHODIMP EndRead(IMFAsyncResult* pResult, ULONG *pcbRead);

    STDMETHODIMP Write(const BYTE *, ULONG, ULONG *);

    STDMETHODIMP BeginWrite(const BYTE *, ULONG ,
                            IMFAsyncCallback *,
                            IUnknown *);

    STDMETHODIMP EndWrite(IMFAsyncResult *,
                          ULONG *);

    STDMETHODIMP Seek(
        MFBYTESTREAM_SEEK_ORIGIN SeekOrigin,
        LONGLONG llSeekOffset,
        DWORD,
        QWORD *pqwCurrentPosition);

    STDMETHODIMP Flush();

    STDMETHODIMP Close();

private:
    class AsyncReadState : public IUnknown
    {
    public:
        AsyncReadState(BYTE *pb, ULONG cb);

        //from IUnknown
        STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppvObject);

        STDMETHODIMP_(ULONG) AddRef(void);

        STDMETHODIMP_(ULONG) Release(void);

        BYTE* pb() const;
        ULONG cb() const;
        ULONG bytesRead() const;

        void setBytesRead(ULONG cbRead);

    private:
        long m_cRef;
        BYTE *m_pb;
        ULONG m_cb;
        ULONG m_cbRead;
    };

    long m_cRef;
    QIODevice *m_stream;
    bool m_ownStream;
    DWORD m_workQueueId;
    QMutex m_mutex;

    void doRead();

private Q_SLOTS:
    void handleReadyRead();

protected:
    void customEvent(QEvent *event);
    IMFAsyncResult *m_currentReadResult;
};

#endif