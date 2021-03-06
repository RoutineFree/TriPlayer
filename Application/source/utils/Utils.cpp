#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include "lang/Lang.hpp"
#include "utils/Utils.hpp"

namespace Utils {
    std::string formatBytes(long long bytes) {
        // Divide until smaller than 1024
        double val = bytes;
        int divs = 0;
        while (val > 1023) {
            val /= 1024;
            divs++;
        }

        // Convert to string and append units
        std::string str = truncateToDecimalPlace(std::to_string(roundToDecimalPlace(val, 1)), 1);
        switch (divs) {
            case 0:
                str += " bytes";
                break;

            case 1:
                str += " KB";
                break;

            case 2:
                str += " MB";
                break;

            case 3:
                str += " GB";
                break;

            case 4:
                str += " TB";
                break;

            default:
                str += " ??";
                break;
        }
        return str;
    }

    std::string getClockString(bool in24) {
        // Get time
        std::time_t ts = std::time(nullptr);
        std::tm * tm = std::localtime(&ts);

        // Print time into buffer
        char buf[20];
        const char * fmt = (in24 ? "%R" : "%I:%M %p");
        std::strftime(buf, 20, fmt, tm);
        return std::string(buf);
    }

    // From: https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c/12468109#12468109
    static bool seedSet = false;
    std::string randomString(size_t length) {
        // Set seed if not already set
        if (!seedSet) {
            srand(time(NULL));
            seedSet = true;
        }

        auto randchar = []() -> char {
            const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[rand() % max_index];
        };
        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        return str;
    }

    std::string removeUnicode(const std::string & str) {
        std::string good;
        for (char c : str) {
            if (isprint(c)) {
                good.push_back(c);
            }
        }
        return good;
    }

    float roundToDecimalPlace(float val, unsigned int p) {
        for (unsigned int i = 0; i < p; i++) {
            val *= 10.0;
        }
        val = std::round(val);
        for (unsigned int i = 0; i < p; i++) {
            val /= 10.0;
        }
        return val;
    }

    std::string secondsToHMS(unsigned int sec) {
        std::string str = "";
        // Hours
        int h = sec/3600;
        if (h > 0) {
            str += std::to_string(h) + ":";
        }

        // Minutes
        int m = ((sec/60)%60);
        if (str.length() > 0 && m < 10) {
            str += "0";
        }
        str += std::to_string(m);

        // Seconds
        str += ":";
        int s = sec%60;
        if (s < 10) {
            str += "0";
        }
        str += std::to_string(s);

        return str;
    }

    std::string secondsToHoursMins(unsigned int sec) {
        // Get hours and minutes
        int h = sec/3600;
        int m = ((sec/60)%60);

        // Return appropriate string
        std::string str = (h == 0 ? "0" : (h == 1 ? "1" : "X"));
        str += "H";
        str += (m == 1 ? "1" : "X");
        str += "M";
        str = "Common.HoursMins." + str;
        return substituteTokens(Lang::string(str), std::to_string(h <= 1 ? m : h), std::to_string(m));
    }

    std::vector<std::string> splitIntoWords(const std::string & str, const char delim) {
        std::vector<std::string> words;

        // Iterate over each word
        std::string word = "";
        size_t pos = 0;
        while (pos < str.length()) {
            // Append chars to word until the delimiter is reached
            if (str[pos] != delim) {
                word.append(1, str[pos]);
            } else {
                // Don't add empty words (i.e. due to repeated spaces)
                if (word.length() > 0) {
                    words.push_back(word);
                    word = "";
                }
            }
            pos++;
        }

        // Check we haven't missed the last word
        if (word.length() > 0) {
            words.push_back(word);
        }

        return words;
    }

    size_t tokenIndex = 1;
    std::string substituteTokens(std::string str, std::string token) {
        str = std::regex_replace(str, std::regex("\\$\\[" + std::to_string(tokenIndex) + "]"), token);
        tokenIndex = 1;
        return str;
    }

    std::string toLowercase(std::string str) {
        for (char & c : str) {
            c = tolower(c);
        }
        return str;
    }

    std::string toUppercase(std::string str) {
        for (char & c : str) {
            c = toupper(c);
        }
        return str;
    }

    std::string truncateToDecimalPlace(std::string str, unsigned int p) {
        size_t dec = str.find(".");
        if (dec == std::string::npos || p >= str.length() - dec) {
            return str;
        }

        // Cut off decimal place if zero
        if (p == 0) {
            dec--;
        }

        return str.substr(0, dec + 1 + p);
    }
};