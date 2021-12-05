//
// Created by pc mine on 12/5/2021.
//

#ifndef CPP_PROJECT_BIGINT_H
#define CPP_PROJECT_BIGINT_H

#include <ostream>
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


    bigint& operator+= (const bigint& other);

    bigint& operator-= (const bigint& other);

    bigint& operator*= (const bigint& other);


    bigint operator+ () const;

    bigint operator- () const;

    bigint& operator++ ();

    bigint& operator-- ();

    const bigint operator++ (int);

    const bigint operator-- (int);

    explicit operator bool () const;

private:
    friend std::istream& operator>> (std::istream&, bigint&);
    friend std::ostream& operator<< (std::ostream&, const bigint&);
    friend bool operator< (const bigint&, const bigint&);
};

bigint operator+ (const bigint& first, const bigint& second);
bigint operator- (const bigint& first, const bigint& second);
bigint operator* (const bigint& first, const bigint& second);

bool operator< ( const bigint& first, const bigint& second );

bool operator>  (const bigint& first, const bigint& second);

bool operator<= (const bigint& first, const bigint& second);

bool operator>= (const bigint& first, const bigint& second);

bool operator== (const bigint& first, const bigint& second);

bool operator!= (const bigint& first, const bigint& second);

/// input and output
std::istream& operator>> (std::istream& is, bigint& num);

std::ostream& operator<< (std::ostream& os, const bigint& num);

#endif //CPP_PROJECT_BIGINT_H
