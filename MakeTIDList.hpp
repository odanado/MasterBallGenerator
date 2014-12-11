
#ifndef INCLUDED_MAKE_TID_LIST_HPP
#define INCLUDED_MAKE_TID_LIST_HPP

#include <array>
#include <string>
#include <sstream>
#include <iomanip>

#include "RNGLib/RNGLib.hpp"

class MakeTIDList {
private:
    RNGLib::Parameters5Gen<RNGLib::ROMType::None> param;
    RNGLib::DateTimeRange range;
    RNGLib::u32 no_count;

    RNGLib::LCG5Gen lcg;
    RNGLib::Calc5GenSeed calc_seed;
    RNGLib::CalcOffset calc_offset;

    std::array<std::string, 65536> result;
    std::array<bool, 65536> flag;

    std::array<std::string, 65536> result_main_timer0;
    std::array<bool, 65536> has_found_main_timer0;

    RNGLib::u32 count;

    RNGLib::u64 calc_pseed(RNGLib::u64);

    std::string convert_result(const RNGLib::DateTime&, const RNGLib::u32&);
    std::array<std::string, 65536> marge_result();

public:
    template<typename T>
    MakeTIDList(const RNGLib::Parameters5Gen<T>& param, const RNGLib::DateTimeRange& range, RNGLib::u32 no_count) : param(param), range(range), no_count(no_count), flag({}), has_found_main_timer0({}) { }
    void set_timer0(RNGLib::u32);

    std::array<std::string, 65536> operator() ();

};

#endif 
