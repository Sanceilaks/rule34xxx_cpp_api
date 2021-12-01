#include "../rule34xxx_cpp_api/include/rule34xxx_cpp_api.h"

#include <iostream>
#include <any>


auto main() noexcept -> int {
	try {
		rule34api::set_proxy("162.247.243.147:80");

		auto resp = rule34api::get_posts();

		for (auto& i : resp) {
			std::cout << i.file_url << std::endl;
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
}

