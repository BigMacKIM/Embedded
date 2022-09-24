# 0.실험 목표

    1.정확한 장비설정 유무 확인
    2.스캐터 파일(.icf)을 통해 원하는 메모리 위치에 프로그램 다운로드 확인
      A. ROM 크기 0x80000
      B. RAM 크기 0x800
    3.레지스터 및 주소 설정 이해 확인
    4.릴레이 모듈 및 모터를 이용한 자동차 구동
      A.조이스틱 UP: 전진(딜레이만큼 릴레이 구동)
      B.User Botton1(PD11):LED1 점멸(Toggle: 딜레이만큼 커졌다 꺼짐)
        C.User Botton2(PD12):LED2 점멸(Toggle:딜레이만큼 커졌다 꺼짐)
      D.조이스틱 Selection(가운데 클릭):LED3 점멸(Toggle:딜레이 만큼 커졌다 꺼짐)
    5. 풀링 방식의 원리 및 문제점 이해 

# 1. 배경 지식
## 1.1 Scatter file

        scatter file이란 complier 와 linker들이 source를 excutable image로 변환할 때,
        linker level에서 사용자가 원하는 위치와 사이즈로  mapping할 수 있게 해주는 기능을 말 한다. 
        즉, image의 용도에 따라 적절한 memory 위치에 적재할 수 있게 해준다. 프로그램을 컴파일하면 
        크게 RO(read-only), RW(read & write), ZI(zero initialize) section으로 나뉜다.
 
        이번 실험에서는 scatter file을 이용하여 image layout을 설정한다.
 
## 1.2 Pull-up/ Pull-down 저항
        pull-up/down 저항은 일반적인 스위치가 갖는 
        floating(0인지 1인지 명확하지 않은 상태)을 방지하기 위해 사용하는 저항이다.
 
        즉, 일반적인 사항에서는 스위치가 열렸을 때 0인지 1인지 모호한 floating 상태가 된다. 
        이 문제를 해결하기 위해 Pull-up/Pull-down 저항을 사용하게 된다.
 
        pull-up은 스위치가 열린다면 ‘1’이 되고, 스위치가 눌리면 ‘0’이 된다. 
        반대로 Pull-down은 스위치가 눌린다면 ‘1’이 되고, 열리면 ‘0’이 된다.
 
## 1.3 Polling/Interrupt
        polling 과 interrupt는 프로세서와 I/O 사이의 통신 방식이다.
        polling 이란 특정 주기마다 스레드를 돌면서 시그널이 들어왔는지 확인하는 방식이다.
        즉, 스프트웨어적인 시그널 확인 방식입니다. 
        그러므로 리소스를 많이 소모하고 다른 작업 중  다른 신호를 읽을 수 없는 치명적인 단점이 존재한다.
        ex) while문을 통해 주기적인 시그널 확인.
        Interrupt 방식은  Interrupt handler가 다른 장치의 변화를 감지하고
        처리가 필요한 경우 이를 프로세서에게 알리는 방식이다.
 
## 1.4 Relay Module
        Relay Module은 전자석의 원리를 이용해 전류가 흐르면 자석이 되어 회로가 연결되고, 
        전류 가 흐르지 않으면 자성을 잃어 회로가 오픈되는 Module로, 간단히 말하면 스위치        
        역할을 하는 Module이다. 이뿐 아니라 저전력에서 고 전력으로 변환하여 사용하기 용이하다는 장점이 있다.
 
        이 Module은 크게 power LED(빨강)와 relay LED(초록)이 있다. power LED는 gnd와 vcc가 연결되면 자동적으로 점등되며,
        relay LED는 input signal이 high 일 때 점등된다. 
 
        이번 실험에서 릴레이 모듈을 사용하는 이유는 보드에서 제공하는 전력만으로 저 전력으로 구동하기 힘든 큰 크기의 외부 모듈을 handling 하기 위함이다.

### 1.4.1 Relay Module – stm32보드 회로연결
릴레이 모듈을 stm32보드로 이용하기 위한 회로 연결 방법은 다음과 같다.
      1. 먼저 VCC에 3.3V를 인가하고 모듈GND에 보드GND를 인가한다.
      2. IN에 PPT에서 제시한 PC8을 연결한다.
      3. 외부전원을 세팅하고 외부전원의 한쪽 극을 모터(큰 전원이 필요한 모듈)에 인가하고  반대쪽 극을 NO에 인가한다.
      4. COM에 모듈을 연결한다.
      해당과정을 수행할 시 보드에서 별도의 신호를 입력할 때 – 이번실험에서는 조이스틱을 UP으로 하였을 때 – 전원이 연결되어 모터가 작동하게 된다.

## 1.5 icf 파일(Linker Configuration File)에 대한 추가 설명
        IAR에서는 icf 파일을 통해 메모리 설정을 한다. 
        컴파일된 오브젝트 파일은 link 작업 시에 icf 파일을 참조하여 메모리와 관련된 정보를 가져오고 그에 맞춰 실행 코드와 
        변수를 해당 주소로 할당하게 된다.

        4주차 미션지를 보면 ROM의 크기는 0x80000, RAM의 크기는 0x8000이다.

        4주차 강의 자료를 보면 SRAM(RAM), Flash(ROM)의 시작 주소가 각각 0x20000000, 0x08000000임을 알 수 있다. 
        그러므로 시작 주소로부터 원하는 크기 -1의 값을 더해주면 각          각의 끝 주소를 구할 수 있다.

        ROM_start = 0x08000000; ROM_end = 0x08000000+(0x80000 -1) = 0x0807FFFF; 
        RAM_start = 0x20000000; RAM_end = 0x20000000+(0x8000 -1) = 0x20007FFF; 
 
# 2. 실험 과정

## 2.1 세부 실험 내용

    1. Datasheet 및 Reference Manual을 참고하여 해당 레지스터 및 주소에 대한 설정 이해
    2. 스캐터 파일을 통해 플래시 메모리에 프로그램 다운로드
    3. 플래시 메모리에 올려진 프로그램 정상적인 동작 확인

## 2.2 실험 방법
    1. J-Link와 보드를 컴퓨터에 연결한다.  
  
    2. IAR EW for Arm을 실행해 project 생성 및 기본 설정을 한다.
    ![22](https://user-images.githubusercontent.com/111568619/192112485-d4fa9f61-4647-4632-9960-13d49745fa83.png)
  
    3. 레퍼런스를 참고하여 알맞는 레지스터를 찾아 미션에서 요구하는 RAM, ROM 크기에 맞게 제공된 스캐터 파일을 적절히 수정한 뒤 업로드 한다.

      3.1. 보드의 SRAM 시작, 끝 레지스터 사이에 RAM크기(0x8000)만큼의Region을 시작, 끝 레지스터의 주소를 전달함으로써 할당한다.
  
      3.2. 보드의 Flash 시작, 끝 레지스터 사이에 ROM크기(0x80000)만큼의 Region을 시작, 끝 레지스터의 주소를 전달함으로써 할당한다.

![23](https://user-images.githubusercontent.com/111568619/192112513-4d61710c-0534-4eb4-a7e5-7cebdd18cb0b.png)

![24](https://user-images.githubusercontent.com/111568619/192112523-ffd53605-5e0f-4683-af57-90f98ac583bb.png)

    4. 임베디드 보드, 릴레이 모듈, 모터를 점퍼선을 활용하여 회로를 구성한다.

      4.1. 임베디드 보드의 GPIO PC8핀과 릴레이 모듈의 제어 신호(IN)단자를 연결한다.
      4.2. 릴레이 모듈의 공통 단자(COM)와 NO 단자를 모터 및 외부전원과 연결한다.
      4.3. 릴레이 모듈의 GND, VCC 단자를 임베디드 보드에 연결한다.


    5. 데이터시트와 레퍼런스를 참고하여 실험에 사용될 레지스터를 찾아 정의한다.

![25](https://user-images.githubusercontent.com/111568619/192112564-6eaf33d6-588f-4374-bb61-c758e96b007b.png)

    6. 각 조작(버튼 누름)에 대응하는 LED 및 모터 동작의 제어가 미션에서 
    주어진 조건에 맞게 폴링(Polling)방식으로 이루어지게끔 만들 코드를 작성한다. 
    (실험 결과 단락에  자세히 설명)

    7. 완성된 코드를 보드에 업로드한 뒤 동작을 확인한다.
    
# 3. 실험 결과 – 코드

        우리는 GPIO와 RCC를 이용한 코드 작성에 대한 편의성과 가독성을 높이기 위해 몇가지 래지스터에 대한 정의를 먼저 해주었다.
![26](https://user-images.githubusercontent.com/111568619/192112589-3c0f4810-7d97-49b5-93ca-e5701c1c6428.png)

        메인함수에서 사용할 모듈의 rest및 Input/output모드를 셋팅해주었다.
![27](https://user-images.githubusercontent.com/111568619/192112612-7eee3ed9-6250-44c8-83fa-07f36e3cd3df.png)


        이후 loop을 통해 조이스틱 또는 User Button을 이용한 기능을 구현하였다.
![28](https://user-images.githubusercontent.com/111568619/192112627-74249e7f-11dc-4950-9a7d-bee911a913e1.png)
        구현을 Polling방식으로 하여서 다른 모듈 구동시 입력한 신호를 받아들이지 못한다는 문제가 있다.

        보드에 올려 수행한 결과는 다음과 같다.
![2222](https://user-images.githubusercontent.com/111568619/192112671-cbb63d5c-ea39-44fc-8afb-2b87a2b06c65.png)

# 4. 결론

        첫 번째 실험을 통해 스케터 파일의 개념과 필요성, IAR에서 icf 파일을 통해 메모리 설정하는 법을 익힐 수 있었다.
        
        두 번째 실험에서는 새롭게 사용된 릴레이 모듈에 대해 이해하고 
        레지스터와 주소를 제어하여 미션에서 요구하는 led, user button, joy stick, motor의 동작을 구현하였다.         
        또한, Interrupt 방식과 Polling 방식의 차이를 배울 수 있었고, 
        실제 Polling 방식으로 구현된 코드를 작동시켜 봄으로써 Polling 방식에 대한 이해를 더 높일 수 있었다.           
        코드를 작성하며 사소한 실수가 있었지만, 
        지난 실험의 지식과 경험을 바탕으로 큰 어려움 없이 이번 실험을 진행할 수 있었다.


