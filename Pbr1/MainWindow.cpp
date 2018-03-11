#include "stdafx.h"
#include "MainWindow.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace Pbr1App;

//////////////////////////////////////////////////////////////////////////


MainWindow::MainWindow(HINSTANCE hInstance, const WindowInfo& wndInfo, const OpenGLInfo& openGLInfo)
	: WindowBase(hInstance, wndInfo, openGLInfo)
{
}

MainWindow::~MainWindow()
{
}

LRESULT MainWindow::windowProcDerived(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, aboutProc);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		{
			int key = (int)wParam;
			//LPARAM keyData = lParam;

			switch (key)
			{
			case VK_ESCAPE:
				DestroyWindow(hWnd);
				break;
			//////////////////////////////////////////////////////////////////////////
			// Translate the camera.
			//////////////////////////////////////////////////////////////////////////
			case 0x57:    // W key
				m_scene.translateCameraY(0.1f);
				break;
			case 0x53:    // S key
				m_scene.translateCameraY(-0.1f);
				break;
			case 0x41:    // A key
				m_scene.translateCameraX(-0.1f);
				break;
			case 0x44:    // D key
				m_scene.translateCameraX(0.1f);
				break;
			//////////////////////////////////////////////////////////////////////////
			// Rotate the camera.
			//////////////////////////////////////////////////////////////////////////
			case VK_UP:
				m_scene.rotateCameraX(2.0f);
				break;
			case VK_DOWN:
				m_scene.rotateCameraX(-2.0f);
				break;
			case VK_LEFT:
				m_scene.rotateCameraY(-2.0f);
				break;
			case VK_RIGHT:
				m_scene.rotateCameraY(2.0f);
				break;
			//////////////////////////////////////////////////////////////////////////
			// Zoom in/out for the camera.
			//////////////////////////////////////////////////////////////////////////
			case VK_NUMPAD0:
				m_scene.scaleCamera(-0.05f);
				break;
			case VK_NUMPAD1:
				m_scene.scaleCamera(0.05f);
				break;
			}
		}
		break;
	}

	return 0;
}

bool MainWindow::initialize()
{
	if (!m_hRC)
	{
		std::wcerr << L"No OpenGL rendering context\n";
		assert(false); return false;
	}

	GLfloat aspectRatio = m_wndInfo.m_clientWidth / (GLfloat)m_wndInfo.m_clientHeight;

	return m_scene.initialize(aspectRatio, m_openGlInfo);
}

void MainWindow::onResizeDerived(int clientWidth, int clientHeight)
{
	GLfloat aspectRatio = clientWidth / (GLfloat)clientHeight;

	m_scene.resize(aspectRatio);
}

void MainWindow::render() const
{
	m_scene.render();

	SwapBuffers(GetDC(m_hWnd));
}
