.psx
.create "hellogpu.bin",0x80010000

; Entry Point of Code
.org 0x80010000

; ----------------------
; IO Port
; ----------------------
IO_BASE_ADDR equ 0x1F80         ; IO Ports Memory map base address

; ----------------------
; GPU Registers
; ----------------------
GP0 equ 0x1810                  ; GP0 @ $1F801810: Render data & VRAM access
GP1 equ 0x1814                  ; GP1 @ $1F801814: Display control & environment setup

Main:
    lui $t0, IO_BASE_ADDR       ; t0 = I/O port base address (mapped at 0x1F90****)

    ; -----------------------------------------------------------------------------
    ; Send commands to GP1 (mapped at 0x1F801814)
    ; The GP1 is for display control and environment setuo
    ; (Command = 8-bit MSB, Parameter = 24-bit LSB)
    ; CCPPPPPP:  CC=Command  PPPPPP=Parameter
    ; -----------------------------------------------------------------------------
    li $t1, 0x00000000          ; Command 00 = Reset GPU
    sw $t1, GP1($t0)            ; Writes the packet to GP1

    li $t1, 0x03000000          ; Command 03 = Display enable
    sw $t1, GP1($t0)            ; Write to GP1

    li $t1, 0x08000001          ; Command 08 = Display mode (320x240, 15-bit, NTSC)
    sw $t1, GP1($t0)            ; Write to GP1

    li $t1, 0x06C60260          ; Command 06 = Horizontal display range 0xXXXxxx (3168-608)
    sw $t1, GP1($t0)            ; Write to GP1

    li $t1, 0x07042018          ; Command 07 = Vertical display range %yyyyyyyyyyYYYYYYYYYY (264-24)
    sw $t1, GP1($t0)            ; Write to GP1


    ; -----------------------------------------------------------------------------
    ; Send commands to GP0 (mapped at 0x1F801810)
    ; These GP0 commands are to setup the drawing area
    ; (Command = 8-bit MSB, Parameter = 24-bit LSB)
    ; CCPPPPPP:  CC=Command  PPPPPP=Parameter
    ; -----------------------------------------------------------------------------
    li $t1, 0xE1000400          ; Command E1 = Draw mode settings
    sw $t1, GP0($t0)            ; Write to GP0

    li $t1, 0xE3000000          ; Command E3 = Drawing area top-left %YYYYYYYYYYXXXXXXXXXX (10 bits for X and 10 bits for Y)
    sw $t1, GP0($t0)            ; Write to GP0 (x=0, y=0)

    li $t1, 0xE403BD3F          ; Command E4 = Drawing area bottom-right %YYYYYYYYYYXXXXXXXXXX (10 bits for X and 10 bits for Y)
    sw $t1, GP0($t0)            ; Write to GP0 (x=319, y=239)

    li $t1, 0xE5000000          ; Command E5 = Drawing offset %YYYYYYYYYYXXXXXXXXXX (10 bits for X and 10 bits for Y)
    sw $t1, GP0($t0)            ; Write to GP0 (x=0, y=0)

    ; -----------------------------------------------------------------------------
    ; Clear the screen
    ; -----------------------------------------------------------------------------
    li $t1, 0x02000000          ; Command 02 = Fill rectangle in VRAM
    sw $t1, GP0($t0)            ; Write to GP0 (color=000000)
    li $t1, 0x00000000          ; Fill rectangle parameter: Top-left corner
    sw $t1, GP0($t0)            ; Write to GP0 (x=0, y=0)
    li $t1, 0x00EF013F          ; Fill rectangle parameter 3: Width & Height
    sw $t1, GP0($t0)            ; Write to GP0 (x=0, y=0)

    ; -----------------------------------------------------------------------------
    ; Draw a shaded triangle
    ; -----------------------------------------------------------------------------
    li $t1, 0x300000FF          ; Command 30 = Shaded three-point polygon
    sw $t1, GP0($t0)

    li $t1, 0x001000A0          ; Vertex 1
    sw $t1, GP0($t0)

    li $t1, 0x0000FF00          ; Color2
    sw $t1, GP0($t0)

    li $t1, 0x00EF013F          ; Vertex 2
    sw $t1, GP0($t0)

    li $t1, 0x00FF0000          ; Color 3
    sw $t1, GP0($t0)

    li $t1, 0x00EF0000          ; Vertex 3
    sw $t1, GP0($t0)

LoopForever:
    j LoopForever                ; Continuous loop
    nop

End:

.close