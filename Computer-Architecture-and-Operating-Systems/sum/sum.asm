global sum
section .text

;argumenty przekazane funkcji:
;rdi - wskaźnik na niepustą tablicę 64-bitowych liczb całkowitych w U2
;rsi - rozmiar tablicy n
sum:
    ;rejestr r10 będzie wskazywać jaka jest nadjalsza komórka t[] nadpisana wynikiem
    mov r10, -0x1 ;początkowo żadna komórka t nie jest nadpisana wynikiem, stąd r10 = -1

    ;zerujemy rejestr rdx - będzie to licznik pętli i
    xor rdx, rdx

    ;rejestry r8 i r9 przechowują kolejno t[i] oraz t[i + 1]
    mov r9, [rdi] ;początkowo, t[i + 1] = t[0]
   
.main_loop:
    cmp rdx, rsi ;if(i == n) jmp .exit
    jz .exit

    ;rcx = nr bitu w wynikowej liczbie na który zostanie przesunięte t[i]
    imul rcx, rdx, 0x40
    imul rcx, rdx ;rcx = i * i * 64

    mov rax, rcx
    mov r11, rdx ;zachowujemy wartośc rdx w r11 chwilowo, bo idiv może zostawić w nim śmieci
    xor rdx, rdx ;zerujemy rdx żeby dzielenie zadziałało
    idiv rsi
    mov rcx, rax ;rcx /= n
    mov rdx, r11 ;przywracamy poprawną wartość rdx

    ;r11 = komórka w wynikowej tablicy w której znajdzie się pierwszy bit t[i]
    ;ale jeśli jest to najmłodszy bit w tej komórce, to komórka-1 (przypadek .equally)
    lea r11, [rcx - 0x1] 
    ;odejmujemy 1 od numeru bitu, żeby liczby które są ,,równo'' w komórkach, ,,wpadły'' do komórki niżej po podzieleniu przez 64
    cmp rcx, 0
    jz .zero_index ;jeżeli rcx = 0, r11 = -1, bo liczba jest ,,równo" z zerowym bitem
    shr r11, 0x6 ;dla r11 >= 0 zadziała jak podzielenie przez 64 z zaookrąŋleniem w dół

.zero_index:
    dec rsi ;pomocne później, przed końcem obrotu pętli wartość zostanie przywrócona
     
    ;aktualizujemy rejestry r8 i r9 (r8 = t[i], r9 = t[i + 1])
    mov r8, r9 
    ;sprawdzamy przypadek brzegowy - jesteśmy w n-1 pozycji t i nie możemy się odwołać do większych indeksów
    cmp rsi, rdx 
    jz .last_position 
    mov r9, [rdi + 0x8*rdx + 0x8]
.last_position:

    ;rax będzie zawierać 64 razy powielony najstarszy bit aktualnie trzymanego wyniku
    xor rax, rax ;zerujemy rax
    cmp rdx, 0
    jz .zero_sign ;jeśli i == 0, po prostu rax = 0

    add [rdi + r10 * 0x8], rax ;dodajemy rax = 0, tylko po to żeby sprawdzić znak
    jns .zero_sign
    mov rax, 0xffffffffffffffff ;ostatni bit wyniku równy jeden
    .zero_sign:

    add r11, 0x2 ;chcemy zrobić sobie zapas na zapisanie wyniku żeby nie wystąpił overflow
.small_loop:
    ;mamy pewność że r11 + 2 >= r10, zatem rozszerzamy wynik o kolejne bloki
    ;ale nigdy go nie skracamy (64*(i-1)*(i-1)/n <= 64*i*i/n <= 64*i)
    cmp r10, r11
    jz .end_small_loop ;wynik jest już roszerzony aż do r11
    cmp r10, rsi
    jz .end_small_loop ;doszliśmy do końca tablicy

    inc r10
    mov [rdi + 0x8*r10], rax ;zapełniamy komórkę o numerze = r10 powielonym bitem znaku
    jmp .small_loop
.end_small_loop:

    sub r11, 0x2 ;przywracamy r11 do stanu sprzed małej pętli
    inc rsi ;przywracamy rsi pierwotną wartość = n

    ;jezeli rcx % 64 == 0 nie ma potrzeby rozdzielenia dodawanej do tablicy liczby na dwie komórki
    test rcx, 0x3f ;and z sześcioma najmłodszymi zapalonymi bitami
    jz .equally

    ;chwilowo w r11 będziemy trzymać numer pierwszego bitu w r11-tej komórce
    imul r11, 0x40

    ;rozdzielamy liczbę na dwie połówki do komórek (stare)r11 i r11 + 1
    ;rejestr rax będzie przechowywać młodszą część liczby
    mov rax, r8
    sub rcx, r11 ;rcx - r11 = przesunięcie bitowe w obrębie bloku, z przedziału 0...63
    shl rax, cl
    ;w rejestrze r8 tymczasowo będziemy trzymać starszą część liczby
    imul rcx, -0x1
    add rcx, 0x40 ;rcx = 64 - (poprzednia wartość rcx) : przesunięcie w starszej komórce
    sar r8, cl
    xor rcx, rcx ;teraz rcx posłuży nam do przechowywania carry w dalszej części kodu

    ;dodajemy uzyskane części liczb do odpowiednich komórek
    shr r11, 0x6 ;przywracamy r11>=0 do poprzedniej wartości, dzieląc przez 64 - wskazuje znowu komórkę, nie bit
    add [rdi + r11*0x8], rax ;dodanie młodszej części
    adc [rdi + r11*0x8 + 0x8], r8 ;dodanie starszej części z bitem przeniesienia z dodania młodszej części
    adc rcx, 0 ;przechowywujemy carry w rcx
   
    inc r11
    jmp .end_equally

.equally:
    ;bit na którym zacznie się liczba po przesunięciu jest najmłodszym bitem w swojej komórce
    inc r11 ;w tym przypadku r11 było mniejsze o jeden
    xor rcx, rcx ;przygotowujemy rcx do przechowania carry
    add [rdi + r11*0x8], r8 ;dodajemy liczbę równo do komórki nr r11
    adc rcx, 0 ;przeniesienie trafia do rcx
.end_equally:

    ;dodajemy potencjalne przeniesienie do komórki r11+1, jeżeli to możliwe
    inc r11 ;r11++
    cmp r11, rsi ;sprawdzamy czy r11 nie wykroczyło poza tablicę
    jz .end_of_loop
    mov rax, rcx ;rax będzie przechowywać powielony bit znaku + przeniesienie
    add r8, 0 ;sprawdzamy znak t[i] - r8 zachowało oryginalny znak t[i] niezależnie od przypadku
    jns .zero_sign_2
    add rax, 0xffffffffffffffff
.zero_sign_2:
    add qword[rdi + r11*0x8], rax
    
.end_of_loop:
    inc rdx ;i++
    jmp .main_loop

.exit:
    ret
