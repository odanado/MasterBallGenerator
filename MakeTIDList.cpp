#include "MakeTIDList.hpp"
#include <cstdio>

void MakeTIDList::set_timer0(RNGLib::u32 timer0) {
    param.set_timer0(timer0);
}


RNGLib::u64 MakeTIDList::calc_pseed(RNGLib::u64 s) {
    calc_offset.bw1(lcg.next(s), false, false);
    return calc_offset.get_seed();
}


std::array<std::string, 65536> MakeTIDList::operator() () {
    count = 0;
    /* ÇÊÇ≠èoÇÈï˚ÇÃTimer0ÇmaxÇ∆Ç∑ÇÈ */
    RNGLib::u32 timer0_min = param.get_timer0_min();
    RNGLib::u32 timer0_max = param.get_timer0_max();

    RNGLib::u32 tid1;
    RNGLib::u32 tid2;

    for(const auto& it : range) {
        param.set_datetime(it);

        param.set_timer0(timer0_min);
        RNGLib::u64 seed1 = calc_pseed(calc_seed(param));

        param.set_timer0(timer0_max);
        RNGLib::u64 seed2 = calc_pseed(calc_seed(param));

        for(RNGLib::u32 i=0; i<=no_count; ++i) {
            tid1 = (((seed1>>32)*0xFFFFFFFFULL)>>32)&0xFFFF;
            tid2 = (((seed2>>32)*0xFFFFFFFFULL)>>32)&0xFFFF;

            if(tid1 == tid2 && !flag[tid1]) {
                ++count;
                flag[tid1] = true;
                result[tid1] = convert_result(it, i);
            }

            if(i==0 && !has_found_main_timer0[tid2]) {
                has_found_main_timer0[tid2] = true;
                result_main_timer0[tid2] = convert_result(it, i);
            }

            seed1 = lcg.next(seed1); 
            seed2 = lcg.next(seed2); 
        }
        if(count == 65536) {
            break;
        }
        if(count%10000 == 0) {
            printf("êiíª %u\r", count);
        }
    }
    printf("\n");

    return marge_result();
}
std::string MakeTIDList::convert_result(const RNGLib::DateTime& date_time, const RNGLib::u32& no_count_) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0');
    ss << date_time.get_year() << "îN";
    ss << std::setw(2) << std::setfill('0');
    ss << date_time.get_month() << "åé";
    ss << std::setw(2) << std::setfill('0');
    ss << date_time.get_day() << "ì˙, "; 
    ss << std::setw(2) << std::setfill('0');
    ss << date_time.get_hour() << "éû";
    ss << std::setw(2) << std::setfill('0');
    ss << date_time.get_minute() << "ï™";
    ss << std::setw(2) << std::setfill('0');
    ss << date_time.get_sec() << "ïb";
	ss << ", Ç¢Ç¢Ç¶Ç";
    ss << no_count_ << "âÒ";

    return ss.str();
}

std::array<std::string, 65536> MakeTIDList::marge_result() {
    std::array<std::string, 65536> result_;
    for(RNGLib::u32 i=0;i<65536;++i) {
        if(flag[i]) {
            result_[i] = result[i];
        }
        else {
            result_[i] = result_main_timer0[i];
        }
    }

    return result_;
}
