#include "RNGLib/RNGLib.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "MakeTIDList.hpp"
#include "ParseSSS4Config.hpp"


void about();

template<typename T>
void input_parameters(RNGLib::Parameters5Gen<T>&);
template<typename T>
void input_timer0(RNGLib::Parameters5Gen<T>&);

void input_secound_range(RNGLib::DateTimeRange&);
void input_no_count(uint32_t&);

void output_result(const std::array<std::string, 65536>&);

template<typename T>
void output_param(RNGLib::Parameters5Gen<T>&);

template<typename T>
void make_result(RNGLib::Parameters5Gen<T>&);

std::string find_tid(RNGLib::u32);

int main() {
    about();

    while(true) {
        std::ifstream ifs("result.txt");
        std::cout<<"result.txt��ǂݍ��݂܂�..."<<std::endl;
        if(!ifs.fail()) {
            std::cout<<"result.txt�̓ǂݍ��ݐ���\n"<<std::endl;
            std::cout<<"result.txt�������[�h�ɂȂ�܂�"<<std::endl;
            RNGLib::u32 tid;
            std::cout<<"����������ID >";
            std::cin>>tid;

            std::cout<<std::endl;

            std::cout<<find_tid(tid)<<std::endl;
            std::cout<<"\n"<<std::endl;
        }
        else {
            std::cout<<"result.txt�̓ǂݍ��ݎ��s\n"<<std::endl;
            std::cout<<"result.txt���쐬���܂�\n"<<std::endl;

            RNGLib::Parameters5Gen<RNGLib::ROMType::None> param;
            ParseSSS4Config parse_sss4_config;

            std::cout<<"config.txt��ǂݍ��݂܂�..."<<std::endl;
            if(parse_sss4_config.load_config()) {
                std::cout<<"config.txt�̓ǂݍ��ݐ���"<<std::endl;
                parse_sss4_config(param);
                
                std::cout<<"���݂̕ێ����Ă���p�����[�^�͎��̒ʂ�ł�"<<std::endl;
                output_param(param);

                if(!parse_sss4_config.is_bw1()) {
                    std::cout<<"config.txt��BW2�̂��̂ł�"<<std::endl;
                    std::cout<<"�p�����[�^���L�[�{�[�h������͂��Ă�������"<<std::endl;
                    input_parameters(param);
                }
                else if(parse_sss4_config.is_old_sss4() || param.get_timer0_min() == param.get_timer0_max()) {
                    std::cout<<"Timer0�̒l����͂��Ă�������"<<std::endl;
                    input_timer0(param);
                }
            }
            else {
                std::cout<<"config.txt�̓ǂݍ��ݎ��s"<<std::endl;
                std::cout<<"�p�����[�^���L�[�{�[�h������͂��Ă�������"<<std::endl;
                input_parameters(param);
            }

            while(1) {
                std::string answer;
                std::cout<<"timer0������l�����܂����H(Y/N) >";
                std::cin>>answer;
                if(answer=="Y" || answer=="y") {
                    break;
                }
                else if(answer=="N" || answer=="n") {
                    param.set_timer0_min(param.get_timer0_max());
                    break;
                }
            }

            make_result(param);
            std::cout<<"result.txt���쐬���܂���\n"<<std::endl;
        }

    }



    return 0;
}



void about() {
    std::cout<<"���̃c�[����XY�EORAS��ID�����ɍ��킹��BW1��ID���������邱�Ƃ�ړI�ɍ���܂���\n���͂��ꂽ�p�����[�^������ID��0����65535�܂ł̋N�����Ԃ��v�Z�����X�g�����܂�\n�ڂ��������́u�����ɂ��ė��������𐬌������邩�v���Q�Ƃ��Ă�������\n"<<std::endl;
}

template<typename T>
void make_result(RNGLib::Parameters5Gen<T>& param) {
    RNGLib::DateTimeRange range;
    uint32_t no_count;

    range.set_year(0,99);
    range.set_month(1,12);
    range.set_day(1,31);
    range.set_hour(0,23);
    range.set_minute(0,59);
    input_secound_range(range);

    input_no_count(no_count);

    MakeTIDList make_tid_list(param, range, no_count);
    auto result = make_tid_list();

    output_result(result);
}
template<typename T>
void input_parameters(RNGLib::Parameters5Gen<T>& param) {

    uint32_t nazo1;
    uint32_t mac_addr1;
    uint32_t mac_addr2;
    uint32_t mac_addr3;
    uint32_t mac_addr4;
    uint32_t mac_addr5;
    uint32_t mac_addr6;
    uint32_t vcount;
    uint32_t gxstat;
    uint32_t frame;

    printf("nazo >0x");
    scanf("%X", &nazo1);

    printf("�}�b�N�A�h���X1 >0x");
    scanf("%X", &mac_addr1);
    printf("�}�b�N�A�h���X2 >0x");
    scanf("%X", &mac_addr2);
    printf("�}�b�N�A�h���X3 >0x");
    scanf("%X", &mac_addr3);
    printf("�}�b�N�A�h���X4 >0x");
    scanf("%X", &mac_addr4);
    printf("�}�b�N�A�h���X5 >0x");
    scanf("%X", &mac_addr5);
    printf("�}�b�N�A�h���X6 >0x");
    scanf("%X", &mac_addr6);

    printf("VCount >0x");
    scanf("%X", &vcount);

    input_timer0(param);

    printf("Frame >0x");
    scanf("%X", &frame);

    printf("GxStat >0x");
    scanf("%X", &gxstat);

    param.set_nazo1(nazo1);
    param.set_nazo2(nazo1 + 0xfc);
    param.set_nazo3(nazo1 + 0xfc);
    param.set_nazo4(nazo1 + 0xfc + 0x4c);
    param.set_nazo5(nazo1 + 0xfc + 0x4c);

    param.set_mac_add(mac_addr1, mac_addr2, mac_addr3, mac_addr4, mac_addr5, mac_addr6);

    param.set_vcount(vcount);
    param.set_frame(frame);
    param.set_gxstat(gxstat);

}
template<typename T>
void input_timer0(RNGLib::Parameters5Gen<T>& param) {
    uint32_t main_timer0;
    uint32_t other_timer0;
    printf("�悭�o��Timer0 >0x");
    scanf("%X", &main_timer0);

    printf("�^�C�}�[�����Timer0 >0x");
    scanf("%X", &other_timer0);

    param.set_timer0(main_timer0);
    param.set_timer0_min(other_timer0);
    param.set_timer0_max(main_timer0);
}
void input_secound_range(RNGLib::DateTimeRange& range) {
    int first,last;
    printf("�b�̉��� >");
    scanf("%u", &first);
    printf("�b�̏�� >");
    scanf("%u", &last);

    range.set_sec(first, last);

}

void input_no_count(uint32_t& no_count_) {
    uint32_t no_count;
    while(1) {
        printf("�������������񐔂̏��(�ő�9) >");
        scanf("%u", &no_count);
        if(no_count<10) break;
    }
    no_count_ = no_count;

}
void output_result(const std::array<std::string, 65536>& result) {
    std::cout<<"�t�@�C���ɏ����o���܂�"<<std::endl;
    std::ofstream ofs("result.txt");
    for(uint32_t i=0; i<65536; i++) {
        ofs << std::setw(5) << std::setfill('0') << i << ", " << result[i] << "\n";

    }
}
std::string find_tid(RNGLib::u32 tid) {
    std::string result;
    std::ifstream is("result.txt");
    is.seekg(tid*48, std::ios_base::beg);

    std::getline(is,result);
    return result;
}
template<typename T>
void output_param(RNGLib::Parameters5Gen<T>& param) {

    printf("nazo : %08x\n", param.get_nazo1());
    printf("MAC�A�h���X : %02x-%02x-%02x-%02x-%02x-%02x\n", 
            param.get_mac_add1(), 
            param.get_mac_add2(), 
            param.get_mac_add3(),
            param.get_mac_add4(),
            param.get_mac_add5(),
            param.get_mac_add6());

    printf("VCount : %02x\n", param.get_vcount());

    printf("�悭�o��Timer0 : %x\n", param.get_timer0_max());
    printf("�^�C�}�[�����Timer0 : %x\n", param.get_timer0_min());

    printf("Frame : %x\n", param.get_frame());

    printf("GxStat : %x\n\n", param.get_gxstat());

}
