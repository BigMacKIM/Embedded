#define RCC_APB2_ENR *(volatile unsigned int *)0x40021018 // GPIO ���� Ŭ�� ��� ��������

#define GPIOD_CRL *(volatile unsigned int *)0x40011400 // GPIOD 0~7��Ʈ�� ���� ����� ��� ����(GD2,3,4,7)
#define GPIOD_BSRR *(volatile unsigned int *)0x40011410 // GPIOD�� ���� Bit Set, Reset Write
#define GPIOD_BRR *(volatile unsigned int *)0x40011414 // GPIOD�� ���� Bit Reset ����

#define GPIOC_CRH *(volatile unsigned int *)0x40011004 // GPIOC 0~7��Ʈ�� ���� ����� ��� ����(GB8)
#define GPIOC_IDR *(volatile unsigned int *)0x40011008 // GPIOC�� ���� LOW, HIGH�� Read

int main()
{
	RCC_APB2_ENR = 0x3E; // GPIOB,D ���� Ŭ�� ����� ������
	/* 0x3E=0b111110, 0b110000��. 0x30�̾ �����Ѱ�? => �����δ� C,D�� Ŭ�� �ΰ���
    */

    /* CRL, CRH �ʱⰪ�� �� ��Ʈ�� CNF = 01 / MODE = 00.
    CNF : input�϶� Floating mode(reset state), output�� �� open-drain
    MODE : input mode (reset state).

    �׷���, LED�� ������ ������ MODE �θ� 01 or 10 or 11 (output mode, �ӵ��� �ٸ�)���� �����ؾ���.
    (MODE�� ����� �ִ��� �𸣰���, �ϴ��� 00. (general purpose output push-pull))

    ���̽�ƽ CNF : floating input, mode : input mode => 0b0100 = 0x4
    */
	GPIOD_CRL = 0x44444444; // GPIOD reset
    GPIOC_CRH = 0x44444444; // GPIOC reset


	GPIOD_CRL = 0x10011100; // LED �κ��� GPIOD2,3,4,7�� output���� ������
	GPIOC_CRH = 0x00444400; // joystick up, down, left, right �κ��� GPIOC2,3,4,5�� input���� ������

	/* line 23 �ʿ��Ѱ�? */
	GPIOD_BSRR |= 0x0; // GPIOD bit reset


	/* BSRR : ODR �������� ��Ʈ�� set �ϴ� ��ȣ�� ����. => 1�� �ٲ�
        BRR : ODR �������� ��Ʈ�� reset �϶�� ��ȣ�� ����. => 0���� �ٲ�
        �� Ŭ�� �ֱ⸸ 1��
        */
        /* IDR/ODR : ��Ʈ�� �ش� ��Ʈ�� ���¸� �����. ODR�� BSRR / BRR �������ͷ� �� �ٲ� �� ����.
        IDR�� ���̽�ƽ�� ���� �Է���ġ�� ���¿� ���� ���� �ٲ�. */
        /* led�� ODR�� ���� 1�� �� ������, 0�� �� ����.
            ���̽�ƽ�� Ǯ�� ����̹Ƿ� �������� �� 0(low), ��ҿ��� 1(high).*/
	while (1) // ��� �ݺ�
	{   // ���̽�ƽ �Է��� ���� �� 0�̹Ƿ�, �տ� not ���� �ٿ������
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
	  else { // �߸�
	  	GPIOD_BRR  |= 0x9C;
	  }

	}
}
