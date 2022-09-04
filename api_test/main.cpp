#include "../rule34xxx_cpp_api/include/rule34xxx_cpp_api.h"

#include <iostream>
#include <any>
#include <format>
#include <exception>
#include <cpr/cpr.h>

auto main() noexcept -> int {
	auto gf = [](std::string_view u, std::string_view l){
		std::cout << u << l << std::endl;
		auto resp = cpr::Get(cpr::Url(std::string(u).append(l)));
		return resp.text;
	};

	auto api = rule34api::rule34api(gf);

	try {
		auto posts = api.get_posts(12);

		for (const auto& i : posts) {
			std::cout << i.file_url << std::endl;
		}

	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}