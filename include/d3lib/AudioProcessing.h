#ifndef __AUDIOPROCESSING_H__
#define __AUDIOPROCESSING_H__

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>
#include <conio.h>
#include <string>
#pragma comment(lib, "Winmm.lib")

namespace d3lib
{
	class Recoder
	{
	public:
		Recoder();
		~Recoder();

		void SetOutPutFile(char*);
		void StartRecording();
		void StopRecording();
		void Close();

		void setAlisas(char*);

	private:
		char mci_command[100];
		char ReturnString[300];
		int mci_error;

		char* alais = NULL;
		char* filename = NULL;

	};

	Recoder::Recoder()
	{
		sprintf(mci_command, "open new type waveaudio alias %s", alais);
		mci_error = mciSendString(mci_command, ReturnString, sizeof(ReturnString), NULL);

		sprintf(mci_command, "set %s time format ms", alais);
		mci_error = mciSendString(mci_command, ReturnString, sizeof(ReturnString), NULL);
	}

	Recoder::~Recoder()
	{
		Close();
	}

	inline void Recoder::SetOutPutFile(char* filename)
	{
		sprintf(mci_command, "save %s %s", alais, filename);
		mci_error = mciSendString(mci_command, ReturnString, sizeof(ReturnString), NULL);
		this->filename = filename;
	}

	inline void Recoder::StartRecording()
	{
		sprintf(mci_command, "record %s notify", alais);
		mci_error = mciSendString(mci_command, ReturnString, sizeof(ReturnString), NULL);
	}

	inline void Recoder::StopRecording()
	{
		sprintf(mci_command, "stop %s", alais);
		mci_error = mciSendString(mci_command, ReturnString, sizeof(ReturnString), NULL);
	}

	inline void Recoder::Close()
	{
		sprintf(mci_command, "close %s", alais);
		mci_error = mciSendString(mci_command, ReturnString, sizeof(ReturnString), NULL);
	}

	inline void Recoder::setAlisas(char* alias)
	{
		if (alias == NULL) alias = filename;
		else this->alais = alias;
	}

	class Player
	{
	public:
		enum mode { unknown, open, playing, paused, stopped, Resumed };

	private:
		mode Mode;
		std::string filename;

	public:
		void Open(std::string filename);
		void Play();
		void Pause();
		void Resume();
		void Stop();
		void Close();
		void Seek(int pos);
		void SetVolume(int vol);
		int Position();
		int GetMode() { return Mode; };
	};


	void Player::Open(std::string filename)
	{
		Mode = mode::open;
		this->filename = filename;
		std::string msgOpen = "open \"" + filename + "\" type mpegvideo alias " + filename;
		mciSendStringA(msgOpen.c_str(), NULL, 0, 0);
	}

	void Player::Play()
	{
		Mode = mode::playing;
		char* buff = new char[10];
		std::string msgPlay = "play " + filename + " from " + itoa(0, buff, 10);
		mciSendStringA(msgPlay.c_str(), NULL, 0, 0);
		delete[] buff;
	}

	void Player::Pause()
	{
		Mode = mode::paused;
		std::string msg = "pause " + filename;
		mciSendStringA(msg.c_str(), NULL, 0, 0);
	}

	void Player::Resume()
	{
		Mode = mode::Resumed;
		std::string msg = "resume " + filename;
		mciSendStringA(msg.c_str(), NULL, 0, 0);
	}

	void Player::Stop()
	{
		Mode = mode::stopped;
		std::string msg = "stop " + filename;
		mciSendStringA(msg.c_str(), NULL, 0, 0);
	}

	void Player::Close()
	{
		std::string msg = "close " + filename;
		mciSendStringA(msg.c_str(), NULL, 0, 0);
	}

	void Player::Seek(int pos)
	{
		char* buff = new char[10];
		std::string msg = "seek " + filename + " to " + itoa(pos, buff, 10);
		mciSendStringA(msg.c_str(), NULL, 0, 0);
		delete[] buff;
	}

	void Player::SetVolume(int vol)
	{
		vol = (vol < 0 ? 0 : (vol > 1000 ? 1000 : vol));
		char* buff = new char[5];
		std::string msg = "setaudio " + filename + " volume to " + itoa(vol, buff, 10);
		mciSendString(msg.c_str(), NULL, 0, 0);
		delete[] buff;
	}

	int Player::Position()
	{
		char* buff = new char[16];
		std::string msg = "status " + filename + " position";
		mciSendString(msg.c_str(), buff, 16, 0);
		int t = atoi(buff);
		delete[] buff;
		return t;
	}
}

#endif