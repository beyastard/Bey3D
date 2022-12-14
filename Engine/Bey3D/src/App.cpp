#include "App.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "BeyConfigs.h"

namespace dx = DirectX;

GDIPlusManager gdipm;

App::App() : wnd(WIDTH, HEIGHT, "Bey3D Engine Demo"), light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	nano.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());

	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
			continue;
		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
			cam.Translate({ 0.0f,0.0f,dt });

		if (wnd.kbd.KeyIsPressed('A'))
			cam.Translate({ -dt,0.0f,0.0f });

		if (wnd.kbd.KeyIsPressed('S'))
			cam.Translate({ 0.0f,0.0f,-dt });

		if (wnd.kbd.KeyIsPressed('D'))
			cam.Translate({ dt,0.0f,0.0f });

		if (wnd.kbd.KeyIsPressed('R'))
			cam.Translate({ 0.0f,dt,0.0f });

		if (wnd.kbd.KeyIsPressed('F'))
			cam.Translate({ 0.0f,-dt,0.0f });
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
			cam.Rotate(float(delta->x), float(delta->y));
	}

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	nano.ShowWindow();

	// present
	wnd.Gfx().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
}

App::~App() = default;

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		DoFrame();
	}
}
