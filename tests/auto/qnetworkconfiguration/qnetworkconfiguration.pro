SOURCES  += tst_qnetworkconfiguration.cpp
HEADERS  += ../qbearertestcommon.h
TARGET = tst_qnetworkconfiguration
CONFIG += testcase

QT = core network

include(../../../common.pri)
CONFIG += mobility
MOBILITY = bearer

symbian {
    TARGET.CAPABILITY = NetworkServices NetworkControl ReadUserData 
}

maemo6|maemo5 {
    CONFIG += link_pkgconfig

    PKGCONFIG += conninet
}
