#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <unordered_map>
#include "Logger.hpp"

const std::string linesep = "\r\n";
const std::string spacesep = " ";
const std::string headersep = ": ";
const std::string suffixsep = ".";
const std::string g_http_version = "HTTP/1.1";
const std::string g_first_page = "index.html";
const std::string g_wwwroot = "wwwroot";
const std::string page_404 = "./wwwroot/404.html";

using namespace NS_LOG_MODULE;

class HttpRequest;

class Util
{
public:
    static std::string ReadLine(std::string httpstr)
    {
        auto pos = httpstr.find(linesep);
        if (pos == std::string::npos) // 如果行分隔符都没有找到，就返回空字符串
        {
            return std::string();
        }
        std::string line = httpstr.substr(0, pos); // 没有将换行符截下来

        // 截取之后要做移除
        httpstr.erase(0, line.size() + linesep.size());

        if (line.empty()) // 读取到的是空行，就返回行分隔符
        {
            return linesep;
        }

        return line;
    }

    // 如果我们要读取二进制文件，vector<char>
    static std::string ReadFile(const std::string &filename)
    {
        // 采用二进制方式读取数据
        std::ifstream in(filename, std::ios::binary); // 以二进制方式打开
        if (!in.is_open())
        {
            return std::string();
        }

        in.seekg(0, in.end);       // 设置到结尾，为了获取获取大小
        int filesize = in.tellg(); // 主体用于获取文件大小
        in.seekg(0, in.beg);       // 重新设置到开头

        std::string content;
        content.resize(filesize);
        in.read((char *)content.c_str(), filesize); // 将打开的文件拷贝到指定字符串中

        in.close();
        return content;

        // // bug
        // std::ifstream in(filename);

        // if (!in.is_open())
        // {
        //     return std::string();
        // }

        // std::string content;
        // std::string line;
        // while (std::getline(in, line))
        // {
        //     content += line;
        // }
        // in.close();

        // return content;
    }
};

class HttpResponse
{
private:
    std::string CodeToDesc(int code)
    {
        switch (code)
        {
        // 1xx
        case 100:
            return "Continue，继续";

        // 2xx
        case 200:
            return "OK，请求成功";
        case 201:
            return "Created，创建成功";
        case 204:
            return "No Content，无返回内容";

        // 3xx
        case 301:
            return "Moved Permanently，永久重定向";
        case 302:
            return "Found，临时重定向";
        case 304:
            return "Not Modified，资源未修改";

        // 4xx
        case 400:
            return "Bad Request，请求参数错误";
        case 401:
            return "Unauthorized，未登录/未认证";
        case 403:
            return "Forbidden，权限不足";
        case 404:
            return "Not Found，资源不存在";
        case 405:
            return "Method Not Allowed，请求方法不允许";
        case 422:
            return "Unprocessable Entity，参数校验失败";
        case 429:
            return "Too Many Requests，请求过于频繁";

        // 5xx
        case 500:
            return "Internal Server Error，服务内部错误";
        case 502:
            return "Bad Gateway，网关错误";
        case 503:
            return "Service Unavailable，服务不可用";
        case 504:
            return "Gateway Timeout，网关超时";

        default:
            return "Unknown，未知状态码";
        }
    }

public:
    HttpResponse()
        : _version(g_http_version),
          _code(0),
          _blank_line(linesep)
    {
    }

    void SetCode(int code) // 设置状态码
    {
        _code = code; // 状态码和状态码所对应的描述是对应的
        _code_desc = CodeToDesc(_code);
    }

    void SetBody(const std::string &content)
    {
        _text = content;
    }

    int BodySize() const
    {
        return _text.size();
    }

    void AddHeader(const std::string &key, const std::string &value)
    {
        // 如果存在就覆盖，如果不存在就添加
        _header[key] = value;
    }

    bool Build(HttpRequest &req); // AI改成只有声明了

    std::string Serialize()
    {
        // 构建状态行
        std::string respstr = _version;
        respstr += spacesep;
        respstr += std::to_string(_code);
        respstr += spacesep;
        respstr += _code_desc;
        respstr += linesep;

        // 构建报头
        for (auto &header : _header)
        {
            std::string line = header.first + headersep + header.second + linesep;
            respstr += line;
        }

        // 添加换行符
        respstr += _blank_line;

        // 添加正文
        respstr += _text;

        return respstr;
    }

private:
    std::string _version;
    int _code; // 状态码是int型的
    std::string _code_desc;
    std::unordered_map<std::string, std::string> _header;
    std::string _blank_line;
    std::string _text;
};

class HttpRequest
{
private:
    bool ParseReqLine(std::string &httpstr)
    {
        std::string req_line = Util::ReadLine(httpstr);
        if (req_line.empty() || req_line == linesep)
        {
            return false;
        }

        std::stringstream ss(req_line);
        ss >> _method >> _uri >> _version;

        if (_uri == "/") // 这里对于路径进行了处理
        {
            _uri += g_first_page; // uri: /-> uri: /index.html
        }
        _uri = g_wwwroot + _uri; // wwwroot/index.html: web 根目录

        // 拿到路径了，就需要判断文件类型
        auto pos = _uri.rfind(suffixsep); // rfind就是逆向搜索
        if (pos == std::string::npos)
        {
            _suffix = ".html";
        }
        else
        {
            _suffix = _uri.substr(pos); // 找到了点，就把点之后的截取出来
        }

        return true;
    }

    bool ParseHeaderkv(std::string &httpstr) // 因为并不只有一行所以，要设计成循环解析
    {
        std::string header_line;
        do
        {
            header_line = Util::ReadLine(httpstr);
            if (header_line.empty())
            {
                return false;
            }

            // 现在提取出了请求报头的一行，然后需要将 key 和 value 截出来
            // 先将key提取出来
            auto pos = header_line.find(headersep);
            if (pos == std::string::npos)
            {
                return false;
            }
            std::string key = header_line.substr(0, pos);
            std::string value = header_line.substr(pos + headersep.size());

            // 记住 unordered_map 的插入方式
            _header.insert(std::make_pair(key, value));
        } while (header_line != linesep);

        return true;
    }

    bool ParseText(std::string &httpstr)
    {
        // 首先确认，到底有没有正文部分 - 报头之中有存正文长度
        if (_header.find("Content-Length") == _header.end())
        {
            _text = "";
        }
        // 正文有多大
        else
        {
            int content_len = std::stoi(_header["Content-Length"]);
            _text = httpstr.substr(0, content_len);
            httpstr.erase(0, content_len); // 因为没有readline，所以需要自己移除
        }

        return true;
    }

public:
    // 反序列化
    bool Deserialize(std::string &httpstr)
    {
        // std::cout << httpstr;

        // 解析请求行
        bool n = ParseReqLine(httpstr);
        (void)n;
        LOG(LogLevel::DEBUG) << "_method: " << _method;
        LOG(LogLevel::DEBUG) << "_uri: " << _uri;
        LOG(LogLevel::DEBUG) << "_version: " << _version;
        // std::cout << httpstr;

        // 解析一行报头
        n = ParseHeaderkv(httpstr);
        (void)n;
        for (auto &header : _header) // 打印，展示一下提取是否成功
        {
            std::cout << header.first << " ## " << header.second << "\r\n\r\n";
        }
        std::cout << "start|" << httpstr << "|end";

        _blank_line = linesep; // 将请求报头解析后，自然设置空行

        // 解析正文部分
        n = ParseText(httpstr);

        return true;
    }

    std::string ReqContent()
    {
        return Util::ReadFile(_uri);
    }

    std::string GetUri()
    {
        return _uri;
    }

    std::string GetSuffix()
    {
        return _suffix;
    }

    void DebugPrint()
    {
        std::cout << "_version: " << _version << std::endl;
        std::cout << "_uri: " << _version << std::endl;
        std::cout << "_method: " << _version << std::endl;

        for (auto &header : _header)
        {
            std::cout << "==>" << header.first << " # " << header.second << std::endl;
        }

        std::cout << _blank_line;

        std::cout << _text << std::endl;
    }

private:
    // 结构化字段
    std::string _method;
    std::string _uri;
    std::string _version;
    std::unordered_map<std::string, std::string> _header;
    std::string _blank_line;
    std::string _text;

    // 私有数据，后缀类型
    std::string _suffix; // 后缀类型
};

inline bool HttpResponse::Build(HttpRequest &req)
{
    std::string target_file = req.GetUri();
    _text = Util::ReadFile(target_file);

    if (_text.empty())
    {
        _code = 404;
        _code_desc = "Not Found";
        return false;
    }

    _code = 200;
    _code_desc = "OK";
    return true;
}

class MiniType
{
public:
    static std::string Suffix2Mimemap(const std::string &suffix)
    {
        // 在哈希表中寻找
        auto it = _mime_map.find(suffix);
        if (it != _mime_map.end())
        {
            // 找到了
            return it->second;
        }
        else
        {
            // 没找到
            return "text/html";
        }
    }

    // 这里加了内联就解决了问题
    inline static std::unordered_map<std::string, std::string> _mime_map = {
            {".html", "text/html"},
            {".css", "text/css"},
            {".js", "application/javascript"},
            {".json", "application/json"},
            {".png", "image/png"},
            {".jpg", "image/jpeg"},
            {".gif", "image/gif"},
            {".svg", "image/svg+xml"},
            {".pdf", "application/pdf"},
            {".txt", "text/plain"}};
};

using service_t = std::function<void(HttpRequest &req, HttpResponse &resp)>;

class HttpProtocol
{
public:
    HttpProtocol()
    {}

    void RegisterService(std::string uri, service_t service)
    {
        uri = g_wwwroot + uri;
        _http_services[uri] = service; // uri -> wwwroot/Login
    }

    bool IsReqService(const std::string &uri)
    {
        auto iter = _http_services.find(uri);
        if (iter == _http_services.end())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    std::string HandlerHttpRequest(std::string &req)
    {
#ifdef DEBUG
        std::cout << "------------------------------------------------------------------------" << std::endl;
        std::cout << req << std::endl;

        std::string status_line = "HTTP/1.1 200 OK\r\n"; // 状态行：version code code_desc
        status_line += "Connection: close";              // header
        status_line += "\r\n";                           // 空行

        std::string html = "<html><body><h1>hello world!</h1></body></html>"; // 正文

        return status_line + html;
#else
        HttpRequest httprequest;
        httprequest.Deserialize(req);
        HttpResponse httpresponse;
        if (IsReqService(httprequest.GetUri())) // 本身就不是静态资源
        {
            // 功能路由
            _http_services[httprequest.GetUri()](httprequest, httpresponse); // 这里是在执行函数，前面是函数头，后面是在传参
        }
        else
        {
            // 保证报文完整性 --- IGN
            // 反序列化
            std::cout << "handler pid: " << getpid() << std::endl; // 因为是多进程处理请求的，所以进程pid应该不同
            HttpRequest httprequest;
            httprequest.Deserialize(req);

            std::cout << "Suffix: " << httprequest.GetSuffix();

            //----------------------------------------------------上请，下应--------------------------------------------------------------------

            // 根据不同的请求方法和请求URI，给用户返回不同的报文：结构化请求 -> 结构化应答
            HttpResponse httpresponse;

            // 这里直接代替了Build的，感觉Build没用了
            std::string content = httprequest.ReqContent(); // 拿到网页内容，用于返回给用户
            if (content.empty())
            {
                // 状态码设置为 404
                // httpresponse.SetCode(404);
                // httpresponse.SetBody(Util::ReadFile(page_404));
                // httpresponse.AddHeader("Content-Length", std::to_string(httpresponse.BodySize()));

                httpresponse.SetCode(302);
                httpresponse.AddHeader("Location", "/404.html");
            }
            else
            {
                httpresponse.SetCode(200);
                httpresponse.SetBody(content);
                httpresponse.AddHeader("Content-Type", MiniType::Suffix2Mimemap(httprequest.GetSuffix()));
                httpresponse.AddHeader("Content-Length", std::to_string(content.size()));
                httpresponse.AddHeader("Set-Cookie", "name=peter;");
                // httpresponse.AddHeader("Connection", "close"); // 关闭长连接的请求
            }
        }
        // 再将结构化的应答序列化
        std::string return_string = httpresponse.Serialize();

        return return_string;

#endif
    }
    ~HttpProtocol()
    {
    }

private:
    std::unordered_map<std::string, service_t> _http_services; // 根据uri(服务路径)进行功能路由
};