/*\!brief playthrough audio
* \file playthough
* \author Sergey Slyutov
* \date October 20, 2022
*/

#include "playthrough.h"

#include <QMediaDevices>

#include <QThread>

#include <QSettings>


CPlaythrough::CPlaythrough()
	: QWidget()
	, m_audiosource(NULL)
	, m_audiosink(NULL)
	, m_iosource(NULL)
{
	init();
};

CPlaythrough::~CPlaythrough()
{
	QSettings().setValue(playthrough_volume_flag, m_audiosink->volume());

	reset();
};

void CPlaythrough::reset(void)
{
	if (m_audiosink) {
		m_audiosink->stop();
		delete m_audiosink;
		m_audiosink = NULL;
	}
	if (m_audiosource) {
		m_audiosource->stop();
		delete m_audiosource;
		m_audiosource = NULL;
	}
}

void CPlaythrough::init(QString audiosrcname, QString audiosinkname)
{

	QAudioFormat format;

	format.setSampleRate(48000);

	format.setChannelCount(2);

	format.setSampleFormat(QAudioFormat::Int16);


	for (auto devinput : QMediaDevices::audioInputs()) {
		if (devinput.description().compare(audiosrcname) == 0) {
			m_audiosource = new QAudioSource(devinput, format);
			break;
		}
	}
	if( m_audiosource == NULL)
		m_audiosource = new QAudioSource(format); // initialize with default input device

	for(auto devoutput: QMediaDevices::audioOutputs()){
		if (devoutput.description().compare(audiosinkname) == 0){
			m_audiosink = new QAudioSink(devoutput, format);
			break;
		}
	}

	if(m_audiosink == NULL)
		m_audiosink = new QAudioSink(format); // initialize with default audio output device

	m_iosource = m_audiosource->start();

	QThread::msleep(200); // this delay sometimes required to for successfull work of both devices

	m_audiosink->setVolume(QSettings().value(playthrough_volume_flag, 1.0).toFloat());

	m_audiosink->start(m_iosource);
	
};



