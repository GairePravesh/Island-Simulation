#pragma once

#include "Renderer.h"
#include "test.h"
#include "imgui/imgui.h"

namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};
}