/*!\brief video recorder form
* \file videorecorder.h
* \author Sergey Slyutov
* \date September 12, 2022
*/

#include "videorecorder.h"

#include <QSettings>

#include <QMediaDevices>

#include <QCameraDevice>

#include <QAudioDevice>

#include <QVideoWidget>

#include <QThread>

#include <QTime>

#include <QFileDialog>

#include <QCameraFormat>

#include "constants.h"

#include "playthrough.h"

#include <QSlider>

#include <QMessageBox>

#include "timersettingswidget.h"

CVideoRecorderForm::CVideoRecorderForm() :
	QWidget()
	, Ui_VideoRecorderForm()
{

	// add save restore for auto stop timer, auto stop checkbox state and for default location
	setupUi(this);

	QByteArray geometry = QSettings().value(settings_recorderGeometry).toByteArray();
	restoreGeometry(geometry);

	// restore settings
	m_stoprectimer = QSettings().value(settings_stoprectimer, 3600 + (16 * 60)).toInt(); // restore settings of autostop recording timer or use a default 1 hour 15 minutes value

	cbStopRecordByTimer->setChecked(QSettings().value(settings_autostopRecordingFlag, false).toBool()); // restore activation flag of autostop recording mode.


	pushPause->setEnabled(false);

	QGridLayout* innergridVideofinder = new QGridLayout();

	gridVideofinder->addWidget(&m_videowidget, 0, 0);
	m_videowidget.setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	gridVideofinder->addLayout(innergridVideofinder, 1, 0);

	labelOutputLocation->setText(QString("output location: %1").arg(outputlocation()));
	qDebug() << QUrl().fromLocalFile(outputlocation());

	//m_recorder.setOutputLocation(outputlocation());
	m_recorder.setOutputLocation(QUrl().fromLocalFile(outputlocation()));
	

	connect(pushOutputLocation, &QPushButton::released, [this]()
		{
			while (true) {
				QUrl selectedoutput = QFileDialog::getExistingDirectoryUrl(this, qAppName(), QUrl::fromLocalFile(outputlocation()));
				if (selectedoutput.isEmpty())
					return;

				QFileInfo dir_info(selectedoutput.toLocalFile());
				if (!dir_info.isWritable()) {
				
					if (QMessageBox::information(this, qAppName(),
						"Selected folder is not writable. Click Yes to select another location.",
						QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
						return;
					}
				
				}else{
					outputlocation(selectedoutput.toLocalFile());
					labelOutputLocation->setText(selectedoutput.toLocalFile());
					m_recorder.setOutputLocation(selectedoutput);
					return;
				}
			}
		});

	m_capturesession.setVideoOutput(&m_videowidget);

	QCamera* pcamera = new QCamera();
	m_capturesession.setCamera(pcamera);
	pcamera->start();
	m_capturesession.setAudioInput(&m_audin);
	
	m_capturesession.setRecorder(&m_recorder);

	connect(cbVideoInputs, SIGNAL(currentIndexChanged(int)), this, SLOT(handleVideoInputstIndexChanged(int)));

	connect(cbAudioInputs, SIGNAL(currentIndexChanged(int)), this, SLOT(handleAudioInputsIndexChanged(int)));

	handleVideoInputsChanged();

	handleAudioInputsChanged();

	displayStopTimer(m_stoprectimer); // display initial auto stop record timer settings

	connect(&m_mediadevices, SIGNAL(videoInputsChanged()), this, SLOT(handleVideoInputsChanged()));

	connect(&m_mediadevices, SIGNAL(audioInputsChanged()), this, SLOT(handleAudioInputsChanged()));

	bool bcon = connect(&m_recorder, &QMediaRecorder::recorderStateChanged, [=](QMediaRecorder::RecorderState state) {handleRecorderStateChanged(state); });

	bcon = connect(&m_recorder, &QMediaRecorder::actualLocationChanged, [=](const QUrl& location) { labelOutputLocation->setText(QString("output location : %1").arg(location.toLocalFile()));  });

	bcon = connect(&m_recorder, &QMediaRecorder::durationChanged, [=](qint64 duration) {
		
		displayRecDuration(duration);

		if (cbStopRecordByTimer->isChecked()) {

			if( m_stoprectimer <= duration/1000 )

				m_recorder.stop();

			displayStopTimer(m_stoprectimer - (duration / 1000));
			
		}
	});

	bcon = connect(pushTimeout, &QPushButton::released, [=]() {

		CTimerSettingsWidget timersettings(m_stoprectimer);
		timersettings.exec();
		
		if (timersettings.result() != QDialog::Accepted)
			return;

		m_stoprectimer = timersettings.timervalue();

		if (m_recorder.recorderState() != QMediaRecorder::RecordingState
			|| !cbStopRecordByTimer->isChecked() ) { // display new auto record timer settings immediately if recording is not in progress or autostop recording is inactive
			displayStopTimer(m_stoprectimer);
		}
	});

	hsVolume->setMinimum(0);
	hsVolume->setMaximum(100);
	hsVolume->setValue(m_playthough.volume());
	connect(hsVolume, &QSlider::valueChanged, [=](int value) {
		m_playthough.volume(value);
	});
};

CVideoRecorderForm::~CVideoRecorderForm()
{
	QSettings().setValue(settings_stoprectimer, m_stoprectimer); // save autostop recording timer settings.

	QSettings().setValue(settings_autostopRecordingFlag, cbStopRecordByTimer->isChecked()); // save activation flag of autostop recording mode.

	QByteArray recorderGeometry = saveGeometry();
	QSettings().setValue(settings_recorderGeometry, recorderGeometry);
};

/*!\brief handle changing cameras selection in combobox wih a list of cameras
* \param index index of the selected item in the combobox.
*/
void CVideoRecorderForm::handleVideoInputstIndexChanged(int index)
{
	(void)index; // not used

	QString vidin = cbVideoInputs->currentData().toString(); // get the description of the currently selected camera.

	for (auto viddev : QMediaDevices::videoInputs()) { // enum all cameras
		 
		if (viddev.description().compare(vidin) == 0) { // find the camera which equal to the selected one

			QSettings().setValue("videoinput", vidin); // save description of the selected camera in registry. it will be used as default selection upon next launch of the application
			
			QCamera* pcamera = new QCamera(); // create new object of QCamera. Qt Multimedia is not perfect, an easy way to avoid complications.
		
			pcamera->setCameraDevice(viddev); // assign the input device selected in the combobox to the new QCamera instance

			pcamera->start(); // start  the new camera 

			QCamera* prevCamera = m_capturesession.camera();

			m_capturesession.setCamera(pcamera); // set the new camera to the capture session as a video input.

			delete prevCamera; // dete
		}
	}
};

void CVideoRecorderForm::handleAudioInputsIndexChanged(int)
{
	QString audin = cbAudioInputs->currentData().toString();
	
	for (auto auddev : QMediaDevices::audioInputs()) {

		if (auddev.description().compare(audin) == 0) {

			QSettings().setValue("audioinput", audin);

			m_audin.setDevice(auddev);

			m_playthough.reset();

			m_playthough.init(auddev.description());
		}
	}
};

void CVideoRecorderForm::handleVideoInputsChanged(void)
{
	QString curvidin;

	if (cbVideoInputs->count()) {

		curvidin = cbVideoInputs->currentData().toString();
	} else {
		curvidin = QSettings().value("videoinput", QMediaDevices::defaultVideoInput().description()).toString();
	}
	
	cbVideoInputs->clear();

	int i = 0, sel = 0;

	QCameraDevice curcamdev = QMediaDevices::defaultVideoInput();

	for (auto vidin: QMediaDevices::videoInputs()) {

		cbVideoInputs->addItem(vidin.description(), vidin.description());

		if (curvidin.compare(vidin.description()) == 0) {

			sel = i;
			curcamdev = vidin;
			
		}
		i++;
	}

	QCamera* newcamera = new QCamera();
	
	if (!curcamdev.isNull()) {

		newcamera->setCameraDevice(curcamdev);

		newcamera->start();

		QCamera* prevCamera = m_capturesession.camera();

		m_capturesession.setCamera(newcamera);

		if (prevCamera)

			delete (prevCamera);

		cbVideoInputs->setCurrentIndex(sel);
	}

};

void CVideoRecorderForm::handleAudioInputsChanged(void)
{
	QString curaudin;

	if (cbAudioInputs->count()) {

		curaudin = cbAudioInputs->currentData().toString();

	}else {

		curaudin = QMediaDevices::defaultAudioInput().description();

	}

	cbAudioInputs->clear();

	QAudioDevice curauddev = QMediaDevices::defaultAudioInput();
	
	int i = 0, sel = 0;
	
	for (auto auddev : QMediaDevices::audioInputs()) {

		cbAudioInputs->addItem(auddev.description(), auddev.description());

		if (curaudin.compare(auddev.description()) == 0) {

			curauddev = auddev;

			sel = i;

		}
		i++;
	}

	if (!curauddev.isNull()) {

		m_audin.setDevice(curauddev);

		cbAudioInputs->setCurrentIndex(sel);
	}

};

void CVideoRecorderForm::on_pushRecord_released()
{
	switch (m_recorder.recorderState()) {
		case QMediaRecorder::PausedState:
			m_recorder.stop();
			break;

		case QMediaRecorder::RecordingState:
			m_recorder.stop();
			break;

		case QMediaRecorder::StoppedState: {
			m_recorder.record();
			break;
		}
	}
};

void CVideoRecorderForm::on_pushPause_released()
{
	switch (m_recorder.recorderState()) {
		case QMediaRecorder::RecordingState:
			m_recorder.pause();
			break;
		case QMediaRecorder::PausedState:
			m_recorder.record(); // resume
			break;
	}
};

void CVideoRecorderForm::handleRecorderStateChanged(QMediaRecorder::RecorderState state)
{
	switch (state) {
		case QMediaRecorder::PausedState:
			pushPause->setEnabled(true);
			pushPause->setText("Resume");
			pushRecord->setText("Stop");
			break;
		case QMediaRecorder::RecordingState:
			pushPause->setEnabled(true);
			pushPause->setText("Pause");
			pushRecord->setText("Stop");
			break;
		case QMediaRecorder::StoppedState:
			pushPause->setEnabled(false);
			pushRecord->setText("Record");
			displayStopTimer(m_stoprectimer);
			break;
	}
};

void CVideoRecorderForm::displayRecDuration(quint64 durvalue)
{
	QTime ti_dur(0, 0, 0, 0);
	ti_dur = ti_dur.addMSecs(durvalue);
	lcddurhour->display(ti_dur.hour());
	lcddurmin->display((ti_dur.minute() == 60) ? 0 : ti_dur.minute());
	lcddursec->display((ti_dur.second() == 60) ? 0 : ti_dur.second());
};

void CVideoRecorderForm::displayStopTimer(quint64 timervalue)
{
	QTime ti_stop(0, 0, 0, 0);
	ti_stop = ti_stop.addSecs(timervalue);
	lcdstophour->display(ti_stop.hour());
	lcdstopmin->display( (ti_stop.minute() == 60) ? 0 : ti_stop.minute() );
	lcdstopsec->display( (ti_stop.second() == 60) ? 0: ti_stop.second() );
};


