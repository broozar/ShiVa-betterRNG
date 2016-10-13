#include "PrecompiledHeader.h"

#include "C_srng.h"
#include <memory>



// PCG64 helpers: C-style baby!
void pcg64_srandom_r(pcg64_random_t* rng, uint64_t seed1, uint64_t seed2, uint64_t seq1, uint64_t seq2) {
	uint64_t mask = ~0ull >> 1;
	// The stream for each of the two generators *must* be distinct
	if ((seq1 & mask) == (seq2 & mask)) 
		seq2 = ~seq2;
	pcg32_srandom_r(rng->gen,   seed1, seq1);
	pcg32_srandom_r(rng->gen+1, seed2, seq2);
}

uint64_t pcg64_random_r(pcg64_random_t* rng) {
	return ((uint64_t)(pcg32_random_r(rng->gen)) << 32)
		| pcg32_random_r(rng->gen+1);
}

uint64_t pcg64_boundedrand_r(pcg64_random_t* rng, uint64_t bound) {
	uint64_t threshold = -bound % bound;
	for (;;) {
		uint64_t r = pcg64_random_r(rng);
		if (r >= threshold)
			return r % bound;
	}
}


// ----------------------------------------------------------------------------

// actual class - initializers
C_srng::C_srng() {
}

C_srng::~C_srng() {
	if (_pmt32 != nullptr)
		delete _pmt32;
	if (_pmt64 != nullptr)
		delete _pmt64;
	if (_ppcg32 != nullptr)
		delete _ppcg32;
	if (_ppcg64 != nullptr)
		delete _ppcg64;
}

bool C_srng::init_rng(const char * algo) {
	
	if (strcmp(algo, "mt32") == 0 && _pmt32 == nullptr) {
		_pmt32 = new std::mt19937(_rd32());
		return true;
	}
	else if (strcmp(algo, "mt64") == 0 && _pmt64 == nullptr) {
		_pmt64 = new std::mt19937_64(_rd64());
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0 && _ppcg32 == nullptr) {
		_ppcg32 = new pcg32_random_t;
		pcg32_srandom_r(_ppcg32, _rd32(), _rd32());
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0 && _ppcg64 == nullptr) {
		_ppcg64 = new pcg64_random_t;
		pcg64_srandom_r(_ppcg64, _rd64(), _rd64(), _rd64(), _rd64());
		return true;
	}

	return false;
}


// ----------------------------------------------------------------------------

// string output for bigger numbers
bool C_srng::dist_int_s(const uint64_t && lower, const uint64_t && upper, std::string & output, const char * algo)
{
	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		output = std::to_string(_dist_mt32(std::move(lower), std::move(upper), _pmt32));
		return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		output = std::to_string(_dist_mt64(std::move(lower), std::move(upper), _pmt64));
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		auto diff = upper - lower;
		output = std::to_string( pcg32_boundedrand_r(_ppcg32, static_cast<uint32_t>(diff+1)) + lower );
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		auto diff = upper - lower;
		output = std::to_string( pcg64_boundedrand_r(_ppcg64, diff+1) + lower );
		return true;
	}

	return false;
}


// ----------------------------------------------------------------------------

// reseeders
bool C_srng::reseed_int(const uint64_t && seed, const char * algo) {
	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		_reseed_mt32(std::move(seed), _pmt32);
		return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		_reseed_mt64(std::move(seed), _pmt64);
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		pcg32_srandom_r(_ppcg32, time(NULL)^seed, seed);
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		pcg64_srandom_r(_ppcg64, time(NULL)^(intptr_t)&pcg64_srandom_r, ~time(NULL)^(intptr_t)&pcg32_random_r, time(NULL)^seed, ~time(NULL)^seed);
		return true;
	}

	return false;
}

bool C_srng::reseed_device(const char * algo)
{
	if (strcmp(algo, "mt32") == 0) {
		if (_reseed_mt32(0, _pmt32))
			return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_reseed_mt64(0, _pmt64)) 
			return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		//pcg32_srandom_r(_ppcg32, time(NULL) ^ (intptr_t)&printf, (intptr_t)&dummyglobal1);
		pcg32_srandom_r(_ppcg32, _rd64(), _rd64());
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		pcg64_srandom_r(_ppcg64, _rd64(), _rd64(), _rd64(), _rd64());
		return true;
	}

	return false;
}

bool C_srng::_reseed_mt32(const uint64_t && seed, std::mt19937 * mt) {
	if (mt == nullptr) return false;

	if (seed == 0) { // SLOWER!
		mt->seed(_rd32());
		return true;
	}

	// otherwise...
	mt->seed(static_cast<uint32_t>(seed));
	return true;
}

bool C_srng::_reseed_mt64(const uint64_t && seed, std::mt19937_64 * mt) {
	if (mt == nullptr) return false;

	if (seed == 0) { // SLOWER!
		mt->seed(_rd64());
		return true;
	}

	// otherwise...
	mt->seed(seed);
	return true;
}


// ----------------------------------------------------------------------------

// random device NG
uint32_t C_srng::_rd32() {
	std::random_device rd;
	return rd();
}

uint64_t C_srng::_rd64() {	
	std::random_device rd;
	return ((static_cast<uint64_t>(rd()) << 32) | rd());
}


// ----------------------------------------------------------------------------

// (uniform) distribution generators
uint32_t C_srng::_dist_mt32(const uint64_t && lower, const uint64_t && upper, std::mt19937 * mt) {
	if (mt == nullptr) return 0;
	std::uniform_int_distribution<uint32_t> dist(lower, upper); // assuming the user read the docs and will not plug in 64bit numbers into a 32bit algorithm
	return dist(*mt);
}

uint64_t C_srng::_dist_mt64(const uint64_t && lower, const uint64_t && upper, std::mt19937_64 * mt) {
	if (mt == nullptr) return 0;
	std::uniform_int_distribution<uint64_t> dist(lower, upper);
	return dist(*mt);
}


// ----------------------------------------------------------------------------

// fun functions!
bool C_srng::coinflips(const int && repetitions, std::string & output, const char * algo)
{
	std::vector<char> tmp;
	tmp.reserve(repetitions);

	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		std::bernoulli_distribution bd;
		for (auto i = 0; i < repetitions; ++i)
			 bd(*_pmt32)? tmp.push_back('H') : tmp.push_back('T');
		output = std::string(tmp.data(), tmp.size());
		return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		std::bernoulli_distribution bd;
		for (auto i = 0; i < repetitions; ++i)
			bd(*_pmt64)? tmp.push_back('H') : tmp.push_back('T');
		output = std::string(tmp.data(), tmp.size());
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		for (auto i = 0; i < repetitions; ++i)
			pcg32_boundedrand_r(_ppcg32, 2) ? tmp.push_back('H') : tmp.push_back('T');
		output = std::string(tmp.data(), tmp.size());
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		for (auto i = 0; i < repetitions; ++i)
			pcg64_boundedrand_r(_ppcg64, 2) ? tmp.push_back('H') : tmp.push_back('T');
		output = std::string(tmp.data(), tmp.size());
		return true;
	}

	return false;
}

bool C_srng::dicerolls(const int && repetitions, std::string & output, const char * algo)
{
	std::vector<uint32_t> tmp32;
	std::vector<uint64_t> tmp64;
	std::stringstream sstr;

	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		tmp32.reserve(repetitions);
		for (auto i = 0; i < repetitions; ++i)
			tmp32.push_back(_dist_mt32(1, 6, _pmt32));
		std::copy(tmp32.begin(), tmp32.end(), std::ostream_iterator<uint32_t>(sstr));
		output = sstr.str();
		return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		tmp64.reserve(repetitions);
		for (auto i = 0; i < repetitions; ++i)
			tmp64.push_back(_dist_mt64(1, 6, _pmt64));
		std::copy(tmp64.begin(), tmp64.end(), std::ostream_iterator<uint64_t>(sstr));
		output = sstr.str();
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		tmp32.reserve(repetitions);
		for (auto i = 0; i < repetitions; ++i)
			tmp32.push_back(pcg32_boundedrand_r(_ppcg32, 6) +1);
		std::copy(tmp32.begin(), tmp32.end(), std::ostream_iterator<uint32_t>(sstr));
		output = sstr.str();
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		tmp64.reserve(repetitions);
		for (auto i = 0; i < repetitions; ++i)
			tmp64.push_back(pcg64_boundedrand_r(_ppcg64, 6) +1);
		std::copy(tmp64.begin(), tmp64.end(), std::ostream_iterator<uint64_t>(sstr));
		output = sstr.str();
		return true;
	}

	return false;
}

bool C_srng::bitnum32(std::string & output, const char * algo)
{
	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		sprintf(buffer, "0x%08x", _dist_mt32(0, UINT32_MAX, _pmt32));
		S3DX::log.warning("PLUG ", buffer);
		output = std::string(buffer, 10);
		return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		sprintf(buffer, "0x%08llx", _dist_mt64(0, UINT32_MAX, _pmt64));
		output = std::string(buffer, 10);
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		sprintf(buffer, "0x%08x", pcg32_random_r(_ppcg32));
		output = std::string(buffer, 10);
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		sprintf(buffer, "0x%08llx", pcg64_random_r(_ppcg64) & 0xFFFFFFFF); // cutoff half
		output = std::string(buffer, 10);
		return true;
	}

	return false;
}

bool C_srng::bitnum64(std::string & output, const char * algo)
{
	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		sprintf(buffer, "0x%08x%08x", _dist_mt32(0, UINT32_MAX, _pmt32), _dist_mt32(0, UINT32_MAX, _pmt32));
		output = std::string(buffer, 18);
		return true;
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		sprintf(buffer, "0x%016llx", _dist_mt64(0, UINT64_MAX, _pmt64));
		output = std::string(buffer, 18);
		return true;
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		sprintf(buffer, "0x%08x%08x", pcg32_random_r(_ppcg32), pcg32_random_r(_ppcg32));
		output = std::string(buffer, 18);
		return true;
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		sprintf(buffer, "0x%016llx", pcg64_random_r(_ppcg64));
		output = std::string(buffer, 18);
		return true;
	}

	return false;
}

bool C_srng::cardshuffle(bool && use32cards, std::string & output, const char * algo)
{
	struct {
		short SUITS = 4, NUMBERS, CARDS;
		std::vector<char> number;
		std::vector<char> suit{'h', 'c', 'd', 's'};
	} eDeck;

	if (use32cards) { 
		eDeck.NUMBERS = 8; eDeck.CARDS = 32; 
		eDeck.number.reserve(8);
		//eDeck.number.push_back({ 'A', '7', '8', '9', 'T', 'J', 'Q', 'K' }); // not yet supported
		eDeck.number.push_back('A');
		eDeck.number.push_back('7');
		eDeck.number.push_back('8');
		eDeck.number.push_back('9');
		eDeck.number.push_back('T');
		eDeck.number.push_back('J');
		eDeck.number.push_back('Q');
		eDeck.number.push_back('K');
	}
	else { 
		eDeck.NUMBERS = 13; eDeck.CARDS = 52; 
		eDeck.number.reserve(13);
		//eDeck.number.push_back({ 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' }); // not yet supported
		eDeck.number.push_back('A');
		eDeck.number.push_back('2');
		eDeck.number.push_back('3');
		eDeck.number.push_back('4');
		eDeck.number.push_back('5');
		eDeck.number.push_back('6');
		eDeck.number.push_back('7');
		eDeck.number.push_back('8');
		eDeck.number.push_back('9');
		eDeck.number.push_back('T');
		eDeck.number.push_back('J');
		eDeck.number.push_back('Q');
		eDeck.number.push_back('K');
	}

	std::vector<short> cards;
	cards.reserve(eDeck.CARDS);

	for (auto && i = 0; i < eDeck.CARDS; ++i)
		cards.push_back(i);


	if (strcmp(algo, "mt32") == 0) {
		if (_pmt32 == nullptr) return false;
		for (auto i = eDeck.CARDS; i > 1; --i) {
			auto chosen = _dist_mt32(0, i+1, _pmt32);
			auto card = cards.at(chosen);
			cards[chosen] = cards.at(i - 1);
			cards[i - 1] = card;
		}
	}
	else if (strcmp(algo, "mt64") == 0) {
		if (_pmt64 == nullptr) return false;
		for (auto i = eDeck.CARDS; i > 1; --i) {
			auto chosen = _dist_mt64(0, i+1, _pmt64);
			auto card = cards.at(chosen);
			cards[chosen] = cards.at(i - 1);
			cards[i - 1] = card;
		}
	}
	else if (strcmp(algo, "pcg32") == 0) {
		if (_ppcg32 == nullptr) return false;
		for (auto i = eDeck.CARDS; i > 1; --i) {
			auto chosen = pcg32_boundedrand_r(_ppcg32, i);
			auto card = cards.at(chosen);
			cards[chosen] = cards.at(i - 1);
			cards[i - 1] = card;
		}
	}
	else if (strcmp(algo, "pcg64") == 0) {
		if (_ppcg64 == nullptr) return false;
		auto i = eDeck.CARDS;
		for (auto i = eDeck.CARDS; i > 1; --i) {
			auto chosen = pcg64_boundedrand_r(_ppcg64, i);
			auto card = cards.at(chosen);
			cards[chosen] = cards.at(i - 1);
			cards[i - 1] = card;
		}		
	}


	std::vector<std::string> suffledcards;
	suffledcards.reserve(eDeck.CARDS);
	char buffer[2];
	std::stringstream sstr;

	for (auto && i = 0; i < eDeck.CARDS; ++i) {
		sprintf(buffer, "%c%c", eDeck.number.at(cards.at(i) / eDeck.SUITS), eDeck.suit.at(cards[i] % eDeck.SUITS));
		suffledcards.push_back(std::string(buffer));
	}

	std::copy(suffledcards.begin(), suffledcards.end(), std::ostream_iterator<std::string>( sstr, ","));
	auto s = sstr.str();
	output = s.erase(s.length() - 1, 1); // remove last comma
	return true;
}
