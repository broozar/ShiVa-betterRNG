--------------------------------------------------------------------------------
--  Handler.......... : onInit
--  Author........... :
--  Description...... :
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
function rngInit.onInit (  )
--------------------------------------------------------------------------------

    -- main INIT for RNG engine creation
    -- possible ALGO strings are stored in this.supportedAlgorithms ( ):

    -- mt32
    -- mt64
    -- pcg32
    -- pcg64


    local algo = this.algorithm ( )

    if algo ~= "default" then
        if srng.init ( algo ) then
            log.message ( "BetterRNG: Using "..algo )
            this._isInitialized ( true )
        else
            log.warning ( "BetterRNG: Init of "..algo.. " FAILED" )
        end

    else
        if srng.init ( "pcg32" ) then
            log.message ( "BetterRNG: Using PCG32" )
            this._isInitialized ( true )
        else
            log.warning ( "BetterRNG: Init FAILED" )
        end
    end

--------------------------------------------------------------------------------
end
--------------------------------------------------------------------------------
