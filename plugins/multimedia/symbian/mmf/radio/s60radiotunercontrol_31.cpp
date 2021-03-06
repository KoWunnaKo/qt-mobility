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

#include "s60radiotunercontrol_31.h"
#include "s60radiotunerservice.h"
#include "s60mmtrace.h"

// from AudioPreference.h
const TInt KAudioPriorityFMRadio = 79;
const TUint KAudioPrefRadioAudioEvent = 0x03000001;

S60RadioTunerControl::S60RadioTunerControl(QObject *parent)
    : QRadioTunerControl(parent)
    , m_error(0)
    , m_tunerState(0)
    , m_apiTunerState(QRadioTuner::StoppedState)
    , m_audioInitializationComplete(false)
    , m_radioError(QRadioTuner::NoError)
    , m_muted(false)
    , m_isStereo(true)
    , m_stereoMode(QRadioTuner::Auto)
    , m_signal(0)
    , m_currentBand(QRadioTuner::FM)
    , m_currentFreq(87500000)
    , m_scanning(false)
    , m_vol(50)
{
    TRACE("S60RadioTunerControl::S60RadioTunerControl" << qtThisPtr());
    initRadio();
}

S60RadioTunerControl::~S60RadioTunerControl()
{
    TRACE("S60RadioTunerControl::~S60RadioTunerControl" << qtThisPtr());
	if (m_tunerUtility) {
	    m_tunerUtility->Close();
		m_tunerUtility->CancelNotifyChange();
		m_tunerUtility->CancelNotifySignalStrength();
		m_tunerUtility->CancelNotifyStereoChange();
		delete m_tunerUtility;
	}
}

bool S60RadioTunerControl::initRadio()
{
	m_available = false;

	TRAPD(tunerError, m_tunerUtility = CMMTunerUtility::NewL(*this, CMMTunerUtility::ETunerBandFm, 1, 
	  										                 CMMTunerUtility::ETunerAccessPriorityNormal));
	if (tunerError != KErrNone) {
        m_radioError = QRadioTuner::OpenError;
        return m_available;
    }
	
	TRAPD(playerError, m_audioPlayerUtility = m_tunerUtility->TunerPlayerUtilityL(*this));
	if (playerError != KErrNone) {
		m_radioError = QRadioTuner::OpenError;
		return m_available;
	}
	
	TRAPD(initializeError, m_audioPlayerUtility->InitializeL(KAudioPriorityFMRadio, 
												             TMdaPriorityPreference(KAudioPrefRadioAudioEvent)));
	if (initializeError != KErrNone) {
		m_radioError = QRadioTuner::OpenError;
		return m_available;
	}
		
	m_tunerUtility->NotifyChange(*this);
	m_tunerUtility->NotifyStereoChange(*this);
	m_tunerUtility->NotifySignalStrength(*this);
	
	TFrequency freq(m_currentFreq);
	m_tunerUtility->Tune(freq);
		
	m_available = true;

    return m_available;
}

void S60RadioTunerControl::start()
{
    TRACE("S60RadioTunerControl::start" << qtThisPtr());
	if (!m_audioInitializationComplete) {
		TFrequency freq(m_currentFreq);
		m_tunerUtility->Tune(freq);
	} else {
		m_audioPlayerUtility->Play();
	}
	m_apiTunerState = QRadioTuner::ActiveState;
	emit stateChanged(m_apiTunerState);
}

void S60RadioTunerControl::stop()
{
    TRACE("S60RadioTunerControl::stop" << qtThisPtr());
    if (m_audioPlayerUtility) {
		m_audioPlayerUtility->Stop();
		m_apiTunerState = QRadioTuner::StoppedState;
		emit stateChanged(m_apiTunerState);
    }
}

QRadioTuner::State S60RadioTunerControl::state() const
{
    return m_apiTunerState;
}

QRadioTuner::Band S60RadioTunerControl::band() const
{
    return m_currentBand;
}

bool S60RadioTunerControl::isBandSupported(QRadioTuner::Band b) const
{
	if(b == QRadioTuner::FM)
		return true;
	else if(b == QRadioTuner::LW)
		return false;
	else if(b == QRadioTuner::AM)
		return true;
	else if(b == QRadioTuner::SW)
		return false;
	else
		return false;
}

void S60RadioTunerControl::setBand(QRadioTuner::Band b)
{
    QRadioTuner::Band tempBand = b;
    if (tempBand != m_currentBand) {
        TRACE("S60RadioTunerControl::setBand" << qtThisPtr() << "band" << b);
        m_currentBand = b;
        emit bandChanged(m_currentBand);
    }
}

int S60RadioTunerControl::frequency() const
{
    return m_currentFreq;
}

void S60RadioTunerControl::setFrequency(int frequency)
{
    TRACE("S60RadioTunerControl::setFrequency" << qtThisPtr() << "frequency" << frequency);
    m_currentFreq = frequency;
    TFrequency freq(m_currentFreq);
    m_tunerUtility->Tune(freq);
}

int S60RadioTunerControl::frequencyStep(QRadioTuner::Band b) const
{
    int step = 0;
    if(b == QRadioTuner::FM)
        step = 100000; // 100kHz steps
    else if(b == QRadioTuner::LW)
        step = 1000; // 1kHz steps
    else if(b == QRadioTuner::AM)
        step = 1000; // 1kHz steps
    else if(b == QRadioTuner::SW)
        step = 500; // 500Hz steps
    return step;
}

QPair<int,int> S60RadioTunerControl::frequencyRange(QRadioTuner::Band band) const
{
	TFrequency bottomFreq;
	TFrequency topFreq;
	int bandError = KErrNone;
	if (m_tunerUtility){
		bandError = m_tunerUtility->GetFrequencyBandRange(bottomFreq, topFreq);	   
		if (!bandError)
			return qMakePair<int,int>(bottomFreq.iFrequency, topFreq.iFrequency);
    }
   return qMakePair<int,int>(0,0);
}

CMMTunerUtility::TTunerBand S60RadioTunerControl::getNativeBand(QRadioTuner::Band b) const
{
    // api match to native s60 bands
    if (b == QRadioTuner::AM)
        return CMMTunerUtility::ETunerBandAm;
    else if (b == QRadioTuner::FM)
        return CMMTunerUtility::ETunerBandFm;
    else if (b == QRadioTuner::LW)
        return CMMTunerUtility::ETunerBandLw;
    else
        return CMMTunerUtility::ETunerNoBand;
}

bool S60RadioTunerControl::isStereo() const
{
    return m_isStereo;
}

QRadioTuner::StereoMode S60RadioTunerControl::stereoMode() const
{
    return m_stereoMode;
}

void S60RadioTunerControl::setStereoMode(QRadioTuner::StereoMode mode)
{
    TRACE("S60RadioTunerControl::setStereoMode" << qtThisPtr() << "mode" << mode);
	m_stereoMode = mode;
	if (m_tunerUtility) {	    
	    if (QRadioTuner::ForceMono == mode)
	        m_tunerUtility->ForceMonoReception(true);
	    else 
	        m_tunerUtility->ForceMonoReception(false);
     }
}

int S60RadioTunerControl::signalStrength() const
{
    // return value is a percentage value
    if (m_tunerUtility) {       
        TInt maxSignalStrength;
        TInt currentSignalStrength;
        m_error = m_tunerUtility->GetMaxSignalStrength(maxSignalStrength);       
        if (m_error == KErrNone) {
            m_error = m_tunerUtility->GetSignalStrength(currentSignalStrength);
            if (m_error == KErrNone) {
				if (maxSignalStrength == 0 || currentSignalStrength == 0) {
					return 0;
				}
                m_signal = ((TInt64)currentSignalStrength) * 100 / maxSignalStrength;
            }           
        }
    }
    return m_signal;
}

int S60RadioTunerControl::volume() const
{
    return m_vol;
}

void S60RadioTunerControl::setVolume(int volume)
{
    TRACE("S60RadioTunerControl::setVolume" << qtThisPtr() << "volume" << volume);
    if (m_audioPlayerUtility) {
		m_vol = volume;
		TInt error = m_audioPlayerUtility->SetVolume(volume/10);
		emit volumeChanged(m_vol);
    }
}

bool S60RadioTunerControl::isMuted() const
{
    return m_muted;
}

void S60RadioTunerControl::setMuted(bool muted)
{
    TRACE("S60RadioTunerControl::setMuted" << qtThisPtr() << "muted" << muted);
    if (m_audioPlayerUtility && m_audioInitializationComplete) {
        m_muted = muted;
        m_audioPlayerUtility->Mute(m_muted);
        emit mutedChanged(m_muted);           
    }
}

bool S60RadioTunerControl::isSearching() const
{
    if (m_tunerUtility) {
    	TUint32 tempState;
    	m_tunerUtility->GetState(tempState);
    	if (tempState == CMMTunerUtility::ETunerStateRetuning || m_scanning) {
			return true;
    	} else
    		return false;
    }
    return true;
}

void S60RadioTunerControl::cancelSearch()
{
    TRACE("S60RadioTunerControl::cancelSearch" << qtThisPtr());
	m_tunerUtility->CancelRetune();
	m_scanning = false;
	emit searchingChanged(false);
}

void S60RadioTunerControl::searchForward()
{
    TRACE("S60RadioTunerControl::searchForward" << qtThisPtr());
	m_scanning = true;
	setVolume(m_vol);
	m_tunerUtility->StationSeek(CMMTunerUtility::ESearchDirectionUp);
	emit searchingChanged(true);
}

void S60RadioTunerControl::searchBackward()
{
    TRACE("S60RadioTunerControl::searchBackward" << qtThisPtr());
	m_scanning = true;
	setVolume(m_vol);
	m_tunerUtility->StationSeek(CMMTunerUtility::ESearchDirectionDown);
	emit searchingChanged(true);
}

bool S60RadioTunerControl::isValid() const
{
    return m_available;
}

bool S60RadioTunerControl::isAvailable() const
{
    return m_available;
}

QtMultimediaKit::AvailabilityError S60RadioTunerControl::availabilityError() const
{
    if (m_available)
        return QtMultimediaKit::NoError;
    else
        return QtMultimediaKit::ResourceError;
}

QRadioTuner::Error S60RadioTunerControl::error() const
{
    return m_radioError;
}

QString S60RadioTunerControl::errorString() const
{
	return m_errorString;
}

void S60RadioTunerControl::MToTuneComplete(TInt aError)
{
    TRACE("S60RadioTunerControl::MToTuneComplete" << qtThisPtr() << "error" << error);
	if (aError == KErrNone) {
		m_scanning = false;
		m_audioPlayerUtility->Play();
		if (!m_audioInitializationComplete) {
		TRAPD(initializeError, m_audioPlayerUtility->InitializeL(KAudioPriorityFMRadio, 
                                                                 TMdaPriorityPreference(KAudioPrefRadioAudioEvent)));
        if (initializeError != KErrNone)
            m_radioError = QRadioTuner::OpenError;
		}
	}
}

void S60RadioTunerControl::MTcoFrequencyChanged(const TFrequency& aOldFrequency, const TFrequency& aNewFrequency)
{
    TRACE("S60RadioTunerControl::MTcoFrequencyChanged" << qtThisPtr()
          << "oldFreq" << aOldFrequency.iFrequency << "newFreq" << aNewFrequency.iFrequency);
	m_currentFreq = aNewFrequency.iFrequency;
	m_scanning = false;
	emit frequencyChanged(m_currentFreq);
}

void S60RadioTunerControl::MTcoStateChanged(const TUint32& aOldState, const TUint32& aNewState)
{
    TRACE("S60RadioTunerControl::MTcoStateChanged" << qtThisPtr()
          << "oldState" << aOldState << "newState" << aNewState);
	if (aNewState == CMMTunerUtility::ETunerStateActive)
		m_apiTunerState = QRadioTuner::ActiveState;
	if (aNewState == CMMTunerUtility::ETunerStatePlaying)
		m_apiTunerState = QRadioTuner::ActiveState;
	if (aOldState != aNewState)
		emit stateChanged(m_apiTunerState);
}

void S60RadioTunerControl::MTcoAntennaDetached()
{
    TRACE("S60RadioTunerControl::MTcoAntennaDetached" << qtThisPtr());
	// no actions
}

void S60RadioTunerControl::MTcoAntennaAttached()
{
    TRACE("S60RadioTunerControl::MTcoAntennaAttached" << qtThisPtr());
	// no actions
}

void S60RadioTunerControl::FlightModeChanged(TBool aFlightMode)
{
    TRACE("S60RadioTunerControl::FlightModeChanged" << qtThisPtr()
          << "enabled" << aFlightMode);
	// no actions
}

void S60RadioTunerControl::MTsoStereoReceptionChanged(TBool aStereo)
{
    TRACE("S60RadioTunerControl::MTsoStereoReceptionChanged" << qtThisPtr()
          << "stereo" << aStereo);
	m_isStereo = aStereo;
	emit stereoStatusChanged(aStereo);
}

void S60RadioTunerControl::MTsoForcedMonoChanged(TBool aForcedMono)
{
    TRACE("S60RadioTunerControl::MTsoForcedMonoChanged" << qtThisPtr()
          << "forcedMono" << aForcedMono);
	if (aForcedMono)
		m_stereoMode = QRadioTuner::ForceMono;
}

void S60RadioTunerControl::MssoSignalStrengthChanged(TInt aNewSignalStrength)
{
    TRACE("S60RadioTunerControl::MssoSignalStrengthChanged" << qtThisPtr()
          << "signalStrength" << aNewSignalStrength);
	m_signal = aNewSignalStrength;
	emit signalStrengthChanged(m_signal);
}

void S60RadioTunerControl::MTapoInitializeComplete(TInt aError)
{
    TRACE("S60RadioTunerControl::MTapoInitializeComplete" << qtThisPtr()
          << "error" << aError);
	if (aError == KErrNone) {
		m_audioInitializationComplete = true;
		m_available = true;
		m_audioPlayerUtility->Play();
		m_apiTunerState = QRadioTuner::ActiveState;
		emit stateChanged(m_apiTunerState);
	} else if (aError != KErrNone) {
		m_radioError = QRadioTuner::OpenError;
	}
}

void S60RadioTunerControl::MTapoPlayEvent(TEventType aEvent, TInt aError, TAny* aAdditionalInfo)
{
    TRACE("S60RadioTunerControl::MTapoPlayEvent" << qtThisPtr()
          << "event" << aEvent << "error" << aError);
	// no actions
}
