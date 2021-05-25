#ifndef __SPEECHRECOGNITION_H__
#define __SPEECHRECOGNITION_H__

#pragma warning(disable : 4996)

#include <sapi.h>
#include <sphelper.h>

extern CComModule _Module;

#define __SafeRelease(_com_) if (_com_) { (_com_).Release(); _com_ = NULL; }

namespace d3lib
{
	class SpeechRecognition
	{
	private:
		HRESULT hr = NULL;
		CComPtr<ISpVoice> pVoice;
		CComPtr<ISpRecognizer> cpRecog;
		CComPtr<ISpObjectToken> objTok;
		CComPtr<ISpRecoContext> recoCon;
		CComPtr<ISpRecoGrammar> recoGram;
		CComPtr<ISpRecoResult> recoRes;
		CComPtr<ISpAudio> cpAudio;

		//CComPtr<IEnumSpObjectTokens> cpEnum;
	public:
		SpeechRecognition();
		~SpeechRecognition();
		void Speak(const char*);
		void Input(char*&);

		char* GetFirstInput()
		{
			char* input;
			Input(input);
			return input;
		}

		enum struct Gender { MALE = 0, Female = 1 };
		void loadSpeak(Gender);
	protected:
		void LoadInput();
		//void loadSpeak(int);

	};

	SpeechRecognition::SpeechRecognition()
	{
		hr = ::CoInitialize(NULL);
	}

	SpeechRecognition::~SpeechRecognition()
	{
		::CoUninitialize();

		__SafeRelease(pVoice);
		__SafeRelease(cpRecog);
		__SafeRelease(objTok);
		__SafeRelease(recoCon);
		__SafeRelease(recoGram);
		__SafeRelease(recoRes);
		__SafeRelease(cpAudio);
	}

	void SpeechRecognition::loadSpeak(Gender Gender)
	{
		hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

		switch (Gender)
		{
		case Gender::MALE: SpFindBestToken(SPCAT_VOICES, L"gender = male", L" ", &objTok); break;
		case Gender::Female: SpFindBestToken(SPCAT_VOICES, L"gender = female", L" ", &objTok); break;
		default: SpFindBestToken(SPCAT_VOICES, L" ", L" ", &objTok);
		}

		pVoice->SetVoice(objTok);
	}

	void SpeechRecognition::Speak(const char* incText)
	{
		int len = strlen(incText) + 1;
		wchar_t* dest = new wchar_t[len];

		mbstowcs(dest, incText, strlen(incText) + 1);
		pVoice->Speak((LPWSTR)dest, SPF_IS_XML, NULL);

		delete[] dest;
	}

	void SpeechRecognition::LoadInput()
	{
		//hr = cpRecog.CoCreateInstance(CLSID_SpSharedRecognizer); 
		hr = cpRecog.CoCreateInstance(CLSID_SpInprocRecognizer);
		SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);
		cpRecog->SetInput(cpAudio, TRUE);
		SpFindBestToken(SPCAT_RECOGNIZERS, L"language=409", NULL, &objTok); // 409 is english
		cpRecog->SetRecognizer(objTok);
		cpRecog->SetRecoState(SPRST_ACTIVE);
		cpRecog->CreateRecoContext(&recoCon);
		recoCon->SetNotifyWin32Event();
		recoCon->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION)); // SPFEI(SPEI_FALSE_RECOGNITION)
		recoCon->CreateGrammar(0, &recoGram);
		recoGram->LoadDictation(NULL, SPLO_STATIC);
		recoGram->SetDictationState(SPRS_ACTIVE);
	}

	void SpeechRecognition::Input(char*& input)
	{
		LoadInput();

		HANDLE hSpeechEvent = recoCon->GetNotifyEventHandle();
		if (hSpeechEvent == INVALID_HANDLE_VALUE) return;

		CSpEvent spEvent;
		LPWSTR coMemResult = NULL;
		HANDLE rghEvents[] = { hSpeechEvent };
		DWORD dMessage = WaitForMultipleObjects(sp_countof(rghEvents), rghEvents, FALSE, INFINITE);

		if (dMessage == WAIT_OBJECT_0)
		{
			spEvent.GetFrom(recoCon);
			recoRes = spEvent.RecoResult();
			recoRes->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &coMemResult, NULL);
		}
		else
			return;

		int bytSize = WideCharToMultiByte(CP_ACP, 0, coMemResult, -1, 0, 0, 0, 0);
		input = new char[bytSize];
		int retr = WideCharToMultiByte(CP_ACP, 0, coMemResult, -1, input, bytSize, 0, 0);
	}
}

#endif