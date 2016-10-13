--------------------------------------------------------------------------------
--  Handler.......... : onInit
--  Author........... :
--  Description...... :
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
function rngDemo.onInit (  )
--------------------------------------------------------------------------------

    -- limiting framerate
    application.setOption ( application.kOptionSwapInterval, 1 )

    -- actual demo code
    local hU = this.getUser ( )

    if user.getAIVariable ( hU, "rngInit", "_isInitialized" ) then
        local algo = user.getAIVariable ( hU, "rngInit", "algorithm" )

        for i=1, 10 do
            log.message ( "Random number 1-1000 #"..i..": "..srng.getIntFromAlgorithm ( 1, 1000, algo ) )
        end

        -- a bit of fun
        log.message ( "32bit numbers: "..srng.get32bitNum ( algo ).." - "..srng.get32bitNum ( algo ).." - "..srng.get32bitNum ( algo ) )
        log.message ( "64bit numbers: "..srng.get64bitNum ( algo ).." - "..srng.get64bitNum ( algo ) )
        log.message ( "24 coinflips: "..srng.getCoinflips ( "24", algo ) )
        log.message ( "24 dicerolls: "..srng.getDicerolls ( "24", algo ) )
        log.message ( "32 Card shuffle: "..srng.shuffle32cards ( algo ) )
        log.message ( "52 Card shuffle: "..srng.shuffle52cards ( algo ) )

    else
        log.warning ( "Not sure if RNG is initialized!" )
    end

--------------------------------------------------------------------------------
end
--------------------------------------------------------------------------------
