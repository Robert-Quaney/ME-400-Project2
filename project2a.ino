#include "lcdhelperv2.h"
#include "irhelper.h"
#include <ESP32Servo.h>
#include "httphelper.h"
#include <DabbleESP32.h>
#include <WiFi.h>

// both arrays for option 1 with 1439 elements, filled
uint16_t song[] = {659, 0, 349, 261, 0, 659, 622, 740, 0, 587, 0, 349, 0, 622, 740, 587, 0, 466, 587, 0, 622, 740, 523, 0, 349, 0, 261, 622, 740, 0, 523, 0, 466, 0, 622, 740, 311, 0, 466, 0, 391, 622, 740, 0, 466, 0, 659, 0, 622, 740, 349, 0, 261, 0, 659, 622, 740, 0, 587, 0, 349, 0, 622, 740, 587, 0, 466, 0, 587, 622, 740, 0, 523, 0, 349, 0, 622, 740, 261, 0, 293, 0, 311, 622, 740, 0, 329, 0, 293, 0, 622, 740, 293, 0, 329, 0, 349, 622, 740, 0, 523, 0, 587, 0, 622, 740, 523, 0, 587, 0, 523, 622, 740, 0, 466, 440, 0, 391, 622, 740, 349, 0, 261, 0, 293, 622, 740, 0, 311, 0, 329, 0, 622, 740, 293, 0, 293, 0, 329, 622, 740, 349, 0, 523, 0, 698, 622, 740, 0, 587, 0, 466, 0, 622, 740, 440, 0, 391, 349, 0, 622, 740, 261, 277, 0, 293, 0, 622, 740, 311, 0, 329, 0, 293, 622, 740, 0, 293, 0, 329, 0, 622, 740, 349, 0, 523, 0, 587, 622, 740, 0, 523, 0, 587, 0, 622, 740, 523, 0, 466, 0, 440, 622, 740, 0, 391, 349, 0, 523, 622, 740, 0, 698, 0, 587, 0, 622, 740, 523, 0, 466, 0, 440, 622, 740, 0, 466, 0, 493, 523, 622, 740, 0, 587, 0, 440, 0, 622, 740, 349, 0, 293, 0, 329, 622, 740, 0, 349, 0, 311, 0, 622, 740, 329, 0, 349, 0, 311, 622, 740, 0, 311, 0, 349, 369, 622, 740, 0, 554, 0, 622, 0, 622, 740, 554, 0, 622, 0, 554, 622, 740, 0, 493, 0, 466, 415, 622, 740, 0, 369, 0, 277, 0, 622, 740, 311, 0, 329, 0, 349, 622, 740, 0, 311, 0, 311, 0, 622, 740, 349, 0, 369, 0, 554, 622, 740, 0, 739, 0, 622, 0, 622, 740, 493, 466, 0, 415, 0, 622, 740, 369, 0, 415, 0, 391, 622, 740, 0, 369, 329, 0, 415, 622, 740, 493, 0, 622, 0, 739, 622, 740, 0, 622, 0, 493, 0, 622, 740, 415, 0, 329, 0, 415, 622, 740, 0, 493, 0, 622, 0, 622, 740, 739, 0, 622, 0, 369, 622, 740, 0, 415, 0, 440, 0, 622, 740, 415, 0, 369, 0, 293, 622, 740, 0, 277, 0, 293, 311, 622, 740, 0, 329, 0, 587, 0, 622, 740, 554, 0, 493, 0, 440, 622, 740, 415, 0, 329, 0, 329, 622, 740, 0, 369, 0, 415, 0, 622, 740, 349, 0, 440, 0, 523, 622, 740, 0, 659, 0, 783, 0, 622, 740, 659, 0, 523, 0, 440, 622, 740, 0, 349, 0, 440, 523, 622, 740, 0, 659, 0, 783, 0, 622, 740, 659, 0, 391, 0, 440, 622, 740, 0, 466, 0, 440, 0, 622, 740, 391, 0, 349, 0, 293, 622, 740, 311, 0, 329, 0, 349, 622, 740, 0, 349, 0, 311, 0, 622, 740, 349, 0, 311, 261, 0, 622, 740, 220, 0, 466, 0, 391, 622, 740, 0, 466, 0, 440, 466, 622, 740, 0, 440, 0, 466, 523, 622, 740, 0, 587, 0, 659, 0, 622, 740, 698, 0, 493, 493, 0, 622, 740, 523, 0, 554, 0, 523, 622, 740, 0, 587, 0, 466, 0, 622, 740, 587, 0, 587, 0, 523, 622, 740, 0, 587, 0, 622, 0, 622, 740, 587, 0, 659, 0, 523, 622, 740, 0, 659, 0, 659, 0, 622, 740, 587, 0, 659, 0, 698, 622, 740, 0, 440, 0, 440, 0, 622, 740, 391, 0, 391, 0, 391, 622, 740, 0, 349, 0, 349, 0, 622, 740, 329, 0, 329, 0, 329, 622, 740, 0, 523, 0, 523, 0, 622, 740, 466, 0, 466, 0, 466, 622, 740, 0, 415, 0, 415, 0, 622, 740, 369, 0, 369, 0, 369, 622, 740, 0, 261, 0, 277, 0, 622, 740, 293, 0, 277, 261, 0, 622, 740, 277, 0, 293, 0, 277, 622, 740, 0, 261, 0, 293, 0, 622, 740, 311, 329, 0, 349, 0, 622, 740, 369, 0, 391, 415, 0, 622, 740, 440, 0, 466, 0, 493, 622, 740, 523, 0, 587, 0, 622, 622, 740, 0, 659, 0, 587, 0, 622, 740, 587, 0, 659, 0, 698, 622, 740, 0, 523, 0, 587, 0, 622, 740, 523, 0, 587, 0, 523, 622, 740, 0, 466, 0, 440, 0, 622, 740, 391, 349, 0, 261, 0, 622, 740, 293, 0, 311, 0, 329, 622, 740, 0, 293, 0, 293, 0, 622, 740, 329, 0, 349, 0, 523, 622, 740, 0, 698, 0, 587, 0, 622, 740, 466, 0, 440, 391, 0, 622, 740, 349, 0, 261, 277, 0, 622, 740, 293, 0, 311, 0, 329, 622, 740, 0, 293, 0, 293, 0, 622, 740, 329, 349, 0, 523, 0, 622, 740, 587, 0, 523, 0, 587, 622, 740, 0, 523, 0, 466, 0, 622, 740, 440, 391, 0, 349, 0, 622, 740, 523, 0, 698, 0, 587, 622, 740, 0, 523, 0, 466, 0, 622, 740, 440, 0, 466, 0, 493, 622, 740, 523, 0, 587, 0, 440, 622, 740, 0, 349, 0, 293, 0, 622, 740, 329, 0, 349, 0, 311, 622, 740, 0, 329, 0, 349, 0, 622, 740, 311, 0, 311, 0, 349, 622, 740, 369, 0, 554, 0, 622, 622, 740, 0, 554, 0, 622, 0, 622, 740, 554, 0, 493, 0, 466, 622, 740, 415, 0, 369, 0, 277, 622, 740, 0, 311, 0, 329, 0, 622, 740, 349, 0, 311, 0, 311, 622, 740, 0, 349, 0, 369, 0, 622, 740, 554, 0, 739, 0, 622, 622, 740, 0, 493, 466, 0, 415, 622, 740, 0, 369, 0, 415, 0, 622, 740, 391, 0, 369, 329, 0, 622, 740, 415, 493, 0, 622, 0, 622, 740, 739, 0, 622, 0, 493, 622, 740, 0, 415, 0, 329, 0, 622, 740, 415, 493, 0, 622, 0, 622, 740, 739, 0, 622, 0, 369, 622, 740, 0, 415, 0, 440, 0, 622, 740, 415, 0, 369, 0, 293, 622, 740, 0, 277, 293, 0, 311, 622, 740, 0, 329, 0, 587, 0, 622, 740, 554, 0, 493, 0, 440, 622, 740, 415, 0, 329, 0, 329, 622, 740, 0, 369, 0, 415, 0, 622, 740, 349, 0, 440, 0, 523, 622, 740, 0, 659, 0, 783, 0, 622, 740, 659, 0, 523, 0, 440, 622, 740, 0, 349, 0, 440, 523, 622, 740, 0, 659, 0, 783, 0, 622, 740, 659, 0, 391, 0, 440, 622, 740, 0, 466, 0, 440, 0, 622, 740, 391, 0, 349, 0, 293, 622, 740, 311, 0, 329, 349, 0, 622, 740, 349, 0, 311, 0, 349, 622, 740, 311, 0, 261, 0, 220, 622, 740, 0, 466, 0, 391, 0, 622, 740, 466, 0, 440, 466, 0, 622, 740, 440, 0, 466, 523, 0, 622, 740, 587, 0, 659, 0, 698, 622, 740, 0, 493, 493, 0, 523, 622, 740, 0, 554, 0, 523, 0, 622, 740, 587, 0, 466, 0, 587, 622, 740, 0, 587, 0, 523, 0, 622, 740, 587, 0, 622, 0, 587, 622, 740, 0, 659, 0, 523, 0, 622, 740, 659, 0, 659, 0, 587, 622, 740, 0, 659, 0, 698, 0, 622, 740, 440, 0, 440, 0, 391, 622, 740, 0, 391, 0, 391, 0, 622, 740, 349, 0, 349, 0, 329, 622, 740, 0, 329, 0, 329, 0, 622, 740, 523, 0, 523, 0, 466, 622, 740, 0, 466, 0, 466, 0, 622, 740, 415, 0, 415, 0, 369, 622, 740, 0, 369, 0, 369, 0, 622, 740, 261, 0, 277, 0, 293, 622, 740, 0, 277, 261, 0, 277, 622, 740, 0, 293, 0, 277, 0, 622, 740, 261, 0, 293, 311, 0, 622, 740, 329, 0, 349, 0, 369, 622, 740, 391, 0, 415, 0, 440, 622, 740, 0, 466, 493, 0, 523, 622, 740, 0, 587, 0, 622, 0, 622, 740, 659, 0, 587, 0, 587, 622, 740, 0, 659, 0, 698, 0, 622, 740, 523, 0, 587, 0, 523, 622, 740, 0, 587, 0, 523, 0, 622, 740, 466, 0, 440, 0, 391, 622, 740, 349, 0, 523, 0, 587, 622, 740, 0, 622, 0, 659, 0, 622, 740, 587, 0, 587, 0, 659, 622, 740, 0, 698, 0, 523, 0, 622, 740, 698, 0, 587, 0, 466, 622, 740, 0, 440, 391, 0, 349, 622, 740};
uint16_t note[] = {113, 114, 113, 113, 115, 113, 248, 496, 114, 113, 114, 113, 115, 248, 496, 113, 114, 113, 113, 115, 248, 496, 113, 114, 113, 1, 113, 248, 496, 114, 113, 114, 113, 114, 248, 496, 113, 115, 113, 114, 113, 248, 496, 1, 113, 114, 113, 114, 248, 496, 113, 1, 113, 114, 113, 248, 496, 114, 113, 115, 113, 114, 248, 496, 113, 114, 113, 1, 113, 248, 496, 114, 113, 114, 113, 1, 248, 496, 113, 1364, 151, 77, 151, 248, 496, 76, 226, 115, 169, 58, 248, 496, 151, 76, 113, 1, 151, 248, 496, 76, 151, 76, 151, 77, 248, 496, 151, 76, 151, 76, 151, 248, 496, 77, 113, 113, 1, 113, 248, 496, 151, 190, 151, 77, 151, 248, 496, 76, 151, 76, 226, 115, 248, 496, 169, 58, 151, 77, 113, 248, 496, 151, 77, 151, 76, 151, 248, 496, 76, 151, 76, 113, 1, 248, 496, 113, 1, 113, 226, 570, 248, 496, 113, 113, 1, 151, 76, 248, 496, 151, 77, 226, 114, 169, 248, 496, 59, 151, 76, 113, 1, 248, 496, 151, 76, 151, 76, 151, 248, 496, 76, 151, 77, 151, 76, 248, 496, 151, 76, 113, 1, 113, 248, 496, 1, 113, 151, 190, 151, 248, 496, 76, 151, 77, 151, 76, 248, 496, 151, 76, 151, 76, 113, 248, 496, 1, 113, 1, 113, 340, 248, 496, 1, 151, 77, 151, 76, 248, 496, 151, 76, 113, 1, 151, 248, 496, 76, 188, 835, 151, 76, 248, 496, 151, 76, 226, 115, 169, 248, 496, 58, 151, 77, 113, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496, 151, 76, 151, 77, 151, 248, 496, 76, 113, 1, 113, 113, 248, 496, 1, 151, 190, 151, 76, 248, 496, 151, 76, 151, 77, 226, 248, 496, 115, 169, 58, 151, 76, 248, 496, 113, 1, 151, 76, 151, 248, 496, 76, 151, 77, 151, 76, 248, 496, 113, 113, 1, 113, 1, 248, 496, 226, 569, 75, 1, 75, 248, 496, 1, 75, 151, 77, 113, 248, 496, 151, 77, 151, 76, 188, 248, 496, 39, 151, 76, 151, 190, 248, 496, 151, 77, 151, 76, 113, 248, 496, 1, 151, 76, 151, 76, 248, 496, 188, 39, 151, 190, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496, 151, 76, 151, 77, 151, 248, 496, 76, 113, 1, 113, 113, 248, 496, 1, 151, 303, 340, 1, 248, 496, 151, 77, 151, 76, 113, 248, 496, 188, 40, 151, 76, 113, 248, 496, 1, 151, 76, 151, 76, 248, 496, 151, 76, 113, 1, 151, 248, 496, 76, 151, 77, 188, 39, 248, 496, 151, 76, 151, 190, 151, 248, 496, 76, 151, 77, 113, 151, 248, 496, 77, 151, 76, 188, 39, 248, 496, 151, 190, 151, 76, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496, 151, 76, 151, 77, 113, 248, 496, 113, 1, 113, 1, 151, 248, 496, 417, 151, 76, 75, 1, 248, 496, 75, 1, 75, 113, 1, 248, 496, 113, 114, 226, 1, 113, 248, 496, 1, 113, 1, 113, 113, 248, 496, 1, 113, 1, 56, 56, 248, 496, 1, 56, 1, 56, 1, 248, 496, 1817, 456, 454, 302, 1, 248, 496, 302, 1, 302, 1, 63, 248, 496, 2, 616, 1, 226, 1, 248, 496, 908, 683, 151, 77, 302, 248, 496, 1, 302, 1, 302, 1, 248, 496, 63, 1, 616, 1, 226, 248, 496, 2, 908, 683, 151, 76, 248, 496, 302, 1, 302, 1, 302, 248, 496, 1, 113, 342, 113, 227, 248, 496, 169, 59, 169, 172, 113, 248, 496, 341, 113, 342, 113, 228, 248, 496, 169, 58, 169, 172, 113, 248, 496, 341, 113, 342, 113, 228, 248, 496, 169, 58, 169, 172, 113, 248, 496, 342, 113, 341, 113, 228, 248, 496, 169, 58, 169, 172, 113, 248, 496, 2160, 169, 1, 56, 1, 248, 496, 169, 2, 56, 169, 2, 248, 496, 56, 1, 169, 1, 56, 248, 496, 1, 75, 1, 75, 1, 248, 496, 75, 75, 1, 75, 1, 248, 496, 75, 1, 75, 75, 1, 248, 496, 75, 1, 75, 1, 75, 248, 496, 75, 1, 151, 76, 151, 248, 496, 77, 226, 115, 169, 58, 248, 496, 151, 76, 113, 1, 151, 248, 496, 76, 151, 76, 151, 77, 248, 496, 151, 76, 151, 76, 151, 248, 496, 76, 113, 1, 113, 1, 248, 496, 113, 151, 190, 151, 76, 248, 496, 151, 77, 151, 76, 226, 248, 496, 115, 169, 58, 151, 76, 248, 496, 113, 1, 151, 76, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496, 113, 1, 113, 113, 1, 248, 496, 226, 570, 113, 113, 1, 248, 496, 151, 76, 151, 76, 226, 248, 496, 115, 169, 59, 151, 76, 248, 496, 113, 151, 77, 151, 76, 248, 496, 151, 76, 151, 77, 151, 248, 496, 76, 151, 76, 113, 1, 248, 496, 113, 113, 1, 151, 190, 248, 496, 151, 76, 151, 77, 151, 248, 496, 76, 151, 76, 151, 76, 248, 496, 113, 1, 113, 1, 113, 248, 496, 340, 1, 151, 76, 151, 248, 496, 77, 151, 76, 113, 1, 248, 496, 151, 76, 188, 835, 151, 248, 496, 76, 151, 76, 226, 115, 248, 496, 169, 58, 151, 77, 113, 248, 496, 151, 77, 151, 76, 151, 248, 496, 76, 151, 76, 151, 77, 248, 496, 151, 76, 113, 1, , 113, 248, 496, 113, 1, 151, 190, 151, 248, 496, 76, 151, 76, 151, 77, 248, 496, 226, 114, 169, 59, 151, 248, 496, 76, 113, 1, 151, 76, 248, 496, 151, 76, 151, 76, 151, 248, 496, 77, 113, 113, 1, 113, 248, 496, 1, 226, 569, 75, 1, 248, 496, 75, 1, 75, 151, 77, 248, 496, 113, 151, 77, 151, 76, 248, 496, 188, 39, 151, 76, 151, 248, 496, 190, 151, 77, 151, 76, 248, 496, 113, 151, 77, 151, 76, 248, 496, 188, 39, 151, 190, 151, 248, 496, 76, 151, 77, 151, 76, 248, 496, 151, 76, 151, 77, 151, 248, 496, 76, 113, 113, 1, 113, 248, 496, 1, 151, 303, 340, 1, 248, 496, 151, 76, 151, 77, 113, 248, 496, 188, 40, 151, 76, 113, 248, 496, 1, 151, 76, 151, 76, 248, 496, 151, 76, 113, 1, 151, 248, 496, 76, 151, 77, 188, 39, 248, 496, 151, 76, 151, 190, 151, 248, 496, 76, 151, 77, 113, 151, 248, 496, 76, 151, 77, 188, 39, 248, 496, 151, 190, 151, 76, 151, 248, 496, 76, 151, 77, 151, 76, 248, 496, 151, 76, 151, 77, 113, 248, 496, 113, 1, 113, 151, 418, 248, 496, 151, 76, 75, 1, 75, 248, 496, 75, 1, 113, 1, 113, 248, 496, 114, 226, 1, 113, 1, 248, 496, 113, 1, 113, 113, 1, 248, 496, 113, 1, 56, 56, 1, 248, 496, 56, 1, 56, 1, 1817, 248, 496, 456, 454, 302, 1, 302, 248, 496, 1, 302, 1, 63, 2, 248, 496, 616, 1, 226, 1, 908, 248, 496, 683, 151, 76, 302, 1, 248, 496, 302, 2, 302, 1, 63, 248, 496, 1, 616, 1, 226, 2, 248, 496, 908, 683, 151, 76, 302, 248, 496, 1, 302, 1, 302, 1, 248, 496, 113, 341, 113, 228, 169, 248, 496, 59, 169, 172, 113, 341, 248, 496, 113, 342, 113, 228, 169, 248, 496, 58, 169, 172, 113, 341, 248, 496, 113, 342, 113, 228, 169, 248, 496, 58, 169, 172, 113, 341, 248, 496, 113, 342, 113, 228, 169, 248, 496, 58, 169, 172, 113, 2160, 248, 496, 169, 1, 56, 1, 169, 248, 496, 2, 56, 169, 2, 56, 248, 496, 1, 169, 1, 56, 1, 248, 496, 75, 1, 75, 75, 1, 248, 496, 75, 1, 75, 1, 75, 248, 496, 75, 1, 75, 1, 75, 248, 496, 1, 75, 75, 1, 75, 248, 496, 1, 151, 76, 151, 77, 248, 496, 226, 114, 169, 59, 151, 248, 496, 76, 113, 1, 151, 76, 248, 496, 151, 76, 151, 76, 151, 248, 496, 77, 151, 76, 151, 76, 248, 496, 113, 1, 113, 1, 113, 248, 496, 151, 190, 151, 76, 151, 248, 496, 77, 151, 76, 226, 115, 248, 496, 169, 58, 151, 76, 113, 248, 496, 1, 151, 76, 151, 77, 248, 496, 151, 76, 151, 76, 113, 248, 496, 1, 113, 113, 1, 340, 248, 496};
//
//  Create an instance of the lcdhelper class named oLCD
//
lcdhelper oLCD(ILI9163_4L, 3, 2, 9, 10, 7);
//
//  Create an instance of irhelper class named oIR.
//
irhelper oIR;
//
//  Define function prototypes.
//

// led pins defined for option 1
const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 7;

const int SW1_PIN = 18;
const int SW2_PIN = 17;
const int SW3_PIN = 16;
const int SW4_PIN = 15;

const int SPEAKER_PIN = 8;

const int MAX_CS_PIN = 10;
const int MAX_CLK_PIN = 12;
const int MAX_SO_PIN = 13;
// MOSFET PIN
const int PWM_PIN = 11;
// optocoupler
const int OPTO_PIN = 38;
// tilt and pan
const int TILT_SERVO_PIN = 35;
const int PAN_SERVO_PIN = 36;

// DAN- add lines 35-40 to finish main document steps 13-15
// Speaker channel
const int SPK_CHANNEL = 1;
// PWM channel
const int PWM_CHANNEL = 2;
// Audio channel
const int AUDIO_CHANNEL = 4;

void ShowDisplay(screen val, char optionstate, char keypressed, bool intdisplay);

// FOR OPTION 4
//  Thread Stuff
TaskHandle_t Task_PlayAudio;

// HTTP helper
httphelper objHTTP;

// File arrays
String picFiles[16];
String audioFiles[16];

// Index
int currentIndex = 0;

Servo pservo;
Servo tservo;

int pangle = 90;             // angle to 90 degrees
int tangle = 90;             // angle to 90 degrees
bool servosAttached = false; // setting servos as unattached

// for three 33 3 3 3
void IRAM_ATTR PWM_ISR();
const int interruptPinPWM = OPTO_PIN;

volatile unsigned long pwmcount = 0;
volatile unsigned long tonecount = 0;

double pwmfrequency = 0.0;
unsigned long lastTime;

const int pulsesPerRevolution = 20;
static String aval = "";
const int pwmFreq = 496; /* 5 KHz */

const int pwmResolution = 8;

bool motorRunning = false;
double desiredDuty = 0.0; // duty cycle entered by user (0–100)

// for three 33 3 3 3

void Option1();
void Option2(char optionstate);
void Option3(char optionstate, char keypressed);
void Option4(char keypressed);
void Option5();

double ReadThermoC();
void sensorReadings();
void updateTemp(double tempC);
void updateCord(double latt, double longitude);
void ReadCord(double &latt, double &longitude);

//
//  Setup()
//
void setup()
{
    Serial.begin(115200);
    Dabble.begin("Group3");
    while (!Serial)
    {
        // wait for serial to start
    }
    // led pins
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);

    // switch pin inputs
    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);
    pinMode(SW3_PIN, INPUT_PULLUP);
    pinMode(SW4_PIN, INPUT_PULLUP);

    // speaker pin
    pinMode(SPEAKER_PIN, OUTPUT);

    // MAX6675 pins
    pinMode(MAX_CS_PIN, OUTPUT);
    pinMode(MAX_CLK_PIN, OUTPUT);
    pinMode(MAX_SO_PIN, INPUT);
    digitalWrite(MAX_CS_PIN, HIGH);

    ESP32PWM::allocateTimer(3); // set servo objects to 20 milliseconds
    // MOSFET Pin
    pinMode(PWM_PIN, OUTPUT);

    // optocouple pin
    pinMode(OPTO_PIN, INPUT);

    // tilt and pan servo pins
    pservo.setPeriodHertz(50);
    tservo.setPeriodHertz(50);
    pinMode(TILT_SERVO_PIN, OUTPUT);
    pinMode(PAN_SERVO_PIN, OUTPUT);
    tservo.attach(TILT_SERVO_PIN);
    pservo.attach(PAN_SERVO_PIN);
    tservo.write(90);
    pservo.write(90);
    delay(500);
    tservo.detach();
    pservo.detach();

    // call to setup & attachpin correction
    ledcSetup(SPK_CHANNEL, 496, 8);
    ledcAttachPin(SPEAKER_PIN, SPK_CHANNEL);

    // Initialize IR Communications
    oIR.IRInitialize();

    // FOR OPTION 4
    xTaskCreate(PlayAudio_loop, "Task_PlayAudio", 1000, NULL, 1, &Task_PlayAudio);

    // Display message connecting to wifi
    //  FOR OPTION 4
    GetFileInfo(picFiles, audioFiles, 16); // MUST be before menu

    // Show main menu when Arduino starts
    // for the 1st time or on reset.
    ShowDisplay(SC_MAIN, ' ', ' ', true);

    // for three 33 3 3 3

    // Setup PWM
    ledcSetup(PWM_CHANNEL, pwmFreq, pwmResolution);
    // Attach the LED PWM Channel to the GPIO Pin
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    ledcWrite(PWM_CHANNEL, 0);

    pinMode(interruptPinPWM, INPUT_PULLUP);

    // for three 33 3 3
}
//
//  Loops(): Perform looping operations
//
void loop()
{
    unsigned long last_key_processed;
    // Determine which key was pressed.
    last_key_processed = oIR.GetKeyPressed();
    // Return to the main menu any time the
    // menu key is pressed.
    if (last_key_processed == KEY_MENU)
    {
        // 3
        motorRunning = false;
        detachInterrupt(digitalPinToInterrupt(interruptPinPWM));
        ledcWrite(PWM_CHANNEL, 0); // stop motor
        aval = "";
        // 3                  // reset duty entry
        ShowDisplay(SC_MAIN, ' ', ' ', false);
    }
    if (last_key_processed == KEY_RETURN && oLCD.SCREEN_STATE == SC_SUB2)
    {
        tservo.detach();
        pservo.detach();
        servosAttached = false;
        ShowDisplay(SC_MAIN, ' ', ' ', false);
    }
    else
    {
        if (oLCD.SCREEN_STATE == SC_MAIN)
        {
            if (last_key_processed == KEY_1)
            {
                ShowDisplay(SC_SUB1, ' ', ' ', false);
            }
            else if (last_key_processed == KEY_2)
            {
                ShowDisplay(SC_SUB2, 'A', ' ', false);
                HandleGamePad();
            }
            else if (last_key_processed == KEY_3)
            {
                ShowDisplay(SC_SUB3, 'A', ' ', false);
            }
            else if (last_key_processed == KEY_4)
            {
                ShowDisplay(SC_SUB4, ' ', ' ', false);
            }
            else if (last_key_processed == KEY_5)
            {
                ShowDisplay(SC_SUB5, ' ', ' ', false);
            }
            else if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB1) // WARNINGS
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB2)
        {

            if (last_key_processed == KEY_RETURN)
            {
                tservo.detach(); // detaching servos when leaving option2
                pservo.detach();
                servosAttached = false; // setting servos to detached
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB3) // WARNINGS
        {
            if (last_key_processed == KEY_MENU)
            {
                motorRunning = false;
                detachInterrupt(digitalPinToInterrupt(interruptPinPWM));
                ledcWrite(PWM_CHANNEL, 0); // stop motor
                aval = "";                 // reset duty entry
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
            else if (last_key_processed == KEY_PLAY) // Play/Pause key
            {
                motorRunning = false;
                detachInterrupt(digitalPinToInterrupt(interruptPinPWM));
                ledcWrite(PWM_CHANNEL, 0);            // stop motor
                aval = "";                            // reset duty entry
                ShowDisplay(SC_SUB3, 'A', ' ', true); // go back to duty entry
            }
            // If the user presses one of the numeric keys, then display the key in the correct place on the screen.
            if (last_key_processed == KEY_0 || last_key_processed == KEY_1 || last_key_processed == KEY_2 || last_key_processed == KEY_3 || last_key_processed == KEY_4 ||
                last_key_processed == KEY_5 || last_key_processed == KEY_6 || last_key_processed == KEY_7 || last_key_processed == KEY_8 || last_key_processed == KEY_9)
            {
                // Serial.print("t=");Serial.println(oLCD.OPTION_STATE);
                if ((oLCD.OPTION_STATE == 'A') || (oLCD.OPTION_STATE == 'B'))
                {
                    ShowDisplay((screen)oLCD.SCREEN_STATE, 'B', oIR.TranslateKey(last_key_processed), false);
                }
                else if ((oLCD.OPTION_STATE == 'C'))
                {
                    // display PWM frequency and value
                    oLCD.printNumF(pwmfrequency, 4, CENTER, 80);
                }
            }
            else if (last_key_processed == KEY_RETURN)
            {
                if (oLCD.OPTION_STATE == 'B')
                {
                    ShowDisplay((screen)oLCD.SCREEN_STATE, 'C', ' ', false);
                }
                else if (oLCD.OPTION_STATE == 'C')
                {
                    // return to A
                    ShowDisplay((screen)oLCD.SCREEN_STATE, 'A', ' ', false);
                }
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB4)
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
        else if (oLCD.SCREEN_STATE == SC_SUB5)
        {
            if (last_key_processed == KEY_RETURN)
            {
                ShowDisplay(SC_MAIN, ' ', ' ', false);
            }
        }
    }
    delay(100);
    // 3
    static unsigned long lastUpdate = 0;
    if (motorRunning && millis() - lastUpdate >= 1000)
    {
        float rpm = measureSpeed();
        oLCD.printNumF(rpm, 1, CENTER, 80);
        lastUpdate = millis();
    }
    // 3
}
// *************************************************************************************
// BEGIN: FUNCTION DEFINITIONS
// *************************************************************************************
//
//  Method to display menus on TFT display
//
void ShowDisplay(screen val, char optionstate, char keypressed, bool initdisplay)
{
    // Store screen state.
    oLCD.SCREEN_STATE = val;
    oLCD.OPTION_STATE = optionstate;
    int selectedpid = 0;
    char text[20];

    if (val == SC_MAIN)
    {
        oLCD.LCDInitialize(LANDSCAPE, initdisplay);
        sprintf(text, "SELECT OPTION");
        oLCD.print(text, 10, 10);
        sprintf(text, "1. Mystery Music");
        oLCD.print(text, 10, 25);
        sprintf(text, "2. Pan and Tilt");
        oLCD.print(text, 10, 40);
        sprintf(text, "3. Run the Motor");
        oLCD.print(text, 10, 55);
        sprintf(text, "4. Stream Audio");
        oLCD.print(text, 10, 70);
        sprintf(text, "5. Query Sensors");
        oLCD.print(text, 10, 85);
    }
    else if (val == SC_SUB1)
    {
        Option1();
    }
    else if (val == SC_SUB2)
    {
        Option2(optionstate);
    }
else if (val == SC_SUB3)
    {
        Option3(oLCD.OPTION_STATE, ' ');
        if (optionstate == 'A')
        {
            aval = "";
            oLCD.LCDInitialize(LANDSCAPE, initdisplay);
            oLCD.print(F("DESIRED DUTY"), CENTER, 10);
            oLCD.print(F("CYCLE (%)"), CENTER, 25);
            oLCD.setBackColor(RCB_BLACK);
            oLCD.print(F("   "), CENTER, 40);
            oLCD.setBackColor(RCB_PURPLE);

            oLCD.print(F("CALC'D SPEED"), CENTER, 65);
            oLCD.print(F("0"), CENTER, 80);
        }
        else if (optionstate == 'B')
        {
            // Limit value of a to 2 digits
            if (aval.length() <= 2)
            {
                aval = aval + keypressed;
            }
            oLCD.setBackColor(RCB_BLACK);
            oLCD.printNumI(aval.toInt(), CENTER, 40, 3, ' ');
            oLCD.setBackColor(RCB_PURPLE);
        }
        else if (optionstate == 'C')
        {

            oLCD.print(F("C"), CENTER, 100);
        }
    }
    else if (val == SC_SUB4)
    {
        Option4(keypressed);
    }
    else if (val == SC_SUB5)
    {
        Option5();
    }
}
//********************************************************************
//  BEGIN OPTION CODE
//********************************************************************
void Option1()
{
    oLCD.LCDInitialize(LANDSCAPE, false);
    // set background color
    oLCD.fillscreen(RCB_PURPLE);
    // print music playing
    oLCD.print("MUSIC PLAYING", CENTER, 10);
    oLCD.print(".....", CENTER, 30);

    bool songFinished = playSong();

    if (songFinished == true)
    {
    }
    else
    {
    }
}
bool playSong()
{
    // arrays at the top

    int N = sizeof(song) / sizeof(song[0]); // # elements in array song is 1439 and notes is 1414, use size of operator

    // NEEDS MORE HELP TO GET SKIPPING RIGHT
    for (int i = 0; i < N; i++)
    {
        if ((i - 6) % 7 == 0 || (i - 6) % 7 == 1)
        {
            if (i >= 6)
                continue;
        }
        int duration = note[i];

        // step 5 - for varing speed
        if (digitalRead(SW1_PIN) == LOW)
        {
            duration = duration / 2;
        }

        else if (digitalRead(SW3_PIN) == LOW)
        {
            duration = duration * 2;
        }
        // step 5a - leds blink with notes
        int ledIndex = i % 4;

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);

        if (ledIndex == 0)
            digitalWrite(LED1, HIGH);
        if (ledIndex == 1)
            digitalWrite(LED2, HIGH);
        if (ledIndex == 2)
            digitalWrite(LED3, HIGH);
        if (ledIndex == 3)
            digitalWrite(LED4, HIGH);

        // 5b + c - playing song
        ledcWriteTone(SPK_CHANNEL, song[i]);
        delay(duration);

        // 5c + d: cleaning up
        ledcWriteTone(SPK_CHANNEL, 0);

        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);

        // exits song when sw4 pressed or return key
        char RemoteControl = oIR.GetKeyPressed();
        if (digitalRead(SW4_PIN) == LOW || RemoteControl == KEY_RETUR))
            {
                ledcWriteTone(SPK_CHANNEL, 0);
                digitalWrite(LED1, LOW);
                digitalWrite(LED2, LOW);
                digitalWrite(LED3, LOW);
                digitalWrite(LED4, LOW);

                oLCD.clrScr();
                oLCD.print("MSC STOPPED", CENTER, 10);
                oLCD.print("PRESS", CENTER, 30);
                oLCD.print("<MODE>", CENTER, 50);
                oLCD.print("TO GO BACK", CENTER, 70);

                return false;
            }
    }
    // song plays all the way through
    oLCD.clrScr();
    oLCD.print("MSC FINISHED", CENTER, 10);
    oLCD.print("PRESS", CENTER, 30);
    oLCD.print("<RETURN>", CENTER, 50);
    oLCD.print("TO GO BACK", CENTER, 70);

    ledcWriteTone(SPK_CHANNEL, 0);

    return true; // song finished
}

// End of option 1

void AttachServos() // initialize a function to attach servos
{
    if (servosAttached == false) // seeing if servos are set to attached
    {
        tservo.attach(TILT_SERVO_PIN); // attaching tilt servo
        pservo.attach(PAN_SERVO_PIN);  // attaching pan servo

        tservo.write(tangle); // moving servo to specified angle
        pservo.write(pangle); // moving servo to specified angle

        servosAttached = true; // setting servos to attached
    }
}
void ServoScreenUpdate()
{
    char text[20]; // create character array
    sprintf(text, "PAN ANGLE");
    oLCD.print(text, CENTER, 30);               // output text(PAN ANGLE) to screen
    oLCD.printNumI(pangle, CENTER, 45, 3, ' '); // output pan angle number to screen

    sprintf(text, "TILT ANGLE");
    oLCD.print(text, CENTER, 60);               // output text(tilt ANGLE) to screen
    oLCD.printNumI(tangle, CENTER, 75, 3, ' '); // output tilt angle number to screen

    sprintf(text, "PRESS <RETURN>");
    oLCD.print(text, CENTER, 90); // output "PRESS <RETURN>" to the screen

    sprintf(text, "TO GO BACK");
    oLCD.print(text, CENTER, 105); // output "TO GO BACK" to the screen
}
void HandleGamePad()
{
    Dabble.processInput(); // starts collecting data from dabble

    if (GamePad.isUpPressed()) // Up button pressed
    {
        tangle += 5;      // increase tilt angle by 5 degrees
        if (tangle > 165) // see if tilt angle is going above maximum
        {
            tangle = 165; // set tilt angle at maximum
        }
        tservo.write(tangle); // write the tilt angle to the specified amount
    }
    if (GamePad.isDownPressed()) // Down button is pressed
    {
        tangle -= 5;     // decrease tilt angle by 5 degrees
        if (tangle < 15) // see if tilt angle is going below minimum
        {
            tangle = 15; // set tilt angle at minimum
        }
        tservo.write(tangle);
    }
    if (GamePad.isLeftPressed()) // Left button is pressed
    {
        pangle += 5;      // pan 5 degrees to the left
        if (pangle > 165) // see if pan angle is at the max
        {
            pangle = 165; // set pan angle to max if going above
        }
        pservo.write(pangle); // move servo to specified angle
    }
    if (GamePad.isRightPressed()) // Right button is pressed
    {
        pangle -= 5;     // pan 5 degrees to the right
        if (pangle < 15) // see if the pan angle is at the min
        {
            pangle = 15; // set pan angle at min if going below
        }
        pservo.write(pangle); // move servo to specified angle
    }
    // update the display of new angles
    ServoScreenUpdate(); // calls function to update ther servo screen

    ////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Option2(char optionstate)
{
    ////////////////////////////////////////////////////////////////////////////////////
    AttachServos(); // call function to attach servos
    oLCD.LCDInitialize(LANDSCAPE, false);
    //////////////////////////////////////////////////////////
    char text[20]; // create character array
    sprintf(text, "PAN ANGLE");
    oLCD.print(text, CENTER, 30);               // output text(PAN ANGLE) to screen
    oLCD.printNumI(pangle, CENTER, 45, 3, ' '); // output pan angle number to screen

    sprintf(text, "TILT ANGLE");
    oLCD.print(text, CENTER, 60);               // output text(tilt ANGLE) to screen
    oLCD.printNumI(tangle, CENTER, 75, 3, ' '); // output tilt angle number to screen

    sprintf(text, "PRESS <RETURN>");
    oLCD.print(text, CENTER, 90); // output "PRESS <RETURN>" to the screen

    sprintf(text, "TO GO BACK");
    oLCD.print(text, CENTER, 105); // output "TO GO BACK" to the screen
    ////////////////////////////////////////////////////////////
}
//
//  Code for Option3
//
void Option3(char optionstate, char keypressed)
{
    ledcWrite(PWM_CHANNEL, 0);
    if (optionstate == 'C') // user pressed return after entering duty cycle
    {
        if (aval.length() > 0)
        {
            desiredDuty = aval.toInt(); // convert entered string to number
            if (desiredDuty > 100)
                desiredDuty = 100; // limit duty cycle
            if (desiredDuty < 0)
                desiredDuty = 0;

            // Start motor at desired duty
            int pwmVal = map(desiredDuty, 0, 100, 0, 255); // 8-bit PWM
            ledcWrite(PWM_CHANNEL, pwmVal);

            pwmcount = 0;
            lastTime = millis();
            attachInterrupt(digitalPinToInterrupt(interruptPinPWM), PWM_ISR, FALLING);
            motorRunning = true;
        }
    }
}

void IRAM_ATTR PWM_ISR()
{
    pwmcount++;
}

void Tone_ISR()
{
    // increase tone count
    tonecount++;
}

float measureSpeed()
{
    unsigned long currentTime = millis();
    float elapsedTime = (currentTime - lastTime) / 100.0;

    if (elapsedTime <= 0)
        return 0.0;

    float rpm = ((float)pwmcount / (float)pulsesPerRevolution) * 60.0 / elapsedTime;

    pwmcount = 0;
    lastTime = currentTime;

    return rpm;
}

//
//  Option to Move between pictures and songs.
//

// OPTION 4 START
void GetFileInfo(String picFiles[], String audioFiles[], int rowcount){
    //5a. display to the screen
    oLCD.fillScreen(RCB_PURPLE);
    oLCD.print("CONNECTING", CENTER, 10);
    oLCD.print("TO WIFI", CENTER, 30);
    oLCD.print(".....", CENTER, 50);

    //5b. HHTP instance
    HTTPClient m_http;

    //5c. connect to network with timeout
    if(objHTTP.connect(30) == true){
        // 5d. call get url
        if(objHTTP.getURL(m_http,"https://intranet.mne.ksu.edu/static/ME400/images.php" , 3000)){
            // call get base files
            GetBaseFiles(m_http, picFiles, audioFiles, rowcount);

            m_http.end();
        }
        objHTTP.disconnect();
    }
}
//
//  Option to Move between pictures and songs.
//

void Option4(char keypressed)
{   
    oLCD.LCDInitialize(LANDSCAPE, false);
    currentIndex = 0;

    while (true)
    {   // 6a. image displayed on tft
        MysteryPic(picFiles[currentIndex]);

        while (oIR.GetKeyPressed() != 0) {
            
        }
        //6b. plays audio with image
        int button = MysteryAudio(audioFiles[currentIndex]);

        // controls for the remote keys
        if(button == 0){
            button = oIR.GetKeyPressed();
            while (button == 0){
                button = oIR.GetKeyPressed();
                delay(50);
            }
        }

        // 6c. Key Forward >>
        if (button == KEY_FORWARD)
        {   sigmaDeltaWrite(AUDIO_CHANNEL, 0);
            ledcDetachPin(SPEAKER_PIN);
        


            currentIndex++;
            if (currentIndex >= 16)
                currentIndex = 0;
        }

        // 6d. Key Reverse <<
        else if (button == KEY_BACK)
        {   sigmaDeltaWrite(AUDIO_CHANNEL, 0);
            ledcDetachPin(SPEAKER_PIN);
            

            currentIndex--;
            if (currentIndex < 0)
                currentIndex = 15;
        }

        // return key exits the function
        else if (button == KEY_RETURN)
        {   sigmaDeltaWrite(AUDIO_CHANNEL, 0);
            ledcDetachPin(SPEAKER_PIN);
            

            return;
        }
        //delay(500);
        
        button = 0;
    }
}

//Myster Pic implementation, ****Not sure about file name
void MysteryPic(String filename){
    //color background screen
    oLCD.fillScreen(RCB_PURPLE);
    //1. create instance http 
    HTTPClient m_http;

    //2. create unsigned array 2560
    unsigned char buff[2560] = {0};

    //3. connect to network
    if(objHTTP.connect(30) == true){
        //4. call get url method
        if(objHTTP.getURL(m_http,"https://intranet.mne.ksu.edu/static/ME400/" + filename, 3000)){
            //5. get file image data
            String OriginalData = m_http.getString();
            

            //6. remove header data
            int inst128 = OriginalData.indexOf("128");
            //finds the first instance after 128 and store all that data in a string
            int ImageStart = OriginalData.indexOf('\n', inst128);
            String ImageData = OriginalData.substring(ImageStart + 1);
            
            Serial.println(ImageStart);

            //7. loop through data to reorder
            for(int i = 0; i < 2560; i++){
                unsigned char OriginalOrder = (unsigned char)ImageData.charAt(i);
                unsigned char reversedByte = 0;

                //reorder bits
                for(int j = 0; j < 8; j++){
                    int OriginalBit = bitRead(OriginalOrder, j);
                    bitWrite(reversedByte, 7 - j, OriginalBit);

                }
                buff[i] = reversedByte; 
            }

            //8. display image to screen.  
            oLCD.drawXBitmap(0, 0, buff, 160, 128, RCB_WHITE);

            //9. Call end function
            m_http.end();

        }
        //10. disconnect
        objHTTP.disconnect();


    }
    // 11. return calling routine


}


//Mystery Audio Implementation
int MysteryAudio(String filename){
    //1.
    HTTPClient m_http;

    //2. detatch speaker pin
    ledcDetachPin(SPEAKER_PIN);

    //3. Store pins and channel 
    sigmaDeltaSetup(SPEAKER_PIN, AUDIO_CHANNEL, 2e6);
    sigmaDeltaWrite(AUDIO_CHANNEL, 0);

    //4 connect to network
    if(objHTTP.connect(30) == true){
        //5. call get url
        if(objHTTP.getURL(m_http, "https://intranet.mne.ksu.edu/static/ME400/" + filename, 3000)){
            //6. copied in code
            circle_init(); // Initialize the circle (ring) buffer
            // Create a tcp stream.
            WiFiClient *stream = m_http.getStreamPtr();
            // Get size of file to download.
            int len = m_http.getSize();
            // If the connection is open and the file length is valid, then continue.
            while (m_http.connected() && (len > 0 || len == -1))
            {
                int irKey = oIR.GetKeyPressed();
                if (irKey != 0) { 
                    sigmaDeltaWrite(AUDIO_CHANNEL, 0);
                    ledcDetachPin(SPEAKER_PIN);
        
                    m_http.end();
                    objHTTP.disconnect();
                    return irKey;
                }

            // Number of bytes of data currently available for download.
            size_t size = stream->available();
            // If the size is greater than 0, then download the content.
            if (size)
            {
            uint8_t buff[200];
            int bytes_to_insert = min(sizeof(buff), size);
            int c = stream->readBytes(buff, bytes_to_insert);

            //7. create loop to iterate elements and delay
            for(int i = 0; i < c; i++){
                while(circle_put(buff[i]) == false){
                    delay(1);
                
                }
            }
            //8. pushing button stops streaming. 
            if(digitalRead(18) == LOW || digitalRead(17) == LOW || digitalRead(16) == LOW || digitalRead(15) == LOW){
                m_http.end();
                objHTTP.disconnect();
                return KEY_RETURN;
            }

            if (len > 0) { len -= c;}
            }

            }
            
        }
        //9. tirn off audio
        sigmaDeltaWrite(AUDIO_CHANNEL, 0);

        // 10. end method
        m_http.end();
    }
    //11. disconnect
    objHTTP.disconnect();
    return 0;

} // END OPTION 4

// option 5 helpers
// screen update readings
void sensorReadings()
{
    char text[20];

    oLCD.LCDInitialize(LANDSCAPE, false);

    // what will print to the screen on the TFT
    sprintf(text, "SENSOR");
    oLCD.print(text, 10, 10);
    sprintf(text, "READINGS");
    oLCD.print(text, 10, 25);
    sprintf(text, "TEMP:");
    oLCD.print(text, 10, 45);
    sprintf(text, "LATT:");
    oLCD.print(text, 10, 65);
    sprintf(text, "LONG:");
    oLCD.print(text, 10, 85);
}
// bit reading (readThemocouple in Celsius)
//
double ReadThermoC()
{
    uint16_t data = 0; // makes a 16-bit integer called data that begins at zero.

    digitalWrite(MAX_CS_PIN, LOW); // CS pin low (CS means Chip select) (LOW tells the max6675 to read starting now)
    delayMicroseconds(10);

    for (int i = 15; i >= 0; i--) // runs 16 times to cycle bits
    {
        digitalWrite(MAX_CLK_PIN, LOW); // clock pin to LOW (clock shows when the sensor presents the next bit)
        delayMicroseconds(10);

        if (digitalRead(MAX_SO_PIN) == HIGH) // SO pin check if it is HIGH (HIGH means bit = 1, not HIGH then bit = 0)
        {
            data = data + (uint16_t)pow(2, i); // bit value is 1, then it adds the value to the data
        }

        digitalWrite(MAX_CLK_PIN, HIGH); // clock pin to high
        delayMicroseconds(10);
    }

    digitalWrite(MAX_CS_PIN, HIGH);

    data = data / 8;    // shift to the right 3 places (removes last three bits)
    data = data % 4096; // keeps lowest 12 bits

    return data * 0.25; // to degrees celsius
}

void updateTemp(double tempC)
{
    oLCD.print("        ", 70, 45);   // spaces act as a refresh to clear old temps
    oLCD.printNumF(tempC, 2, 70, 45); // prints new temp to 2 decimal places
}

void ReadCord(double &latt, double &longitude)
{
    longitude = Sensor.getGPSlongitude();
    latt = Sensor.getGPSLatitude();
}

void updateCord(double latt, double longitude)
{
    oLCD.print("        ", 70, 65); // clears old lattitude and enters new
    oLCD.printNumF(latt, 2, 70, 65);

    oLCD.print("        ", 70, 85); // clears old longitude and enters the new
    oLCD.printNumF(longitude, 2, 70, 85);
}

void Option5()
{
    double tempC = 0.0;
    double latt = 0.0;
    double longitude = 0.0;
    // button pressed starts at nothing pressed
    // runs until RETURN on the remote is pressed
    unsigned long button = KEY_NONE;

    sensorReadings();

    while (button != KEY_RETURN)

    // check for dabble data (latt and long)
    {
        Dabble.processInput();
        // read thermocouple in degrees Celsius
        tempC = ReadThermoC();
        // Read the location of the phone
        ReadCord(latt, longitude);
        // update the temps to the current temp
        updateTemp(tempC);
        // update the location to the current location of the phone
        updateCord(latt, longitude);
        // check buttons pressed (return leaves option 5)
        button = oIR.GetKeyPressed();
        delay(300);
    }
}

//********************************************************************
//  END OPTION CODE
//********************************************************************
