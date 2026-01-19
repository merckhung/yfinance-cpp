#include "../hpp/base.h"
#include "../hpp/benchmark.h"


int main(int argc, char** argv) {

	// Initialize the Ticker object:
	yfinance::Symbol* tk;
	tk = new yfinance::Symbol("TSLA");

	// Loading options into struct:
	auto options = tk->get_options();

	// Print Option:
	std::cout << options["calls"][0];

	// Benchmarking:
	auto f = std::bind(
		&yfinance::Symbol::get_options, tk);

	auto timeit = Benchmarking::Timeit(
		10, f);

	// Printing benchmark results:
	std::cout << timeit;
}
