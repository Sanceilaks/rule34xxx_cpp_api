#pragma once

#include <string>
#include <vector>
#include <optional>

#include "json.hpp"

namespace rule34api {
	using string = std::string;

	void set_proxy(const string& proxy);

	struct post_t {
		string preview_url;
		string sample_url;
		string file_url;
		uint32_t directory;
		string hash;
		uint32_t height;
		uint64_t id;
		string image;
		uint64_t change;
		string owner;
		uint64_t parent_id;
		string rating;
		uint32_t sample;
		uint32_t sample_height;
		uint32_t sample_width;
		uint32_t score;
		std::vector<string> tags;
		uint32_t width;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(post_t, preview_url, sample_url, file_url, directory, hash, height, id, image,
			change, owner, parent_id, rating, sample, sample_height, sample_width, score, tags, width);
	};
	
	std::vector<post_t> get_posts(std::optional<uint32_t> limit = std::nullopt, std::optional<uint32_t> pid = std::nullopt, std::optional<std::vector<string>> tags = std::nullopt,
		std::optional<uint32_t> cid = std::nullopt, std::optional<uint32_t> id = std::nullopt);
}