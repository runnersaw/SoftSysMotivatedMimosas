#ifndef NM_H_
#define NM_H_

// Need this to read ELF files
#include <elf.h>

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

unsigned int function_symbol(char* file, char* func);
unsigned int global_symbol(char* file, char* func);
int print_symbol_table(char* file);

#endif
