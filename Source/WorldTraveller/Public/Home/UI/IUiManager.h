#pragma once

namespace NHome
{
	class IUiManager
	{
	public:
		virtual bool GetUiEnabled() const = 0;
		virtual void SetUiEnabled(bool bEnabled) = 0;
	};
}
