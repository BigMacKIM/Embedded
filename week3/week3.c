#define RCC_APB2_ENR *(volatile unsigned int *)0x40021018 // GPIO 핀의 클락 사용 레지스터

#define GPIOD_CRL *(volatile unsigned int *)0x40011400 // GPIOD 0~7포트에 대한 입출력 사용 여부(GD2,3,4,7)
#define GPIOD_BSRR *(volatile unsigned int *)0x40011410 // GPIOD에 대한 Bit Set, Reset Write
#define GPIOD_BRR *(volatile unsigned int *)0x40011414 // GPIOD에 대한 Bit Reset 설정

#define GPIOC_CRH *(volatile unsigned int *)0x40011004 // GPIOC 0~7포트에 대한 입출력 사용 여부(GB8)
#define GPIOC_IDR *(volatile unsigned int *)0x40011008 // GPIOC에 대해 LOW, HIGH를 Read

int main()
{
	RCC_APB2_ENR = 0x3E; // GPIOB,D 핀의 클락 사용을 설정함
	/* 0x3E=0b111110, 0b110000임. 0x30이어도 가능한가? => 실제로는 C,D핀 클락 인가함
    */

    /* CRL, CRH 초기값은 각 포트에 CNF = 01 / MODE = 00.
    CNF : input일때 Floating mode(reset state), output일 때 open-drain
    MODE : input mode (reset state).

    그래서, LED를 제어할 때에는 MODE 부를 01 or 10 or 11 (output mode, 속도만 다름)으로 설정해야함.
    (MODE는 상관이 있는지 모르겠음, 일단은 00. (general purpose output push-pull))

    조이스틱 CNF : floating input, mode : input mode => 0b0100 = 0x4
    */
	GPIOD_CRL = 0x44444444; // GPIOD reset
    GPIOC_CRH = 0x44444444; // GPIOC reset


	GPIOD_CRL = 0x10011100; // LED 부분인 GPIOD2,3,4,7을 output으로 설정함
	GPIOC_CRH = 0x00444400; // joystick up, down, left, right 부분인 GPIOC2,3,4,5을 input으로 설정함

	/* line 23 필요한가? */
	GPIOD_BSRR |= 0x0; // GPIOD bit reset


	/* BSRR : ODR 레지스터 비트에 set 하는 신호를 보냄. => 1로 바꿈
        BRR : ODR 레지스터 비트에 reset 하라는 신호를 보냄. => 0으로 바꿈
        한 클럭 주기만 1임
        */
        /* IDR/ODR : 포트의 해당 비트의 상태를 출력함. ODR은 BSRR / BRR 레지스터로 값 바꿀 수 있음.
        IDR은 조이스틱과 같은 입력장치의 상태에 따라 값이 바뀜. */
        /* led는 ODR의 값이 1일 때 켜지고, 0일 때 꺼짐.
            조이스틱은 풀업 방식이므로 조작했을 때 0(low), 평소에는 1(high).*/
	while (1) // 계속 반복
	{   // 조이스틱 입력이 들어올 때 0이므로, 앞에 not 연산 붙여줘야함
      if (~GPIOC_IDR & (1 << 2)) { // DOWN
		GPIOD_BRR  |= 0x94;
	    GPIOD_BSRR |= 0x08;
      }
      else if (~GPIOC_IDR & (1 << 3)) { // LEFT
      	GPIOD_BRR  |= 0x8C;
      	GPIOD_BSRR |= 0x10;
      }
      else if (~GPIOC_IDR & (1 << 4)) { // RIGHT
      	GPIOD_BRR  |= 0x98;
      	GPIOD_BSRR |= 0x04;
      }
      else if (~GPIOC_IDR & (1 << 5)) { // UP
      	GPIOD_BRR  |= 0x1C;
      	GPIOD_BSRR |= 0x80;
      }
	  else { // 중립
	  	GPIOD_BRR  |= 0x9C;
	  }

	}
}
