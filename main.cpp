#include <iostream>
#include <string>
#include <vector>

class bigint;
bool operator> (const bigint& , const bigint& );

class bigint {
private:
    std::vector<uint8_t> _digits;
    bool positive ;

public:

    bigint () {
        positive = true;
        _digits.push_back(0);
    }

    bigint (int number) : positive{} {
        if (number < 0) {
            number *= -1;
            positive = false;
        } else {
            positive = true;
        }
        while (number != 0 || _digits.empty()){
            _digits.push_back(number%10);
            number = number / 10;
        }
    }

    bigint (const bigint& other) = default;
    ~bigint () = default;
    bigint& operator= (const bigint& other) = default;


    bigint& operator+= (const bigint& other) {
        if (positive == other.positive){
            const std::vector<uint8_t>& top =
                    _digits.size() >= other._digits.size() ? _digits : other._digits;

            const std::vector<uint8_t>& bottom =
                    _digits.size() < other._digits.size() ? _digits : other._digits;

            std::vector<uint8_t> sum(top.size());

            bool carry = false;

            for (size_t i = 0; i < top.size(); i ++) {
                sum[i] = uint8_t(carry) + top[i] + (i < bottom.size() ? bottom[i] : uint8_t(0));
                carry = sum[i] >= uint8_t(10);
                sum[i] %= uint8_t(10);
            }

            if (carry)
                sum.push_back(uint8_t(1));

            _digits = sum;
            return *this;
        }

        if (positive != other.positive) {
            bool sign_of_result;
            std::vector<uint8_t> big;
            std::vector<uint8_t> small;

            if ((*this).positive) {
                if (*this > -other) {
                    big = _digits;
                    small = other._digits;
                    sign_of_result = positive;
                } else {
                    sign_of_result = !positive;
                    small = _digits;
                    big = other._digits;
                }

            }
            else {
                if (-(*this) > other) {
                    big = _digits;
                    small = other._digits;
                    sign_of_result = positive;
                } else {
                    sign_of_result = !positive;
                    small = _digits;
                    big = other._digits;
                }
            }

            std::vector<uint8_t> diff(big.size());

            bool carry = false;

            for (size_t i = 0; i < big.size(); i ++) {
                bool need_to_carry =
                        big[i] <
                        (i < small.size() ? small[i] : uint8_t(0)) + uint8_t(carry);

                diff[i] = need_to_carry ? uint8_t(10) : uint8_t(0);
                diff[i] += big[i];
                diff[i] -= (i < small.size() ? small[i] : uint8_t(0));
                diff[i] -= uint8_t(carry);

                carry = need_to_carry;
            }

            _digits = diff;
            positive = sign_of_result;


            while (_digits[_digits.size() - 1] == 0 && _digits.size() <= 1) {
                _digits.pop_back();
            }
        }

        return *this;
    }

    bigint& operator-= (const bigint& other) {
        return *this += (-other);
    }

    bigint& operator*= (const bigint& other) {
        bool sign_result= (positive == other.positive);

        std::vector<uint8_t> result( _digits.size() + other._digits.size(), 0 );

        for (size_t j = 0; j < other._digits.size(); j++) {
            size_t i = 0;
            for (size_t t = j; t < j + _digits.size(); t++) {
                result[t] += other._digits[j] * _digits[i];
                if (result[t] > 9) {
                    result[t+1] += result[t]/10;
                    result[t] = result[t]%10;
                }
                i++;
            }
        }


        _digits = result;

        while (_digits[_digits.size() - 1] == 0 && !_digits.empty() && _digits.size() - 1 != 0 ) {
            _digits.pop_back();
        }

        positive = !(_digits.size() == 1 && _digits[0] == 0) && sign_result;

        return *this;
    }


    bigint operator+ () const {
        return *this;
    }

    bigint operator- () const {
        bigint cpy(*this);
        cpy.positive = !cpy.positive;
        return cpy;
    }

    bigint& operator++ () {
        return operator+=(bigint(1));
    }

    bigint& operator-- () {
        return operator-=(bigint(1));
    }

    const bigint operator++ (int) {
        bigint cpy(*this);
        operator++();
        return cpy;
    }

    const bigint operator-- (int) {
        bigint cpy(*this);
        operator--();
        return cpy;
    }

    explicit operator bool () const {
        /// if the number is 0, then return false, otherwise return true.
        return _digits.size() > 1 || _digits[0] != 0;
    }

    std::string to_string() const {
        std::string str;
        if (positive)
            for (size_t i = 0 ; i < _digits.size() ; i++)
                str += (_digits[_digits.size() - 1 - i] + '0');
        else {
            str += '-';
            for (size_t i = 0 ; i < _digits.size() ; i++)
                str += (_digits[_digits.size() - 1 - i] + '0');
        }
        return str;
    }

private:
    friend std::istream& operator>> (std::istream&, bigint&);
    friend std::ostream& operator<< (std::ostream&, const bigint&);
    friend bool operator< (const bigint&, const bigint&);
};

bigint operator+ (const bigint& first, const bigint& second) { return bigint(first) += second; }
bigint operator- (const bigint& first, const bigint& second) { return bigint(first) -= second; }
bigint operator* (const bigint& first, const bigint& second) { return bigint(first) *= second; }

bool operator< ( const bigint& first, const bigint& second ) {
    if (first.positive != second.positive) {
        return first.positive < second.positive;
    }

    if (first.positive && second.positive) {
        if (first._digits.size() != second._digits.size()) {
            return first._digits.size() < second._digits.size();
        }

        for (ssize_t i = first._digits.size() - 1; i >= 0; i--) {
            if (first._digits[i] != second._digits[i]) {
                return first._digits[i] < second._digits[i];
            }
        }

        /// The case when the numbers are equal.
    }

    if (!first.positive && !second.positive) {
        if (first._digits.size() != second._digits.size()) {
            return first._digits.size() > second._digits.size();
        }

        for (ssize_t i = first._digits.size()-1; i >= 0 ; i --) {
            if (first._digits[i] != second._digits[i]) {
                return first._digits[i] > second._digits[i];
            }
        }
    }
    /// The case when the numbers are equal.
    return false;
}

bool operator> (const bigint& first, const bigint& second ){
    return second < first;
}

bool operator<= ( const bigint& first, const bigint& second ){
    return !(second < first);
}

bool operator>= ( const bigint& first, const bigint& second ){
    return !(first < second);
}

bool operator== (const bigint& first, const bigint& second) {
    return !(first < second) && !(second < first);
}

bool operator!= (const bigint& first, const bigint& second) {
    return (first < second) || (second < first);
}

//input and output
std::istream& operator>> (std::istream& is, bigint& num) {
    /// Need to clear the old digits.
    num._digits.clear();

    std::string str;
    is >> str;

    bool inputted_non_zero_digit = false;

    if (str[0] == '-'){
        num.positive = false;
        for (size_t i = 0; i< str.size()-1; i ++){
            str[i] = str[i+1];
        }
        str.erase(str.size() - 1,1);
    } else {
        num.positive = true;
    }

    for (size_t i = 0; i < str.size(); i ++) {
        if (str[i] >= '1' && str[i] <= '9') {
            num._digits.push_back(str[i] - '0');
            inputted_non_zero_digit = true;
        } else if (str[i] == '0') {
            if (inputted_non_zero_digit) {
                num._digits.push_back(0);
            }
        } else {
            break;
        }
    }

    /// reverse:
    for (size_t i = 0; i < num._digits.size() / 2; i ++) {
        const uint8_t tmp = num._digits[i];
        num._digits[i] = num._digits[num._digits.size() - 1 - i];
        num._digits[num._digits.size() - 1 - i] = tmp;
    }

    /// The case when user inputted only zeros:
    if (num._digits.empty()){
        num._digits.push_back(0);
        num.positive = true;
    }

    return is;
}

std::ostream& operator<< (std::ostream& os, const bigint& num) {
    if (!num.positive) {
        os << '-';
    }
    for (ssize_t i = num._digits.size() - 1 ; i >= 0; i --) {
        os << (int) num._digits[i];
    }

    return os;
}

int main() {
    bigint a, b(123456789);
    std::cin >> a;
    std::cout << a * b << std::endl;
}
