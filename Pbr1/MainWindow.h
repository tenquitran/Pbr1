#pragma once

#include "Scene.h"


namespace Pbr1App
{
	class MainWindow
		: public CommonLibOgl::WindowBase
	{
	public:
		// Parameters: hInstance - application instance;
		//             wndInfo - information about the window;
		//             openGLInfo - OpenGL settings.
		// Throws: Exception, std::bad_alloc
		MainWindow(HINSTANCE hInstance, const CommonLibOgl::WindowInfo& wndInfo, const CommonLibOgl::OpenGLInfo& openGLInfo);

		virtual ~MainWindow();

	private:
		// Initialize the derived window.
		// Usually used to set up OpenGL scene: load objects, etc.
		virtual bool initialize() override;

		// Render the derived window contents.
		virtual void render() const override;

		// Notify the derived window about resizing.
		virtual void onResizeDerived(int clientWidth, int clientHeight) override;

		// The derived window procedure.
		// The base class window procedure processes WM_CREATE, WM_DESTROY, WM_SIZE and redirects WM_KEYDOWN here.
		virtual LRESULT windowProcDerived(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:
		Scene m_scene;
	};
}
