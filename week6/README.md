# 1. 배경 지식
## 1.1 Polling / Interrupt
 
Polling 방식은 하드웨어의 변화를 지속적으로 읽어 들이며 이벤트의 발생을 주기적으로
확인해 신호가 들어왔을 때 이벤트를 실행하는 방식이다. 다른 작업을 진행하는 중에는
신호를 읽을 수 없다는 단점이 있다.

Interrupt 방식은 하드웨어의 변화를 감지하여 외부로부터의 입력을 CPU 가 알아채는
방식이다. 특정 이벤트 발생 시 현재 작업을 멈추고 해당 interrupt handler(ISR)를 수행한 후
다시 이전의 작업으로 돌아가게 된다.


## 1.2 HW / SW Interrupt
Interrupt 는 크게 Hardware interrupt 와 Software interrupt 로 나눌 수 있다.
Hardware interrupt 는 비동기식 이벤트 처리로 CPU 외부의 디스크 컨트롤러나 주변장치의
요청에 의해 발생하며 높은 우선순위를 갖는다. 그 예로 하드디스크 읽기 요청, 디스크 읽기
끝남, 키보드 입력 발생 등이 있다. 우리는 이번 실험에서 각 모듈의 입력을 통해 들어오는
Hardware interrupt 를 다룰 것이다.

Software interrupt 는 동기식 이벤트 처리로 외부가 아닌 CPU 내부에서 자신이 실행한 명령
또는 CPU 의 명령 실행에 관련된 모듈이 변화하는 경우 발생하며 낮은 우선순위를 갖는다.
예를 들어 프로그램상 처리 불가능한 오류나 이벤트(Trap / Exception)를 알리기 위해
사용된다.


## 1.3 EXTI
External interrupt 는 외부에서 신호가 입력될 경우 장치에 event 나 interrupt 가 발생하는
기능이다. Event / Interrupt mode 중 선택하여 사용할 수 있으며, 이번 실험에서는 interrupt
mode 로 설정한다. interrupt mode 로 설정되면 interrupt 가 발생할 때, interrupt handler 가
동작하게 된다. 각 Edge Detector Line 이 Rising / Falling Trigger 를 감지하고 설정에 맞춰
EXTI 에서 interrupt 로 처리하게 된다.

Interrupt request 는 mask 레지스터를 통해 확인할 수 있으며, pending 레지스터를 통해서는
어떤 interrupt 가 발생하였는지 알 수 있다. 프로세서가 interrupt 를 인지하게 되면 pending
레지스터를 검사하여 발생된 interrupt 중에 우선순위가 가장 높은 interrupt 를 처리하게
된다.

## 1.4 NVIC
Nested Vectored Interrupt Controller 는 중첩된 interrupt 를 제어하는 기능을 한다. 즉,
interrupt 가 발생해 interrupt handler 를 수행하는 도중에 우선순위가 더 높은 interrupt 가
발생하면 수행하던 interrupt handler 를 멈추고 우선순위가 더 높은 interrupt 의 ISR 를 먼저
처리한 후 계속해서 작업을 진행하게 된다. 예를 들어 사용자 버튼 S1 의 interrupt handler 가
수행되고 있는 도중 우선순위가 더 높은 interrupt 가 들어온다면 새로 들어온 interrupt 의
ISR 이 먼저 수행된 후에 S1 버튼의 ISR 이 이어서 진행될 것이다.


# 2. 실험 과정

## 2.1 세부 실험 내용
    1. Data Sheet 및 Reference Manual 을 참고하여 해당 레지스터 및 주소에 대한 설정 이해
    2. NVIC 와 EXTI 를 이용하여 GPIO 에 인터럽트 핸들링 세팅
    보드를 켜면 LED 물결 기능 유지 (LED 1->2->3->4->1->2->3->4->1->… 반복)
    A LED 물결 방향 변경 - 1->2->3->4
    B LED 물결 방향 변경 - 4->3->2->1
   3. 조이스틱 Up : A 동작, 조이스틱 Down : B 동작
   4. PC 의 Putty 에서 a, b 문자 입력하여 보드 제어 (PC -> 보드 명령) (‘a’ : A 동작, ‘b’ : B 동작)
   5. S1 버튼을 누를 경우 Putty 로 “TEAMXX.\r\n“ 출력
## 2.2 실험 방법
    1. J-Link 와 보드를 컴퓨터에 연결한다.
    2. IAR EW for Arm 을 실행해 project 생성 및 기본 설정을 한다.
    3. 코드 작성 시, stm32f10x., exti, gpio, usart, rcc.h 헤더 파일에 정의된 변수, 함수, 구조체들을
    참고하고 이를 활용한다.
    4. 이전 실험과 같이 헤더를 이용하여 사용할 GPIO 및 UART RCC enable 및 configuration 을
    해준다.
    5. 이번 실험에서는 EXTI 를 사용해 Interrupt 를 사용할 것이기 때문에 사용할 Pin 에 해당하는
    EXTI 를 연결해 주어야한다
    
    ![dd](https://user-images.githubusercontent.com/111568619/199389190-27658204-c3e1-4960-a325-fe3392ec2e54.png)

    
     6. EXTI line 을 통해 interrupt 신호를 받으면 레지스터들의 설정을 비교하여 NVIC controller 에
     보내야한다. (단, 이번 실험에서는 따로 priority 를 건들여 줄 필요가 없었다.)
     
![dd2](https://user-images.githubusercontent.com/111568619/199389225-014d254d-8783-4d7d-aa5a-ebe0ec7deaf9.png)

    7. EXIT 핸들러의 호출에 따라 실습 목표에 맞추어 함수 정의 혹은 main 문 절차를 작성하여
    조절해준다.
    8. 보드에 실습 코드를 올리고 조이스틱과 버튼을 통해 결과를 확인한다.
    9. 임베디드 보드의 Serial prot 를 PC 와 연결하고 PC 에서 putty 를 활여 시리얼 통신이
    이루어지는지 확인한다.
    
## 3. 실험 결과 – 코드

이번 실험은 추가 라이브러리의 구조체와 함수를 사용해 해당 레지스터에 값을 입력하는
방식을 사용했다. 또, polling 방식이 아닌 interrupt 방식을 사용했다.
첫 번째로 사용하고자 하는 포트에 clock 을 인가해준다. 해당 함수는 아래와 같다.    
![dd3](https://user-images.githubusercontent.com/111568619/199389791-8a3a646a-b4e0-4117-8dae-2cefd4c547fa.png)
TX/RX 를 위한 GPIOA 와 Alternate, Joystick UP 버튼을 위한 GPIOC, LED 를 켜기위한 GPIOD,
USART1 통신을 위한 USART1 에 Clock 을 ENABLE 시켜준다.
![dd4](https://user-images.githubusercontent.com/111568619/199389828-f2368474-0f79-4f8a-a525-9231445776a5.png)
다음으로 사용하는 GPIO 포트를 아래 구조체와 함수를 이용해 초기화 해준다.

![dd5](https://user-images.githubusercontent.com/111568619/199389852-2762bc09-1a7a-4439-916f-49eacd535ee0.png)
GPIO 포트를 초기화 할 때 사용하는 구조체로 사용하는 Pin 번호, Clock speed,
Mode(input/output 등)을 저장한다.
![dd6](https://user-images.githubusercontent.com/111568619/199389872-f835abfc-5cde-47b4-823b-183f9baff85e.png)

GPIO 포트를 초기화 할 때 사용하는 함수로 초기화 하고자하는 포트를 위에서 설정 한 구조체
대로 초기화 시켜준다
![dd7](https://user-images.githubusercontent.com/111568619/199389914-72476a1f-627f-4c2f-8177-adba566e225c.png)

Joystick UP, Down – pin 5, 2 를 IPU(Input, Pull-Up)모드로 설정
UserButton 1 – pin 11 을 IPU(Input, Pull-Up)모드로 설정
LED 1, 2, 3, 4 – pin 2, 3, 4, 7 을 Out_PP(Output, Pull-up/Pull-down)모드로 설정
TX – pin9 를 AF_PP(Alternate Function, Pull-up/Pull-down)모드로 설정
RX – pin 10 을 IPU(Input, Pull-Up)모드로 설정
다음은 EXTI(External Interrupt)를 구성하는 코드로 아래 구조체와 함수를 사용하여 초기화 한다.
![dd8](https://user-images.githubusercontent.com/111568619/199389939-2d916522-1c57-4ccf-a2f1-dbe89326b9eb.png)

모든 GPIO pin 들은 EXTI line 을 통해 연결돼있고 같은 번호의 pin 들은 같은 line 에 연결돼있다.
활성화 할 EXTI line 번호, EXTI mode(Interrupt or Event), EXTI 를 어떤 엣지에 Trigger 시킬지 (Rising
Edge, Falling-Edge, Rising & Falling-Edge), EXTI line 을 활성화 시킬지 비활성화 시킬지를 저장한다.

![dd9](https://user-images.githubusercontent.com/111568619/199389955-20ebfb64-0105-495c-a8ea-2c941ddc69a2.png)

위 함수로 GPIO port 의 원하는 pin 을 EXTILine 에 구성하고
![dd10](https://user-images.githubusercontent.com/111568619/199390130-d7deb225-c5dd-45e2-bdb4-3840e9ce72e2.png)
EXTI_Init 함수로 위 구조체에 저장된 설정으로 초기화 한다.
![dd11](https://user-images.githubusercontent.com/111568619/199390161-6b019e5a-bf25-43db-a873-9f8319f605de.png)
Joystick Down(GPIOC pin 2), Joystick UP(GPIOC pin 5), User 1(GPIOD pin 11)을 Interrupt 모드,
Falling – edge 에 trigger 되도록 설정한다.
다음으로 USART1 을 아래 구조체와 함수를 통해 초기화 한다.
![dd12](https://user-images.githubusercontent.com/111568619/199390179-0e247b1d-dd01-43f3-a9ef-8dd0bae5d7a9.png)
BaudRate, WorldLength, StopBit, Parity, Mode(Enable/Disable), H/W Flow Control 을 구성한다.

![dd13](https://user-images.githubusercontent.com/111568619/199390214-046b1c58-f0f7-4fdb-b262-50ea9a130038.png)
위 함수로 USART1 을 ENABLE 한다.

![dd14](https://user-images.githubusercontent.com/111568619/199390245-b02a7576-074a-47e5-ae26-d98d43e8b7e2.png)
위 함수로 USART1 을 초기화 한다.
![dd111](https://user-images.githubusercontent.com/111568619/199390565-28efcbbf-6799-409f-bbcd-6d88fd23b516.png)


![dd111](https://user-images.githubusercontent.com/111568619/199390592-d54bd038-3cab-44de-a8c8-a63c821459a2.png)
위 함수로 USART1 RX 인터럽트를 ENABLE 시킨다. (Putty 에서 보내는 신호를 받기 위함)

![dd15](https://user-images.githubusercontent.com/111568619/199390258-4ffb81d4-489d-43f8-8c15-e300ed19d403.png)
BaudRate – 9600
HardwareFlowControl – None
Mode – RX, TX 사용
Parity – 사용하지 않음
StopBit – 1
WorldLength – 8bits

다음은 NVIC(Nested Vectored Interrupt Controller)를 이용하여 interrupt 우선순위를 정한다
![dd16](https://user-images.githubusercontent.com/111568619/199390621-2595c642-b9f0-405a-9089-768eeb4ac977.png)
이 실험에서 interrupt 순위는 모두 같게 설정해 주었다.
다음은 USART1 으로 data 를 받았을 때 처리해주는 함수이다.
![dd17](https://user-images.githubusercontent.com/111568619/199390661-216361dc-798a-47f7-963c-1e06e5328303.png)
Idx 는 전역변수로 설정해 flag 역할을 하도록 하고 word 에 전송받은 data 를 저장한다.
data == ’a’ – idx = 0 (LED 순방향 출력)
data == ‘b’ – idx = 1 (LED 역방향 출력)

다음 Interrupt 발생시 interrupt 별 기능을 정의한다.
![dd18](https://user-images.githubusercontent.com/111568619/199390720-2f2268e9-3500-4c09-9ed9-637293aa9d2a.png)
EXTI15_10 (User button 1 -> line 11) – UART1 을 통해 msg 를 출력한다.
EXTI2 (Joystick down -> line 2) – idx = 1 (LED 역방향 출력)
EXTI9_5 (Joystick UP -> line 5) – idx = 0 (LED 순방향 출력)

다음은 idx(flag) 별로 LED 동작을 제어하는 함수이다.

![dd19](https://user-images.githubusercontent.com/111568619/199390757-7ec3f95d-7e50-4e17-8682-5f12951feccb.png)
arr 은 각 LED 의 pin 번호를 저장하고 있고 led 변수는 현재 켜져있는 led 의 index 를 저장한다.
idx == 0 – LED 순방향 출력
idx == 1 – LED 역방향 출력
마지막 while 반복문으로 위에서 정의한 LED 제어 함수를 동작시킨다.
![dd20](https://user-images.githubusercontent.com/111568619/199390798-566acde5-c668-49bf-bb7a-f01f1b792121.png)

LED 제어 모습
LED 제어 동영상 링크: https://photos.app.goo.gl/MVJKDDyQSkUckQYw6
![dd21](https://user-images.githubusercontent.com/111568619/199390837-2c6dd7a2-6779-4700-bd77-68140c0991ae.png)
Putty 메시지 출력 모습

## 4. 결론
이번 실험을 통해 두 가지 새로운 방식으로 stm32보드를 제어하는 법을 배웠다. 먼저 헤더에서
제공하는 구조체를 이용하여 여러 모듈을 제어할 수 있었는데, 종래에는 직접 메모리에 접근하는
방식 또는 라이브러리를 활용하되 클럭 인가나 초기화 등을 다 따로 해주었지만, 이번에는
구조체 내부의 변수에 임의의 값을 할당하고 Init 함수를 이용하여 더 쉽게 제어할 수 있었다.
또한 기존의 Polling 방식이 아닌 Interrupt 방식(EXTI 를 활용한 외부 Interrupt)을 이용하여 더
효율적으로 보드를 제어할 수 있었다.
