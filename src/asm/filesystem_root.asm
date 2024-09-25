
section .data
global file_system_table_start
file_system_table_start:
    ;ata block address, type [0x00: directory, 0x01: file], name;
    filesystem_root dw 3
    db 0x00
    db "root", 0