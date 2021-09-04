#include "reassembler.hpp"
#include <string_view>

// example
int main()
{
	constexpr std::string_view input_file = "input.txt";
	try {
		std::cout << "Searching for \"" << input_file << "\"...\n";

		std::ifstream file(reassembler::get_path_to_input_file(input_file));
		std::cout << "Reassembling data...\n";
		auto reassembly_result = reassembler::reassemble_data(file);
		if (reassembly_result.has_value()) {
			std::cout << "Reassembly successful, the result is:\n"
					  << *reassembly_result;
		} else {
			std::cout << "Reassembly failed.\n";
		}
	}
	catch (std::exception& e) {
		std::cerr << "Caught a standard exception:\n"
				  << "--------------------\n"
				  << e.what() << "--------------------\n";
	}
	catch (...) {
		std::cerr << "Caught an unknown exception :(";
	}
	return 0;
}
