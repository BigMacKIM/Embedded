# 실험 과정
## 2.1 세부 실험 내용
    1. TFT-LCD를 보드에 올바르게 장착
    2. lcd.c에서 write관련 코드(TODO) 작성
    3. TFT-LCD에 Text(Team 03) 출력
    4.ADC channel과 인터럽트를 사용하여 조도 센서 값을 전역변수에 저장
    5.LCD 터치 시(main에서 폴링 방식) 해당 위치에 작은 원을 그리고 좌표(X, Y), 
전역변수에 저장했던 조도센서 값 출력
(LCD, Touch 라이브러리 파일 참고, LCD:240 x 320)

![그림1](https://user-images.githubusercontent.com/111568619/209890729-6fd800d0-0481-4bcb-8f8f-c0e691189f12.png)

## 2.2 실험 방법
    1. J-Link와 보드를 컴퓨터에 연결한다.
    2. IAR EW for Arm을 실행해 project 생성 및 기본 설정을 한다.
    3. 실험에 제공된 LCD, Touch, Font 라이브러리 파일을 프로젝트에 넣는다.
    4. lcd.c에 lcd write관련 코드를 작성한다
        - COMMAND : D/C를 Low, CS를 Low, WR을 Low로 두고 Command 전송,
	       전송 후 CS를 High, WR을 High로 돌려놓는다.
        - DATA : D/C를 High, CS를 Low, WR을 Low로 두고 Data를 Display에게 전송,
	       전송 후 CS를 High, WR을 High로 돌려놓는다.
         
![그림2](https://user-images.githubusercontent.com/111568619/209890680-719e260a-762c-4f3f-a4fd-05f56359c101.png)


    5. main에서 사용할 함수들을 정의 및 구현한다.
      - RCC_Configure() : Clock을 인가하는 함수
      - GPIO_Configure() : 사용하는 GPIO Pin을 설정하는 함수 (Pin번호, In/Out등 설정)
      - ADC_Configured() : 사용하는 ADC(ADC1)을 설정하는 함수(모드, 채널 등)
      - NVIC_Configure() : 인터럽트 우선순위를 설정하는 함수
      - ADC1_2_IRQHandler() : ADC1을 인터럽트로 사용하여 조도센서 값을 읽게 하는 함수
    6. LCD, Touch 라이브러리를 참고하여 main함수의 while문을 작성한다.
      - Team 03” Text LCD에 출력
      - LCD터치 시 해당 위치에 작은 원 출력, 조도센서 값 출력, 해당 좌표 출력

