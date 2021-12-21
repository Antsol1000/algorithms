//
// Created by antsol on 21.12.2021.
//

#ifndef ALGORITHMS_BIG_NUMBER_H
#define ALGORITHMS_BIG_NUMBER_H


#include <deque>
#include <string>

class big_number {

private:

    enum SIGN {
        PLUS, MINUS
    };

    typedef std::string STR;
    typedef std::deque<int> DI;
    typedef std::pair<DI, SIGN> SDI;

    static const int BASE = 1e9;

    DI number;
    SIGN sign;

public:

    big_number();
    explicit big_number(int a);
    explicit big_number(const STR& s, int base = 10);
    
    big_number& operator+=(const big_number& a);
    big_number& operator-=(const big_number& a);
    big_number& operator*=(const big_number& a);
    big_number& operator^=(const big_number& a);
    big_number& operator/=(const big_number& a);
    big_number& operator%=(const big_number& a);
    
private:

    explicit big_number(DI a, SIGN s = PLUS);

    static SIGN change_sign(const SIGN& sign);

    static SDI from_string(const STR& s, int base = 10);
    static STR to_string(const SDI& a, int base = 10);

    static int compare(const big_number& a, const big_number& b);

    static big_number add_operation(const big_number& a, const big_number& b);
    static big_number sub_operation(const big_number& a, const big_number& b);
    static big_number mul_operation(const big_number& a, const big_number& b);
    static big_number pow_operation(const big_number& a, const big_number& b);
    static big_number div_operation(const big_number& a, const big_number& b);
    static big_number mod_operation(const big_number& a, const big_number& b);

    static DI add(const DI& a, const DI& b, int shift = 0);
    static SDI sub(const DI& a, const DI& b);
    static DI mul(const DI& a, int b);
    static DI mul(const DI& a, const DI& b);
    static DI pow(const DI& a, const DI& b);
    static DI div(const DI& a, int b);
    static DI div(const DI& a, const DI& b);
    static int mod(const DI& a, int b);
    static DI mod(const DI& a, const DI& b);

    friend std::istream& operator>>(std::istream& in, big_number& a);
    friend std::ostream& operator<<(std::ostream& out, big_number& a);

    friend STR to_string(const big_number&, int);
    friend bool compare(const big_number&, const big_number&);
    friend big_number abs(const big_number&);
    
    friend big_number operator+(const big_number& a);
    friend big_number operator-(const big_number& a);

    friend big_number operator+(const big_number& a, const big_number& b);
    friend big_number operator-(const big_number& a, const big_number& b);
    friend big_number operator*(const big_number& a, const big_number& b);
    friend big_number operator^(const big_number& a, const big_number& b);
    friend big_number operator/(const big_number& a, const big_number& b);
    friend big_number operator%(const big_number& a, const big_number& b);

    friend bool operator==(const big_number& a, const big_number& b);
    friend bool operator!=(const big_number& a, const big_number& b);
    friend bool operator<(const big_number& a, const big_number& b);
    friend bool operator<=(const big_number& a, const big_number& b);
    friend bool operator>(const big_number& a, const big_number& b);
    friend bool operator>=(const big_number& a, const big_number& b);

};

std::string to_string(const big_number& a, int base = 10);
bool compare(const big_number& a, const big_number& b);
big_number abs(const big_number& a);

#endif //ALGORITHMS_BIG_NUMBER_H
