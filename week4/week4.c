#define RCC_APB2_ENR                *(volatile unsigned int *)0x40021018 // GPIO 핀의 클락 사용 레지스터

#define GPIOB_CRH                   *(volatile unsigned int *)0x40010C04 // GPIOB 8~15포트에 대한 입출력 사용 여부(selection 버튼 / GB8 -> input)
#define GPIOB_IDR                   *(volatile unsigned int *)0x40010C08 // GPIOB 입력 여부 Read(selection 버튼 GB8)

#define GPIOC_CRL                   *(volatile unsigned int *)0x40011000 // GPIOC 0~7포트에 대한 입출력 사용 여부 (조이스틱 UP / GC5 -> input)
#define GPIOC_CRH                   *(volatile unsigned int *)0x40011004 // GPIOC 8~15포트에 대한 입출력 사용 여부 (릴레이모듈 / GC8 -> output)
#define GPIOC_IDR                   *(volatile unsigned int *)0x40011008 // GPIOC 입력 여부 Read (조이스틱 UP 버튼 / GC5)
#define GPIOC_BSRR                  *(volatile unsigned int *)0x40011010 // GPIOD에 대한 Bit Set, Reset Write (릴레이모듈 / GC8)
#define GPIOC_BRR                   *(volatile unsigned int *)0x40011014 // GPIOD에 대한 Bit Reset 설정 (릴레이모듈 / GC8)

#define GPIOD_CRL                   *(volatile unsigned int *)0x40011400 // GPIOD 0~7포트에 대한 입출력 사용 여부 (LED 1, 2, 3 / GD2,3,4 -> output)
#define GPIOD_CRH                   *(volatile unsigned int *)0x40011404 // GPIOD 8~15포트에 대한 입출력 사용 여부 (User Button 1, 2 / GD11, 12 -> input)
#define GPIOD_BSRR                  *(volatile unsigned int *)0x40011410 // GPIOD에 대한 Bit Set, Reset Write (LED 1, 2, 3 / GD2, 3, 4)
#define GPIOD_BRR                   *(volatile unsigned int *)0x40011414 // GPIOD에 대한 Bit Reset 설정 (LED 1, 2, 3 / GD2, 3, 4)
#define GPIOD_IDR                   *(volatile unsigned int *)0x40011408 // GPIOC 입력 여부 Read (User Button 1, 2 / GD11, 12)



void delay() {
  int i;
  for (i = 0; i < 10000000; i++) {}
}

int main()
{
  RCC_APB2_ENR = 0x38; // GPIO B, C, D 핀의 클락 사용을 설정함


  /*        reset       */

  GPIOB_CRH = 0x44444444;
  GPIOD_CRL = 0x44444444;
  GPIOD_CRH = 0x44444444;
  GPIOC_CRL = 0x44444444;
  GPIOC_CRH = 0x44444444;

  /*      reset end     */


  /*    input/output    */

  GPIOB_CRH = 0x00000008; // joystick selection / GB8 input
  GPIOC_CRL = 0x00800000; // joystick up / GC5 input
  GPIOC_CRH = 0x00000001; // 릴레이 모듈 / GC8 output
  GPIOD_CRL = 0x00011100; // LED 1, 2, 3 / GD 2, 3, 4 output
  GPIOD_CRH = 0x00088000; // User Button 1, 2 / GD11, 12 input

  /*  input/output end  */

  /*      bit reset     */ // 필요한가?

  GPIOD_BSRR |= 0x0; // GPIOD bit reset
  GPIOC_BSRR |= 0x0;

  /*    bit reset end   */


  while (1) // 계속 반복
  {   // 조이스틱 입력이 들어올 때 0이므로, 앞에 not 연산 붙여줘야함
    if (~GPIOC_IDR & (1 << 5)) {        // UP read / GC5
      GPIOC_BSRR  |= 0x100;             // 릴레이모듈 set / GC8
      delay();
      GPIOC_BRR |= 0x100;               // 릴레이모듈 reset / GC8
    }
    else if (~GPIOD_IDR & (1 << 11)) {  // User Button 1 / GD11
      GPIOD_BSRR |= 0x4;                // LED 1 set / GD2
      delay();
      GPIOD_BRR |= 0x4;                 // LED 1 reset / GD2
    }
    else if (~GPIOD_IDR & (1 << 12)) { // User Button 2 / GD12
      GPIOD_BSRR |= 0x8;                // LED 2 set / GD3
      delay();
      GPIOD_BRR |= 0x8;                 // LED 2 set / GD3
    }
    else if (~GPIOB_IDR & (1<<8)) {     // selection read / GB8
      GPIOD_BSRR |= 0x10;               // LED 3 set / GD4
      delay();
      GPIOD_BRR |= 0x10;                // LED 3 reset / GD4
    }
    else { // 아무 버튼 동작 안 할 시
        GPIOD_BRR  |= 0x9C;             // 모든 LED reset / GD2, 3, 4, 7
        GPIOC_BRR |= 0x100;             // 릴레이모듈 reset / GC8
    }

  }
}
