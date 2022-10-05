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

CVideoRecorderForm::CVideoRecorderForm() :
	QWidget(),
	Ui_VideoRecorderForm(),
	m_labelfilename("output location:")
{

	setupUi(this);
	pushPause->setEnabled(false);

	QGridLayout* innergridVideofinder = new QGridLayout();

	gridVideofinder->addWidget(&m_videowidget, 0, 0);
	m_videowidget.setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	gridVideofinder->addLayout(innergridVideofinder, 1, 0);
	innergridVideofinder->addWidget(&m_labelfilename, 0, 0);
	m_labelfilename.setText(QString("output location: %1").arg(m_recorder.actualLocation().toString()));
	m_labelfilename.setSizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Minimum);

	innergridVideofinder->addWidget(&m_pushLocation, 0, 1);
	m_pushLocation.setText("Location");
	m_pushLocation.setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Minimum);
	connect(&m_pushLocation, &QPushButton::released, [this]() {
		QUrl  selurl = QFileDialog::getExistingDirectoryUrl();
		if (!selurl.isEmpty()) {
			m_recorder.setOutputLocation(selurl);
		}
		});

	gridVideofinder->addWidget(&m_labelDuration, 2, 0);

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

	connect(&m_mediadevices, SIGNAL(videoInputsChanged()), this, SLOT(handleVideoInputsChanged()));

	connect(&m_mediadevices, SIGNAL(audioInputsChanged()), this, SLOT(handleAudioInputsChanged()));

	bool bcon = connect(&m_recorder, &QMediaRecorder::recorderStateChanged, [=](QMediaRecorder::RecorderState state) {handleRecorderStateChanged(state); });

	bcon = connect(&m_recorder, &QMediaRecorder::actualLocationChanged, [=](const QUrl& location) { m_labelfilename.setText(QString("output location : %1").arg(location.toLocalFile()));  });

	bcon = connect(&m_recorder, &QMediaRecorder::durationChanged, [=](qint64 duration) {
		QTime time(0, 0, 0, 0);
		QTime ti = time.addMSecs(duration);
		m_labelDuration.setText(QString("duration: %1").arg(ti.toString("hh:mm:ss.zzz")));
		});


	QMediaFormat medfor = m_recorder.mediaFormat();

	int sel = 0;
	QList<QMediaFormat::VideoCodec> videoCodes = medfor.supportedVideoCodecs((QMediaFormat::ConversionMode)QMediaFormat::Encode);
	cbVideoCodec->addItem("Unspecified", (int)QMediaFormat::VideoCodec::Unspecified);
	for (auto vidcod : videoCodes) {
		cbVideoCodec->addItem(medfor.videoCodecName(vidcod), (int)vidcod);
		sel = (medfor.videoCodec() == vidcod) ? cbVideoCodec->count() : sel;
	}
	cbVideoCodec->setCurrentIndex(sel);


	sel = 0;
	QList<QMediaFormat::AudioCodec> audioCodes = medfor.supportedAudioCodecs((QMediaFormat::ConversionMode)QMediaFormat::Encode);
	cbAudioCodec->addItem("Unspecified", (int)QMediaFormat::AudioCodec::Unspecified);
	for (auto audcod : audioCodes) {
		cbAudioCodec->addItem(medfor.audioCodecName(audcod), (int)audcod);
		sel = (medfor.audioCodec() == audcod) ? cbAudioCodec->count() : sel;
	}
	cbAudioCodec->setCurrentIndex(sel);

	sel = 0;
	QList<QMediaFormat::FileFormat> fileFormats = medfor.supportedFileFormats((QMediaFormat::ConversionMode)QMediaFormat::Encode);
	cbFileFormat->addItem("Unspecified", (int)QMediaFormat::FileFormat::UnspecifiedFormat);
	for (auto filfor : fileFormats) {
		cbFileFormat->addItem(medfor.fileFormatName(filfor), (int)filfor);
		sel = (medfor.fileFormat() == filfor) ? cbFileFormat->count() : sel;
	}
	cbFileFormat->setCurrentIndex(sel);

	medfor.setAudioCodec((QMediaFormat::AudioCodec)cbAudioCodec->currentData().toInt());
	medfor.setFileFormat((QMediaFormat::FileFormat)cbFileFormat->currentData().toInt());
	medfor.setVideoCodec((QMediaFormat::VideoCodec)cbVideoCodec->currentData().toInt());

	m_recorder.setMediaFormat(medfor);

	cbAudioCodec->setVisible(false);
	cbVideoCodec->setVisible(false);
	cbFileFormat->setVisible(false);

	pushVideoSettings->setVisible(false);
	pushAudioSettings->setVisible(false);


};

CVideoRecorderForm::~CVideoRecorderForm()
{
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
	
	newcamera->setCameraDevice(curcamdev);

	newcamera->start();

	QCamera* prevCamera = m_capturesession.camera();

	m_capturesession.setCamera(newcamera);

	if (prevCamera)

		delete (prevCamera);

	cbVideoInputs->setCurrentIndex(sel);

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

	m_audin.setDevice(curauddev);

	cbAudioInputs->setCurrentIndex(sel);

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

			QMediaFormat mediaformat = m_recorder.mediaFormat();
			mediaformat.setVideoCodec((QMediaFormat::VideoCodec) cbVideoCodec->currentData().toInt());
			mediaformat.setAudioCodec((QMediaFormat::AudioCodec) cbAudioCodec->currentData().toInt());
			mediaformat.setFileFormat((QMediaFormat::FileFormat)cbFileFormat->currentData().toInt());
			m_recorder.setMediaFormat(mediaformat);

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
			break;
	}
};


void CVideoRecorderForm::on_pushVideoSettings_released()
{

}
void CVideoRecorderForm::on_pushAudioSettings_released()
{

}
