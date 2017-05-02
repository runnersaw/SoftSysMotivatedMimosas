/*
We used a variety of references to create this, ranging from documentation to other
implementations of NM that were much more complex than ours. Much of the code below is
modified from multiple sources.

lseek: http://pubs.opengroup.org/onlinepubs/7908799/xsh/lseek.html

elf.h source: http://www.mcs.anl.gov/OpenAD/OpenADFortTkExtendedDox/elf_8h_source.html

mmap: http://pubs.opengroup.org/onlinepubs/007904875/functions/mmap.html

elf format sections: http://www.shrubbery.net/solaris9ab/SUNWdev/LLM/p38.html

elf man page: http://man7.org/linux/man-pages/man5/elf.5.html

symbol table constants: http://www.sco.com/developers/gabi/2000-07-17/ch4.symtab.html

a sample NM and objdump implementation where much of the logic for accessing the correct 
areas of the ELF data was realized: https://github.com/nsarno/nm-objdump/

nm man page for determining reasonble return functionality: https://linux.die.net/man/1/nm

*/
#include <elf.h>
//Need this to read elf files
#include <stdio.h>
#include <fcntl.h>
//Need this for open()
#include <unistd.h>
//Need this for SEEK_END and lseek
#include <sys/mman.h>
//Need this to use mmap and related constants
#include <malloc.h>
#include <string.h>

//All ELF files start with this magic number
#define ELF_ID "\177ELF"

// This is our smalles symbol unit
// It defines the location of a symbol table entry
// It uses the elf.h provided types Elf64_Sym to hold
typedef struct s_symloc {
    //Start of the symbol table entry
    Elf64_Sym *start;
    //End of the symbol table entry (aka the next entry)
    Elf64_Sym *end;
    // The string table associated with the symbol
    char *strtab;

}    symloc;

// This is our local version of Elf64_Sym
// It hold only the values we care about, namely 
// Value, Type and Name
typedef struct s_symbol {

    unsigned int value;
    char type;
    char const *name;

}    symbol;

// This is how we store our "symbol table"
// We use the built in classes to store headers
typedef struct s_elf_struct {
    // The ELF file header
    Elf64_Ehdr const *ehdr;
    // The ELF section headers (this is technically an array)
    Elf64_Shdr const *shdr;
    // The length of the symbol table
    unsigned int len;
    // The end of our symbol table
    void *end;
    // A symbol location reference for storing a single symbol
    symloc sym;

}    elf_struct;

// This method reads basic information about the ELF header into
// our ELF struct
int read_file(char const *file, elf_struct *elf) {

    int f;

    // Open the file as read only
    f = open(file, O_RDONLY);

    // lseek puts the file offset to the end and returns that offset (the file length)
    elf->len = lseek(f, 0, SEEK_END);

    // We map our address memory to that of the file
    // This returns the address of the mapping, which contains the "file" elf header
    // in our own address space. PROT_READ is for reading, MAP_SHARED means
    // changes in one are propogated to both.
    elf->ehdr = mmap(NULL, elf->len, PROT_READ, MAP_SHARED, f, 0);

    // Find the end of the ELF table
    elf->end = (void *)elf->ehdr + elf->len;

    //Do a check here to make sure we actually have an ELF file
    // We check the length of the header and then compare the ELF_ID to the header's ID
    if (elf->len < sizeof(Elf64_Ehdr) || memcmp(elf->ehdr, ELF_ID, sizeof(ELF_ID)-1)) {
	    printf("Error: Not an ELF format file");
	    return 0;
    }
    return 1;
}

// A helper function to determine the letter NM associates with the Elf64_Sym type given
char find_type(elf_struct *elf, Elf64_Sym *sym){
    //st_shndx holds the relative position of the table entry in a section
    //If it is one of a few special values, then the symbol is important to us

    //a common unallocated block
    if (sym->st_shndx == SHN_COMMON) return ('C');
    
    //a symbol of absolute value that will not change due to relocation
    if (sym->st_shndx == SHN_ABS) return ('A');
    
    //an undefined symbol
    if (sym->st_shndx == SHN_UNDEF) return ('U');
    
    //SHT_Null identifies the section as inactive
    //No flags are enabled
    if (elf->shdr[sym->st_shndx].sh_type  == SHT_NULL &&
        elf->shdr[sym->st_shndx].sh_flags == 0) {
        return 'U';
    }
    //SHT_NOBITS: section that occupies no space
    //SHF_ALLOC: memory during process execution
    //SHF_WRITE: contains data that should be writable during process execution
    //NM: unitialized data section
    if (elf->shdr[sym->st_shndx].sh_type  == SHT_NOBITS &&
        elf->shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) {
        return 'B';
    }
    //SHT_PROGBITS: information defined by the program
    //SHF_ALLOC: memory used during process execution
    if (elf->shdr[sym->st_shndx].sh_type  == SHT_PROGBITS &&
        elf->shdr[sym->st_shndx].sh_flags == SHF_ALLOC) {
        return 'R';
    }
    //See above labels
    if (elf->shdr[sym->st_shndx].sh_type  == SHT_PROGBITS &&
        elf->shdr[sym->st_shndx].sh_flags == 0) {
        return 'R';
    }
    //See above labels
    //NM: initialized data section
    if (elf->shdr[sym->st_shndx].sh_type  == SHT_PROGBITS &&
        elf->shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) {
        return 'D';
    } 
    // SHF_EXECINSTR: contains executable machine instructions
    // This is what our functions will be labeled as
    if (elf->shdr[sym->st_shndx].sh_type  == SHT_PROGBITS &&
        elf->shdr[sym->st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) {
        return 'T';
    }
    return ('?');
}   

//This loads an Elf64_Sym into our own symbol type
int load_symbol (elf_struct *elf, Elf64_Sym *sym, symbol *symbol) {
    symbol->value = (unsigned int)sym->st_value;
    symbol->type = find_type(elf, sym);

    //Lower case letters can mean that the type is local instead of global
    if (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) {
        //Lowercase it if it already isn't
        if (symbol->type >= 'A' && symbol->type <= 'Z'){
            symbol->type = symbol->type + 0x20;
        }
    }
    //Much like a global, but weaker. Has a special letter
    if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
        symbol->type = 'w';
    }
    //Pull the name from the string table
    symbol->name = &elf->sym.strtab[sym->st_name];
    return 1;
}


//The function loops through all of the symbol entries and loads them into
// an array of symbols
symbol* read_symbols(elf_struct *elf){
    
    //We define our section headers using the section offset included 
    // in the ELF header (This is an array)
    elf->shdr = (Elf64_Shdr *)((void *)elf->ehdr + elf->ehdr->e_shoff);
    int i = -1;

    // Loop through our sections using e_shnum, the number of section header table
    // entry count
    while (++i != elf->ehdr->e_shnum){
        // Make sure that the section header type is a symbol table
        if (elf->shdr[i].sh_type == SHT_SYMTAB){
            // We use elf->sym to store the symbol table we want to look at
            // Start from the ELF header and use the section header offset to move the start
            elf->sym.start = (Elf64_Sym *)((void *)elf->ehdr + elf->shdr[i].sh_offset); 
            
            // Start from the end and add the section header size
            elf->sym.end = (Elf64_Sym *)((void *)elf->sym.start + elf->shdr[i].sh_size);
        
            // Set the string table by using the sh_link to access the section containing the
            // string table
            elf->sym.strtab = (char *)elf->ehdr + elf->shdr[elf->shdr[i].sh_link].sh_offset;
        }
    }

    symbol *symbol_list;
    Elf64_Sym *tmp;
    
    symbol_list = malloc(sizeof(symbol) * (elf->sym.end - elf->sym.start));
    
    memset(symbol_list, 0, sizeof(symbol) * (elf->sym.end - elf->sym.start));

    //allocate memory to our symbol list and set it all to 0

    tmp = elf->sym.start;
    i = 0;
    while (tmp < elf->sym.end) {
        if (tmp->st_info != STT_FILE &&
            tmp->st_info != STT_SECTION &&
            tmp->st_info != STT_NOTYPE) {
            // st_info tells us about the symbol type and binding
            // We don't want any file, section or non-typed symbols
            load_symbol(elf, tmp, &symbol_list[i]);
            // only increment if we actually added something
            i++;
        }
        // We can move through our different symbols by incrementing from our start sym
        // Fuzzy on the details, but I think this is akin to pointer arithmetic?
        tmp++;
    }

    return (symbol_list);
}

int print_function_symbol(char* file, char* func) {
    symbol *symbols;

    // Our main ELF structure
    elf_struct elf;

    //Read the file into our elf struct
    read_file(file, &elf);

    //Now find the symbols
    symbols = read_symbols(&elf);

    //If the symbol exists with all of the fields, it should be elgible to be printed
    // Here our value is our address
    int i = 0; 
    int count = 0;
    while (symbols[i].value || symbols[i].type || symbols[i].name){
        //Check if it is a function ('T') and matches the name of the given function
        if (symbols[i].type == 'T' && strcmp(symbols[i].name,func)==0 ){
            if (symbols[i].value) {
                count++;
                printf("%016x %c %s\n", symbols[i].value, symbols[i].type, symbols[i].name);
            }
            //else {
            //    printf("%18c %s\n", symbols[i].type, symbols[i].name);
            //}        
        }
        i++;
    }
    free(symbols);
    if (count == 0) return 0;
    return 1;
}

int print_symbol_table(char* file){
    symbol *symbols;

    // Our main ELF structure
    elf_struct elf;

    //Read the file into our elf struct
    read_file(file, &elf);

    //Now find the symbols
    symbols = read_symbols(&elf);

    //If the symbol exists with all of the fields, it should be elgible to be printed
    // Here our value is our address
    int i = 0; 
    int count = 0;
    while (symbols[i].value || symbols[i].type || symbols[i].name){
        //Check if it is a function ('T') and matches the name of the given function
        if (symbols[i].value) {
            count++;
            printf("%016x %c %s\n", symbols[i].value, symbols[i].type, symbols[i].name);
        }
        else {
            printf("%18c %s\n", symbols[i].type, symbols[i].name);
        }        
        i++;
    }
    free(symbols);
    if (count == 0) return 0;
    return 1;

}
