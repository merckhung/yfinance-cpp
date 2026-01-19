#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>
#include "../hpp/base.h"

int main() {
    yfinance::Symbol tsla("TSLA");
    
    std::cout << "Fetching TSLA option chain for the next 3 weeks..." << std::endl;

    try {
        // 1. Get all expiration dates
        std::vector<time_t> expirations = tsla.get_expiration_dates();
        
        if (expirations.empty()) {
            std::cout << "No expiration dates found." << std::endl;
            return 0;
        }

        // 2. Calculate the 3-week window
        time_t now = std::time(nullptr);
        time_t three_weeks_later = now + (21 * 24 * 60 * 60);

        std::cout << "Filtering for expirations between now and " 
                  << std::put_time(std::localtime(&three_weeks_later), "%Y-%m-%d") 
                  << std::endl;

        int processed_dates = 0;
        for (const auto& exp_date : expirations) {
            // Filter dates
            if (exp_date < now || exp_date > three_weeks_later) {
                continue;
            }

            processed_dates++;
            struct tm* lt = std::localtime(&exp_date);
            std::cout << "\n========================================" << std::endl;
            std::cout << "Expiration Date: " << std::put_time(lt, "%Y-%m-%d") << std::endl;
            std::cout << "========================================" << std::endl;

            // 3. Fetch options for this date
            try {
                Utils::Types::Options opts = tsla.get_options(exp_date);
                
                // 4. Display summary
                std::cout << "Calls: " << opts["calls"].size() << std::endl;
                if (!opts["calls"].empty()) {
                    // Show first few calls
                    std::cout << "First 3 Calls:" << std::endl;
                    for (size_t i = 0; i < std::min((size_t)3, opts["calls"].size()); ++i) {
                        const auto& opt = opts["calls"][i];
                        std::cout << "  Strike: " << std::setw(8) << opt.m_strike.value_or(0.0f)
                                  << " Price: " << opt.m_lastPrice.value_or(0.0f) 
                                  << " IV: " << opt.m_impliedVolatility.value_or(0.0f) << std::endl;
                    }
                }

                std::cout << "Puts: " << opts["puts"].size() << std::endl;
                if (!opts["puts"].empty()) {
                    // Show first few puts
                    std::cout << "First 3 Puts:" << std::endl;
                    for (size_t i = 0; i < std::min((size_t)3, opts["puts"].size()); ++i) {
                        const auto& opt = opts["puts"][i];
                        std::cout << "  Strike: " << std::setw(8) << opt.m_strike.value_or(0.0f)
                                  << " Price: " << opt.m_lastPrice.value_or(0.0f) 
                                  << " IV: " << opt.m_impliedVolatility.value_or(0.0f) << std::endl;
                    }
                }

            } catch (const std::exception& e) {
                std::cerr << "Failed to fetch options for this date: " << e.what() << std::endl;
            }
        }

        if (processed_dates == 0) {
            std::cout << "No expiration dates found within the next 3 weeks." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
