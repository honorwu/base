#include "experimental/__net_ts/steady_timer.hpp"

namespace kit
{
	class TickCounter
	{
	public:
		TickCounter()
		{
			Restart();
		}

		void Restart()
		{
			time_point_ = std::chrono::steady_clock::now();
		}

		unsigned long Elapsed() const
		{
			std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - time_point_;
			return elapsed.count() * 1000;
		}

	private:
		std::chrono::steady_clock::time_point time_point_;
	};
}
