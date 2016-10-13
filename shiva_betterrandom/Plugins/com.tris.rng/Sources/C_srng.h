#pragma once

// STL
#include <random>
#include <string>
#include <vector>
#include <sstream> // :( replace?
#include <iterator>
#include <vector>
#include <numeric>

// PCG
#include <time.h>
#include "pcg_basic.h"



typedef struct {
	pcg32_random_t gen[2];
} pcg64_random_t;



class C_srng
{
private:
	// ALGORITHM POINTERS
	std::mt19937		*	_pmt32	= nullptr;
	std::mt19937_64		*	_pmt64	= nullptr;
	pcg32_random_t		*	_ppcg32	= nullptr;
	pcg64_random_t		*	_ppcg64	= nullptr;

	// RANDOM DEVICE
	uint32_t _rd32();
	uint64_t _rd64();

	char buffer[500];

	/*
	std::mt19937	_mt32;	std::mt19937	*	_pmt32;
	std::mt19937_64	_mt64;	std::mt19937_64	*	_pmt64;
	pcg32_random_t	_pcg32;	pcg32_random_t	*	_ppcg32;
	pcg64_random_t	_pcg64;	pcg64_random_t	*	_ppcg64;
	std::random_device rd;
	*/

	int dummyglobal1 = 1701;
	int dummyglobal2 = 4711;

	// DISTRIBUTIONS
	uint32_t _dist_mt32(const uint64_t && lower, const uint64_t && upper, std::mt19937		* mt);
	uint64_t _dist_mt64(const uint64_t && lower, const uint64_t && upper, std::mt19937_64	* mt);

	// RESEED
	bool _reseed_mt32(const uint64_t && seed, std::mt19937		* mt); // internal staic_cast to 32bit seed
	bool _reseed_mt64(const uint64_t && seed, std::mt19937_64	* mt);


public:

	// CONST DEST
	C_srng();
	~C_srng();

	// CONSTANTS
	const std::string error = "ERROR";

	// MANUAL ENGINE INIT
	bool init_rng(const char* algo);

	// GET
	//bool dist_int	(const uint64_t && lower, const uint64_t && upper, float & output,		const char * algo	);
	bool dist_int_s	(const uint64_t && lower, const uint64_t && upper, std::string & output, const char * algo	);

	// RESEED
	bool reseed_int		(const uint64_t && seed,	const char * algo);
	bool reseed_device	(							const char * algo);

	// FUN!
	bool coinflips	(const int && repetitions,	std::string & output, const char * algo);
	bool dicerolls	(const int && repetitions,	std::string & output, const char * algo);
	bool bitnum32	(							std::string & output, const char * algo);
	bool bitnum64	(							std::string & output, const char * algo);
	bool cardshuffle(bool && use32cards,		std::string & output, const char * algo);
	
};

