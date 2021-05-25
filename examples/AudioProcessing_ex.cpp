#include "../d3lib/AudioProcessing.h"

int main()
{
	d3lib::Recoder rec;
	rec.StartRecording();
	getch();
	rec.StopRecording();
	rec.SetOutPutFile("output.mp3");

	d3lib::Player play;
	play.Open("output.mp3");
	play.Play();
	getch();
	play.Stop();
}