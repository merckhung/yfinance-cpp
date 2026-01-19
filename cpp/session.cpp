#include "../hpp/session.h"
#include "../hpp/requests.h"
#include <iostream>

namespace yfinance {

    Session& Session::getInstance() {
        static Session instance;
        return instance;
    }

    Session::Session() {
        m_user_agent = USER_AGENT;
        m_session.SetHeader(cpr::Header{{"User-Agent", m_user_agent}});
        refreshCredentials();
    }

    void Session::refreshCredentials() {
        // 1. Get cookie from fc.yahoo.com
        m_session.SetUrl(cpr::Url{"https://fc.yahoo.com"});
        m_session.Get();

        // 2. Get crumb from getcrumb
        m_session.SetUrl(cpr::Url{"https://query2.finance.yahoo.com/v1/test/getcrumb"});
        cpr::Response r2 = m_session.Get();
        
        if (r2.status_code == 200) {
            m_crumb = r2.text;
        } else {
            // Fallback or error handling
            std::cerr << "Warning: Failed to get crumb, status code: " << r2.status_code << std::endl;
        }
    }

    cpr::Response Session::Get(cpr::Url url, cpr::Parameters parameters, cpr::Header headers) {
        m_session.SetUrl(url);
        if (!m_crumb.empty()) {
            parameters.Add({"crumb", m_crumb});
        }
        m_session.SetParameters(parameters);
        if (!headers.empty()) {
            // Merge headers if needed, but for now just append/set
            // Note: SetHeader replaces all headers. We might want to preserve User-Agent.
            headers.insert({"User-Agent", m_user_agent});
            m_session.SetHeader(headers);
        } else {
            m_session.SetHeader(cpr::Header{{"User-Agent", m_user_agent}});
        }
        return m_session.Get();
    }

    cpr::Response Session::Get(cpr::Url url, cpr::Header headers) {
        return Get(url, {}, headers);
    }

}