/*!\brief constants of the application
* \file constants.h
* \date September 16, 2022
* \author Sergey Slyutov
*/

#ifndef _CONTANTS_H

#define _CONSTANTS_H

#include <QMap>

#include <QString>

#include <QMediaFormat>

QMap<QMediaFormat::AudioCodec, QString> mapAudioCodecs = {
	{QMediaFormat::AudioCodec::WMA		  , "Windows Media Audio"},
	{QMediaFormat::AudioCodec::AC3		  ,	"Dolby Digital"},
	{QMediaFormat::AudioCodec::AAC		  ,	"Advanced Audio Coding"},
	{QMediaFormat::AudioCodec::ALAC		  ,	"Apple Lossless Audio Codec"},
	{QMediaFormat::AudioCodec::DolbyTrueHD,	"Dolby TrueHD"},
	{QMediaFormat::AudioCodec::EAC3		  ,	"Dolby Digital Plus (EAC3)"},
	{QMediaFormat::AudioCodec::MP3		  ,	"MPEG-1 Audio Layer III or MPEG-2 Audio Layer III"},
	{QMediaFormat::AudioCodec::Wave		  ,	"Waveform Audio File Format"},
	{QMediaFormat::AudioCodec::Vorbis	  ,	"Ogg Vorbis"},
	{QMediaFormat::AudioCodec::FLAC		  ,	"Free Lossless Audio Codec"},
	{QMediaFormat::AudioCodec::Opus		  ,	"Opus Audio Format"}
};

QMap<QMediaFormat::VideoCodec, QString> mapVideoCodecs = {
	{QMediaFormat::VideoCodec::MPEG1,	  "MPEG-1"},
	{QMediaFormat::VideoCodec::MPEG2,	  "MPEG-2"},
	{QMediaFormat::VideoCodec::MPEG4,	  "MPEG-4"},
	{QMediaFormat::VideoCodec::H264,	  "Advanced Video Coding"},
	{QMediaFormat::VideoCodec::H265,	  "High Efficiency Video Coding (HEVC)"},
	{QMediaFormat::VideoCodec::VP8,		  "VP8"},
	{QMediaFormat::VideoCodec::VP9,		  "VP9"},
	{QMediaFormat::VideoCodec::AV1,		  "AOMedia Video 1"},
	{QMediaFormat::VideoCodec::Theora,	  "Theora"},
	{QMediaFormat::VideoCodec::WMV,		  "Windows Media Video"},
	{QMediaFormat::VideoCodec::MotionJPEG,"MotionJPEG"}
};

QMap<QMediaFormat::FileFormat, QString> mapFileFormats = {
	{QMediaFormat::FileFormat::WMA,			"Windows Media Audio"},
	{QMediaFormat::FileFormat::AAC,			"Advanced Audio Coding"},
	{QMediaFormat::FileFormat::Matroska,	"Matroska(MKV)"},
	{QMediaFormat::FileFormat::WMV,			"Windows Media Video"},
	{QMediaFormat::FileFormat::MP3,			"MPEG - 1 Audio Layer III or MPEG - 2 Audio Layer III"},
	{QMediaFormat::FileFormat::Wave,		"Waveform Audio File Format"},
	{QMediaFormat::FileFormat::Ogg,			"Ogg"},
	{QMediaFormat::FileFormat::MPEG4,		"MPEG - 4"},
	{QMediaFormat::FileFormat::AVI,			"Audio Video Interleave"},
	{QMediaFormat::FileFormat::QuickTime,	"QuickTime"},
	{QMediaFormat::FileFormat::WebM,		"WebM"},
	{QMediaFormat::FileFormat::Mpeg4Audio,	"MPEG - 4 Part 3 or MPEG - 4 Audio(formally ISO / IEC 14496 - 3)"},
	{QMediaFormat::FileFormat::FLAC,		"Free Lossless Audio Codec"}
};

#endif