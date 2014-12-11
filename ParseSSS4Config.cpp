#include "ParseSSS4Config.hpp"

std::vector<std::string> ParseSSS4Config::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}
RNGLib::u32 ParseSSS4Config::stohex(const std::string& s) {
    RNGLib::u32 hex;

    sscanf(s.c_str(), "%x", &hex);

    return hex;
}

void ParseSSS4Config::parse_mac_addr(const std::string& s) {
    auto mac_addrs = split(s, '-');
    mac_addr1 = ParseSSS4Config::stohex(mac_addrs[0]);
    mac_addr2 = ParseSSS4Config::stohex(mac_addrs[1]);
    mac_addr3 = ParseSSS4Config::stohex(mac_addrs[2]);
    mac_addr4 = ParseSSS4Config::stohex(mac_addrs[3]);
    mac_addr5 = ParseSSS4Config::stohex(mac_addrs[4]);
    mac_addr6 = ParseSSS4Config::stohex(mac_addrs[5]);
}

bool ParseSSS4Config::is_bw1() {
    return is_bw1_;
}

bool ParseSSS4Config::is_old_sss4() {
    return is_old_sss4_;
}

void ParseSSS4Config::parse() {
    is_bw1_ = false;
    is_old_sss4_ = false;
    if(config_lines.size() == 6) {
        vcount = ParseSSS4Config::stohex(config_lines[0]);
        timer0 = ParseSSS4Config::stohex(config_lines[1]);
        gxstat = ParseSSS4Config::stohex(config_lines[2]);
        frame  = ParseSSS4Config::stohex(config_lines[3]);
        parse_mac_addr(config_lines[4]);
        nazo1 = ParseSSS4Config::stohex(config_lines[5]);
        nazo2 = nazo1 + 0xfc;
        nazo3 = nazo1 + 0xfc;
        nazo4 = nazo1 + 0xfc + 0x4c;
        nazo5 = nazo1 + 0xfc + 0x4c;

        timer0_min = timer0;
        timer0_max = timer0;
        is_bw1_ = true;
        is_old_sss4_ = true;
    }
    else {
        vcount = ParseSSS4Config::stohex(config_lines[0]);
        timer0_min = ParseSSS4Config::stohex(config_lines[1]);
        timer0_max = ParseSSS4Config::stohex(config_lines[2]);
        timer0 = timer0_min;
        gxstat = ParseSSS4Config::stohex(config_lines[4]);
        frame  = ParseSSS4Config::stohex(config_lines[5]);
        parse_mac_addr(config_lines[6]);
        if(config_lines[7] == "3") {
            is_bw1_ = true;
            nazo1 = RNGLib::ROMType::B1Ja::nazo1;
            nazo2 = RNGLib::ROMType::B1Ja::nazo2;
            nazo3 = RNGLib::ROMType::B1Ja::nazo3;
            nazo4 = RNGLib::ROMType::B1Ja::nazo4;
            nazo5 = RNGLib::ROMType::B1Ja::nazo5;
        }
        if(config_lines[7] == "4") {
            is_bw1_ = true;
            nazo1 = RNGLib::ROMType::W1Ja::nazo1;
            nazo2 = RNGLib::ROMType::W1Ja::nazo2;
            nazo3 = RNGLib::ROMType::W1Ja::nazo3;
            nazo4 = RNGLib::ROMType::W1Ja::nazo4;
            nazo5 = RNGLib::ROMType::W1Ja::nazo5;
        }
        if(config_lines[7] == "5") {
            is_bw1_ = true;
            nazo1 = ParseSSS4Config::stohex(config_lines[8]);
            nazo2 = nazo1 + 0xfc;
            nazo3 = nazo1 + 0xfc;
            nazo4 = nazo1 + 0xfc + 0x4c;
            nazo5 = nazo1 + 0xfc + 0x4c;
        }

    }

}

bool ParseSSS4Config::load_config() {
    std::string str;
    std::ifstream ifs("config.txt");
    if(ifs.fail()) {
        return false;
    }
    while(ifs>>str) {
        config_lines.push_back(str);
    }

    parse();

    return true;
}

