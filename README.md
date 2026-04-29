# libasm

42 본 과정의 어셈블리 입문 프로젝트. 평소 C에서 호출하던 libc 함수 일부를 **NASM x86_64 어셈블리**로 직접 다시 짜고, `ar`로 묶어 정적 라이브러리 **`libasm.a`** 를 만든다. 이 프로젝트의 학습 핵심은:

- System V AMD64 호출 규약 (인자가 `rdi`/`rsi`/`rdx`/...에 들어오는 것)
- macOS BSD syscall ABI (Linux와 다른 syscall 번호 인코딩)
- `errno` 같은 libc 글로벌 상태에 어셈블리에서 어떻게 접근하는지

> 원본 과제 명세: [`subject.pdf`](./subject.pdf) (영문)

## 과제 개요 / 요구사항

- **결과물**: `libasm.a` (정적 라이브러리)
- **언어**: NASM 어셈블리 (`*.s` 파일)
- **타깃 플랫폼**: **macOS x86_64** — `nasm -f macho64`로 Mach-O 64-bit 오브젝트 생성
- **mandatory (6개 함수)** — 모두 표준 libc 함수의 시그니처·동작을 그대로 재현

  | 함수 | 시그니처 | 비고 |
  |---|---|---|
  | `ft_strlen` | `size_t ft_strlen(const char *)` | 단순 카운트 |
  | `ft_strcpy` | `char *ft_strcpy(char *dst, const char *src)` | NUL 포함 복사 |
  | `ft_strcmp` | `int ft_strcmp(const char *, const char *)` | 첫 다른 바이트의 차이 반환 |
  | `ft_strdup` | `char *ft_strdup(const char *)` | `malloc` 호출 + 실패 시 `errno` 설정 |
  | `ft_write` | `ssize_t ft_write(int fd, void *, size_t)` | `write(2)` syscall, 실패 시 `-1` + `errno` |
  | `ft_read` | `ssize_t ft_read(int fd, void *, size_t)` | `read(2)` syscall, 실패 시 `-1` + `errno` |

- **bonus** (선택, 본 레포는 미구현)
  - `ft_atoi_base` — 임의 진수 문자열 → 정수
  - 단방향 연결 리스트: `ft_list_push_front`, `ft_list_size`, `ft_list_sort`, `ft_list_remove_if`

- **공통 제약**
  - libc 함수를 호출해서 우회하지 말 것 (다만 `ft_strdup`은 `malloc` 사용 가능, syscall 함수는 직접 syscall)
  - **에러 시 `errno` 정확히 설정** — `ft_strdup`/`ft_write`/`ft_read`
  - Norminette 적용 안 됨 (어셈블리 파일이라 코드 스타일은 자유)

## 레포 구조

```
.
├── Makefile           # nasm -f macho64 + ar rcs libasm.a
├── libasm.h           # 6개 함수 선언만 — main.c에서 include
├── ft_strlen.s
├── ft_strcpy.s
├── ft_strcmp.s
├── ft_strdup.s        # _malloc, _ft_strlen, _ft_strcpy, ___error 호출
├── ft_write.s         # syscall + ___error
├── ft_read.s          # syscall + ___error
├── main.c             # 전 함수 케이스별 검증 + 표준 libc 결과와 비교
└── hello.txt          # ft_write 테스트용 (빈 파일)
```

## 풀이 노트

### macOS의 어셈블리 함정 4가지

다른 OS(특히 Linux)와 ABI가 다른 부분이 학습 자체. 본 코드는 macOS 관행을 그대로 따른다:

1. **함수 심볼 prefix `_`** — macOS는 C 함수가 어셈블리 레벨에서 underscore 하나가 앞에 붙는다. C에서 `ft_strlen`을 호출하면 어셈블리는 `_ft_strlen`이라는 심볼을 찾는다.

   ```asm
   global  _ft_strlen      ; C의 ft_strlen() ↔ 이 심볼
   ```

2. **syscall 번호 인코딩** — macOS BSD syscall은 `(syscall_class << 24) | syscall_number` 형식. `write(2)`는 BSD class(2) + number(4) = `0x2000004`.

   ```asm
   mov rax, 0x2000004       ; write
   syscall
   ```

   같은 함수가 Linux ELF에선 `mov rax, 1` (syscall 번호 1)로 끝난다 — ABI가 완전히 다름.

3. **에러 검사는 carry flag** — macOS BSD syscall 규약은 **실패 시 CF=1로 반환하고 `rax`에 errno 값**을 둔다. 그래서 `jc`(jump if carry)로 분기:

   ```asm
   syscall
   jc  _err                 ; 실패면 _err로
   ret                      ; 성공이면 그대로 (rax에 결과 바이트 수)
   ```

   Linux는 같은 syscall이 성공 시 결과·실패 시 `-errno`를 `rax`에 바로 넣는 다른 규약.

4. **`errno`는 함수 호출로 가져옴** — `errno`는 단순 전역 변수가 아니라 thread-local. macOS는 `int *__error(void)`(어셈블리 심볼 `___error` — underscore 3개)로 현재 스레드의 `errno` 주소를 받아온다:

   ```asm
   _err:
       push rax            ; syscall이 rax에 둔 errno 값을 잠시 저장
       call ___error       ; rax ← &errno
       pop rdx             ; rdx ← 저장해둔 errno 값
       mov [rax], rdx      ; *errno_ptr = errno 값
       mov rax, -1         ; 표준 libc는 실패 시 -1 반환
       ret
   ```

   Linux는 `__errno_location`이 같은 역할.

### 함수별 메모

- **`ft_strlen`** — `rax`를 카운터로 두고 `BYTE [rdi + rax]`를 NUL이 나올 때까지 읽으며 `inc rax`. 가장 단순한 형태.

- **`ft_strcmp`** — 두 문자를 `al`·`bl`에 로드해 `cmp` → 다르면 `sub rax, rbx`로 차이 반환, 같으면 `cmp al, 0`로 NUL 도달 검사. 같은 문자열이면 `mov rax, 0` 후 `ret`.

- **`ft_strdup`** — 어셈블리 함수가 다른 함수를 호출할 때의 인자·복귀값 보존이 핵심:

  ```asm
  push rdi                ; 원본 src 포인터 잠시 저장
  call _ft_strlen         ; rax = 길이
  inc rax                 ; +1 for NUL
  mov rdi, rax            ; rdi = 할당 크기 (1st arg of malloc)
  call _malloc            ; rax = 새 버퍼
  jc _err                 ; 실패 시 errno 설정 후 -1 반환
  mov rdi, rax            ; rdi = dst (1st arg of strcpy)
  pop rsi                 ; rsi = src (원래 인자, 2nd arg)
  call _ft_strcpy         ; tail call — strcpy의 반환값(=dst)을 그대로 반환
  ret
  ```

- **`ft_write`/`ft_read`** — 호출 규약상 `rdi`/`rsi`/`rdx`에 이미 fd/buf/count가 들어와 있으므로 syscall 번호만 `rax`에 넣고 `syscall`. 결과·에러 처리만 추가.

### Makefile — 빌드 단계

```make
%.o : %.s
    nasm -f macho64 $<      # Mach-O 64-bit object 생성

$(NAME) : $(OBJS)
    ar rcs $(NAME) $(OBJS)  # 정적 라이브러리로 묶기
```

`test` 타겟은 `gcc main.c -L. -lasm`으로 라이브러리 링크해 검증 바이너리를 만든다.

## 빌드 / 사용법

```sh
make            # libasm.a 생성
make test       # libasm.a + main.c → ./test (검증 실행 파일)
./test          # 모든 함수에 대해 표준 libc 결과와 나란히 출력 비교
make clean      # .o 제거
make fclean     # libasm.a, test까지 제거
make re         # fclean + all
```

`main.c`는 케이스별로 `ft_strlen` / `ft_strcpy` / `ft_strcmp` / `ft_write` / `ft_read` / `ft_strdup`를 호출하고, 각 결과를 표준 libc 결과와 같은 줄에 인쇄해 눈으로 비교할 수 있게 짜여 있다 (NUL 중간 포함, 빈 문자열, 잘못된 fd, 큰 입력 등).

## 메모

- 본 레포는 **macOS 전용**. Linux로 옮기려면:
  - `_` prefix 제거
  - `nasm -f elf64`로 빌드
  - syscall 번호 변경 (`write` = 1, `read` = 0)
  - 에러 처리: `jc` 대신 `cmp rax, 0; jl _err` + `__errno_location` 사용
- `Makefile`의 `fclean` 룰에 `$(NAME)`이 두 번 적혀 있다 — 동작에는 무해하지만 중복.
- bonus(`ft_atoi_base` + 연결 리스트 4종)는 미구현. mandatory만 통과.
- `subject.pdf`는 [evgenkarlson/ALL_SCHOOL_42 — Libft-ASM](https://github.com/evgenkarlson/ALL_SCHOOL_42/tree/master/00_Projects__(%D0%9E%D1%81%D0%BD%D0%BE%D0%B2%D0%BD%D0%BE%D0%B5_%D0%9E%D0%B1%D1%83%D1%87%D0%B5%D0%BD%D0%B8%D0%B5)/01_Unix/10_1_0____Libft-ASM)의 영문 판본 (libasm new).
