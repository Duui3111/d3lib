#ifndef __CAMERAPROCESSING_H__
#define __CAMERAPROCESSING_H__

#include <iostream>
#include <functional>

#include <dshow.h>
#include <comdef.h> 
#include <mfapi.h>
#include <mfidl.h>
#include <gdiplus.h>

#pragma comment(lib, "strmbase.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Gdiplus.lib")

namespace d3lib
{
    class Camera
    {
    private:
        struct VideoInfo
        {
            unsigned int numodevices;
            wchar_t devname[256];
            wchar_t devdescription[256];
            wchar_t devpath[256];
            wchar_t devclsid[256];
        };

    public:
        enum class ImageType { bmp, jpeg, gif, tiff, png };
        enum class VideoType { avi, mp4 };

        bool IsCapOpened = false;

        VideoInfo VideoInfo;
        Camera();
        ~Camera();

        void SaveCapture(const char* filename, ImageType type, int Height = 450, int Width = 500);
        int ShowVideoWindow(const wchar_t*, int, int, int, int);
        void Update(HWND, std::function<void(void)>);
        HWND GetHWND() { return paranthwnd; }

    private:
        IGraphBuilder* GraphBuilder = nullptr;
        ICaptureGraphBuilder2* CaptureGraphBuilder2 = nullptr;
        IMediaControl* MediaControl = nullptr;
        IBaseFilter* DeviceFilter = nullptr;
        ICreateDevEnum* CreateDevEnum = nullptr;
        IEnumMoniker* EnumMoniker = nullptr;
        IMoniker* Moniker = nullptr;
        IVideoWindow* VideoWindow = nullptr;
        IPropertyBag* PropertyBag = nullptr;
        IMFActivate** imfactivate = nullptr;
        IMFAttributes* imfAttributes = nullptr;

        HWND paranthwnd;

        void StartDeviceInfo();
    };

    Camera::Camera()
    {
        CoInitialize(NULL);
        CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (PVOID*)&CreateDevEnum);
        CreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &EnumMoniker, 0);

        if (EnumMoniker == NULL)
            MessageBox(0, "No Divices Found", "Error", MB_ICONERROR);

        EnumMoniker->Reset();

        EnumMoniker->Next(1, &Moniker, NULL);
        Moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&PropertyBag);
        Moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&DeviceFilter);

        IsCapOpened = true;

        StartDeviceInfo();
    }

    Camera::~Camera()
    {
        IsCapOpened = false;

        EnumMoniker->Release();
        CreateDevEnum->Release();

        Moniker->Release();
        PropertyBag->Release();
        MediaControl->Release();
        CaptureGraphBuilder2->Release();
        GraphBuilder->Release();
        VideoWindow->Release();

        CoUninitialize();
    }

    void Camera::SaveCapture(const char* filename, ImageType type, int Height, int Width)
    {
        IStream* istream;
        HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;

        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        {
            HDC scrdc, memdc;
            HBITMAP membit;
            scrdc = ::GetDC(paranthwnd);
            memdc = CreateCompatibleDC(scrdc);
            membit = CreateCompatibleBitmap(scrdc, Height, Width);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
            BitBlt(memdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), scrdc, 0, 0, SRCCOPY);
            

            Gdiplus::Bitmap bitmap(membit, NULL);

            CLSID clsid;
            HRESULT hr = 0;

            if (type == ImageType::bmp)	hr = CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &clsid);
            if (type == ImageType::jpeg) hr = CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &clsid);
            if (type == ImageType::gif) hr = CLSIDFromString(L"{557cf402-1a04-11d3-9a73-0000f81ef32e}", &clsid);
            if (type == ImageType::tiff) hr = CLSIDFromString(L"{557cf405-1a04-11d3-9a73-0000f81ef32e}", &clsid);
            if (type == ImageType::png) hr = CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid);

            bitmap.Save(istream, &clsid, NULL);

            Gdiplus::Bitmap bmp(istream, NULL);

            bmp.Save(_bstr_t(filename), &clsid, NULL);

            DeleteObject(memdc);
            DeleteObject(membit);
            ReleaseDC(0, scrdc);
        }

        Gdiplus::GdiplusShutdown(gdiplusToken);
    }

    void Camera::StartDeviceInfo()
    {
        MFCreateAttributes(&imfAttributes, 1);
        imfAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
        MFEnumDeviceSources(imfAttributes, &imfactivate, &VideoInfo.numodevices);
        imfAttributes->Release();
        
        VARIANT var;
        var.vt = VT_BSTR;
        PropertyBag->Read(L"FriendlyName", &var, 0);
        WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)VideoInfo.devname, sizeof(VideoInfo.devname), 0, 0);
        PropertyBag->Read(L"Description", &var, 0);
        WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)VideoInfo.devdescription, sizeof(VideoInfo.devdescription), 0, 0);
        PropertyBag->Read(L"DevicePath", &var, 0);
        WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)VideoInfo.devpath, sizeof(VideoInfo.devpath), 0, 0);
        PropertyBag->Read(L"CLSID", &var, 0);
        WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, (LPSTR)VideoInfo.devclsid, sizeof(VideoInfo.devclsid), 0, 0);
        VariantClear(&var);
    }

    int Camera::ShowVideoWindow(const wchar_t* Caption, int Left, int Top, int Width, int Height)
    {
        HINSTANCE hinstance = GetModuleHandle(NULL);
        RegisterClassA(new WNDCLASSA({ CS_HREDRAW | CS_VREDRAW, DefWindowProc, 0L, 0L, hinstance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_CROSS), (HBRUSH)GetStockObject(WHITE_BRUSH), 0L, "CAPWINDOW" }));
        paranthwnd = CreateWindowExW(0, L"CAPWINDOW", Caption, WS_OVERLAPPEDWINDOW, 500, 150, Width, Height, 0, 0, hinstance, 0);

        CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID*)&GraphBuilder);
        CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (LPVOID*)&CaptureGraphBuilder2);
        CaptureGraphBuilder2->SetFiltergraph(GraphBuilder);
        GraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*)&MediaControl);
        GraphBuilder->AddFilter(DeviceFilter, L"Device Filter");
        CaptureGraphBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, NULL, DeviceFilter, NULL, NULL);
        GraphBuilder->QueryInterface(IID_IVideoWindow, (LPVOID*)&VideoWindow);
        VideoWindow->put_Owner((OAHWND)paranthwnd);
        VideoWindow->SetWindowPosition(Left, Top, Width, Height);
        VideoWindow->put_WindowStyle(WS_CLIPCHILDREN);

        RECT rc;
        GetClientRect(paranthwnd, &rc);
        VideoWindow->SetWindowPosition(0, 0, rc.right, rc.bottom);

        VideoWindow->put_Visible((-1));
        ShowWindow(paranthwnd, SW_SHOWDEFAULT);
        MediaControl->Run();

        return 0;
    }

    void Camera::Update(HWND win, std::function<void(void)> func)
    {
        BOOL bRet;
        MSG msg;
        while ((bRet = GetMessage(&msg, win, 0, 0)))
        {   
            if (bRet == -1) exit(0);   

            func();

            TranslateMessage(&msg);

            switch (msg.message)
            {
                case WM_DESTROY: PostQuitMessage(0); break;
                default: DispatchMessage(&msg); break;
            }
        }
    } 
}

#endif
