
typedef struct {
  unsigned long address;
  unsigned long previousInstruction;
  int pid;
} Breakpoint;

Breakpoint *make_breakpoint();
void insertBreakpoint(Breakpoint *breakpoint);
void resumeBreakpoint(Breakpoint *breakpoint);

