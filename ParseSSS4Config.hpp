
#ifndef INCLUDED_PARSE_SS$_CONFIG_HPP
#define INCLUDED_PARSE_SS$_CONFIG_HPP
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "RNGLib/RNGLib.hpp"


class ParseSSS4Config{ 
private:
    std::vector<std::string> split(const std::string&, char);

    RNGLib::u32 vcount;
    RNGLib::u32 timer0;
    RNGLib::u32 timer0_min;
    RNGLib::u32 timer0_max;
    RNGLib::u32 gxstat;
    RNGLib::u32 frame;
    RNGLib::u32 mac_addr1;
    RNGLib::u32 mac_addr2;
    RNGLib::u32 mac_addr3;
    RNGLib::u32 mac_addr4;
    RNGLib::u32 mac_addr5;
    RNGLib::u32 mac_addr6;
    RNGLib::u32 nazo1;
    RNGLib::u32 nazo2;
    RNGLib::u32 nazo3;
    RNGLib::u32 nazo4;
    RNGLib::u32 nazo5;
    RNGLib::u32 nazo6;

    void parse();
    void parse_mac_addr(const std::string&);

    bool is_bw1_;
    bool is_old_sss4_;

    RNGLib::u32 stohex(const std::string&);

public:
    std::vector<std::string> config_lines;
    bool load_config();
    
    template<typename T>
    void operator() (RNGLib::Parameters5Gen<T>& param) {
        param.set_nazo1(nazo1);
        param.set_nazo2(nazo2);
        param.set_nazo3(nazo3);
        param.set_nazo4(nazo4);
        param.set_nazo5(nazo5);

        param.set_mac_add(mac_addr1, mac_addr2, mac_addr3, mac_addr4, mac_addr5, mac_addr6);

        param.set_vcount(vcount);
        param.set_gxstat(gxstat);
        param.set_frame(frame);
        param.set_timer0(timer0);
        param.set_timer0_min(timer0_min);
        param.set_timer0_max(timer0_max);
    }

    bool is_bw1();
    bool is_old_sss4();

};
#endif
