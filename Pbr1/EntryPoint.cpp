#include "stdafx.h"
#include "Resource.h"
#include "MainWindow.h"

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace Pbr1App;

//////////////////////////////////////////////////////////////////////////


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Redirect output from std::wcout and std::wcerr to the log file.
	std::wofstream out("log.txt");
	std::wcout.rdbuf(out.rdbuf());
	std::wcerr.rdbuf(out.rdbuf());

	int res = 1;

	try
	{
		const int OpenGLVersionMajor = 4;
		const int OpenGLVersionMinor = 4;

		// Field of view angle.
		const GLfloat FieldOfView = 45.0f;

		// Frustum boundaries.
		const GLfloat FrustumNear = 0.1f;
		const GLfloat FrustumFar  = 1000.0f;

		// Width and height of the window client area.
		const int ClientWidth  = 800;
		const int ClientHeight = 600;

		// NOTE: window title ID, class ID, icon ID and menu ID were copied from the Visual Studio-generated window code.
		WindowInfo wndInfo(ClientWidth, ClientHeight, IDS_APP_TITLE, IDC_PBR1, IDI_PBR1, IDI_SMALL, IDC_PBR1);

		OpenGLInfo openGLInfo(OpenGLVersionMajor, OpenGLVersionMinor, FieldOfView, FrustumNear, FrustumFar);

		MainWindow mainWindow(hInstance, wndInfo, openGLInfo);

		res = mainWindow.runMessageLoop(nCmdShow);
	}
	catch (const Exception& ex)
	{
		std::wcerr << ex.message() << '\n';
		return 1;
	}
	catch (const std::bad_alloc&)
	{
		std::wcerr << L"Memalloc failure\n";
		return 1;
	}
	catch (...)
	{
		std::wcerr << L"Unknown exception\n";
		return 1;
	}

	return res;
}
