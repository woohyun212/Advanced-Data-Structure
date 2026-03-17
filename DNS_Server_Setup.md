# 내부망 DNS 서버 구성 가이드 (BIND9 + Raspberry Pi OS)

> 내부망 루프백 문제를 해결하기 위해 Raspberry Pi OS에 BIND9 기반 내부 DNS 서버를 구성하는 절차를 정리한 문서입니다.

---

## 목차

1. [환경 개요](#1-환경-개요)
2. [사전 준비](#2-사전-준비)
3. [BIND9 설치](#3-bind9-설치)
4. [네트워크 인터페이스 고정 IP 설정](#4-네트워크-인터페이스-고정-ip-설정)
5. [BIND9 기본 설정 (`named.conf.options`)](#5-bind9-기본-설정-namedconfoptions)
6. [로컬 존 선언 (`named.conf.local`)](#6-로컬-존-선언-namedconflocal)
7. [정방향 조회 존 파일 작성](#7-정방향-조회-존-파일-작성)
8. [역방향 조회 존 파일 작성](#8-역방향-조회-존-파일-작성)
9. [설정 검증 및 서비스 재시작](#9-설정-검증-및-서비스-재시작)
10. [클라이언트 DNS 설정](#10-클라이언트-dns-설정)
11. [동작 확인 및 테스트](#11-동작-확인-및-테스트)
12. [방화벽 설정](#12-방화벽-설정)
13. [문제 해결 (Troubleshooting)](#13-문제-해결-troubleshooting)

---

## 1. 환경 개요

| 항목 | 내용 |
|------|------|
| OS | Raspberry Pi OS (Debian 기반, 64-bit 권장) |
| DNS 소프트웨어 | BIND9 (Berkeley Internet Name Domain v9) |
| 내부 도메인 예시 | `home.local` |
| DNS 서버 IP 예시 | `192.168.1.100` |
| 내부 네트워크 대역 예시 | `192.168.1.0/24` |

> **참고**: 아래 예시에서 IP 주소(`192.168.1.x`), 도메인(`home.local`)은 실제 환경에 맞게 변경하세요.

---

## 2. 사전 준비

### 2-1. 시스템 패키지 업데이트

```bash
sudo apt update && sudo apt upgrade -y
```

### 2-2. 현재 네트워크 인터페이스 확인

```bash
ip addr show
# 또는
hostname -I
```

### 2-3. 현재 DNS 설정 확인

```bash
cat /etc/resolv.conf
```

---

## 3. BIND9 설치

```bash
sudo apt install -y bind9 bind9utils bind9-doc dnsutils
```

### 설치 확인

```bash
named -v
# 예시 출력: BIND 9.18.x (Stable Release) ...
```

### 서비스 상태 확인

```bash
sudo systemctl status bind9
```

---

## 4. 네트워크 인터페이스 고정 IP 설정

DNS 서버는 IP가 고정되어야 합니다. Raspberry Pi OS에서는 `dhcpcd` 또는 `NetworkManager`를 사용합니다.

### `dhcpcd` 방식 (기본값)

```bash
sudo nano /etc/dhcpcd.conf
```

파일 하단에 아래 내용을 추가합니다 (인터페이스명은 `ip addr`로 확인):

```
interface eth0
static ip_address=192.168.1.100/24
static routers=192.168.1.1
static domain_name_servers=127.0.0.1 8.8.8.8
```

설정 적용:

```bash
sudo systemctl restart dhcpcd
ip addr show eth0
```

---

## 5. BIND9 기본 설정 (`named.conf.options`)

```bash
sudo nano /etc/bind/named.conf.options
```

아래 내용으로 수정합니다:

```conf
options {
    directory "/var/cache/bind";

    // 재귀 질의를 허용할 클라이언트 범위
    recursion yes;
    allow-recursion { 127.0.0.1; 192.168.1.0/24; };

    // 외부 DNS 포워더 (상위 DNS 서버)
    forwarders {
        8.8.8.8;    // Google DNS
        8.8.4.4;    // Google DNS (보조)
        1.1.1.1;    // Cloudflare DNS
    };
    forward only;

    // 모든 인터페이스에서 수신
    listen-on { any; };
    listen-on-v6 { any; };

    // 버전 정보 숨김 (보안)
    version "not disclosed";

    // DNSSEC 검증 비활성화 (내부망 전용)
    dnssec-validation no;
};
```

---

## 6. 로컬 존 선언 (`named.conf.local`)

```bash
sudo nano /etc/bind/named.conf.local
```

아래 내용을 추가합니다:

```conf
// 정방향 조회 존 (도메인 → IP)
zone "home.local" {
    type master;
    file "/etc/bind/zones/db.home.local";
};

// 역방향 조회 존 (IP → 도메인)
// 내부 대역이 192.168.1.x 인 경우
zone "1.168.192.in-addr.arpa" {
    type master;
    file "/etc/bind/zones/db.192.168.1";
};
```

---

## 7. 정방향 조회 존 파일 작성

### 존 파일 디렉터리 생성

```bash
sudo mkdir -p /etc/bind/zones
```

### 존 파일 생성

```bash
sudo nano /etc/bind/zones/db.home.local
```

아래 내용을 입력합니다:

```dns
;
; 정방향 조회 존: home.local
;
$TTL    604800
@   IN  SOA ns1.home.local. admin.home.local. (
                  2024010101  ; Serial (YYYYMMDDNN 형식으로 변경 시 증가)
                  3600        ; Refresh (1시간)
                  1800        ; Retry (30분)
                  604800      ; Expire (1주일)
                  86400 )     ; Negative Cache TTL (1일)

; 네임서버 레코드
@       IN  NS      ns1.home.local.

; 네임서버 A 레코드
ns1     IN  A       192.168.1.100

; 호스트 레코드 (예시)
@       IN  A       192.168.1.100
router  IN  A       192.168.1.1
pi      IN  A       192.168.1.100
server  IN  A       192.168.1.200
```

---

## 8. 역방향 조회 존 파일 작성

```bash
sudo nano /etc/bind/zones/db.192.168.1
```

아래 내용을 입력합니다:

```dns
;
; 역방향 조회 존: 192.168.1.0/24
;
$TTL    604800
@   IN  SOA ns1.home.local. admin.home.local. (
                  2024010101  ; Serial
                  3600        ; Refresh
                  1800        ; Retry
                  604800      ; Expire
                  86400 )     ; Negative Cache TTL

; 네임서버 레코드
@       IN  NS      ns1.home.local.

; PTR 레코드 (마지막 옥텟만 사용)
1       IN  PTR     router.home.local.
100     IN  PTR     pi.home.local.
200     IN  PTR     server.home.local.
```

---

## 9. 설정 검증 및 서비스 재시작

### 설정 파일 문법 검사

```bash
sudo named-checkconf
```

오류가 없으면 아무 출력도 나타나지 않습니다.

### 존 파일 검사

```bash
sudo named-checkzone home.local /etc/bind/zones/db.home.local
sudo named-checkzone 1.168.192.in-addr.arpa /etc/bind/zones/db.192.168.1
```

정상 출력 예시:

```
zone home.local/IN: loaded serial 2024010101
OK
```

### BIND9 서비스 재시작 및 자동 시작 등록

```bash
sudo systemctl restart bind9
sudo systemctl enable bind9
sudo systemctl status bind9
```

---

## 10. 클라이언트 DNS 설정

### Raspberry Pi 자신이 DNS를 사용하도록 설정

```bash
sudo nano /etc/resolv.conf
```

```
nameserver 127.0.0.1
nameserver 8.8.8.8
```

> **참고**: `/etc/resolv.conf`는 `dhcpcd`에 의해 덮어씌워질 수 있습니다.  
> 영구 적용을 위해서는 `/etc/dhcpcd.conf`의 `static domain_name_servers=127.0.0.1 8.8.8.8`으로 설정하세요.

### 다른 클라이언트(Windows / Linux / macOS) DNS 설정

각 기기의 네트워크 어댑터 DNS 서버 주소를 `192.168.1.100`(DNS 서버 IP)으로 변경합니다.

- **Windows**: `네트워크 및 인터넷 설정 → 어댑터 옵션 변경 → IPv4 속성 → 기본 설정 DNS 서버`
- **Linux (systemd-resolved)**: `/etc/systemd/resolved.conf`에 `DNS=192.168.1.100` 추가 후 `sudo systemctl restart systemd-resolved`
- **macOS**: `시스템 설정 → 네트워크 → DNS 탭`에서 추가

---

## 11. 동작 확인 및 테스트

### 정방향 조회 테스트

```bash
# DNS 서버에 직접 질의
dig @192.168.1.100 pi.home.local
dig @192.168.1.100 server.home.local

# 외부 도메인 포워딩 테스트
dig @192.168.1.100 google.com
```

### 역방향 조회 테스트

```bash
dig @192.168.1.100 -x 192.168.1.100
```

### nslookup으로 테스트

```bash
nslookup pi.home.local 192.168.1.100
nslookup 192.168.1.100 192.168.1.100
```

### 정상 응답 예시 (`dig` 출력)

```
;; ANSWER SECTION:
pi.home.local.		604800	IN	A	192.168.1.100
```

---

## 12. 방화벽 설정

Raspberry Pi OS에 `ufw`가 설치된 경우 DNS 포트(UDP/TCP 53)를 허용합니다:

```bash
sudo apt install -y ufw

# DNS 포트 허용
sudo ufw allow 53/udp
sudo ufw allow 53/tcp

# SSH 허용 (원격 접속 유지)
sudo ufw allow 22/tcp

# 방화벽 활성화
sudo ufw enable
sudo ufw status
```

---

## 13. 문제 해결 (Troubleshooting)

### 로그 확인

```bash
sudo journalctl -u bind9 -f
# 또는
sudo tail -f /var/log/syslog | grep named
```

### 자주 발생하는 오류

| 오류 메시지 | 원인 | 해결 방법 |
|-------------|------|-----------|
| `zone ... loading from master file ... failed` | 존 파일 문법 오류 | `named-checkzone`으로 존 파일 검사 |
| `couldn't add command channel 127.0.0.1#953` | rndc 포트 충돌 | `sudo systemctl stop bind9` 후 재시작 |
| `permission denied` | 파일 권한 문제 | `sudo chown -R bind:bind /etc/bind/zones` |
| `SERVFAIL` 반환 | 포워더 연결 불가 | 인터넷 연결 및 포워더 IP 확인 |
| `NXDOMAIN` 반환 | 존 레코드 없음 | 존 파일에 해당 호스트 레코드 추가 |

### 파일 권한 수정

```bash
sudo chown -R root:bind /etc/bind
sudo chmod -R 755 /etc/bind
sudo chown bind:bind /etc/bind/zones/db.home.local
sudo chown bind:bind /etc/bind/zones/db.192.168.1
```

### Serial 번호 갱신 (존 파일 수정 후 반드시 증가)

존 파일을 수정할 때마다 `Serial` 값을 증가시켜야 변경 사항이 적용됩니다:

```
; 수정 전
2024010101
; 수정 후 (오늘 날짜 기준으로 증가)
2024010102
```

수정 후 서비스 재시작:

```bash
sudo systemctl reload bind9
```

---

## 요약 체크리스트

- [ ] `sudo apt update && sudo apt upgrade -y`
- [ ] `sudo apt install -y bind9 bind9utils dnsutils`
- [ ] 고정 IP 설정 (`/etc/dhcpcd.conf`)
- [ ] `/etc/bind/named.conf.options` 수정 (재귀 질의, 포워더 설정)
- [ ] `/etc/bind/named.conf.local` 수정 (존 선언)
- [ ] `/etc/bind/zones/db.home.local` 생성 (정방향 존)
- [ ] `/etc/bind/zones/db.192.168.1` 생성 (역방향 존)
- [ ] `sudo named-checkconf` 및 `sudo named-checkzone` 검증
- [ ] `sudo systemctl restart bind9 && sudo systemctl enable bind9`
- [ ] `dig` / `nslookup` 으로 동작 확인
- [ ] 클라이언트 DNS 주소를 `192.168.1.100` 으로 변경
- [ ] 방화벽 포트 53 허용 (필요 시)
