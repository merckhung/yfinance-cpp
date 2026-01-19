#pragma once
#include <string>
#include <cpr/cpr.h>

namespace yfinance {

    class Session {
    public:
        static Session& getInstance();

        cpr::Response Get(cpr::Url url, cpr::Parameters parameters = {}, cpr::Header headers = {});
        cpr::Response Get(cpr::Url url, cpr::Header headers);

        const std::string& getCrumb() const { return m_crumb; }

    private:
        Session();
        void refreshCredentials();

        std::string m_crumb;
        cpr::Session m_session;
        std::string m_user_agent;
    };

}
