typedef struct {
  unsigned long address;
  unsigned long previousInstruction;
  int pid;
} Breakpoint;

unsigned long insertBreakpoint(Breakpoint *breakpoint);