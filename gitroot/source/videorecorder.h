/*\!brief video recorder
* \file videorecorder.h
* \author Sergey Slyutov
* \date September 12, 2022
*/

#include <QWidget>

#include <ui_videorecorder.h>

#include <QMediaCaptureSession>

#include <QCamera>

#include <QAudioInput>

#include <QVideoWidget>

#include <QLabel>

#include <QMediaDevices>

#include <QMediaRecorder>

#include "playthrough.h"

#include <QMediaPlayer>

#include <QSettings>

#include <QStandardPaths>

#include <QDir>

class CVideoRecorderForm :

	public QWidget,

	public Ui_VideoRecorderForm

{
	Q_OBJECT

	public: 

		CVideoRecorderForm();

		virtual ~CVideoRecorderForm();


	protected slots:

		void handleVideoInputstIndexChanged(int);
		
		void handleAudioInputsIndexChanged(int);

		void handleVideoInputsChanged();

		void handleAudioInputsChanged();

		void on_pushRecord_released();

		void on_pushPause_released();

		void handleRecorderStateChanged(QMediaRecorder::RecorderState);

	private:

		void refreshVideoInput(void);

		QVideoWidget	m_videowidget;

		QMediaCaptureSession	m_capturesession;
						
		QAudioInput		m_audin;

		QMediaDevices	m_mediadevices;

		QMediaRecorder	m_recorder;

		CPlaythrough	m_playthough;

		const char*		settings_stoprectimer = "AutostopRecordingTimer";
		qint64			m_stoprectimer;

		const char*		settings_autostopRecordingFlag = "AutostopRecordingFlag";

		// output location for media
		const char* settings_outputlocation = "outputLocation";

		const char* settings_recorderGeometry = "recorderGeometry";

		QString outputlocation(void) {    
			QString outputlocation = QSettings().value(
				settings_outputlocation, 
				QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)).toString();

			if ( !QDir(outputlocation).exists() )
				return QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

			return outputlocation;
		};

		void outputlocation(QString location)
		{
			QSettings().setValue(settings_outputlocation, location);
		};

		void displayRecDuration(quint64 durvalue);

		/* refresh timer UI controls with the new timer value
		* \param timervalue value in seconds that has to be displayed
		*/
		void displayStopTimer(quint64 timervalue);

};
