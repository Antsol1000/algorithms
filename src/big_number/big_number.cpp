//
// Created by Antoni Solarski
// github.com/Antsol1000
//

#include <algorithm>
#include <utility>
#include "big_number.h"

big_number::big_number() {
    number.push_back(0);
    sign = PLUS;
}

big_number::big_number(int a) {
    number.push_back(a < 0 ? -a : a);
    sign = a < 0 ? MINUS : PLUS;
}

big_number::big_number(const STR& s, int base) {
    SDI n = from_string(s, base);
    number = n.first;
    sign = n.second;
}

big_number &big_number::operator+=(const big_number &a) {
    big_number x = *this + a;
    this->number = x.number;
    this->sign = x.sign;
    return *this;
}

big_number &big_number::operator-=(const big_number &a) {
    big_number x = *this - a;
    this->number = x.number;
    this->sign = x.sign;
    return *this;
}

big_number &big_number::operator*=(const big_number &a) {
    big_number x = *this * a;
    this->number = x.number;
    this->sign = x.sign;
    return *this;
}

big_number &big_number::operator^=(const big_number &a) {
    big_number x = *this ^ a;
    this->number = x.number;
    this->sign = x.sign;
    return *this;
}

big_number &big_number::operator/=(const big_number &a) {
    big_number x = *this / a;
    this->number = x.number;
    this->sign = x.sign;
    return *this;
}

big_number &big_number::operator%=(const big_number &a) {
    big_number x = *this % a;
    this->number = x.number;
    this->sign = x.sign;
    return *this;
}

big_number::big_number(DI a, SIGN s) {
    number = std::move(a);
    sign = s;
}

big_number::SIGN big_number::change_sign(const big_number::SIGN &sign) {
    return sign == PLUS ? MINUS : PLUS;
}

std::string to_string(const big_number &a, int base) {
    return big_number::to_string(big_number::SDI(a.number, a.sign), base);
}

bool compare(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) < 1;
}

big_number abs(const big_number& a) {
    return big_number(a.number);
}

big_number operator+(const big_number &a) {
    return big_number(a.number, a.sign);
}

big_number operator-(const big_number &a) {
    return big_number(a.number, big_number::change_sign(a.sign));
}

big_number operator+(const big_number &a, const big_number &b) {
    return big_number::add_operation(a, b);
}

big_number operator-(const big_number &a, const big_number &b) {
    return big_number::sub_operation(a, b);
}

big_number operator*(const big_number &a, const big_number &b) {
    return big_number::mul_operation(a, b);
}

big_number operator^(const big_number &a, const big_number &b) {
    return big_number::pow_operation(a, b);
}

big_number operator/(const big_number &a, const big_number &b) {
    return big_number::div_operation(a, b);
}

big_number operator%(const big_number &a, const big_number &b) {
    return big_number::mod_operation(a, b);
}

bool operator==(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) == 0;
}

bool operator!=(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) != 0;
}

bool operator<(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) == -1;
}

bool operator<=(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) < 1;
}

bool operator>(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) == 1;
}

bool operator>=(const big_number &a, const big_number &b) {
    return big_number::compare(a, b) > -1;
}

std::istream &operator>>(std::istream &in, big_number &a) {
    std::string s;
    in >> s;
    a = big_number(s);
    return in;
}

std::ostream &operator<<(std::ostream &out, big_number &a) {
    out << to_string(a);
    return out;
}

big_number::SDI big_number::from_string(const big_number::STR &s, int base) {
    //FIXME
    DI n, b, t;
    n.push_back(0); b.push_back(1);
    bool sign = s[0] == '-' || s[0] == '+';
    bool minus = s[0] == '-';
    for (auto c = s.begin() + sign; c != s.end(); c++) {
        n = mul(n, base);
        t.push_back(*c > '9' ? *c - 'A' + 10 : *c - '0');
        n = add(n, t);
        t.clear();
    }
    while (n.front() == 0 && n.size() > 1) {
        n.pop_front();
    }
    return {n, minus ? MINUS : PLUS};
}

big_number::STR big_number::to_string(const big_number::SDI &a, int base) {
    //FIXME
    if (a.first.back() == 0) {
        return "0";
    }
    DI ca(a.first);
    STR result;
    while (ca.back() > 0) {
        int x = ca.back() % base;
        result.push_back((char) (x < 10 ? x + '0' : x + 'A' - 10));
        ca = div(ca, base);
    }
    if (a.second == MINUS) {
        result.push_back('-');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int big_number::compare(const big_number &a, const big_number &b) {
    int aSign = a.sign == MINUS ? -1 : 1;
    int bSign = b.sign == MINUS ? -1 : 1;
    int len = a.number.size() == b.number.size() ? 0 : a.number.size() < b.number.size() ? -1 : 1;
    if (aSign < bSign) {
        return -1;
    } else if (aSign > bSign) {
        return 1;
    } else if ((aSign < 0 && len == 1) || (aSign > 0 && len == -1)) {
        return -1;
    } else if ((aSign > 0 && len == 1) || (aSign < 0 && len == -1)) {
        return 1;
    } else {
        for (auto i = a.number.begin(), j = b.number.begin(); i != a.number.end(); i++, j++) {
            if (*i < *j) {
                return -1;
            } else if (*i > *j) {
                return 1;
            }
        }
    }
    return 0;
}

big_number big_number::add_operation(const big_number &a, const big_number &b) {
    if (a.sign == MINUS && b.sign == MINUS) {
        return big_number(add(a.number, b.number), MINUS);
    } else if (a.sign == PLUS && b.sign == MINUS) {
        SDI result = sub(a.number, b.number);
        return big_number(result.first, result.second);
    } else if (a.sign == MINUS && b.sign == PLUS) {
        SDI result = sub(a.number, b.number);
        return big_number(result.first, change_sign(result.second));
    } else {
        return big_number(add(a.number, b.number));
    }
}

big_number big_number::sub_operation(const big_number &a, const big_number &b) {
    if (a.sign == MINUS && b.sign == PLUS) {
        return big_number(add(a.number, b.number), MINUS);
    } else if (a.sign == PLUS && b.sign == PLUS) {
        SDI result = sub(a.number, b.number);
        return big_number(result.first, result.second);
    } else if (a.sign == MINUS && b.sign == MINUS) {
        SDI result = sub(a.number, b.number);
        return big_number(result.first, change_sign(result.second));
    } else {
        return big_number(add(a.number, b.number));
    }
}

big_number big_number::mul_operation(const big_number &a, const big_number &b) {
    return a.sign == b.sign ?
        big_number(mul(a.number, b.number)) : big_number(mul(a.number, b.number), MINUS);
}

big_number big_number::pow_operation(const big_number &a, const big_number &b) {
    if (b.sign < 0) {
        throw "unsupported operation";
    }
    return a.sign == MINUS && mod(b.number, 2) == 1 ?
        big_number(pow(a.number, b.number), MINUS) : big_number(pow(a.number, b.number));
}

big_number big_number::div_operation(const big_number &a, const big_number &b) {
    return a.sign == b.sign ?
           big_number(div(a.number, b.number)) : big_number(div(a.number, b.number), MINUS);
}

big_number big_number::mod_operation(const big_number &a, const big_number &b) {
    // TODO
    mod(a.number, b.number);
    return {};
}

big_number::DI big_number::add(const big_number::DI &a, const big_number::DI &b, int shift) {
    DI s;
    int carry = 0;
    auto i = a.end(), j = b.end();
    i--; j--;
    for (int k = 0; k < shift; k++) {
        s.push_front(*i);
        i--;
    }
    while (i >= a.begin() && j >= b.begin()) {
        int sum = (*i) + (*j) + carry;
        i--; j--;
        s.push_front(sum % BASE);
        carry = sum / BASE;
    }
    while (i >= a.begin() || j >= b.begin()) {
        if (i >= a.begin()) {
            carry += *i;
            i--;
        } else {
            carry += *j;
            j--;
        }
        s.push_front(carry % BASE);
        carry /= BASE;
    }
    if (carry) {
        s.push_front(carry);
    }
    return s;
}

big_number::SDI big_number::sub(const big_number::DI &a, const big_number::DI &b) {
    if (a.size() < b.size()) {
        return {sub(b, a).first, MINUS};
    }
    DI s;
    int carry = 0;
    auto i = a.end(), j = b.end();
    i--; j--;
    while (j >= b.begin()) {
        int sum = (*i) - (*j) + carry;
        if (sum < 0 && i != a.begin()) {
            sum += BASE;
            carry = -1;
        } else {
            carry = 0;
        }
        i--; j--;
        s.push_front(sum);
    }
    while (i >= a.begin()) {
        int sum = (*i) + carry;
        if (sum < 0 && i != a.begin()) {
            sum += BASE;
            carry = -1;
        } else {
            carry = 0;
        }
        i--;
        s.push_front(sum);
    }
    while (*s.begin() == 0 && s.size() > 1) {
        s.pop_front();
    }
    if (s.front() < 0) {
        s.front() *= -1;
        return {s, MINUS};
    } else {
        return {s, PLUS};
    }
}

big_number::DI big_number::mul(const big_number::DI &a, int b) {
    DI s;
    int carry = 0;
    auto i = a.end();
    i--;
    while (i >= a.begin()) {
        long long sum = (long long) (*i) * b + carry;
        i--;
        s.push_front(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        s.push_front(carry);
    }
    return s;
}

big_number::DI big_number::mul(const big_number::DI &a, const big_number::DI &b) {
    DI s;
    s.push_front(0);
    auto j = b.end();
    j--;
    int shift = 0;
    while (j >= b.begin()) {
        s = add(s, mul(a, *j), shift++);
        j--;
    }
    return s;
}

big_number::DI big_number::pow(const big_number::DI &a, const big_number::DI &b) {
    DI s, aa = a, bb = b;
    s.push_back(1);
    while (bb.back() != 0) {
        if (bb.back() % 2 == 1) {
            s = mul(s, aa);
        }
        aa = mul(aa, aa);
        bb = div(bb, 2);
    }
    return s;
}

big_number::DI big_number::div(const big_number::DI &a, int b) {
    DI s;
    long long carry = 0;
    auto i = a.begin();
    while (i != a.end()) {
        carry = carry * BASE + (*i);
        i++;
        s.push_back(carry / b);
        carry %= b;
    }
    while (*s.begin() == 0 && s.size() > 1) {
        s.pop_front();
    }
    return s;
}

big_number::DI big_number::div(const big_number::DI &a, const big_number::DI &b) {
    // TODO
    return {};
}

int big_number::mod(const big_number::DI &a, int b) {
    return a.back() % b;
}

big_number::DI big_number::mod(const big_number::DI &a, const big_number::DI &b) {
    // TODO
    return {};
}
