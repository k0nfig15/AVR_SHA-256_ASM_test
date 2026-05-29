extern "C" {
  uint32_t RotateASM(uint32_t num, uint8_t n);
  uint32_t RotateASM0Offset(uint32_t num, uint8_t n, uint8_t otherN);
  uint32_t RotateASM8Offset(uint32_t num, uint8_t n, uint8_t otherN);
  uint32_t RotateASM16Offset(uint32_t num, uint8_t n, uint8_t otherN);
  uint32_t RotateASM24Offset(uint32_t num, uint8_t n, uint8_t otherN);
}

#define RotateASM0(value, n) RotateASM0Offset((value), ((n) % 8), ((32 - (n)) % 8))
#define RotateASM8(value, n) RotateASM8Offset((value), ((n) % 8), ((32 - (n)) % 8))
#define RotateASM16(value, n) RotateASM16Offset((value), ((n) % 8), ((32 - (n)) % 8))
#define RotateASM24(value, n) RotateASM24Offset((value), ((n) % 8), ((32 - (n)) % 8))

/*#define RotateASM0(value, n) RotateASM((value), (n))
#define RotateASM8(value, n) RotateASM((value), (n))
#define RotateASM16(value, n) RotateASM((value), (n))
#define RotateASM24(value, n) RotateASM((value), (n))*/


#define letA letters[0]
#define letB letters[1]
#define letC letters[2]
#define letD letters[3]
#define letE letters[4]
#define letF letters[5]
#define letG letters[6]
#define letH letters[7]


#define SHOW_OUTPUT false
#define SHOW_OUTPUTALL false
#define SHOW_TIMERS true
#define ONLY_ONE true
#define ONLY_ONE_WAIT_TIME 500000


uint32_t message[16];

const uint32_t h[8] = {
  0x6a09e667,
  0xbb67ae85,
  0x3c6ef372,
  0xa54ff53a,
  0x510e527f,
  0x9b05688c,
  0x1f83d9ab,
  0x5be0cd19
};


const uint32_t k[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t w[64];

uint32_t letters[8];

uint32_t intermediateHash[8];


void Copy(uint32_t* src, uint32_t* dst, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    *dst++ = *src++;
  }
}



uint32_t Rotate(uint32_t num, uint8_t n) {
  return (num >> n) | (num << (32 - n));
}


void ClearMessage() {
  for (int i = 0; i < 16; i++) message[i] = 0;
}


void PrintTimer(unsigned long us, char* mes = " us", bool newLine = true) {
  Serial.print(us);
  Serial.print(" us ");
  Serial.print(mes);
  if (newLine) Serial.println('\t');
}


void PrintFullBits(uint32_t mes, bool newLine = true) {
  for (uint32_t mask = 0x80000000UL; mask != 0; mask >>= 1) {
    if ((mes & mask) == 0) Serial.print(0, BIN);
    else {
      Serial.print(mes, BIN);
      break;
    }
  }
  if (newLine) Serial.println('\t');
}


void PrintFullHex(uint32_t mes, bool newLine = true) {
  for (uint32_t mask = 0xF0000000UL; mask != 0; mask >>= 4) {
    if ((mes & mask) == 0) Serial.print(0, BIN);
    else {
      Serial.println(mes, HEX);
      return;
    }
  }
  if (newLine) Serial.println('\t');
}


void PrintMessage(char* mes = {}) {
  Serial.println(mes);
  for (int i = 0; i < 16; i++) {
    Serial.print(i, DEC);
    Serial.print("   -\t0b");
    PrintFullBits(message[i]);
    //Serial.println(message[i], BIN);
  }
  Serial.println('\t');
  Serial.println('\t');
}


void Print(uint32_t* value, int len, char* mes = {}) {
  Serial.println(mes);
  for (int i = 0; i < len; i++) {
    Serial.print(i, DEC);
    Serial.print("   -\t0x");
    PrintFullHex(value[i]);
    //Serial.println(value[i], HEX);
  }
  Serial.println('\t');
}


void PreprocesMessage(char* input, const int size) {
  uint32_t temp;
  uint8_t i = 0;
  while (i < size && i < 56) {
    temp = input[i];
    message[i >> 2] |= temp << (24 - (i % 4) * 8);
    i++;
  }
  temp = 0x80;
  message[i >> 2] |= temp << (24 - (i % 4) * 8);
  message[15] = (uint32_t)size * 8;
}

// sigma0 = S[7](x) XOR S[18](x) XOR R[3](x)
// sigma1 = S[17](x) XOR S[19](x) XOR R[10](x)
//
// S = right rotate
// R = right shift

uint32_t Sigma0(uint32_t value) {
  return RotateASM0(value, 7 /*7*/) ^ RotateASM16(value, 2 /*18*/) ^ (value >> 3);
}

uint32_t Sigma1(uint32_t value) {
  return RotateASM16(value, 1 /*17*/) ^ RotateASM16(value, 3 /*19*/) ^ (value >> 10);
}

uint32_t GetW(uint8_t index) {
  return Sigma1(w[index - 2]) + w[index - 7] + Sigma0(w[index - 15]) + w[index - 16];
}

void CalculateW() {
  Copy(message, w, 16);
  for (int i = 16; i < 64; i++) {
    w[i] = GetW(i);
  }
}

void Initialize() {
  Copy(h, letters, 8);
}



// large sigma0 (x) = S[2](x) XOR S[13](x) XOR S[22](x)
// large sigma1 (x) = S[6](x) XOR S[11](x) XOR S[25](x)
//
// S = right rotate
// R = right shift

// Ch (x, y, z) = (x AND y) XOR (NOT x AND z)
//
// Maj = (x AND y) XOR (x AND z) XOR (y AND z)

uint32_t Ch(const uint32_t x, const uint32_t y, const uint32_t z) {
  return (x & y) ^ ((~x) & z);
}

uint32_t Maj(const uint32_t x, const uint32_t y, const uint32_t z) {
  return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t LargeSigma0(const uint32_t value) {
  return RotateASM0(value, 2 /*2*/) ^ RotateASM8(value, 5 /*13*/) ^ RotateASM16(value, 6 /*22*/);
}

uint32_t LargeSigma1(const uint32_t value) {
  return RotateASM0(value, 6 /*6*/) ^ RotateASM8(value, 3 /*11*/) ^ RotateASM24(value, 1 /*25*/);
}


void MainLoop() {
  uint32_t t1;
  uint32_t t2;

  for (int i = 0; i < 64; i++) {
    t1 = letH + LargeSigma1(letE) + Ch(letE, letF, letG) + k[i] + w[i];
    t2 = LargeSigma0(letA) + Maj(letA, letB, letC);

    letH = letG;       // h <- g
    letG = letF;       // g <- f
    letF = letE;       // f <- e
    letE = letD + t1;  // e <- d + T1
    letD = letC;       // d <- c
    letC = letB;       // c <- b
    letB = letA;       // b <- a
    letA = t1 + t2;    // a <- T1 + T2
  }
}


uint32_t* InterHash() {
  for (int i = 0; i < 8; i++) {
    intermediateHash[i] = letters[i] + h[i];
  }
  return intermediateHash;
}

#if SHOW_TIMERS
unsigned long startPreProc, finishPreProc;
unsigned long startCalW, finishCalW;
unsigned long startInit, finishInit;
unsigned long startMain, finishMain;
unsigned long startInter, finishInter;

uint32_t* SHA(char* input, const int size) {
  startPreProc = micros();
  PreprocesMessage(input, size);
  finishPreProc = micros() - startPreProc;

  startCalW = micros();
  CalculateW();
  finishCalW = micros() - startCalW;

  startInit = micros();
  Initialize();
  finishInit = micros() - startInit;

  startMain = micros();
  MainLoop();
  finishMain = micros() - startMain;

  startInter = micros();
  uint32_t* pointer = InterHash();
  finishInter = micros() - startInter;

  return pointer;
}
#endif

#if !SHOW_TIMERS
uint32_t* SHA(char* input, const int size) {

  PreprocesMessage(input, size);
  CalculateW();
  Initialize();
  MainLoop();
  return InterHash();
}
#endif






bool printOut = false;
uint8_t hashCount = 0;

ISR(TIMER1_COMPA_vect) {
  printOut = true;
}



void InitTimer() {
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;  // set entire TCCR1A register to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1 = 0;   // initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;  // = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();  //allow interrupts
}



void setup() {
  Serial.begin(1200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println('\t');
#if !ONLY_ONE
  InitTimer();
#endif
}


const char inputMessage[] = "abc";
uint32_t finalHash[8];

#if SHOW_TIMERS
unsigned long startTotal, finishTotal;
#endif



void loop() {
  ClearMessage();

#if SHOW_TIMERS
  startTotal = micros();
#endif
  digitalWrite(LED_BUILTIN, HIGH);


  uint32_t* finalMessagePointer = SHA(inputMessage, 3);
  Copy(finalMessagePointer, finalHash, 8);
  hashCount++;


  digitalWrite(LED_BUILTIN, LOW);
#if SHOW_TIMERS
  finishTotal = micros() - startTotal;
  PrintTimer(finishPreProc, "- Preproces message");
  PrintTimer(finishCalW, "- Calculate W");
  PrintTimer(finishInit, "- Initialize");
  PrintTimer(finishMain, "- Main loop");
  PrintTimer(finishInter, "- Inter hash");
  PrintTimer(finishTotal, "- total time");
#endif

#if !SHOW_OUTPUT && !SHOW_OUTPUTALL && SHOW_TIMERS
  Serial.println("-------------------------------");
#elif SHOW_OUTPUT && !SHOW_OUTPUTALL && SHOW_TIMERS
  Serial.println('\t');
  Print(finalHash, 8);
  Serial.println('\t');
  Serial.println("-------------------------------");
#endif

#if SHOW_OUTPUTALL
  Serial.println('\t');
  Serial.print("Input message: ");
  Serial.println(inputMessage);
  Print(finalHash, 8);
  Serial.println('\t');
  Serial.println("-------------------------------");
#endif



  //Print(finalHash, 8);
  if (printOut) {
    printOut = false;
    Serial.print(hashCount, DEC);
    Serial.println(" - Hash/s");
    hashCount = 0;

#if SHOW_OUTPUT
    Serial.println('\t');
    Print(finalHash, 8);
    Serial.println('\t');
    Serial.println("-------------------------------");
#endif
  }

#if ONLY_ONE
  delay(ONLY_ONE_WAIT_TIME);
#endif
}
