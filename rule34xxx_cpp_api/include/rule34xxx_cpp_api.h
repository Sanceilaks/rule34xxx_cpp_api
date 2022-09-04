#pragma once

#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <format>

#include "json.hpp"

namespace network{

	class callparams {
		std::unordered_map<std::string, std::string> _map;

	public:
		callparams() {}
		callparams(const std::unordered_map<std::string, std::string> &map) : _map(map) {}

		void add_param(std::string_view key, std::string_view data) {
			_map.emplace(key, data);
		}

		void remove_param(std::string_view key) {
			_map.erase(key.data());
		}

		std::string construct_string() const {
			std::string str;
			for (auto i = _map.begin(); i != _map.end(); ++i)
				str.append(i->first).append("=").append(i->second).append(i == --_map.end() ? "" : "&");
			return std::move(str);
		}
	};
}

namespace rule34api
{
	using string = std::string;
	using request_result = std::string;
	using get_function_t = std::function<request_result(std::string_view, std::string_view)>;


	struct post_t {
		string preview_url;
		string sample_url;
		string file_url;
		string md5;
		string height;
		string id;
		string change;
		string parent_id;
		string creator_id;
		string rating;
		string sample_height;
		string sample_width;
		string score;
		std::vector<string> tags;
		string width;
		string has_children;
		string created_at;
		string status;
		string source;
		string has_notes;
		string has_comments;
		string preview_width;
		string preview_height;
		string comments_url;
		string type;
		string creator_url;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(post_t, preview_url, sample_url, file_url, md5, height, id, creator_id, has_children,
									   change, parent_id, rating, sample_height, sample_width, score, tags, width,
									   created_at, status, source, has_notes, has_comments, preview_width, preview_height, comments_url,
									   type, creator_url);
	};

	class rule34api {
		get_function_t httpget;

	public:
		rule34api();
		rule34api(get_function_t getfn) : httpget(getfn) {}

		nlohmann::json api_request(const network::callparams& params);

		std::vector<post_t> get_posts(std::optional<uint32_t> limit = {}, std::optional<uint32_t> pid = {},
									  std::optional<std::vector<string>> tags = {},
									  std::optional<uint32_t> cid = {}, std::optional<uint32_t> id = {});
	};
}