# AVR_SHA-256_ASM_test
SHA-256 on an AVR Processor with an Assembly Rotate Function.

Main parts of this code have to be set directly in the source code, which makes it very impractical, and the input value has to be between 0 and 447 bits (one block of data)(default value: "abc").

The main goal of this project was to try the SHA-256 algorithm on an AVR processor, specifically on my Arduino R3 (ATmega328P 8-bit CPU).

One thing I noticed was that the code was quite slow because of the "Rotate" function. This function was originally written in the main code, but after noticing how slow it was, I decided to try optimizing it in pure assembly.

The main optimization of the assembly code is that I don't have to move bits one at a time in each instruction, but instead can move entire bytes, which saves processing power.

Credits:  
	SHA-256 specification document: https://eips.ethereum.org/assets/eip-2680/sha256-384-512.pdf  
	YouTube video explaining the algorithm very well: https://www.youtube.com/watch?v=orIgy2MjqrA
