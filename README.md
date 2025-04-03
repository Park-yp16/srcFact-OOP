# 📦 srcFacts-OOP

C++ 기반의 객체지향 정적 분석 도구로, `srcML` 형태로 변환된 XML 소스코드를 입력받아  
파일 수, 함수 수, 주석 수 등 다양한 코드 요소를 파싱하고 통계를 생성합니다.  
초기에는 모든 기능이 단일 `main` 함수에 집중된 구조였으나,  
이번 리팩토링을 통해 **객체지향 설계** 및 **제어의 역전(IoC)** 원리를 적용하여 구조적 개선을 이루었습니다.

---

## 🚀 주요 기능

- `srcML` 기반 XML 코드 파싱
- 클래스, 함수, 주석, 변수 등 주요 구성요소 수 카운팅
- **템플릿 메서드 패턴** 기반 통계 리포트 생성
- **핸들러(Handler)** 구조를 통한 출력 확장성 확보
- **대규모 코드베이스** 분석 지원 (예: Linux Kernel → 5초 이내 처리)

---

## 🔧 단계별 구현 내역

### 🔹 V1: 리팩토링 기초

- 기존 구조 유지하며 기능 개선 (예: return 문 수 통계)
- `refillContent()` 함수 분리 및 코드 스타일 정비
- XML 파싱 관련 기능 → `xml_parser.hpp/.cpp`로 추출

### 🔹 V2: 모듈화 및 클래스화

- `XMLParser` 클래스 생성 및 namespace 분리
- 내부 변수 멤버화 → 파라미터 축소 및 상태 보존
- 독립적으로 동작 가능한 구조로 개선

### 🔹 V3: IoC 구조 및 응용 확장

- **람다 기반 이벤트 핸들러 등록 방식** 설계
- `XMLParser::parse()` 실행 시 외부 핸들러 주입
- 파생 프로그램 개발:
  - `xmlstats`: XML 구조적 요소 통계 리포트 생성
  - `identity`: 입력 XML을 동일한 구조로 재출력

---

## 🧠 객체지향 설계 포인트

| 설계 원리 | 적용 방식 |
|-----------|------------|
| **IoC (제어의 역전)** | XML 이벤트 핸들러를 외부에서 등록받아 실행 흐름 역전 |
| **Template Method Pattern** | 리포트 생성 알고리즘을 추상화하여 다양화 가능 |
| **Handler Pattern** | 결과 출력 로직을 독립 모듈로 분리 |
| **UML 기반 설계** | 시퀀스 다이어그램, 클래스 다이어그램 기반 설계 문서 작성 |

---
