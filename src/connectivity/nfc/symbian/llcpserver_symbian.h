/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef LLCPSERVER_SYMBIAN_H_
#define LLCPSERVER_SYMBIAN_H_

#include <e32base.h>
#include <nfcserver.h>                      // RNfcServer
#include <llcpprovider.h>                   // CLlcpProvider
#include <llcpconnorientedlistener.h>       // MLlcpConnOrientedListener
#include <qmobilityglobal.h>
#include "../qllcpserver_symbian_p.h"


class CLlcpSocketType2;

class CLlcpServer : public CBase,
                    public MLlcpConnOrientedListener
   {
public:
   /*
    * Creates a new CLlcpServer object.
    */
   static CLlcpServer* NewL(QtMobility::QLlcpServerPrivate&);

   /*
    * Destructor
    */
   ~CLlcpServer();

public:
   TBool Listen( const TDesC8& aServiceName);
   void StopListening();
   TBool isListening() const;
   CLlcpSocketType2 *nextPendingConnection();
   TBool hasPendingConnections() const;
   const TDesC8& serviceUri() const;

private: // From MLlcpConnOrientedListener
    void RemoteConnectRequest( MLlcpConnOrientedTransporter* aConnection );

private:
    // Constructor
    CLlcpServer(QtMobility::QLlcpServerPrivate&);

    // Second phase constructor
    void ConstructL();

private:

    RPointerArray<CLlcpSocketType2>  iLlcpSocketArray;

   /*
    * Handle to NFC-server.
    * Own.
    */
   RNfcServer iNfcServer;

   /*
    * Pointer to CLlcpProvider object.
    * Own.
    */
   CLlcpProvider* iLlcp;

   TBool iSocketListening;

   RBuf8 iServiceName;

   QtMobility::QLlcpServerPrivate& iCallback;
   };

#endif /* LLCPSERVER_SYMBIAN_H_ */
