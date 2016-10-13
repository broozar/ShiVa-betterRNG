# ShiVa-betterRNG
RNG plugin for ShiVa 2.0 b7+
Licensed under Apache 2.0

## History of betterRNG
While trying to get better at C++11, I wanted to experiment with the new "random" header. During my studies, I came across the PCG library at http://www.pcg-random.org/ which a lot of the code in this plugin is based on.

# betterRNG API
- bOK = srng.init ( sAlgorithm ) <br>
initializes an RNG engine and seeds it. possible choices for sAlgorithm are:<br>
mt32, mt64, pcg32, pcg64<br>
Those are Mersenne Twister 32/64bit and PCG 32/64 bit.<br>

- sRandomInt = srng.getIntFromAlgorithm ( sLowerBoundInclusive, sUpperBoundInclusive, sAlgorithm )<br>
returns a random int string from a previously initialzed algorithm

- bOK = srng.reseed ( sSeedInt, sAlgorithm )<br>
reseeds an initialized algorithm with a seend integer string

- bOK = srng.reseedDevice ( sAlgorithm )<br>
reseeds an initialized algorithm using the random device (slower!)

## FUN functions
- sNum = srng.get32bitNum ( sAlgorithm )<br>
get a bogus pointer style number like 0xab25f3fa, algorithm must be initialized

- sNum = srng.get64bitNum ( sAlgorithm )<br>
get a bogus pointer style number like 0xab25f3fade436ab1, algorithm must be initialized

- sHeadsTails = srng.getCoinflips ( sRepetitions, sAlgorithm )<br>
flip a coin for H and T, algorithm must be initialized

- s1to6 = srng.getDicerolls ( sRepetitions, sAlgorithm )<br>
sRandomInt = srng.getIntFromAlgorithm ( sLowerBoundInclusive, sUpperBoundInclusive, sAlgorithm )

- sCardsCSV = srng.shuffle32cards ( sAlgorithm )<br>
returns a CSV string of a shuffled deck of 32 cards, algorithm must be initialized first

- sCardsCSV = srng.shuffle52cards ( sAlgorithm )<br>
returns a CSV string of a shuffled deck of 52 cards, algorithm must be initialized first

## Caveats
Try to stay within your architecture, and use sensible inputs. That means:
- call 32 bit RNG if you just need 32bit numbers, and only feed them with 32bit number strings
- be wary of generating 64bit numbers with 32bit generators, they are just a hack!
- no floats, only ints
- everything is unsigned, do not feed negative values into the algorithms

# Demo
A Demo project is included. Watch the output in the log module. It should read something like this:

BetterRNG: Using pcg64<br>
Random number 1-1000 1: 214<br>
Random number 1-1000 2: 693<br>
Random number 1-1000 3: 287<br>
Random number 1-1000 4: 417<br>
Random number 1-1000 5: 366<br>
Random number 1-1000 6: 135<br>
Random number 1-1000 7: 9<br>
Random number 1-1000 8: 1<br>
Random number 1-1000 9: 73<br>
Random number 1-1000 10: 684<br>
32bit numbers: 0x5e906e75 - 0x080c10d3 - 0xcb1c2e4c<br>
64bit numbers: 0xfef13335d7f1594d - 0x6ed60853dc1c3b46<br>
24 coinflips: THTTTTTTHHTHHHTHHHTTTTTH<br>
24 dicerolls: 514415643635362545254234<br>
32 Card shuffle: 9s,7d,7s,Ah,Ac,Th,8c,Js,9d,Kd,Jh,Qs,Jc,8s,7h,Tc,Qc,Kh,8h,Qd,As,7c,Ad,Ks,Jd,8d,9c,Td,9h,Ts,Qh,Kc<br>
52 Card shuffle: Jd,5s,9c,3s,Kd,2h,4d,4s,6d,5d,8h,Qh,Ks,Js,Qs,2d,6c,8s,9d,Ts,8d,Kc,Jc,5h,Qc,6s,Th,As,6h,7s,3d,Ad,7c,Qd,Tc,3c,7h,8c,Ac,2s,4c,9s,3h,Kh,2c,7d,Jh,4h,Td,Ah,5c,9h

# Precomiled libraries
Ready-to-use Libraries are included for
- Windows 32/64
- Windows RT arm/32/64
- Linux 32/64

## Linux caveats
The card shuffling algorithm currently produces a buffer overflow on linux, but not on any other platform. Can you find the mistake? Please let me know!
