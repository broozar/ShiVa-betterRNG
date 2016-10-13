//-----------------------------------------------------------------------------
#ifndef __srng_h__
#define __srng_h__
//-----------------------------------------------------------------------------
#include "S3DXAIPackage.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Package declaration
//-----------------------------------------------------------------------------
class srngPackage : public S3DX::AIPackage
{
public :

    //-------------------------------------------------------------------------
    //  Constructor / Destructor
	//-------------------------------------------------------------------------

                                srngPackage         ( ) ;
                               ~srngPackage         ( ) ;

	//-------------------------------------------------------------------------
    //  Accessors
	//-------------------------------------------------------------------------

    const   char               *GetName             ( ) const ;
            S3DX::uint32        GetFunctionCount    ( ) const ;
            S3DX::uint32        GetConstantCount    ( ) const ;
    const   S3DX::AIFunction   *GetFunctionAt       ( S3DX::uint32 _iIndex ) const ;
    const   S3DX::AIConstant   *GetConstantAt       ( S3DX::uint32 _iIndex ) const ;

} ;

//-----------------------------------------------------------------------------
// Package API declaration
//-----------------------------------------------------------------------------
class srngAPI
{
public :

    //-------------------------------------------------------------------------
    //  API Constructor
	//-------------------------------------------------------------------------
                                srngAPI       ( ) 
                                {
                                    pfn_srng_shuffle52cards = NULL ;
                                    pfn_srng_shuffle32cards = NULL ;
                                    pfn_srng_get64bitNum = NULL ;
                                    pfn_srng_get32bitNum = NULL ;
                                    pfn_srng_getDicerolls = NULL ;
                                    pfn_srng_getCoinflips = NULL ;
                                    pfn_srng_getIntFromAlgorithm = NULL ;
                                    pfn_srng_getIntFromDevice = NULL ;
                                    pfn_srng_reseedDevice = NULL ;
                                    pfn_srng_reseed = NULL ;
                                    pfn_srng_init = NULL ;

                                }

	//-------------------------------------------------------------------------
	//  API Callbacks 
	//-------------------------------------------------------------------------

    S3DX::AICallback    pfn_srng_shuffle52cards ;
    S3DX::AICallback    pfn_srng_shuffle32cards ;
    S3DX::AICallback    pfn_srng_get64bitNum ;
    S3DX::AICallback    pfn_srng_get32bitNum ;
    S3DX::AICallback    pfn_srng_getDicerolls ;
    S3DX::AICallback    pfn_srng_getCoinflips ;
    S3DX::AICallback    pfn_srng_getIntFromAlgorithm ;
    S3DX::AICallback    pfn_srng_getIntFromDevice ;
    S3DX::AICallback    pfn_srng_reseedDevice ;
    S3DX::AICallback    pfn_srng_reseed ;
    S3DX::AICallback    pfn_srng_init ;

	//-------------------------------------------------------------------------
	//  API Functions 
	//-------------------------------------------------------------------------

    inline S3DX::AIVariable     shuffle52cards ( const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_01( sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_shuffle52cards ) pfn_srng_shuffle52cards ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     shuffle32cards ( const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_01( sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_shuffle32cards ) pfn_srng_shuffle32cards ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     get64bitNum ( const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_01( sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_get64bitNum ) pfn_srng_get64bitNum ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     get32bitNum ( const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_01( sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_get32bitNum ) pfn_srng_get32bitNum ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     getDicerolls ( const S3DX::AIVariable& sRepetitions, const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_02( sRepetitions, sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_getDicerolls ) pfn_srng_getDicerolls ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     getCoinflips ( const S3DX::AIVariable& sRepetitions, const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_02( sRepetitions, sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_getCoinflips ) pfn_srng_getCoinflips ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     getIntFromAlgorithm ( const S3DX::AIVariable& sLowerBoundInclusive, const S3DX::AIVariable& sUpperBoundInclusive, const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_03( sLowerBoundInclusive, sUpperBoundInclusive, sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_getIntFromAlgorithm ) pfn_srng_getIntFromAlgorithm ( 3, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     getIntFromDevice ( const S3DX::AIVariable& sLowerBoundInclusive, const S3DX::AIVariable& sUpperBoundInclusive ) { S3DX_DECLARE_VIN_02( sLowerBoundInclusive, sUpperBoundInclusive ) ; S3DX::AIVariable vOut ; if ( pfn_srng_getIntFromDevice ) pfn_srng_getIntFromDevice ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     reseedDevice ( const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_01( sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_reseedDevice ) pfn_srng_reseedDevice ( 1, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     reseed ( const S3DX::AIVariable& sSeedInt, const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_02( sSeedInt, sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_reseed ) pfn_srng_reseed ( 2, vIn, &vOut ); return vOut ; }
    inline S3DX::AIVariable     init ( const S3DX::AIVariable& sAlgorithm ) { S3DX_DECLARE_VIN_01( sAlgorithm ) ; S3DX::AIVariable vOut ; if ( pfn_srng_init ) pfn_srng_init ( 1, vIn, &vOut ); return vOut ; }

 	//-------------------------------------------------------------------------
	//  API Constants 
	//-------------------------------------------------------------------------


} ;

extern srngAPI srng;

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
