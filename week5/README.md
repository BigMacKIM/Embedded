
## 1. 배경 지식
### 1.1 Clock
Clock 이란 컴퓨터 등과 같은 디지털 시스템 내에서 전자 회로 또는 장치로서, 같은 시스템
내에서 동작하는 각 구성요소의 모든 동작의 동기화와 특정 조작을 위한 동작 시간 조절을
해주는 것이다.
우리가 사용하는 보드는 3 가지의 SYSCLK 을 가지고 있다. 각각 HSI, HSE, PLL clock 이다.
HIS - High Speed Internal 의 약자로, 보드 내부에 존재하고 8Mhz RC 오실레이터에서 생성된다.
HSE - High Speed external 의 약자로, 말 그대로 보드 외부에서 입력되고 25Mhz 의 주파수
클럭을 생성한다.
PLL - Phase Lock Loop 의 약자로, 위의 HSI 이나 HSE clock 을 증폭 또는 축소시켜 알맞게 clock
주파수를 맞추기 위해 사용한다.
이번 실험에서는 25Mhz Crystal 을 가진 보드의 HSE clock 을 알맞게 증폭/축소시켜 내가
원하는 주파수를 PLL clock 으로 받아와 사용할 것이다.

### 1.2 Clock Tree
Clock Tree 는 위 Clock 이 회로의 어디에 위치하고, 어떤 회로를 거쳐서 출력되는지
보여주는 일종의 회로도 이다.
HSI, HSE,PLL 중 SW MUX 를 통해 시스템 클럭을 설정해줄 수 있게 해준다. 생성된 시스템
클럭은 MOC 를 통해 오실로스코프로 확인이 가능하다.

### 1.3 시리얼 통신
시리얼 통신이란 하나의 데이터 선을 이용해 데이터를 차례로 보내는 방법이다. 속도는
느리나, 데이터 선을 연장하기 위한 비용이 적게 든다는 장점이 있다.
우리는 이번 실험에서 컴퓨터와의 UART 시리얼 통신을 할 것이다. PC와 Clock의 주파수 등
여러 가지 설정을 맞춰 줌으로서 PC와의 통신이 가능하다. 먼저 UART 통신에 대해서 간단한
설명을 하자면 비동기식 통신이고 직렬로 데이터를 송수신하기 때문에, 따로 clock 동기화에
필요한 clock 핀 연결을 요구하지 않고, 하나의 연결로만 데이터를 주고받는다.
데이터 비트의 크기는 8bit로 설정하고 보드 내부의 clock 주파수를 조절해서 PC와의
통신을 연결한다. 보드의 PORT A의 9번과 10번이 각각 TX와 RX의 역할을 하는데 우리는
TX를 통해서 데이터를 전달하고, RX를 통해서 데이터를 받는다.
PC는 putty를 통해서 상호간의 약속을 설정하고 통신을 시도한다.

### 1.4 Baud rate
보율이란 전송 속도 비율이라고도 하며, 시리얼 통신에서 데이터의 전송 속도를 나타내는
단위이고 , 1초간 전송되는 신호의 수로 나타낸 값을 말한다. 이를 계산해 주어 PC와 보드
간의 통신을 속도를 동기화 해줄 것이다.


# 2. 실험 과정
### 2.1 세부 실험 내용
    1. Datasheet 및 Reference Manual 을 참고하여 해당 레지스터 및 주소에 대한 설정 이해
    2. 반드시 HSE OSC 의 25MHz 클럭을 사용하며, PREDIV2 와 PLL2MUL 을 포함시킬 것
    3. 예제 코드에서 설정되는 Clock 값을 파악하고, 지정된 Clock 으로 설정
    4. 예제 설정 항목에 따라 UART 를 설정하고, 지정된 Baud rate 로 설정
    SYSCLK : 32MHz
    PCLK2 : 16MHz
    Baud Rate : 14400
    5. User S1 버튼을 누르는 동안 터미널 프로그램(Putty)을 통해 “Hello TeamXX”을 출력 후
    줄 바꿈 (다음 “Hello TeamXX”는 다음 줄에서 출력될 수 있도록)
    6. MCO 를 통해 나오는 System Clock 을 오실로스코프로 수치 확인
### 2.2 실험 방법
    1. J-Link 와 보드를 컴퓨터에 연결한다.
    2. IAR EW for Arm 을 실행해 project 생성 및 기본 설정을 한다.
    3. 코드 작성 시, stm32f10x.h 헤더 파일에 정의된 변수명 또는 함수명을 참고하여 이를
    활용해 정의된 주소 값을 사용한다.
    4. Reference Manual 의 Clock Tree 를 참고하고 PLL 을 활용해 HSE OSC 에서 생성된
    25MHz 클럭을 SYSCLK 에 32MHz 클럭으로, PCLK2 에 16MHz 클럭으로 만들어 입력한다.
    5. User S1 버튼과 UART 를 사용할 수 있게끔 GPIO 포트에 클럭을 인가하고 각 핀에 대
    해 초기화 및 적절한 MODE, CNF 를 설정한다.
    6. Reference Manual 의 Baud rate 계산 예제를 참고하여 14400 의 Baud rate 를 사용할 것
    을 염두에 두고 USART_BRR 에 입력될 값을 계산하여 입력한다.
    7. 코드의 main 함수 while 문 내에 동작에 맞는 적절한 제어문을 작성한다.
    8. 임베디드 보드의 Serial Port 를 PC 와 연결하고 PC 에서 putty 를 활용하여 시리얼 통신
    이 이루어지는지 확인한다.
    9. MCO MUX 에서 SYSCLK 을 출력하게 끔 만들고, 임베디드 보드의 MCO 핀과 오실로스코
    프를 연결하여 의도한대로 SYSCLK 이 32MHz 로 만들어졌는지 확인한다.
