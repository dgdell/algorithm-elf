#include "parse.h"

/**
 * 转换 typename 为首字母大写的格式。
 */
char *
format_typename(char *typename)
{
  if (islower(*typename))
    *typename = toupper(*typename);
  return typename;
}

void
make_entry_paras(PFUNC pfunc)
{
  FILE * f = fopen("_paras_", "w");
  if (f <= 0){
    fprintf(stderr, "%s\n", _("Error when create parameters file"));
    return;
  }
  assert(pfunc);
  PPARA ppara = pfunc -> paras;
  int i;

  fprintf(f, "%s", "{");
  for (i = 0; i < MAX_PARA_NUMBER; i ++, ppara ++) {
    if (! ppara -> name)
      break;
    fprintf(f, "'%s':'%s',", ppara -> name, ppara -> type);
  }
  fprintf(f, "%s", "}");

  /*
  fprintf(f, "%s", "<xml>");
  for (i = 0; i < MAX_PARA_NUMBER; i ++, ppara ++) {
    if (! ppara -> name)
      break;
    fprintf(f,
	    "<variable><name>%s</name><type>%s</type><value></value></variable>\n",
            ppara -> name,
	    ppara -> type
	    );
  }
  fprintf(f, "%s", "</xml>");
  */
  fclose(f);
}


void
print_file_header(PCONTEXT pcontext)
{
  printf("\
# -*- coding: utf-8 -*-\n\
#\n\
# This script is generated by Algorithm Elf 1.2.1\n\
\n\
import sys\n\
import pascal\n\
from aftype import DRIVER, datapool\n\
from aftype import Char, Integer, Boolean, Real\n\
from aftype import String, Array\n\
from aftype import Enum, Set, Record, Pointer\n\
from aftype import Queue, Tree, Indexpointer\n\
\n");
}

void
print_file_footer(PCONTEXT pcontext)
{
  printf("\n");
  printf("def init():\n");
  printf("    vlist = []\n");
  printf("%s\n", pcontext -> datapool);
  printf("    return vlist\n");
  printf("var_list = init()\n");
  printf("\n");
  printf("# End of Class %sAlgorithm\n", pcontext -> algorithm);
}

void
print_class_header(PCONTEXT pcontext)
{
  printf("\nclass AlgorithmInstance(object):\n\n");
  printf("\
    def __init__(self):\n");

  /* 声明全局变量 */
  int indent = 8;
  PVAR pvar = pcontext -> vars;
  char *s;
  if (!pvar -> name)
    printf("\
        pass\n");
  while (pvar -> name){
    if (pvar -> name[0] == '@')
      s = pvar -> name + 1;
    else
      s = pvar -> name;
    i_printf(indent, "%s.declare()\n", s);
    free( pvar -> name );
    free( pvar -> type );
    pvar -> name = 0;
    pvar -> type = 0;
    pvar ++;
  }
  printf("\n");
}

void
print_class_footer(PCONTEXT pcontext)
{
  /*
   if ((pcontext -> entry == 0) && (pcontext -> findex ==0))
     fprintf(stderr, "%s\n", "Error: no any procedure");
   else if (pcontext -> entry){
     int i;
     PFUNC pfunc = pcontext -> funcs;
     for (i = 0; i < pcontext -> findex; i ++, pfunc ++)
       if (strcmp(pcontext -> entry, pfunc -> name) == 0)
	 return;
     fprintf(stderr, "Error: entry '%s' not found\n", pcontext -> entry);
   }
  */
}

void
print_main_entry1(PFUNC pfunc)
{
  PPARA ppara = pfunc -> paras;
  int i;

  /* 打印 run 方法 */
  i_printf(4, "%s\n", "def run(self):");
  i_printf(8, "self.%s(", pfunc -> name);

  for (i = 0; i < MAX_PARA_NUMBER; i ++, ppara ++) {
    if (! ppara -> name)
      break;
    printf("%s=Vars['%s'],", ppara -> name, ppara -> name);
  }
  printf("%s\n", ")");
  i_printf(8, "%s\n\n", "DRIVER.simulate_function_return()");
}

void
print_main_entry(PCONTEXT pcontext, int lineno)
{
  /* 打印 run 方法 */
  int indent = pcontext -> indent;
  i_printf(indent, "%s\n", "def run(self):");

  indent += INDENT_UNIT;
  i_printf(indent,
           "DRIVER.simulate_program_entry('%s', %d)\n",
           pcontext -> algorithm,
           lineno
           );
}

void
print_func_header(PCONTEXT pcontext, int lineno)
{
  int indent = pcontext -> indent;
  int i;
  PFUNC pfunc = pcontext -> funcs + pcontext -> findex;
  PPARA ppara = pfunc -> paras;
  char * funcname = pfunc -> name;

  /*
  if (((pcontext -> entry == 0) && (pcontext -> findex ==0))
      || (pcontext -> entry && (strcmp(pcontext -> entry, funcname) == 0))){
    make_entry_paras(pfunc);
    print_main_entry(pfunc);
  }
  */

  /* 打印函数头部 */
  i_printf(indent, "def %s(self", pfunc -> name);
  for (i = 0; i < MAX_PARA_NUMBER ; i ++, ppara ++) {
    if (! ppara -> name)
      break;
    printf(", %s", ppara -> name);

  }
  printf("%s\n", "):");

  indent += INDENT_UNIT;

  i_printf(indent,
	   "DRIVER.simulate_function_call('%s', %d, sys._getframe())\n",
	   funcname,
	   lineno
	   );

  /* 参数处理，如果是值参，需要进行克隆 */
  ppara = pfunc -> paras;
  for (i = 0; i < MAX_PARA_NUMBER ; i ++, ppara ++) {
    if (! ppara -> name)
      break;
    if (ppara -> varflag ==0){
      i_printf(indent, "%s = %s.clone()\n", ppara -> name, ppara -> name);
      i_printf(indent, "%s.declare()\n", ppara -> name);
    }
    else
      i_printf(indent, "%s.frame = sys._getframe()\n", ppara -> name);
  }
  i_printf(indent, "%s\n\n", "result = None");
}

void
print_func_footer(PCONTEXT pcontext)
{
  int indent = pcontext -> indent;
  int i;
  PFUNC pfunc = pcontext -> funcs + pcontext -> findex;
  PPARA ppara = pfunc -> paras;

  if (ppara -> name)  printf("\n");
  for (i = 0; i < MAX_PARA_NUMBER ; i ++, ppara ++) {
    if (! ppara -> name)
      break;
    if (ppara -> varflag ==1)
      i_printf(indent, "%s.frame = sys._getframe(1)\n", ppara -> name);
    else
      i_printf(indent, "%s.destroy()\n", ppara -> name);
  }
  PVAR pvar = pcontext -> vars;
  if (pvar -> name) printf("\n");
  while (pvar -> name){
    i_printf(indent, "%s.destroy()\n", pvar -> name);
    free ( pvar -> name );
    free ( pvar -> type );
    pvar -> name = 0;
    pvar -> type = 0;
    pvar ++;
  }
  printf("\n");
  i_printf(indent, "%s\n\n", "return result");
}

void
print_stmt_expr(PCONTEXT pcontext, int lineno, const char *expr)
{
  i_printf(pcontext -> indent,
           "DRIVER.simulate_statement(%d, sys._getframe())\n",
           lineno);
  i_printf(pcontext -> indent, "%s\n", expr);
}


void
print_stmt_case(PCONTEXT pcontext, int lineno, const char *expr)
{
  i_printf(pcontext -> indent,
           "DRIVER.simulate_statement(%d, sys._getframe())\n",
           lineno);
  i_printf(pcontext -> indent, "_case_clause_value = %s\n", expr);
  i_printf(pcontext -> indent, "%s\n", "_case_clause = None");
}

void
print_case_clause(PCONTEXT pcontext, int lineno, const char *expr)
{
  int indent  = pcontext -> indent;
  i_printf(indent, "%s\n", "if _case_clause is None:");
  indent += INDENT_UNIT;
  i_printf(indent, "DRIVER.simulate_statement(%d, sys._getframe())\n", lineno);
  i_printf(indent, "if _case_clause_value == %s:\n", expr);
  indent += INDENT_UNIT;
  i_printf(indent, "%s\n", "_case_clause = True");
}

void
print_case_else_clause(PCONTEXT pcontext, int lineno)
{
  int indent  = pcontext -> indent;
  i_printf(indent, "%s\n", "if _case_clause is None:");
  indent += INDENT_UNIT;
  i_printf(indent, "DRIVER.simulate_statement(%d, sys._getframe())\n", lineno);
  i_printf(indent, "%s\n", "_case_clause = True");
}

void
print_expr_func(PCONTEXT pcontext, int lineno, const char *funcname, const char *args)
{
  int indent = pcontext -> indent;
  i_printf(indent, "DRIVER.simulate_statement(%d, sys._getframe())\n", lineno);
  i_printf(indent, "_%s = self.%s(%s)\n", funcname, funcname, args);
  i_printf(indent, "%s\n", "DRIVER.simulate_function_return()");
}

char *
format_tokname(const char *tokname)
{
  if (strcasecmp(tokname, "exit") == 0)
    return strdup("return");
  else if (strcasecmp(tokname, "nil") == 0)
    return strdup("Pointer()");
  else if (strcasecmp(tokname, "none") == 0)
    return strdup("None");
  else
    return strdup(tokname);
}

void
print_type(PCONTEXT pcontext, char *typename, char *basename)
{
  i_printf(pcontext -> indent,
	   "class %s(%s): pass\n",
	   format_typename(typename),
	   format_typename(basename)
	   );
  printf("\n");
}

void
print_type_enum(PCONTEXT pcontext, char *typename)
{
/*
   class Te(Enum):
       def __init__(self, value=None):
           Enum.__init__(self, value=value)
           self.frame = sys._getframe(1)
           self.data_dict = {'red': 0, 'green': 1, 'blue': 2, }
   global red
   red = 0
   global green
   green = 1
   global blue
   blue = 2
*/
  int indent = pcontext -> indent;
  i_printf(indent, "class %s(Enum):\n", format_typename(typename));
  indent += INDENT_UNIT;
  i_printf(indent, "%s\n", "def __init__(self, value=None):");
  indent += INDENT_UNIT;
  i_printf(indent, "%s\n", "Enum.__init__(self, value=value)");
  i_printf(indent, "%s\n", "self.frame = sys._getframe(1)");
  i_printf(indent, "%s", "self.data_dict = {");
  PVAR pvar = pcontext -> vars;
  int i;
  for ( i = 0; i < MAX_VARS_NUMBER; i ++, pvar ++ ) {
    if ( ! pvar -> name )
      break;
    printf("'%s':%d,", pvar -> name, i);
  }
  printf("}\n");

  indent -= INDENT_UNIT - INDENT_UNIT;
  pvar = pcontext -> vars;
  for ( i = 0; i < MAX_VARS_NUMBER; i ++, pvar ++ ) {
    if ( ! pvar -> name )
      break;
    i_printf(indent, "global %s\n", pvar -> name);
    i_printf(indent, "%s = %d\n", pvar -> name, i);
    free ( pvar -> name );
    free ( pvar -> type );
    pvar -> name = 0;
    pvar -> type = 0;
  }
  /*memset(pcontext -> vars, 0, sizeof(pcontext -> vars));*/
  printf("\n");
}

void
print_type_pointer(PCONTEXT pcontext, char *typename, char *basename)
{
  /*
   class PTx(Pointer):
       def __init__(self, cls=Tx):
           Pointer.__init__(self, cls=cls)
           self.frame = sys._getframe(1)
  */
  int indent = pcontext -> indent;
  i_printf(indent, "class %s(Pointer):\n", format_typename(typename));
  indent += INDENT_UNIT;
  i_printf(indent,
           "def __init__(self, value=None, cls=%s):\n",
           format_typename(basename)
           );
  indent += INDENT_UNIT;
  i_printf(indent, "%s\n", "Pointer.__init__(self, cls=cls)");
  i_printf(indent, "%s\n", "self.frame = sys._getframe(1)");
  printf("\n");
}
void
print_type_record(PCONTEXT pcontext, char *typename)
{
  /*
   class Tr(Record):
       def __init__(self, value={'name':String(), 'value':Real()}):
           Record.__init__(self, value=value)
           self.frame = sys._getframe(1)
  */
  int indent = pcontext -> indent;
  i_printf(indent, "class %s(Record):\n", format_typename(typename));
  indent += INDENT_UNIT;
  i_printf(indent, "%s", "def __init__(self, value={");
  PVAR pvar = pcontext -> vars;
  int i;
  for ( i = 0; i < MAX_VARS_NUMBER; i ++, pvar ++ ) {
    if ( ! pvar -> name )
      break;
    printf("'%s':%s(),", pvar -> name, format_typename(pvar -> type));
    free(pvar -> name);
    free(pvar -> type);
    pvar -> name = 0;
    pvar -> type = 0;
  }
  /* memset(pcontext -> vars, 0, sizeof(pcontext -> vars)); */
  printf("}):\n");
  indent += INDENT_UNIT;
  i_printf(indent, "%s\n", "Record.__init__(self, value=value)");
  i_printf(indent, "%s\n", "self.frame = sys._getframe(1)");
  printf("\n");
}

void
print_type_array1(PCONTEXT pcontext, char *typename, char *basename)
{
  /*
   class Ta(Array):
       def __init__(self, value={}):
           Array.__init__(self, value=value, cls=Integer)
           self.frame = sys._getframe(1)
  */
  int indent = pcontext -> indent;
  i_printf(indent, "class %s(Array):\n", format_typename(typename));
  indent += INDENT_UNIT;
  i_printf(indent, "%s\n", "def __init__(self, value={}):");
  indent += INDENT_UNIT;
  i_printf(indent,
           "Array.__init__(self, value=value, cls=%s)\n",
           format_typename(basename));
  i_printf(indent, "%s\n", "self.frame = sys._getframe(1)");
  printf("\n");
}

void
print_type_array(PCONTEXT pcontext, char *typename, char *basename, char *start, char *end)
{
  /*
   class Tfa(Array):
       def __init__(self, value=dict([ [i, Integer()] for i in range(Low, High) ])):
           Array.__init__(self, value=value, cls=Integer)
           self.frame = sys._getframe(1)
  */
  int indent = pcontext -> indent;
  i_printf(indent, "class %s(Array):\n", format_typename(typename));
  indent += INDENT_UNIT;
  i_printf(indent,
	   "def __init__(self, value=dict([ [i, %s()] for i in range(%s, %s) ])):\n",
	   format_typename(basename),
	   start,
	   end
	   );
  indent += INDENT_UNIT;
  i_printf(indent,
           "Array.__init__(self, value=value, cls=%s)\n",
           format_typename(basename));
  i_printf(indent, "%s\n", "self.frame = sys._getframe(1)");
  printf("\n");
}

void
print_type_set(PCONTEXT pcontext, char *typename, char *basename)
{
  int indent = pcontext -> indent;
  i_printf(indent, "class %s(Set): pass\n", format_typename(typename));
  printf("\n");
}

void
print_var_list(PCONTEXT pcontext)
{
  int indent = pcontext -> indent;
  PVAR pvar = pcontext -> vars;
  int i;
  char *s;
  for ( i = 0; i < MAX_VARS_NUMBER; i ++, pvar ++ ) {
    if ( ! pvar -> name )
      break;
    if (pvar -> name[0] == '@'){
      int k = strlen(pcontext -> datapool);
      int n = strlen(pvar -> name);
      s = pvar -> name + 1;
      snprintf(pcontext -> datapool + k,
               MAX_ITEM_SIZE - n,
               "    global %s\n"
               "    vlist.append(('%s', '%s'))\n"
               "    %s = datapool('%s', '%s')\n",
               s,
               s,
               pvar -> type,               
               s,
               s,
               pvar -> type               
               );
    }
    else{
      i_printf(indent, "%s = %s()\n", pvar -> name, pvar -> type);
      if (pcontext -> findex > 0)
        i_printf(indent, "%s.declare()\n", pvar -> name);
    }

  }
  /*memset(pcontext -> vars, 0, sizeof(pcontext -> vars));*/
}

char *
get_func_scope(const char * name)
{
  if ((strcasecmp(name, "addr") == 0)
      || (strcasecmp(name, "copy") == 0)
      || (strcasecmp(name, "cos") == 0)
      || (strcasecmp(name, "delete") == 0)
      || (strcasecmp(name, "dispose") == 0)
      || (strcasecmp(name, "exp") == 0)
      || (strcasecmp(name, "frac") == 0)
      || (strcasecmp(name, "high") == 0)
      || (strcasecmp(name, "insert") == 0)
      || (strcasecmp(name, "int") == 0)
      || (strcasecmp(name, "length") == 0)
      || (strcasecmp(name, "ln") == 0)
      || (strcasecmp(name, "low") == 0)
      || (strcasecmp(name, "move") == 0)
      || (strcasecmp(name, "new") == 0)
      || (strcasecmp(name, "odd") == 0)
      || (strcasecmp(name, "ord") == 0)
      || (strcasecmp(name, "ptr") == 0)
      || (strcasecmp(name, "pos") == 0)
      || (strcasecmp(name, "setlength") == 0)
      || (strcasecmp(name, "sin") == 0)
      || (strcasecmp(name, "sqr") == 0)
      || (strcasecmp(name, "sqrt") == 0)
      || (strcasecmp(name, "trunc") == 0)
      || (strcasecmp(name, "write") == 0)
      || (strcasecmp(name, "writeln") == 0))
    return "pascal.";
  else if ((strcasecmp(name, "abs") == 0)
           || (strcasecmp(name, "chr") == 0)
           || (strcasecmp(name, "round") == 0))
    return "";
  else
    return "self.";
}

void
set_para_type(PCONTEXT pcontext, char *typename, int varflag)
{
  PFUNC pfunc = pcontext -> funcs + pcontext -> findex;
  int i;
  for ( i = 0; i < MAX_PARA_NUMBER; i ++ ) {
    if ( pfunc -> paras [ i ].name == 0 )
      break;
    if ( pfunc -> paras [ i ].type == 0 ) {
      pfunc -> paras [ i ].varflag = varflag;
      pfunc -> paras [ i ].type = format_typename(strdup(typename));
    }
  }
}

void
push_para_name(PCONTEXT pcontext, char *name)
{
  PFUNC pfunc = pcontext -> funcs + pcontext -> findex;
  int i = 0;
  while ( pfunc -> paras [ i ].name ) {
    i ++;
    if ( i > MAX_PARA_NUMBER ) {
      fprintf(stderr, "%s\n", "Error: The stack of parameters overflowed");
      exit(-2);
    }
  }
  pfunc -> paras[i].name = strdup(name);
}

void
set_var_type(PCONTEXT pcontext, char *typename)
{
  PVAR pvar = pcontext -> vars;
  int i;
  char *s;
  for ( i = 0; i < MAX_VARS_NUMBER; i ++ ) {
    if ( pvar -> name == 0 )
      break;
    if ( pvar -> type == 0 ){
      asprintf(&s, "%s", typename);
      pvar -> type = format_typename(s);
    }
    pvar ++;
  }
}

void
push_var_name(PCONTEXT pcontext, char *name, int atflag)
{
  PVAR pvar = pcontext -> vars;
  int i = 0;
  while ( pvar -> name ) {
    i ++;
    pvar ++;
    if ( i > MAX_VARS_NUMBER ) {
      fprintf(stderr, "%s\n", "Error: The stack of variables overflowed");
      exit(-2);
    }
  }
  if (atflag)
    asprintf(&(pvar -> name), "@%s", name);
  else
    pvar -> name = strdup(name);
}

void
a_sprintf(char **strp, const char *fmt, ...)
{
  va_list ap;
  int ret;
  int size = 256;

  int nchars;

  *strp = (char *) malloc(size);
  if (*strp == NULL){
    fprintf(stderr, "Error: %s\n", "allocating memory fails");
    exit(-1);
  }
  memset(*strp, 0, size);

  va_start(ap, fmt);
  /* Try to print in the allocated space. */
  nchars = vsnprintf (*strp, size, fmt, ap);
  if (nchars >= size)
    {
      /* Reallocate *strp now that we know
	 how much space is needed. */
      size = nchars + 1;
      *strp = (char *)realloc(*strp, size);
      memset(*strp, 0, size);

      if (*strp != NULL)
	vsnprintf(*strp, size, fmt, ap);
    }
  va_end(ap);
  /* The last call worked, return the string. */
  if (*strp == NULL){
    fprintf(stderr, "Error: %s\n", "allocating memory fails");
    exit(-1);
  }
}

void
i_printf(int indent, const char *fmt, ...)
{
  assert((indent >= 0) && (indent <= 1024));
  char *s = (char*)malloc(indent + 1);
  if (s == NULL){
    fprintf(stderr, "Error: %s\n", "allocating memory fails");
    exit(-1);
  }
  memset(s, 32, indent);
  *(s + indent) = 0;
  printf("%s", s);
  free(s);

  va_list ap;
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
}


