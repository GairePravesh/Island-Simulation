#pragma once

//#include <functional>
//#include <vector>

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltatime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	//class TestMenu : public Test
	//{
	//public:
	//	TestMenu();
	//	~TestMenu();
	//
	//	void OnImGuiRender() override;
	//private:
	//	Test* m_CurrentTest;
	//	std::vector<std::pair<std::string, std::function<Test *()>>> m_Tests;
	//};
}