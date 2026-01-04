#pragma once

namespace Lights
{
	class Trace
	{
	public:
		static void TraceOn(bool traceState) { traceOn = traceState; }
		inline static bool TraceOn() { return traceOn; }

	private:
		static bool traceOn;
	};
}