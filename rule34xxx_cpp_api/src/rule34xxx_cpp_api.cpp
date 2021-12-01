#include "rule34xxx_cpp_api.h"

#include <string>
#include <curl/curl.h>

#define GET_VALUE_FROM_JSON(j, value, name) j[name].get<decltype(value)>()

#define API_URL std::string("https://api.rule34.xxx")

using json = nlohmann::json;
using request_result = std::string;
using string = std::string;

namespace network {
	std::optional<std::string> proxy;

	inline request_result get_request(std::string_view host, std::string_view path) {
		httplib::Client cli(host.data());
		if (auto prox = proxy.value(); proxy.has_value())
			cli.set_proxy(std::string(prox.begin(), prox.begin() + prox.find(":")).c_str(), 
				std::stoi(std::string(prox.begin() + prox.find(":") + 1, prox.end())));
		cli.set_read_timeout(std::chrono::seconds(5));
		auto resp = cli.Get(path.data());
		if (resp.error() != httplib::Error::Success)
			throw std::exception(httplib::to_string(resp.error()).c_str());
		return resp.value();
	}

	class callparams {
		std::unordered_map<std::string, std::string> _map;
	public:
		callparams() {}
		callparams(const std::unordered_map<std::string, std::string>& map) : _map(map) {}

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

	inline json api_request(const network::callparams& params) {
		return std::move(json::parse(std::move(get_request(API_URL, std::string("/index.php?").append(params.construct_string())))));
	}
}

namespace rule34api {
	void set_proxy(const string& _proxy) {
		network::proxy = _proxy.empty() ? std::nullopt : std::optional(_proxy);
	}

	std::vector<post_t> get_posts(std::optional<uint32_t> limit, std::optional<uint32_t> pid, std::optional<std::vector<string>> tags,
			std::optional<uint32_t> cid, std::optional<uint32_t> id) {
		network::callparams params;

		params.add_param("page", "dapi");
		params.add_param("s", "post");
		params.add_param("q", "index");
		params.add_param("json", "1");

		if (limit.has_value()) params.add_param("limit", std::to_string(limit.value()));
		if (pid.has_value()) params.add_param("pid", std::to_string(pid.value()));

		if (tags.has_value()) {
			const auto join_string = [](const std::vector<string>& str) -> string {
				string out;
				for (auto i = str.begin(); i != str.end(); ++i)
					out.append(*i).append(i == str.end() - 1 ? "" : " ");
				return out;
			};

			auto& vec = tags.value();
			params.add_param("tags", join_string(vec));
		}

		if (cid.has_value()) params.add_param("cid", std::to_string(cid.value()));
		if (id.has_value()) params.add_param("id", std::to_string(id.value()));

		return network::api_request(params).get<std::vector<post_t>>();
	}
}