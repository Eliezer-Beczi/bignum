#include "bigint.h"

std::unordered_map<char, unsigned short> bigint::char2digit { { '0', 0 }, { '1', 1 }, { '2', 2 }, { '3', 3 }, { '4', 4 }, { '5', 5 }, { '6', 6 }, { '7', 7 }, { '8', 8 }, { '9', 9 } };
std::unordered_map<unsigned short, char> bigint::digit2char { { 0, '0' }, { 1, '1' }, { 2, '2' }, { 3, '3' }, { 4, '4' }, { 5, '5' }, { 6, '6' }, { 7, '7' }, { 8, '8' }, { 9, '9' } };

bigint::bigint() : sign(), num() { }
bigint::bigint(const bigint& bigintObj) : sign(bigintObj.sign), num(bigintObj.num) { }

bigint::bigint(const std::string& str) : num(str) {
	if(str[0] == '-') {
		sign = 1;
		num.erase(0, 1);
	}
	else {
		sign = 0;
	}
}

bigint::bigint(const char * const s) : num(s) {
	if(s[0] == '-') {
		sign = 1;
		num.erase(0, 1);
	}
	else {
		sign = 0;
	}
}

bigint::~bigint() { }

bigint& bigint::operator=(bigint bigintObj) {
	if(this == &bigintObj) {
		return *this;
	}

	sign = bigintObj.sign;
	num.swap(bigintObj.num);

	return *this;
}

bigint& bigint::operator+=(bigint bigintObj) {
	// Fake addition
	if(this->sign && !bigintObj.sign) {
		this->sign = 0;
		return (*this = bigintObj - *this);
	}

	// Fake addition
	if(!this->sign && bigintObj.sign) {
		bigintObj.sign = 0;
		return (*this -= bigintObj);
	}

	// Get sign of result
	bool signFlag = this->sign;

	// Unsign numbers
	this->sign = 0;
	bigintObj.sign = 0;

	// Begin actual algorithm
	unsigned short carry = 0;
	unsigned short currentNum = 0;

	int i = this->num.size() - 1;
	int j = bigintObj.num.size() - 1;

	while(i >= 0 && j >= 0) {
		currentNum = char2digit[this->num[i]] + char2digit[bigintObj.num[j]] + carry;
		this->num[i] = digit2char[currentNum % 10];
		carry = currentNum / 10;

		--i;
		--j;
	}

	while(i >= 0) {
		currentNum = char2digit[this->num[i]] + carry;
		this->num[i] = digit2char[currentNum % 10];
		carry = currentNum / 10;

		--i;
	}

	while(j >= 0) {
		currentNum = char2digit[bigintObj.num[j]] + carry;
		this->num.insert(0, 1, digit2char[currentNum % 10]);
		carry = currentNum / 10;

		--j;
	}

	if(carry > 0) {
		this->num.insert(0, 1, digit2char[carry]);
	}

	// Apply sign to result
	this->sign = signFlag;

	return *this;
}

bigint& bigint::operator-=(bigint bigintObj) {
	// Fake subtraction
	if(this->sign && !bigintObj.sign) {
		bigintObj.sign = 1;
		return (*this += bigintObj);
	}

	// Fake subtraction
	if(!this->sign && bigintObj.sign) {
		bigintObj.sign = 0;
		return (*this += bigintObj);
	}

	// Get sign of result
	bool signFlag;

	if(bigintObj > *this) {
		signFlag = !bigintObj.sign;
		swap(this->num, bigintObj.num);
	}
	else {
		signFlag = this->sign;
	}

	// Unsign numbers
	this->sign = 0;
	bigintObj.sign = 0;

	// Begin actual algorithm
	int i = this->num.size() - 1;
	int j = bigintObj.num.size() - 1;

	char currentDigit;
	unsigned short currentNum = 0;

	while(j >= 0) {
		if(this->num[i] < bigintObj.num[j]) {
			int k = i - 1;

			while(this->num[k] == '0') {
				this->num[k] = '9';
				--k;
			}

			this->num[k] -= 1;

			currentNum = char2digit[this->num[i]] + 10;
			currentDigit = digit2char[currentNum - char2digit[bigintObj.num[j]]];
		}
		else {
			currentDigit = digit2char[char2digit[this->num[i]] - char2digit[bigintObj.num[j]]];
		}

		this->num[i] = currentDigit;

		--i;
		--j;
	}

	(*this).normalize();

	// Apply sign to result
	this->sign = signFlag;

	return *this;
}

bigint& bigint::operator*=(bigint bigintObj) {
	// Get sign of result
	bool signFlag = this->sign ^ bigintObj.sign;

	// Unsign numbers
	this->sign = 0;
	bigintObj.sign = 0;

	// Begin actual algorithm
	size_t offset = 0;
	bigint vegEredmeny;

	for(int i = bigintObj.num.size() - 1; i >= 0; --i) {
		bigint reszEredmeny;

		for(unsigned short k = 0; k < offset; ++k) {
			reszEredmeny.num += '0';
		}

		unsigned short carry = 0;

		for(int j = this->num.size() - 1; j >= 0; --j) {
			unsigned short currentNum = (char2digit[this->num[j]] * char2digit[bigintObj.num[i]]) + carry;
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

	swap(this->num, vegEredmeny.num);

	// Apply sign to result
	this->sign = signFlag;

	return *this;
}

bigint& bigint::operator/=(bigint bigintObj) {
	if(bigintObj.num == "0") {
		throw std::invalid_argument("division by zero!");
	}

	// Get sign of result
	bool signFlag = this->sign ^ bigintObj.sign;

	// Unsign numbers
	this->sign = 0;
	bigintObj.sign = 0;

	// Begin actual algorithm
	bigint osztando;
	bigint vegEredmeny;

	size_t i = 0;
	size_t hossz = this->num.size();

	while(i < hossz) {
		if(osztando.num.empty() && this->num[i] == '0') {
			vegEredmeny.num += '0';
			++i;

			continue;
		}

		while(i < hossz && bigintObj > osztando) {
			osztando.num += this->num[i];
			++i;
		}

		bigint hanyados(1);
		bigint prevEloFordulas;
		bigint currentEloFordulas(bigintObj);

		while(hanyados.num[0] <= '9' && osztando > currentEloFordulas) {
			++hanyados.num[0];
			prevEloFordulas = currentEloFordulas;
			currentEloFordulas = bigintObj * hanyados;
			// currentEloFordulas = bigintObj + currentEloFordulas;
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

	swap(this->num, vegEredmeny.num);

	// Apply sign to result
	this->sign = signFlag;

	return *this;
}

bigint& bigint::operator%=(bigint bigintObj) {
	if(bigintObj.num == "0") {
		throw std::invalid_argument("division by zero!");
	}

	// Get sign of result
	bool signFlag = this->sign;

	// Unsign numbers
	this->sign = 0;
	bigintObj.sign = 0;

	// Begin actual algorithm
	bigint osztando;

	size_t i = 0;
	size_t hossz = this->num.size();

	while(i < hossz) {
		if(osztando.num.empty() && this->num[i] == '0') {
			++i;
			continue;
		}

		while(i < hossz && bigintObj > osztando) {
			osztando.num += this->num[i];
			++i;
		}

		bigint hanyados(1);
		bigint prevEloFordulas;
		bigint currentEloFordulas(bigintObj);

		while(hanyados.num[0] <= '9' && osztando > currentEloFordulas) {
			++hanyados.num[0];
			prevEloFordulas = currentEloFordulas;
			currentEloFordulas = bigintObj * hanyados;
			// currentEloFordulas = bigintObj + currentEloFordulas;
		}

		if(currentEloFordulas == osztando) {
			osztando.num.clear();
		}
		else {
			osztando -= prevEloFordulas;
		}
	}

	if(osztando.num.empty()) {
		*this = 0;
	}
	else {
		this->num = osztando.num;
		this->sign = signFlag; // Apply sign to result
	}

	return *this;
}

bigint& bigint::operator++() {
	*this += 1;
	return *this;
}

bigint bigint::operator++(int) {
	bigint tmp(*this);
	operator++();
	return tmp;
}

bigint& bigint::operator--() {
	*this -= 1;
	return *this;
}

bigint bigint::operator--(int) {
	bigint tmp(*this);
	operator--();
	return tmp;
}

// This function removes trailing zeros from number
void bigint::normalize() {
	size_t i = 0;
	size_t hossz = num.size();

	while(i < hossz - 1 && num[i] == '0') {
		++i;
	}

	num.erase(0, i);
}
