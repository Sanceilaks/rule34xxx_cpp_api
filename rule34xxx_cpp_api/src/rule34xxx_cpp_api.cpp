#include "rule34xxx_cpp_api.h"

#include <string>
#include <iostream>
#include <curl/curl.h>

#define GET_VALUE_FROM_JSON(j, value, name) j[name].get<decltype(value)>()

#define API_URL std::string("http://r34-json-api.herokuapp.com/")

using json = nlohmann::json;
using string = std::string;

namespace network {
	inline rule34api::request_result get_request(std::string_view host, std::string_view path) {
		auto write_fn = [](char* ptr, size_t size, size_t nmemb, rule34api::request_result* data) -> size_t {
			data->append(std::string(ptr, size * nmemb));
			return size * nmemb;
		};

		auto url = std::string(host).append(path);

		std::cout << "GET: " << url << std::endl;

		rule34api::request_result content;
		char errbuf[CURL_ERROR_SIZE] = {};

		auto curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_fn);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

		auto res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if(res != CURLE_OK) {
			size_t len = strlen(errbuf);
			fprintf(stderr, "\nlibcurl: (%d) ", res);
			if(len)
			fprintf(stderr, "%s%s", errbuf,
					((errbuf[len - 1] != '\n') ? "\n" : ""));
			else
			fprintf(stderr, "%s\n", curl_easy_strerror(res));
  		}

		if (res) throw std::exception(curl_easy_strerror(res));
		return content;
	}
}

std::vector<rule34api::post_t> rule34api::rule34api::get_posts(std::optional<uint32_t> limit, std::optional<uint32_t> pid, std::optional<std::vector<string>> tags,
										 std::optional<uint32_t> cid, std::optional<uint32_t> id) {
	network::callparams params;

	//params.add_param("page", "dapi");
	//params.add_param("s", "post");
	//params.add_param("q", "index");
	params.add_param("json", "1");

	if (limit.has_value())
		params.add_param("limit", std::to_string(limit.value()));
	if (pid.has_value())
		params.add_param("pid", std::to_string(pid.value()));

	if (tags.has_value()) {
		const auto join_string = [](const std::vector<string> &str) -> string {
			string out;
			for (auto i = str.begin(); i != str.end(); ++i)
				out.append(*i).append(i == str.end() - 1 ? "" : " ");
			return out;
		};

		auto &vec = tags.value();
		params.add_param("tags", join_string(vec));
	}

	if (cid.has_value())
		params.add_param("cid", std::to_string(cid.value()));
	if (id.has_value())
		params.add_param("id", std::to_string(id.value()));

	return this->api_request(params).get<std::vector<post_t>>();
}

rule34api::rule34api::rule34api() {
	this->httpget = network::get_request;
}

nlohmann::json rule34api::rule34api::api_request(const network::callparams& params) {
	return std::move(json::parse(std::move(
		this->httpget( API_URL, std::format("posts?{}", params.construct_string()) )
	)));
}