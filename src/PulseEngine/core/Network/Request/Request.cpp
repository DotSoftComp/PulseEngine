#include "Request.h"

Request::Request(const std::string& ip, const std::string& port)
: asio::ip::tcp::iostream(ip, port), ip(ip), port(port) {
    if (!*this) {
        throw std::runtime_error("Could not connect to " + ip + ":" + port);
    }
}

std::string Request::buildQuery(const std::map<std::string,std::string>& query) {
    if (query.empty()) return "";
    std::string q = "?";
    for (auto it = query.begin(); it != query.end(); ++it) {
        if (it != query.begin()) q += "&";
        q += it->first + "=" + it->second;
    }
    return q;
}

nlohmann::json Request::sendRequest(const std::string& method,
                                    const std::string& route,
                                    const std::string& body,
                                    const std::map<std::string,std::string>& headers) {
    // clear old buffer
    this->clear();

    // send request
    *this << method << " " << route << " HTTP/1.1\r\n";
    *this << "Host: " << ip << "\r\n";
    *this << "Connection: close\r\n";

    for (auto& kv : headers) {
        *this << kv.first << ": " << kv.second << "\r\n";
    }

    if (!body.empty()) {
        *this << "Content-Type: application/json\r\n";
        *this << "Content-Length: " << body.size() << "\r\n";
    }

    *this << "\r\n";

    if (!body.empty()) {
        *this << body;
    }

    // read response
    std::string line;
    std::string response;
    while (std::getline(*this, line)) {
        response += line + "\n";
    }

    // find JSON start
    auto pos = response.find("{");
    if (pos != std::string::npos) {
        return nlohmann::json::parse(response.substr(pos));
    }

    return nlohmann::json(); // empty JSON
}

nlohmann::json Request::Get(const std::string& route,
                            const std::map<std::string,std::string>& query,
                            const std::map<std::string,std::string>& headers) {
    return sendRequest("GET", route + buildQuery(query), "", headers);
}

nlohmann::json Request::Post(const std::string& route,
                             const nlohmann::json& body,
                             const std::map<std::string,std::string>& headers) {
    return sendRequest("POST", route, body.dump(), headers);
}

nlohmann::json Request::Put(const std::string& route,
                            const nlohmann::json& body,
                            const std::map<std::string,std::string>& headers) {
    return sendRequest("PUT", route, body.dump(), headers);
}

nlohmann::json Request::Delete(const std::string& route,
                               const std::map<std::string,std::string>& headers) {
    return sendRequest("DELETE", route, "", headers);
}
