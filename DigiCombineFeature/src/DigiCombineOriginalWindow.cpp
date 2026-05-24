#include "../include/DigiCombineOriginalWindow.h"

namespace DigiCombine::OriginalWindow
{
	MainUi::MainUi(CombineFamily family)
		: family_(family)
	{
	}

	CombineFamily MainUi::Family() const
	{
		return family_;
	}

	WindowMode MainUi::Mode() const
	{
		return mode_;
	}

	void MainUi::SetMode(WindowMode mode)
	{
		mode_ = mode;
	}
}
