#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

class bigint {
private:
	bool sign;
	std::string num;

	static std::unordered_map<char, unsigned short> char2digit;
	static std::unordered_map<unsigned short, char> digit2char;

public:
	bigint();
	bigint(const bigint&);
	bigint(const std::string&);
	bigint(const char * const);

	template <class T>
	bigint(const T& num) : num(std::to_string(num)) {
		if(num < 0) {
			this->sign = 1;
			this->num.erase(0, 1);
		}
		else {
			this->sign = 0;
		}
	}

	~bigint();

	bigint& operator=(bigint);
	bigint& operator+=(bigint);
	bigint& operator-=(bigint);
	bigint& operator*=(bigint);
	bigint& operator/=(bigint);
	bigint& operator%=(bigint);

	bigint& operator++();
	bigint operator++(int);
	
	bigint& operator--();
	bigint operator--(int);

	void normalize(); // This function removes trailing zeros from number

	friend std::istream& operator>>(std::istream& is, bigint& bigintObj) {
		is >> bigintObj.num;

		if(bigintObj.num[0] == '-') {
			bigintObj.sign = 1;
			bigintObj.num.erase(0, 1);
		}
		else {
			bigintObj.sign = 0;
		}

		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const bigint& bigintObj) {
		if(bigintObj.sign && bigintObj.num != "0") {
			os << '-';
		}

		os << bigintObj.num << std::endl;

		return os;
	}

	friend bool operator<(const bigint& num1, const bigint& num2) {
		if(num1.sign && !num2.sign) {
			return true;
		}

		if(!num1.sign && num2.sign) {
			return false;
		}

		size_t hossz1 = num1.num.size();
		size_t hossz2 = num2.num.size();

		if(hossz1 < hossz2) {
			return true;
		}

		if(hossz1 > hossz2) {
			return false;
		}

		size_t i = 0;

		while(i < hossz1 && num1.num[i] == num2.num[i]) {
			++i;
		}

		return num1.num[i] < num2.num[i];
	}

	friend bool operator>(const bigint& num1, const bigint& num2) {
		if(num1.sign && !num2.sign) {
			return false;
		}

		if(!num1.sign && num2.sign) {
			return true;
		}

		size_t hossz1 = num1.num.size();
		size_t hossz2 = num2.num.size();

		if(hossz1 < hossz2) {
			return false;
		}

		if(hossz1 > hossz2) {
			return true;
		}

		size_t i = 0;

		while(i < hossz1 && num1.num[i] == num2.num[i]) {
			++i;
		}

		return num1.num[i] > num2.num[i];
	}

	friend bool operator<=(const bigint& num1, const bigint& num2) {
		if(num1.sign && !num2.sign) {
			return true;
		}

		if(!num1.sign && num2.sign) {
			return false;
		}

		size_t hossz1 = num1.num.size();
		size_t hossz2 = num2.num.size();

		if(hossz1 < hossz2) {
			return true;
		}

		if(hossz1 > hossz2) {
			return false;
		}

		size_t i = 0;

		while(i < hossz1 && num1.num[i] == num2.num[i]) {
			++i;
		}

		return num1.num[i] <= num2.num[i];
	}

	friend bool operator>=(const bigint& num1, const bigint& num2) {
		if(num1.sign && !num2.sign) {
			return false;
		}

		if(!num1.sign && num2.sign) {
			return true;
		}

		size_t hossz1 = num1.num.size();
		size_t hossz2 = num2.num.size();

		if(hossz1 < hossz2) {
			return false;
		}

		if(hossz1 > hossz2) {
			return true;
		}

		size_t i = 0;

		while(i < hossz1 && num1.num[i] == num2.num[i]) {
			++i;
		}

		return num1.num[i] >= num2.num[i];
	}

	friend bool operator==(const bigint& num1, const bigint& num2) {
		return (num1.sign == num2.sign) && (num1.num == num2.num);
	}

	friend bool operator!=(const bigint& num1, const bigint& num2) {
		return (num1.sign != num2.sign) || (num1.num != num2.num);
	}

	friend bigint operator+(const bigint& bigintObj) {
		return bigint(bigintObj);
	}

	friend bigint operator+(bigint num1, bigint num2) {
		// Fake addition
		if(num1.sign && !num2.sign) {
			num1.sign = 0;
			return num2 - num1;
		}

		// Fake addition
		if(!num1.sign && num2.sign) {
			num2.sign = 0;
			return num1 - num2;
		}

		// Get sign of result
		bool signFlag = num1.sign;

		// Unsign numbers
		num1.sign = 0;
		num2.sign = 0;

		// Begin actual algorithm
		bigint result;

		unsigned short carry = 0;
		unsigned short currentNum = 0;

		int i = num1.num.size() - 1;
		int j = num2.num.size() - 1;

		while(i >= 0 && j >= 0) {
			currentNum = char2digit[num1.num[i]] + char2digit[num2.num[j]] + carry;
			result.num += digit2char[currentNum % 10];
			carry = currentNum / 10;

			--i;
			--j;
		}

		while(i >= 0) {
			currentNum = char2digit[num1.num[i]] + carry;
			result.num += digit2char[currentNum % 10];
			carry = currentNum / 10;

			--i;
		}

		while(j >= 0) {
			currentNum = char2digit[num2.num[j]] + carry;
			result.num += digit2char[currentNum % 10];
			carry = currentNum / 10;

			--j;
		}

		if(carry > 0) {
			result.num += digit2char[carry];
		}

		std::reverse(result.num.begin(), result.num.end());

		// Apply sign to result
		result.sign = signFlag;

		return result;
	}

	friend bigint operator-(const bigint& bigintObj) {
		bigint result(bigintObj);
		result.sign = !result.sign;

		return result;
	}

	friend bigint operator-(bigint num1, bigint num2) {
		// Fake subtraction
		if(num1.sign && !num2.sign) {
			num2.sign = 1;
			return num1 + num2;
		}

		// Fake subtraction
		if(!num1.sign && num2.sign) {
			num2.sign = 0;
			return num1 + num2;
		}

		// Get sign of result
		bool signFlag;

		if(num2 > num1) {
			signFlag = !num2.sign;
			swap(num1.num, num2.num);
		}
		else {
			signFlag = num1.sign;
		}

		// Unsign numbers
		num1.sign = 0;
		num2.sign = 0;

		// Begin actual algorithm
		bigint result;

		int i = num1.num.size() - 1;
		int j = num2.num.size() - 1;

		char currentDigit;
		unsigned short currentNum = 0;

		while(j >= 0) {
			if(num1.num[i] < num2.num[j]) {
				int k = i - 1;

				while(num1.num[k] == '0') {
					num1.num[k] = '9';
					--k;
				}

				num1.num[k] -= 1;

				currentNum = char2digit[num1.num[i]] + 10;
				currentDigit = digit2char[currentNum - char2digit[num2.num[j]]];
			}
			else {
				currentDigit = digit2char[char2digit[num1.num[i]] - char2digit[num2.num[j]]];
			}

			result.num += currentDigit;

			--i;
			--j;
		}

		while(i >= 0) {
			result.num += num1.num[i];
			--i;
		}

		std::reverse(result.num.begin(), result.num.end());
		result.normalize();

		// Apply sign to result
		result.sign = signFlag;

		return result;
	}

	friend bigint operator*(bigint num1, bigint num2) {
		// Get sign of result
		bool signFlag = num1.sign ^ num2.sign;

		// Unsign numbers
		num1.sign = 0;
		num2.sign = 0;

		// Begin actual algorithm
		size_t offset = 0;
		bigint vegEredmeny;

		for(int i = num2.num.size() - 1; i >= 0; --i) {
			bigint reszEredmeny;

			for(unsigned short k = 0; k < offset; ++k) {
				reszEredmeny.num += '0';
			}

			unsigned short carry = 0;

			for(int j = num1.num.size() - 1; j >= 0; --j) {
				unsigned short currentNum = (char2digit[num1.num[j]] * char2digit[num2.num[i]]) + carry;
				reszEredmeny.num += digit2char[currentNum % 10];
				carry = currentNum / 10;
			}

			if(carry > 0) {
				reszEredmeny.num += digit2char[carry];
			}

			std::reverse(reszEredmeny.num.begin(), reszEredmeny.num.end());
			vegEredmeny += reszEredmeny;

			++offset;
		}

		// Apply sign to result
		vegEredmeny.sign = signFlag;

		return vegEredmeny;
	}

	friend bigint operator/(bigint num1, bigint num2) {
		if(num2.num == "0") {
			throw std::invalid_argument("division by zero!");
		}

		// Get sign of result
		bool signFlag = num1.sign ^ num2.sign;

		// Unsign numbers
		num1.sign = 0;
		num2.sign = 0;

		// Begin actual algorithm
		bigint osztando;
		bigint vegEredmeny;

		size_t i = 0;
		size_t hossz1 = num1.num.size();

		while(i < hossz1) {
			if(osztando.num.empty() && num1.num[i] == '0') {
				vegEredmeny.num += '0';
				++i;

				continue;
			}

			while(i < hossz1 && num2 > osztando) {
				osztando.num += num1.num[i];
				++i;
			}

			bigint hanyados(1);
			bigint prevEloFordulas;
			bigint currentEloFordulas(num2);

			while(hanyados.num[0] <= '9' && osztando > currentEloFordulas) {
				++hanyados.num[0];
				prevEloFordulas = currentEloFordulas;
				currentEloFordulas = num2 * hanyados;
				// currentEloFordulas = num2 + currentEloFordulas;
			}

			if(currentEloFordulas == osztando) {
				osztando.num.clear();
				vegEredmeny.num += hanyados.num;
			}
			else {
				osztando -= prevEloFordulas;
				vegEredmeny.num += (hanyados.num[0] - 1);
			}
		}

		// Apply sign to result
		vegEredmeny.sign = signFlag;

		return vegEredmeny;
	}

	friend bigint operator%(bigint num1, bigint num2) {
		if(num2.num == "0") {
			throw std::invalid_argument("division by zero!");
		}

		// Get sign of result
		bool signFlag = num1.sign;

		// Unsign numbers
		num1.sign = 0;
		num2.sign = 0;

		// Begin actual algorithm
		bigint osztando;

		size_t i = 0;
		size_t hossz1 = num1.num.size();

		while(i < hossz1) {
			if(osztando.num.empty() && num1.num[i] == '0') {
				++i;
				continue;
			}

			while(i < hossz1 && num2 > osztando) {
				osztando.num += num1.num[i];
				++i;
			}

			bigint hanyados(1);
			bigint prevEloFordulas;
			bigint currentEloFordulas(num2);

			while(hanyados.num[0] <= '9' && osztando > currentEloFordulas) {
				++hanyados.num[0];
				prevEloFordulas = currentEloFordulas;
				currentEloFordulas = num2 * hanyados;
				// currentEloFordulas = num2 + currentEloFordulas;
			}

			if(currentEloFordulas == osztando) {
				osztando.num.clear();
			}
			else {
				osztando -= prevEloFordulas;
			}
		}

		if(osztando.num.empty()) {
			return 0;
		}
		else {
			osztando.sign = signFlag; // Apply sign to result
			return osztando;
		}
	}
};
