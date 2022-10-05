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

		void on_pushVideoSettings_released();

		void on_pushAudioSettings_released();

	private:

		void refreshVideoInput(void);

		QVideoWidget	m_videowidget;

		QLabel			m_labelfilename;

		QPushButton		m_pushLocation;

		QMediaCaptureSession	m_capturesession;
						
		QAudioInput		m_audin;

		QMediaDevices	m_mediadevices;

		QMediaRecorder	m_recorder;

		QLabel			m_labelDuration;

};
