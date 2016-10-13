//-----------------------------------------------------------------------------
#include "PrecompiledHeader.h"
#include "srng.h"
//-----------------------------------------------------------------------------
#include <memory>// STL
#include <random>
#include <string>
#include <vector>
#include <sstream> // :( replace?
#include <iterator>
#include <string>

#include "C_srng.h"

#ifdef S3DX_DLL
	srngAPI srng ;
#endif


// C++11: put class on heap
std::unique_ptr<C_srng> RNG(new C_srng);

//-----------------------------------------------------------------------------
//  Callbacks
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

int Callback_srng_init ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.init" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOK ;

	bOK.SetBooleanValue(RNG->init_rng(sAlgorithm.GetStringValue()));

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOK ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_reseed ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.reseed" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sSeedInt = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOK ;

	bOK.SetBooleanValue(RNG->reseed_int(std::stoi(sSeedInt.GetStringValue()), sAlgorithm.GetStringValue()));

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOK ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_reseedDevice ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.reseedDevice" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable bOK ;

	bOK.SetBooleanValue(RNG->reseed_device(sAlgorithm.GetStringValue()));

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = bOK ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_getIntFromAlgorithm ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.getIntFromAlgorithm" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sLowerBoundInclusive = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sUpperBoundInclusive = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sRandomInt ;

	static std::string out = RNG->error;
	RNG->dist_int_s(std::stoll(sLowerBoundInclusive.GetStringValue()), std::stoll(sUpperBoundInclusive.GetStringValue()), out, sAlgorithm.GetStringValue());
	sRandomInt.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sRandomInt ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_getCoinflips ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.getCoinflips" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sRepetitions = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sHeadsTails ;

	auto rep = sRepetitions.GetStringValue();
	static std::string out = RNG->error;
	RNG->coinflips(std::stoi(rep), out, sAlgorithm.GetStringValue());
	sHeadsTails.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sHeadsTails ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_getDicerolls ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.getDicerolls" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sRepetitions = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable s1to6 ;

	auto rep = sRepetitions.GetStringValue();
	static std::string out = RNG->error;
	RNG->dicerolls(std::stoi(rep), out, sAlgorithm.GetStringValue());
	s1to6.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = s1to6 ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_get32bitNum ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.get32bitNum" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sNum ;

	static std::string out = RNG->error;
	RNG->bitnum32(out, sAlgorithm.GetStringValue());
	sNum.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sNum ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_get64bitNum ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.get64bitNum" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sNum ;

	static std::string out = RNG->error;
	RNG->bitnum64(out, sAlgorithm.GetStringValue());
	sNum.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sNum ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_shuffle32cards ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.shuffle32cards" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sCardsCSV ;

	static std::string out = RNG->error;
	RNG->cardshuffle(true, out, sAlgorithm.GetStringValue());
	sCardsCSV.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sCardsCSV ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------

int Callback_srng_shuffle52cards ( int _iInCount, const S3DX::AIVariable *_pIn, S3DX::AIVariable *_pOut )
{
    S3DX_API_PROFILING_START( "srng.shuffle52cards" ) ;

    // Input Parameters
    int iInputCount = 0 ;
    S3DX::AIVariable sAlgorithm = ( iInputCount < _iInCount ) ? _pIn[iInputCount++] : S3DX::AIVariable ( ) ;

    // Output Parameters 
    S3DX::AIVariable sCardsCSV ;

	static std::string out = RNG->error;
	RNG->cardshuffle(false, out, sAlgorithm.GetStringValue());
	sCardsCSV.SetStringValue(out.c_str());

    // Return output Parameters 
    int iReturnCount = 0 ;
    _pOut[iReturnCount++] = sCardsCSV ;

    S3DX_API_PROFILING_STOP( ) ;
    return iReturnCount;
}

//-----------------------------------------------------------------------------
//  Constructor / Destructor
//-----------------------------------------------------------------------------

srngPackage::srngPackage ( )
{
#ifdef S3DX_DLL
	
    srng.pfn_srng_init = Callback_srng_init ;
    srng.pfn_srng_reseed = Callback_srng_reseed ;
    srng.pfn_srng_reseedDevice = Callback_srng_reseedDevice ;
    srng.pfn_srng_getIntFromAlgorithm = Callback_srng_getIntFromAlgorithm ;
    srng.pfn_srng_getCoinflips = Callback_srng_getCoinflips ;
    srng.pfn_srng_getDicerolls = Callback_srng_getDicerolls ;
    srng.pfn_srng_get32bitNum = Callback_srng_get32bitNum ;
    srng.pfn_srng_get64bitNum = Callback_srng_get64bitNum ;
    srng.pfn_srng_shuffle32cards = Callback_srng_shuffle32cards ;
    srng.pfn_srng_shuffle52cards = Callback_srng_shuffle52cards ;

#endif
}

//-----------------------------------------------------------------------------

srngPackage::~srngPackage ( )
{

}

//-----------------------------------------------------------------------------
//  Functions table
//-----------------------------------------------------------------------------

static S3DX::AIFunction aMyFunctions [ ] =
{
    { "init", Callback_srng_init, "bOK", "sAlgorithm", "initializes an RNG engine and seeds it", 0 },
    { "reseed", Callback_srng_reseed, "bOK", "sSeedInt, sAlgorithm", "reseeds an initialized algorithm", 0 },
    { "reseedDevice", Callback_srng_reseedDevice, "bOK", "sAlgorithm", "reseeds an initialized algorithm using the random device (slower!)", 0 },
    { "getIntFromAlgorithm", Callback_srng_getIntFromAlgorithm, "sRandomInt", "sLowerBoundInclusive, sUpperBoundInclusive, sAlgorithm", "returns a random int string from a previously initialzed algorithm", 0 },
    { "getCoinflips", Callback_srng_getCoinflips, "sHeadsTails", "sRepetitions, sAlgorithm", "flip a coin for H and T, algorithm must be initialized", 0 },
    { "getDicerolls", Callback_srng_getDicerolls, "s1to6", "sRepetitions, sAlgorithm", "roll a 6 sided die, algorithm must be initialized", 0 },
    { "get32bitNum", Callback_srng_get32bitNum, "sNum", "sAlgorithm", "get a bogus pointer style number like 0xab25f3fa, algorithm must be initialized", 0 },
    { "get64bitNum", Callback_srng_get64bitNum, "sNum", "sAlgorithm", "get a bogus pointer style number like 0xab25f3fade436ab1, algorithm must be initialized", 0 },
    { "shuffle32cards", Callback_srng_shuffle32cards, "sCardsCSV", "sAlgorithm", "returns a CSV string of a shuffled deck of 32 cards, algorithm must be initialized first", 0 },
    { "shuffle52cards", Callback_srng_shuffle52cards, "sCardsCSV", "sAlgorithm", "returns a CSV string of a shuffled deck of 52 cards, algorithm must be initialized first", 0 }
    //{ NULL, NULL, NULL, NULL, NULL, 0}
} ;

//-----------------------------------------------------------------------------
//  Constants table
//-----------------------------------------------------------------------------

static S3DX::AIConstant aMyConstants [ ] =
{
	{ NULL, 0, NULL, 0}
} ;

//-----------------------------------------------------------------------------
//  Accessors
//-----------------------------------------------------------------------------

const char *srngPackage::GetName ( ) const
{
    return "srng" ;
}

//-----------------------------------------------------------------------------

S3DX::uint32 srngPackage::GetFunctionCount ( ) const
{
	if ( aMyFunctions[0].pName == NULL )
	{
		return 0 ;
	}
	else
	{
		return sizeof( aMyFunctions ) / sizeof( S3DX::AIFunction ) ;
	}
}

//-----------------------------------------------------------------------------

S3DX::uint32 srngPackage::GetConstantCount ( ) const
{
	if ( aMyConstants[0].pName == NULL )
	{
		return 0 ;
	}
	else
	{
		return sizeof( aMyConstants ) / sizeof( S3DX::AIConstant ) ;
	}
}

//-----------------------------------------------------------------------------

const S3DX::AIFunction *srngPackage::GetFunctionAt ( S3DX::uint32 _iIndex ) const
{
    return &aMyFunctions[ _iIndex ] ;
}

//-----------------------------------------------------------------------------

const S3DX::AIConstant *srngPackage::GetConstantAt ( S3DX::uint32 _iIndex ) const
{
    return &aMyConstants[ _iIndex ] ;
}
