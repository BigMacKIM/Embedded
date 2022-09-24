#define RCC_APB2_ENR                *(volatile unsigned int *)0x40021018 // GPIO ���� Ŭ�� ��� ��������

#define GPIOB_CRH                   *(volatile unsigned int *)0x40010C04 // GPIOB 8~15��Ʈ�� ���� ����� ��� ����(selection ��ư / GB8 -> input)
#define GPIOB_IDR                   *(volatile unsigned int *)0x40010C08 // GPIOB �Է� ���� Read(selection ��ư GB8)

#define GPIOC_CRL                   *(volatile unsigned int *)0x40011000 // GPIOC 0~7��Ʈ�� ���� ����� ��� ���� (���̽�ƽ UP / GC5 -> input)
#define GPIOC_CRH                   *(volatile unsigned int *)0x40011004 // GPIOC 8~15��Ʈ�� ���� ����� ��� ���� (�����̸�� / GC8 -> output)
#define GPIOC_IDR                   *(volatile unsigned int *)0x40011008 // GPIOC �Է� ���� Read (���̽�ƽ UP ��ư / GC5)
#define GPIOC_BSRR                  *(volatile unsigned int *)0x40011010 // GPIOD�� ���� Bit Set, Reset Write (�����̸�� / GC8)
#define GPIOC_BRR                   *(volatile unsigned int *)0x40011014 // GPIOD�� ���� Bit Reset ���� (�����̸�� / GC8)

#define GPIOD_CRL                   *(volatile unsigned int *)0x40011400 // GPIOD 0~7��Ʈ�� ���� ����� ��� ���� (LED 1, 2, 3 / GD2,3,4 -> output)
#define GPIOD_CRH                   *(volatile unsigned int *)0x40011404 // GPIOD 8~15��Ʈ�� ���� ����� ��� ���� (User Button 1, 2 / GD11, 12 -> input)
#define GPIOD_BSRR                  *(volatile unsigned int *)0x40011410 // GPIOD�� ���� Bit Set, Reset Write (LED 1, 2, 3 / GD2, 3, 4)
#define GPIOD_BRR                   *(volatile unsigned int *)0x40011414 // GPIOD�� ���� Bit Reset ���� (LED 1, 2, 3 / GD2, 3, 4)
#define GPIOD_IDR                   *(volatile unsigned int *)0x40011408 // GPIOC �Է� ���� Read (User Button 1, 2 / GD11, 12)



void delay() {
  int i;
  for (i = 0; i < 10000000; i++) {}
}

int main()
{
  RCC_APB2_ENR = 0x38; // GPIO B, C, D ���� Ŭ�� ����� ������


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
  GPIOC_CRH = 0x00000001; // ������ ��� / GC8 output
  GPIOD_CRL = 0x00011100; // LED 1, 2, 3 / GD 2, 3, 4 output
  GPIOD_CRH = 0x00088000; // User Button 1, 2 / GD11, 12 input

  /*  input/output end  */

  /*      bit reset     */ // �ʿ��Ѱ�?

  GPIOD_BSRR |= 0x0; // GPIOD bit reset
  GPIOC_BSRR |= 0x0;

  /*    bit reset end   */


  while (1) // ��� �ݺ�
  {   // ���̽�ƽ �Է��� ���� �� 0�̹Ƿ�, �տ� not ���� �ٿ������
    if (~GPIOC_IDR & (1 << 5)) {        // UP read / GC5
      GPIOC_BSRR  |= 0x100;             // �����̸�� set / GC8
      delay();
      GPIOC_BRR |= 0x100;               // �����̸�� reset / GC8
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
    else { // �ƹ� ��ư ���� �� �� ��
        GPIOD_BRR  |= 0x9C;             // ��� LED reset / GD2, 3, 4, 7
        GPIOC_BRR |= 0x100;             // �����̸�� reset / GC8
    }

  }
}
