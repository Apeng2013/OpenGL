#pragma once

namespace Test {
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float delta_time) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}