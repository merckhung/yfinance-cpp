#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>
#include "../hpp/base.h"

int main() {
    yfinance::Symbol tsla("TSLA");
    
    // Calculate time range: Last 5 days to ensure we catch the last trading session
    // (e.g. if today is Sunday, we need to go back to Friday)
    time_t now = std::time(nullptr);
    time_t start = now - (5 * 24 * 60 * 60); 
    
    std::cout << "Fetching TSLA 1m data..." << std::endl;

    try {
        // Fetch 1m data
        // Note: arguments are rvalues reference (time_t&&), so we use std::move or temporaries
        Structures::Quotes quotes = tsla.get_quotes("1m", std::move(start), std::move(now));
        
        int n = quotes.m_unix.size();
        std::cout << "Total data points fetched: " << n << std::endl;
        
        if (n > 0) {
            // Identify the last trading day from the data
            time_t last_timestamp = quotes.m_unix.back();
            struct tm* lt = std::localtime(&last_timestamp);
            
            // Store the day components to filter
            int target_y = lt->tm_year;
            int target_m = lt->tm_mon;
            int target_d = lt->tm_mday;
            
            char date_buf[20];
            std::strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", lt);
            std::cout << "Displaying data for last trading day: " << date_buf << std::endl;
            std::cout << "-----------------------------------------------------------------------------" << std::endl;
            std::cout << std::left << std::setw(25) << "Time" 
                      << std::setw(10) << "Open"
                      << std::setw(10) << "High"
                      << std::setw(10) << "Low"
                      << std::setw(10) << "Close"
                      << std::setw(10) << "Volume" << std::endl;
            std::cout << "-----------------------------------------------------------------------------" << std::endl;

            int count = 0;
            for(int i = 0; i < n; ++i) {
                time_t t = quotes.m_unix[i];
                struct tm* tt = std::localtime(&t);
                
                // Filter for the same day
                if (tt->tm_year == target_y && tt->tm_mon == target_m && tt->tm_mday == target_d) {
                    std::cout << std::put_time(tt, "%Y-%m-%d %H:%M:%S") 
                              << std::setw(5) << " " 
                              << std::setw(10) << quotes.m_open[i]
                              << std::setw(10) << quotes.m_high[i]
                              << std::setw(10) << quotes.m_low[i]
                              << std::setw(10) << quotes.m_close[i]
                              << std::setw(10) << quotes.m_volume[i] << std::endl;
                    count++;
                }
            }
            std::cout << "-----------------------------------------------------------------------------" << std::endl;
            std::cout << "Total points for this day: " << count << std::endl;
        } else {
            std::cout << "No data returned." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error fetching data: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
