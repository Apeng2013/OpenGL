#pragma once

#include "GLFW/glfw3.h"
#include <memory>

class ImGuiLayer
{
public:
	ImGuiLayer();
	~ImGuiLayer();

	void Begin() const;
	void End() const;

	void OnAttach() const;
	void OnDetach() const;
	void RenderGui() const;
};