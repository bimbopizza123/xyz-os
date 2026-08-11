//basic hardware
unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : "a"(val), "Nd"(port));
}

void outw(unsigned short port, unsigned short val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

unsigned short inw(unsigned short port) {
    unsigned short val;
    __asm__ volatile("inw %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

//ATA disk drive

//waits until disk is ready
void WaitForDisk(void) {
    unsigned char status;
    for (int timeout = 0; timeout < 10000; timeout++) {
        status = inb(0x1F7);
        if (status == 0xFF) return;
        if(!(status & 0x80) && (status & 0x40)) return;
    }
}

//writes 512 bytes onto the disk
void WriteSection(unsigned int lba, unsigned short* buffer) {
    WaitForDisk();
    outb(0x1F6, ( 0xE0 | ((lba >> 24) & 0x0F)));
    outb(0x1F2, 1);
    outb(0x1F3, (unsigned char)lba);
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x30);
    WaitForDisk();
    for (int i = 0; i < 256; i++) {
        outw(0x1F0, buffer[i]);
    }
}

//reads 512 bytes from the disk
void ReadSection(unsigned int lba, unsigned short* buffer) {
    WaitForDisk();
    outb(0x1F6, ( 0xE0 | ((lba >> 24) & 0x0F)));
    outb(0x1F2, 1);
    outb(0x1F3, (unsigned char)lba);
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x30);
    WaitForDisk();

    for (int i = 0; i < 256; i++) {
        buffer[i] = inw(0x1F0);
    }
}

//returns lenght of string
unsigned int strlen(const char* str) {
    unsigned int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

//checks if two strings are the same, returns 1 if true, 0 if false
int equstr(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return 0;
        i++;
    }
    return (s1[i] == s2[i]);
}

//saves a text value onto the disk
void save(const char* registryname, const char* text) {
    unsigned char buffer[512];
    for (int i = 0; i < 512; i++) buffer[i] = 0;
    unsigned int lenght = strlen(text);
    if (lenght > 470) lenght = 470;
    unsigned int lenghtname = strlen(registryname);
    for (unsigned int i = 0; i < lenghtname && i < 31; i++) {
        buffer[i] = (unsigned char)registryname[i];
    }
    buffer[31] = '\0';
    buffer[32] = (lenght & 0xFF);
    buffer[33] = ((lenght >> 8) & 0xFF);
    buffer[34] = ((lenght >> 16) & 0xFF);
    buffer[35] = ((lenght >> 24) & 0xFF);

    for (unsigned int i = 0; i < lenght; i++) {
        buffer[36+i] = (unsigned char)text[i];
    }
    int destination = -1;
    unsigned char bufferx[512];
    for (int s = 1; s < 100; s++) {
        ReadSection(s, (unsigned short*)bufferx);
        char* ExistingName = (char*)bufferx;
        if (equstr(ExistingName, registryname)) {
            destination = s;
            break;
        }
        if (bufferx[0] == 0 && destination == -1) {
            destination = s;
        }
    }

    if (destination == -1) destination = 1;
    WriteSection(destination, (unsigned short*)buffer);
}  

//self explanatory
int load(char* destination, const char* registryname) {
    unsigned char buffer[512];
    for (int s =1; s=100; s++) {
        ReadSection(s, (unsigned short*)buffer);
        char* name = (char*)buffer;
        if (equstr(name, registryname)) {
            
        }
    }
}