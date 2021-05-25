#include "../d3lib/CameraProcessing.h"

int main()
{
    d3lib::Camera cam;
	cam.ShowVideoWindow(L"Cam Window", 500, 500, 600, 500);

	cam.Update(cam.GetHWND(), [&]() {
		cam.SaveCapture("me.png", d3lib::Camera::ImageType::png, 500, 450);
	});
}