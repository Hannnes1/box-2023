#define a3f 208
#define b3f 233
#define b3 247
#define c4 261
#define c4s 277
#define e4f 311
#define f4 349
#define a4 440
#define a4f 415
#define b4f 466
#define b4 493
#define c5 523
#define c5s 554
#define e5f 622
#define f5 698
#define f5s 740
#define a5 440
#define a5f 831
#define g5 784
#define g5s 831
#define g5f 740
#define d5s 622
#define e5 659
#define c6 1047
#define rest -1

const float beatSeparation = 0.3;

int rickRollMelody[] =
    {b4f, b4f, a4f, a4f,
     f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
     c5s, c5s, c5s, c5s,
     c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
     b4f, b4f, a4f, a4f,
     f5, f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
     c5s, c5s, c5s, c5s,
     c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest};

int rickRollRhythmn[] =
    {1, 1, 1, 1,
     3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
     1, 1, 1, 1,
     3, 3, 3, 1, 2, 2, 2, 4, 8,
     1, 1, 1, 1,
     3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
     1, 1, 1, 1,
     3, 3, 3, 1, 2, 2, 2, 4, 8, 4};

// int zeldaMelody[] = {g5, f5s, d5s, a5, g5s, e5, g5s, c5};
int zeldaMelody[] = {g5, g5f, e5f, a4, a4f, e5, g5s, c6};

int zeldaRhythmn[] = {1, 1, 1, 1, 1, 1, 1, 1};

const int lightBreakpoint = 400;
const int heatBreakpoint = 10;
const int alcoholBreakpoint = 500;

// The code to the combination lock is 268.
char* lockCode[] = {"..---", "-....", "---.."};

const int dotDelay = 75;

bool lightSolved = false;
bool heatSolved = false;
bool alcoholSolved = false;

int initHeat = 0;

int solvedCount = 0;

void playRickRoll() {
    for (int i = 0; i < sizeof(rickRollMelody) / sizeof(int); i++) {
          int notelength = 100 * rickRollRhythmn[i];
          if (rickRollMelody[i] > 0) {
            tone(9, rickRollMelody[i], notelength);
          }

      // Delay for the current note + the separation between notes.
      delay(notelength + notelength * beatSeparation);
    }
}

void playZelda() {
    for (int i = 0; i < sizeof(zeldaMelody) / sizeof(int); i++) {
          int notelength = 150 * zeldaRhythmn[i];
          if (zeldaMelody[i] > 0) {
            tone(9, zeldaMelody[i], notelength);
          }

      // Delay for the current note + the separation between notes.
      delay(notelength + notelength * beatSeparation);
    }
}

void onSolved() {
    switch (++solvedCount) {
        case 1:
            digitalWrite(11, HIGH);
            break;
        case 2:
            digitalWrite(12, HIGH);
            break;
        case 3:
            digitalWrite(10, HIGH);
            break;
        default:
            break;
    }

    playZelda();
}

void flashSequence() {
    for (int i = 0; i < 3; i++) {
        int ii = 0;
        while (lockCode[i][ii] != NULL) {
            dotOrDash(lockCode[i][ii]);
            ii++;
        }

        // Delay between words.
        delay(dotDelay * 3);
    }

    delay(dotDelay * 6);
}

// Make the sound and light for a dot or dash.
void dotOrDash(char character) {
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);

    // Sound the buzzer.
    if (character == '.') {
        tone(9, 1000, dotDelay);
        delay(dotDelay);
    } else {
        tone(9, 1000, dotDelay * 3);
        delay(dotDelay * 3);
    }

    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);

    // Delay between characters.
    delay(dotDelay);
}

void setup() {
    Serial.begin(9600);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(12, OUTPUT);

    delay(2000);

    // playRickRoll();

    initHeat = analogRead(A2);
}

void loop() {
    if (solvedCount >= 3) {
        flashSequence();
        return;
    }

    int light = analogRead(A1);
    int heat = analogRead(A2);
    int alcohol = analogRead(A3);

    Serial.print("Light: ");
    Serial.print(light);
    Serial.print(" | Heat: ");
    Serial.print(heat - initHeat);
    Serial.print(" | Alcohol: ");
    Serial.println(alcohol);

    // Go through each sensor and check if they are solved.
    // They should only be checked once, to avoid running the
    // success logic more than once.
    if (!lightSolved && light > lightBreakpoint) {
        lightSolved = true;
        onSolved();
    }
    if (!heatSolved && (heat - initHeat) > heatBreakpoint) {
        heatSolved = true;
        onSolved();
    }
    if (!alcoholSolved && alcohol > alcoholBreakpoint) {
        alcoholSolved = true;
        onSolved();
    }
}